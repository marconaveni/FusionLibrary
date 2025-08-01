#include "renderer.h"
#include "stb_truetype.h"
#include "font.h"
#include "utf8.h"

namespace Fusion
{
    Renderer::Renderer()
    {
        m_MaxQuads = 1000;
        m_MaxVertices = m_MaxQuads * 4;
        m_MaxIndices = m_MaxQuads * 6;
        m_Vertices.resize(m_MaxVertices);
    }

    Renderer::~Renderer()
    {
        glDeleteVertexArrays(1, &m_BatchVAO);
        glDeleteBuffers(1, &m_BatchVBO);
        glDeleteBuffers(1, &m_BatchEBO);
    }

    void Renderer::BeginRender()
    {
        m_VertexCount = 0;
        m_CurrentTextureID = 0;
    }

    void Renderer::EndRender()
    {
        if (m_VertexCount > 0)
        {
            Flush();
        }
    }

    void Renderer::DrawTexture(const Texture &texture, const Rectangle &source, const Rectangle &dest, const Vector2f &origin, float rotation, const Color &color)
    {

        // 1. Verifica se o shader de textura precisa ser ativado
        if (m_CurrentShader == nullptr || m_TextureShader.ID != m_CurrentShader->ID)
        {
            Flush(); // Desenha qualquer lote antigo com o shader antigo

            m_CurrentShader = &m_TextureShader;
            m_CurrentShader->use();

            // 2. Envia os uniformes necessários para este shader (SÓ UMA VEZ POR TROCA)
            m_CurrentShader->getUniformMatrix4("projection", m_Projection);
        }

        // A lógica de flush por mudança de textura ou shader permanece a mesma
        if ((m_VertexCount + 4) > m_MaxVertices || (texture.GetId() != m_CurrentTextureID && m_CurrentTextureID != 0))
        {
            Flush();
        }

        // Define a textura para o novo lote (se necessário)
        if (m_VertexCount == 0)
        {
            m_CurrentTextureID = texture.GetId();
        }

        // --- MÁGICA DA TRANSFORMAÇÃO ---

        // 1. Construir a matriz 'model'
        glm::mat4 model = glm::mat4(1.0f);
        // A ordem importa: primeiro movemos para a posição final, depois rotacionamos em torno da origem
        model = glm::translate(model, glm::vec3(dest.x, dest.y, 0.0f));
        model = glm::translate(model, glm::vec3(origin.x, origin.y, 0.0f));              // Move o pivô de rotação para a origem do objeto
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotaciona
        model = glm::translate(model, glm::vec3(-origin.x, -origin.y, 0.0f));            // Move o pivô de volta
        model = glm::scale(model, glm::vec3(dest.width, dest.height, 1.0f));             // Aplica a escala (tamanho)

        // 2. Definir os cantos de um quad local (de 0,0 a 1,1)
        glm::vec4 positions[4] = {
            {0.0f, 0.0f, 0.0f, 1.0f}, // Top-left
            {1.0f, 0.0f, 0.0f, 1.0f}, // Top-right
            {1.0f, 1.0f, 0.0f, 1.0f}, // Bottom-right
            {0.0f, 1.0f, 0.0f, 1.0f}  // Bottom-left
        };

        // 3. Transformar cada canto pela matriz 'model'
        for (int i = 0; i < 4; i++)
        {
            positions[i] = model * positions[i];
        }

        // Coordenadas de textura (UVs) - não mudam
        const float u1 = source.x / texture.GetSize().width;
        const float v1 = source.y / texture.GetSize().height;
        const float u2 = (source.x + source.width) / texture.GetSize().width;
        const float v2 = (source.y + source.height) / texture.GetSize().height;
        glm::vec2 uv1 = {u1, v1}; // UV Top-left
        glm::vec2 uv2 = {u2, v1}; // UV Top-right
        glm::vec2 uv3 = {u2, v2}; // UV Bottom-right
        glm::vec2 uv4 = {u1, v2}; // UV Bottom-left

        // Cor
        glm::vec4 glmColor = {color.r, color.g, color.b, color.a};

        // 4. Adicionar os vértices JÁ TRANSFORMADOS ao lote
        m_Vertices[m_VertexCount++] = {glm::vec3(positions[3]), glmColor, uv4}; // Vértice 0: Bottom-left
        m_Vertices[m_VertexCount++] = {glm::vec3(positions[2]), glmColor, uv3}; // Vértice 1: Bottom-right
        m_Vertices[m_VertexCount++] = {glm::vec3(positions[1]), glmColor, uv2}; // Vértice 2: Top-right
        m_Vertices[m_VertexCount++] = {glm::vec3(positions[0]), glmColor, uv1}; // Vértice 3: Top-left
    }

    void Renderer::DrawText(const Font &font, const std::string &text, Vector2f position, Vector2f origin, float rotation, float scale, Color color)
    {
        // 1. Verifica se o shader de textura precisa ser ativado
        if (m_CurrentShader == nullptr || m_TextShader.ID != m_CurrentShader->ID)
        {
            Flush(); // Desenha qualquer lote antigo com o shader antigo

            m_CurrentShader = &m_TextShader;
            m_CurrentShader->use();

            // 2. Envia os uniformes necessários para este shader (SÓ UMA VEZ POR TROCA)
            m_CurrentShader->getUniformMatrix4("projection", m_Projection);
            glUniform4f(m_CurrentShader->getUniformLocation("textColor"), color.r, color.g, color.b, color.a);
        }

        // A lógica de flush por mudança de textura ou shader permanece a mesma
        if ((m_VertexCount + 4) > m_MaxVertices || (font.GetId() != m_CurrentTextureID && m_CurrentTextureID != 0))
        {
            Flush();
        }

        if (m_VertexCount == 0)
        {
            m_CurrentTextureID = font.GetId();
        }

        glm::vec3 pivot = glm::vec3(position.x + origin.x, position.y + origin.y, 0.0f);

        // 2. Cria a matriz de rotação que gira em torno do pivô
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, pivot);
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::translate(transform, -pivot);

        // Converte a cor da engine para glm::vec4
        glm::vec4 glmColor = {color.r, color.g, color.b, color.a};

        float xpos = position.x;
        float ypos = position.y + font.GetTopToBaseline();

        float x = font.GetTopToBaseline();
        const utf8_int8_t *p = reinterpret_cast<const utf8_int8_t *>(text.c_str());

        while (*p)
        {
            int32_t codepoint = 0;
            const utf8_int8_t *next_p = utf8codepoint(p, &codepoint);
            if (codepoint == 0)
                break;

            auto itGlyph = font.GetCharData().find(codepoint);
            if (itGlyph == font.GetCharData().end())
            {
                codepoint = '?';
                itGlyph = font.GetCharData().find(codepoint);
                if (itGlyph == font.GetCharData().end())
                {
                    p = next_p;
                    continue;
                }
            }

            stbtt_packedchar pc = itGlyph->second; // Atribuição direta!

            stbtt_aligned_quad q;
            stbtt_GetPackedQuad(&pc, 512, 512, 0, &xpos, &ypos, &q, 0);

            // Checa se ainda há espaço no buffer para mais um quad
            if ((m_VertexCount + 4) > m_MaxVertices)
            {
                Flush();
            }

            // Calcula os 4 vértices para o caractere atual e adiciona ao lote
            glm::vec4 p1 = {q.x0 * scale, q.y0 * scale, 0.0f, 1.0f}; // Top-left
            glm::vec4 p2 = {q.x1 * scale, q.y0 * scale, 0.0f, 1.0f}; // Top-right
            glm::vec4 p3 = {q.x1 * scale, q.y1 * scale, 0.0f, 1.0f}; // Bottom-right
            glm::vec4 p4 = {q.x0 * scale, q.y1 * scale, 0.0f, 1.0f}; // Bottom-left

            p1 = transform * p1;
            p2 = transform * p2;
            p3 = transform * p3;
            p4 = transform * p4;

            glm::vec2 uv1 = {q.s0, q.t0}; // UV Top-left
            glm::vec2 uv2 = {q.s1, q.t0}; // UV Top-right
            glm::vec2 uv3 = {q.s1, q.t1}; // UV Bottom-right
            glm::vec2 uv4 = {q.s0, q.t1}; // UV Bottom-left

            m_Vertices[m_VertexCount++] = {glm::vec3(p4), glmColor, uv4}; // Vértice 0: Bottom-left
            m_Vertices[m_VertexCount++] = {glm::vec3(p3), glmColor, uv3}; // Vértice 1: Bottom-right
            m_Vertices[m_VertexCount++] = {glm::vec3(p2), glmColor, uv2}; // Vértice 2: Top-right
            m_Vertices[m_VertexCount++] = {glm::vec3(p1), glmColor, uv1}; // Vértice 3: Top-left

            p = next_p;
        }
    }

    void Renderer::Init()
    {
        InitBatch();

        // =======ESTADO DE RENDERIZAÇÃO PADRÕES ========
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // ==================================================

        // ====== Start Shader program ======
        m_TextureShader.LoadShader(DEFAULT_SHADER);
        m_TextShader.LoadShader(TEXT_DEFAULT_SHADER);
        // ==================================================

        m_Projection = glm::ortho(0.0f, static_cast<float>(800), static_cast<float>(600), 0.0f); // tela 800x600
    }

    void Renderer::InitBatch()
    {
        glGenVertexArrays(1, &m_BatchVAO);
        glBindVertexArray(m_BatchVAO);

        // VBO (Vertex Buffer Object) - Para os dados dos vértices
        glGenBuffers(1, &m_BatchVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_BatchVBO);
        glBufferData(GL_ARRAY_BUFFER, m_MaxVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

        // Gerar os índices para todos os quads possíveis. O padrão é 0,1,2, 2,3,0 -> 4,5,6, 6,7,4, etc.
        std::vector<GLuint> indices;
        indices.resize(m_MaxIndices);
        uint32_t offset = 0;
        for (size_t i = 0; i < indices.size(); i += 6)
        {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 2 + offset;
            indices[i + 3] = 2 + offset;
            indices[i + 4] = 3 + offset;
            indices[i + 5] = 0 + offset;
            offset += 4;
        }

        // EBO (Element Buffer Object) - Para os índices
        glGenBuffers(1, &m_BatchEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BatchEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        // Configura os atributos do vértice
        // Posição
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
        // cores
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
        // Coordenadas de Textura
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));

        glBindVertexArray(0);
    }

    void Renderer::Flush()
    {
        if (m_VertexCount == 0)
            return;

        if (m_CurrentShader)
            m_CurrentShader->use();

        glBindVertexArray(m_BatchVAO);
        glBindTexture(GL_TEXTURE_2D, m_CurrentTextureID);

        // Envia os dados acumulados para o VBO na GPU
        glBindBuffer(GL_ARRAY_BUFFER, m_BatchVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_VertexCount * sizeof(Vertex), m_Vertices.data());

        // Desenha tudo de uma vez!
        // O número de índices é (vertexCount / 4) * 6
        glDrawElements(GL_TRIANGLES, (m_VertexCount / 4) * 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        // Reseta o contador para o próximo lote
        m_VertexCount = 0;
    }

} // namespace Fusion
