#include "renderer.h"

#include <cmath> // Necessário para sin() e cos()
#include <iostream>

#include "font.h"
#include "fusion_utf8.h"
#include "shader_code.h"
#include "sprite.h"
#include "stb_truetype.h"
#include "text.h"
#include "texture.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Fusion
{
    Renderer::Renderer()
        : m_textureShader(), m_textShader()
    {
        m_maxQuads = 1000;
        m_maxVertices = m_maxQuads * 4;
        m_maxIndices = m_maxQuads * 6;
        m_vertices.resize(m_maxVertices);

        m_view = glm::mat4(1.0f);
    }

    void Renderer::Shutdown()
    {
        m_textureShader.Unload();
        m_textShader.Unload();

        glDeleteTextures(1, &m_defaultTextureID);

        glDeleteVertexArrays(1, &m_batchVAO);
        glDeleteBuffers(1, &m_batchVBO);
        glDeleteBuffers(1, &m_batchEBO);

        m_batchVAO = 0;
        m_batchVBO = 0;
        m_batchEBO = 0;
        m_defaultTextureID = 0;
    }

    void Renderer::BeginRender()
    {
        m_vertexCount = 0;
        m_currentTextureID = 0;
        m_currentShader = nullptr;
    }

    void Renderer::EndRender()
    {
        if (m_vertexCount > 0)
        {
            Flush();
        }
    }

    void Renderer::DrawTexture(const Sprite& sprite, Shader* customShader)
    {

        if (sprite.GetTexture() == nullptr)
        {
            return;
        }

        Shader* shaderToUse = customShader ? customShader : &m_textureShader;

        // 1. Verifica se o shader de textura precisa ser ativado
        if (m_currentShader == nullptr || shaderToUse->m_id != m_currentShader->m_id)
        {
            Flush(); // Desenha qualquer lote antigo (como texto) com o shader antigo

            m_currentShader = shaderToUse;
            SetProjection(m_projection);
        }

        if ((m_vertexCount + 4) > m_maxVertices || (sprite.GetTexture()->GetId() != m_currentTextureID && m_currentTextureID != 0))
        {
            Flush();
        }

        if (m_vertexCount == 0)
        {
            m_currentTextureID = sprite.GetTexture()->GetId();
        }

        const std::vector<Vertex>& textureVertices = sprite.GetVertices();

        if (textureVertices.empty())
        {
            return;
        }

        for (const auto& vertex : textureVertices)
        {
            if ((m_vertexCount + 1) > m_maxVertices)
            {
                Flush();
            }
            m_vertices[m_vertexCount++] = vertex;
        }
    }

    void Renderer::DrawText(const Text& text, Shader* customShader)
    {

        const Font& font = *text.GetFont();
        const std::string& textContent = text.GetText();
        const Color color = text.GetColor();

        Shader* shaderToUse = customShader ? customShader : &m_textShader;

        // 1. Verifica se o shader de textura precisa ser ativado
        if (m_currentShader == nullptr || shaderToUse->m_id != m_currentShader->m_id)
        {
            Flush(); // Desenha qualquer lote antigo com o shader antigo

            m_currentShader = shaderToUse;
            SetProjection(m_projection);
            glUniform4f(m_currentShader->GetUniformLocation("textColor"), color.r, color.g, color.b, color.a);
        }

        // A lógica de flush por mudança de textura ou shader permanece a mesma
        if ((m_vertexCount + 4) > m_maxVertices || (font.GetId() != m_currentTextureID && m_currentTextureID != 0))
        {
            Flush();
        }

        if (m_vertexCount == 0)
        {
            m_currentTextureID = font.GetId();
        }

        // O Renderer agora é simples: ele só pede os vértices prontos!
        const std::vector<Vertex>& textVertices = text.GetVertices();

        if (textVertices.empty())
        {
            return;
        }

        // Adiciona os vértices do cache do texto ao lote do renderer
        for (const auto& vertex : textVertices)
        {
            // A checagem de buffer cheio agora é feita por vértice
            if ((m_vertexCount + 1) > m_maxVertices)
            {
                Flush();
            }
            m_vertices[m_vertexCount++] = vertex;
        }
    }

    void Renderer::DrawRectangle(int x, int y, int width, int height, Color color)
    {

        CheckFlushShape();

        glm::vec4 glmColor = {color.r, color.g, color.b, color.a}; // Converte a cor da Fusion para um glm::vec4
        glm::vec2 texCoords = {0.0f, 0.0f}; // Coordenadas de textura podem ser qualquer coisa, já que a textura é 1x1

        // Adiciona os 4 vértices do retângulo ao lote
        m_vertices[m_vertexCount++] = {{(float)x, (float)(y + height), 0.0f}, glmColor, texCoords};           // Bottom-left
        m_vertices[m_vertexCount++] = {{(float)(x + width), (float)(y + height), 0.0f}, glmColor, texCoords}; // Bottom-right
        m_vertices[m_vertexCount++] = {{(float)(x + width), (float)y, 0.0f}, glmColor, texCoords};            // Top-right
        m_vertices[m_vertexCount++] = {{(float)x, (float)y, 0.0f}, glmColor, texCoords};                      // Top-left
    }

    void Renderer::DrawCircle(Vector2f center, float radius, Color color)
    {
        // Garante que o estado do renderizador está pronto para desenhar formas

        CheckFlushShape();

        const int segments = 36; // Define o número de fatias da nossa "pizza"
        glm::vec4 glmColor = {color.r, color.g, color.b, color.a};
        glm::vec2 texCoords = {0.0f, 0.0f}; // Usamos a textura branca

        glm::vec3 centerVertex = {center.x, center.y, 0.0f}; // Vértice central, comum a todos os triângulos

        for (int i = 0; i < segments; i++)
        {
            // Verifica se o buffer está cheio antes de adicionar um novo triângulo (3 vértices)
            // Para manter o batch de quads, adicionamos 4 vértices, mas o último é uma cópia.
            if ((m_vertexCount + 4) > m_maxVertices)
            {
                Flush();
            }

            // Calcula o ângulo para o ponto atual e o próximo ponto na borda do círculo
            float angle = 2.0f * M_PI * (float)i / segments;
            float nextAngle = 2.0f * M_PI * (float)(i + 1) / segments;

            // Calcula os pontos na borda
            glm::vec3 p1 = {center.x + radius * cosf(angle), center.y + radius * sinf(angle), 0.0f};
            glm::vec3 p2 = {center.x + radius * cosf(nextAngle), center.y + radius * sinf(nextAngle), 0.0f};

            // Adiciona os vértices do triângulo (fatia da pizza) ao lote
            // Para manter a estrutura de quads, o último vértice é uma cópia,
            // criando um triângulo degenerado que a GPU ignora.
            m_vertices[m_vertexCount++] = {centerVertex, glmColor, texCoords};
            m_vertices[m_vertexCount++] = {p1, glmColor, texCoords};
            m_vertices[m_vertexCount++] = {p2, glmColor, texCoords};
            m_vertices[m_vertexCount++] = {p2, glmColor, texCoords}; // Vértice duplicado
        }
    }

    void Renderer::DrawCircleLines(Vector2f center, float radius, Color color)
    {
        CheckFlushShape();

        const int segments = 32; // Define o número de fatias da nossa "pizza"
        glm::vec4 glmColor = {color.r, color.g, color.b, color.a};
        glm::vec2 texCoords = {0.0f, 0.0f}; // Usamos a textura branca

        glm::vec3 centerVertex = {center.x, center.y, 0.0f}; // Vértice central, comum a todos os triângulos

        for (int i = 0; i < segments; i++)
        {
            // Verifica se o buffer está cheio antes de adicionar um novo triângulo (3 vértices)
            // Para manter o batch de quads, adicionamos 4 vértices, mas o último é uma cópia.
            if ((m_vertexCount + 4) > m_maxVertices)
            {
                Flush();
            }

            // Calcula o ângulo para o ponto atual e o próximo ponto na borda do círculo
            float startAngle = 2.0f * M_PI * (float)i / segments;
            float endAngle = 2.0f * M_PI * (float)(i + 1) / segments;

            glm::vec2 startPos = {center.x + radius * cosf(startAngle), center.y + radius * sinf(startAngle)};
            glm::vec2 endPos = {center.x + radius * cosf(endAngle), center.y + radius * sinf(endAngle)};

            glm::vec2 delta = endPos - startPos;
            glm::vec2 normal = glm::normalize(glm::vec2(-delta.y, delta.x));
            glm::vec2 offset = normal * (1 / 2.0f);

            glm::vec3 p1 = {startPos.x + offset.x, startPos.y + offset.y, 0.0f};
            glm::vec3 p2 = {startPos.x - offset.x, startPos.y - offset.y, 0.0f};
            glm::vec3 p3 = {endPos.x - offset.x, endPos.y - offset.y, 0.0f};
            glm::vec3 p4 = {endPos.x + offset.x, endPos.y + offset.y, 0.0f};

            m_vertices[m_vertexCount++] = {p2, glmColor, texCoords}; // Bottom-left
            m_vertices[m_vertexCount++] = {p3, glmColor, texCoords}; // Bottom-right
            m_vertices[m_vertexCount++] = {p4, glmColor, texCoords}; // Top-right
            m_vertices[m_vertexCount++] = {p1, glmColor, texCoords}; // Top-left
        }
    }

    void Renderer::DrawTriangle(Vector2f v1, Vector2f v2, Vector2f v3, Color color)
    {
        CheckFlushShape();

        glm::vec4 glmColor = {color.r, color.g, color.b, color.a}; // Converte a cor da Fusion para um glm::vec4
        glm::vec2 texCoords = {0.0f, 0.0f}; // Coordenadas de textura podem ser qualquer coisa, já que a textura é 1x1

        // Adiciona os 4 vértices do retângulo ao lote
        m_vertices[m_vertexCount++] = {{v1.x, v1.y, 0.0f}, glmColor, texCoords}; // p1
        m_vertices[m_vertexCount++] = {{v2.x, v2.y, 0.0f}, glmColor, texCoords}; // p2
        m_vertices[m_vertexCount++] = {{v3.x, v3.y, 0.0f}, glmColor, texCoords}; // p3
        m_vertices[m_vertexCount++] = {{v3.x, v3.y, 0.0f}, glmColor, texCoords}; // p3 Vértice duplicado
    }

    void Renderer::DrawLine(Vector2f startPos, Vector2f endPos, float thick, Color color)
    {
        CheckFlushShape();

        glm::vec4 glmColor = {color.r, color.g, color.b, color.a}; // Converte a cor da Fusion para um glm::vec4
        glm::vec2 texCoords = {0.0f, 0.0f}; // Coordenadas de textura podem ser qualquer coisa, já que a textura é 1x1

        // Converte os vetores da Fusion para vetores glm para facilitar a matemática
        glm::vec2 start = {startPos.x, startPos.y};
        glm::vec2 end = {endPos.x, endPos.y};

        // 1. Calcula o vetor direcional e o normal
        glm::vec2 delta = end - start;
        glm::vec2 normal = glm::normalize(glm::vec2(-delta.y, delta.x));

        // 2. Calcula o deslocamento da espessura
        float halfThick = thick / 2.0f;
        glm::vec2 offset = normal * halfThick;

        // 3. Calcula os 4 cantos do retângulo que forma a linha
        glm::vec3 p1 = {start.x + offset.x, start.y + offset.y, 0.0f}; // Canto superior esquerdo
        glm::vec3 p2 = {start.x - offset.x, start.y - offset.y, 0.0f}; // Canto inferior esquerdo
        glm::vec3 p3 = {end.x - offset.x, end.y - offset.y, 0.0f};     // Canto inferior direito
        glm::vec3 p4 = {end.x + offset.x, end.y + offset.y, 0.0f};     // Canto superior direito

        // 4. Adiciona os vértices ao lote na ordem correta para o index buffer
        m_vertices[m_vertexCount++] = {p2, glmColor, texCoords}; // Bottom-left
        m_vertices[m_vertexCount++] = {p3, glmColor, texCoords}; // Bottom-right
        m_vertices[m_vertexCount++] = {p4, glmColor, texCoords}; // Top-right
        m_vertices[m_vertexCount++] = {p1, glmColor, texCoords}; // Top-left
    }

    void Renderer::DrawRectangleLines(int x, int y, int width, int height, float thick, Color color)
    {
        // Converte as coordenadas para Vector2f para usar na função DrawLine
        const Vector2f topLeftV = {(float)x, (float)y};
        const Vector2f topLeftH = {(float)x, (float)y + thick / 2};
        const Vector2f topRightV = {(float)(x + width), (float)y};
        const Vector2f topRightH = {(float)(x + width), (float)y + thick / 2};
        const Vector2f bottomLeftV = {(float)x, (float)(y + height)};
        const Vector2f bottomLeftH = {(float)x, (float)(y + height) - thick / 2};
        const Vector2f bottomRightV = {(float)(x + width), (float)(y + height)};
        const Vector2f bottomRightH = {(float)(x + width), (float)(y + height) - thick / 2};

        // Desenha as 4 linhas que formam o retângulo
        DrawLine(topLeftH, topRightH, thick, color);       // Linha de cima
        DrawLine(topRightV, bottomRightV, thick, color);   // Linha da direita
        DrawLine(bottomRightH, bottomLeftH, thick, color); // Linha de baixo
        DrawLine(bottomLeftV, topLeftV, thick, color);     // Linha da esquerda
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

    void Renderer::BeginBlendMode(BlendMode mode)
    {
        Flush();

        switch (mode)
        {
            case BLEND_ADDITIVE:
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                glBlendEquation(GL_FUNC_ADD);
                break;
            case BLEND_MULTIPLIED:
                glBlendFunc(GL_DST_COLOR, GL_ZERO);
                glBlendEquation(GL_FUNC_ADD);
                break;
            case BLEND_ALPHA:
            default:
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Modo padrão
                glBlendEquation(GL_FUNC_ADD);
                break;
        }
    }

    void Renderer::EndBlendMode()
    {
        Flush();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Modo padrão
        glBlendEquation(GL_FUNC_ADD);
    }

    void Renderer::Init(int width, int height)
    {
        InitBatch();

        CreateDefaultTexture();

        // =======ESTADO DE RENDERIZAÇÃO PADRÕES ========
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // ==================================================

        // ====== Start Shader program ======
        m_textureShader.LoadShader(GetDefaultVertexShader(), GetDefaultFragmentShader());
        m_textShader.LoadShader(GetDefaultTextVertexShader(), GetDefaultTextFragmentShader());
        // ==================================================

        m_projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f); // tela 800x600
    }

    void Renderer::SetProjection(const glm::mat4& projection)
    {
        m_projection = projection;

        // Se um shader estiver ativo, atualiza seu uniforme de projeção imediatamente
        if (m_currentShader)
        {
            m_currentShader->Use();
            m_currentShader->GetUniformMatrix4("projection", m_projection);
        }
    }

    void Renderer::InitBatch()
    {
        glGenVertexArrays(1, &m_batchVAO);
        glBindVertexArray(m_batchVAO);

        // VBO (Vertex Buffer Object) - Para os dados dos vértices
        glGenBuffers(1, &m_batchVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_batchVBO);
        glBufferData(GL_ARRAY_BUFFER, m_maxVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

        // Gerar os índices para todos os quads possíveis. O padrão é 0,1,2, 2,3,0 -> 4,5,6, 6,7,4, etc.
        std::vector<GLuint> indices;
        indices.resize(m_maxIndices);
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
        glGenBuffers(1, &m_batchEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_batchEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        // Configura os atributos do vértice
        // Posição
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        // cores
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        // Coordenadas de Textura
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        glBindVertexArray(0);
    }

    void Renderer::CreateDefaultTexture()
    {
        glGenTextures(1, &m_defaultTextureID);
        glBindTexture(GL_TEXTURE_2D, m_defaultTextureID);

        // Define os parâmetros da textura
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Cria o pixel branco
        unsigned char whitePixel[4] = {255, 255, 255, 255}; // R, G, B, A

        // Envia o pixel para a GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
        glBindTexture(GL_TEXTURE_2D, 0); // Desvincula para segurança
    }

    void Renderer::Flush()
    {
        if (m_vertexCount == 0)
        {
            return;
        }

        if (m_currentShader)
        {
            m_currentShader->Use();
            m_currentShader->GetUniformMatrix4("projection", m_projection);
            m_currentShader->GetUniformMatrix4("view", m_view); // ENVIA A MATRIZ DE VISTA PARA O SHADER
        }

        glBindVertexArray(m_batchVAO);
        glBindTexture(GL_TEXTURE_2D, m_currentTextureID);

        // Envia os dados acumulados para o VBO na GPU
        glBindBuffer(GL_ARRAY_BUFFER, m_batchVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexCount * sizeof(Vertex), m_vertices.data());

        // Desenha tudo de uma vez!
        // O número de índices é (vertexCount / 4) * 6
        glDrawElements(GL_TRIANGLES, (m_vertexCount / 4) * 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        // Reseta o contador para o próximo lote
        m_vertexCount = 0;
        //m_Vertices.clear();  // tem custo
    }

    void Renderer::CheckFlushShape()
    {
        // Se o lote atual estiver usando uma textura diferente, desenha o lote antigo.
        if (m_currentTextureID != m_defaultTextureID)
        {
            Flush();
        }

        if (m_currentShader != &m_textureShader) // Garante que o shader de textura padrão esteja ativo
        {
            Flush();
            m_currentShader = &m_textureShader;
        }

        m_currentTextureID = m_defaultTextureID; // Define a textura atual como a nossa textura branca

        if ((m_vertexCount + 4) > m_maxVertices) // Verifica se o buffer de vértices está cheio
        {
            Flush();
        }
    }

} // namespace Fusion
