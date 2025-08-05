#include "sprite.h"

namespace Fusion
{

    Sprite::Sprite(const Texture &texture)
        : Transformable(), m_Color(Color{1.0f, 1.0f, 1.0f, 1.0f})
    {
        SetTexture(texture);
    }

    void Sprite::SetTexture(const Texture &texture)
    {
        m_Texture = &texture;
        m_Source.width = texture.GetSize().width;
        m_Source.height = texture.GetSize().height;
        m_Position.width = texture.GetSize().width;
        m_Position.height = texture.GetSize().height;
    }

    void Sprite::UpdateGeometry() const
    {
        m_Vertices.clear(); // Limpa o cache antigo

        // 1. Construir a matriz 'model'
        glm::mat4 model = glm::mat4(1.0f);
        // A ordem importa: primeiro movemos para a posição final, depois rotacionamos em torno da origem
        model = glm::translate(model, glm::vec3(m_Position.x, m_Position.y, 0.0f));
        model = glm::translate(model, glm::vec3(m_Origin.x, m_Origin.y, 0.0f));            // Move o pivô de rotação para a origem do objeto
        model = glm::rotate(model, glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotaciona
        model = glm::translate(model, glm::vec3(-m_Origin.x, -m_Origin.y, 0.0f));          // Move o pivô de volta
        model = glm::scale(model, glm::vec3(m_Position.width, m_Position.height, 1.0f));   // Aplica a escala (tamanho)

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
        const float u1 = m_Source.x / m_Texture->GetSize().width;
        const float v1 = m_Source.y / m_Texture->GetSize().height;
        const float u2 = (m_Source.x + m_Source.width) / m_Texture->GetSize().width;
        const float v2 = (m_Source.y + m_Source.height) / m_Texture->GetSize().height;
        glm::vec2 uv1 = {u1, v1}; // UV Top-left
        glm::vec2 uv2 = {u2, v1}; // UV Top-right
        glm::vec2 uv3 = {u2, v2}; // UV Bottom-right
        glm::vec2 uv4 = {u1, v2}; // UV Bottom-left

        // Cor
        glm::vec4 glmColor = {m_Color.r, m_Color.g, m_Color.b, m_Color.a};

        // 4. Adicionar os vértices JÁ TRANSFORMADOS ao lote
        m_Vertices.push_back({glm::vec3(positions[3]), glmColor, uv4}); // Vértice 0: Bottom-left
        m_Vertices.push_back({glm::vec3(positions[2]), glmColor, uv3}); // Vértice 1: Bottom-right
        m_Vertices.push_back({glm::vec3(positions[1]), glmColor, uv2}); // Vértice 2: Top-right
        m_Vertices.push_back({glm::vec3(positions[0]), glmColor, uv1}); // Vértice 3: Top-left
    }
}