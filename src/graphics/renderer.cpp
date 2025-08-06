#include "renderer.h"
#include "stb_truetype.h"
#include "texture.h"
#include "font.h"
#include "text.h"
#include "sprite.h"

#include "fusion_utf8.h"

namespace Fusion
{
    Renderer::Renderer()
    : m_TextureShader(DEFAULT_SHADER)
    , m_TextShader(TEXT_DEFAULT_SHADER)
    {
        m_MaxQuads = 1000;
        m_MaxVertices = m_MaxQuads * 4;
        m_MaxIndices = m_MaxQuads * 6;
        m_Vertices.resize(m_MaxVertices);

        m_View = glm::mat4(1.0f);
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
        m_CurrentShader = nullptr;
    }

    void Renderer::EndRender()
    {
        if (m_VertexCount > 0)
        {
            Flush();
        }
    }

    void Renderer::DrawTexture(const Sprite &sprite, Shader* customShader)
    {

        Shader* shaderToUse = customShader ? customShader : &m_TextureShader;

        // 1. Verifica se o shader de textura precisa ser ativado
        if (m_CurrentShader == nullptr || shaderToUse->ID != m_CurrentShader->ID)
        {
            Flush(); // Desenha qualquer lote antigo (como texto) com o shader antigo

            m_CurrentShader = shaderToUse;
            SetProjection(m_Projection);
        }

        if ((m_VertexCount + 4) > m_MaxVertices || (sprite.GetTexture()->GetId() != m_CurrentTextureID && m_CurrentTextureID != 0))
        {
            Flush();
        }

        if (m_VertexCount == 0)
        {
            m_CurrentTextureID = sprite.GetTexture()->GetId();
        }

        const std::vector<Vertex> &textureVertices = sprite.GetVertices();

        if (textureVertices.empty())
            return;

        for (const auto &vertex : textureVertices)
        {
            if ((m_VertexCount + 1) > m_MaxVertices)
            {
                Flush();
            }
            m_Vertices[m_VertexCount++] = vertex;
        }
    }

    void Renderer::DrawText(const Text &text, Shader* customShader)
    {

        const Font &font = *text.GetFont();
        const std::string &textContent = text.GetText();
        const Color color = text.GetColor();

        Shader* shaderToUse = customShader ? customShader : &m_TextShader;

        // 1. Verifica se o shader de textura precisa ser ativado
        if (m_CurrentShader == nullptr || shaderToUse->ID != m_CurrentShader->ID)
        {
            Flush(); // Desenha qualquer lote antigo com o shader antigo

            m_CurrentShader = shaderToUse;
            SetProjection(m_Projection);
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

        // O Renderer agora é simples: ele só pede os vértices prontos!
        const std::vector<Vertex> &textVertices = text.GetVertices();

        if (textVertices.empty())
        {
            return;
        }

        // Adiciona os vértices do cache do texto ao lote do renderer
        for (const auto &vertex : textVertices)
        {
            // A checagem de buffer cheio agora é feita por vértice
            if ((m_VertexCount + 1) > m_MaxVertices)
            {
                Flush();
            }
            m_Vertices[m_VertexCount++] = vertex;
        }
    }

    void Renderer::BeginScissorMode(int x, int y, int width, int height)
    {
        glEnable(GL_SCISSOR_TEST);
        glScissor(x, y, width, height);
    }

    void Renderer::EndScissorMode()
    {
        // 2. Desativa o teste para voltar ao normal.
        glDisable(GL_SCISSOR_TEST);
    }

    void Renderer::Init(int width, int height)
    {
        InitBatch();

        // =======ESTADO DE RENDERIZAÇÃO PADRÕES ========
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // ==================================================

        // ====== Start Shader program ======
        m_TextureShader.LoadShader();
        m_TextShader.LoadShader();
        // ==================================================

        m_Projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f); // tela 800x600
    }

    void Renderer::SetProjection(const glm::mat4 &projection)
    {
        m_Projection = projection;

        // Se um shader estiver ativo, atualiza seu uniforme de projeção imediatamente
        if (m_CurrentShader)
        {
            m_CurrentShader->use();
            m_CurrentShader->getUniformMatrix4("projection", m_Projection);
        }
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
        {
            return;
        }

        if (m_CurrentShader)
        {
            m_CurrentShader->use();
            m_CurrentShader->getUniformMatrix4("projection", m_Projection);
            m_CurrentShader->getUniformMatrix4("view", m_View); // ENVIA A MATRIZ DE VISTA PARA O SHADER
        }

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
        m_Vertices.clear();
    }

} // namespace Fusion
