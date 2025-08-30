#include "sprite.h"

#include <algorithm>

namespace Fusion
{
    Sprite::Sprite()
        : Transformable(), m_color(Color{1.0f, 1.0f, 1.0f, 1.0f})
    {
    }

    Sprite::Sprite(const Texture& texture)
        : Transformable(), m_color(Color{1.0f, 1.0f, 1.0f, 1.0f})
    {
        SetTexture(texture);
    }

    void Sprite::SetTexture(const Texture& texture)
    {
        m_texture = &texture;
        m_source.width = texture.GetSize().width;
        m_source.height = texture.GetSize().height;
        m_position.width = texture.GetSize().width;
        m_position.height = texture.GetSize().height;
    }

    void Sprite::UpdateGeometry() const
    {
        m_vertices.clear(); // Limpa o cache antigo

        if (!m_texture)
        {
            return;
        }
        
        // 1. Construir a matriz 'model'
        glm::mat4 model = glm::mat4(1.0f);
        // A ordem importa: primeiro movemos para a posição final, depois rotacionamos em torno da origem
        model = glm::translate(model, glm::vec3(m_position.x, m_position.y, 0.0f));
        model = glm::translate(model, glm::vec3(m_origin.x, m_origin.y, 0.0f)); // Move o pivô rotação(origem do objeto)
        model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotaciona
        model = glm::translate(model, glm::vec3(-m_origin.x, -m_origin.y, 0.0f));          // Move o pivô de volta
        model = glm::scale(model, glm::vec3(m_position.width, m_position.height, 1.0f));   // Aplica a escala (tamanho)

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


        const float texW = static_cast<float>(m_texture->GetSize().width);
        const float texH = static_cast<float>(m_texture->GetSize().height);

        // 1. Calcula as coordenadas U (horizontal)
        const float leftU = m_source.x / texW;
        const float rightU = (m_source.x + m_source.width) / texW;

        const bool flipV = m_texture->IsFboTexture();

        const float topV = (m_source.y + (flipV ? m_source.height : 0.0f)) / texH;
        const float bottomV = (m_source.y + (flipV ? 0.0f : m_source.height)) / texH;

        // 4. Monta os vetores de UV finais
        glm::vec2 uvTopLeft = {leftU, topV};
        glm::vec2 uvTopRight = {rightU, topV};
        glm::vec2 uvBottomRight = {rightU, bottomV};
        glm::vec2 uvBottomLeft = {leftU, bottomV};

        // Cor
        glm::vec4 glmColor = {m_color.r, m_color.g, m_color.b, m_color.a};

        // Adicionar os vértices JÁ TRANSFORMADOS ao lote
        // (A ordem dos vértices não muda)
        m_vertices.push_back({glm::vec3(positions[3]), glmColor, uvBottomLeft});  // Vértice 0: Bottom-left
        m_vertices.push_back({glm::vec3(positions[2]), glmColor, uvBottomRight}); // Vértice 1: Bottom-right
        m_vertices.push_back({glm::vec3(positions[1]), glmColor, uvTopRight});    // Vértice 2: Top-right
        m_vertices.push_back({glm::vec3(positions[0]), glmColor, uvTopLeft});
    }
} // namespace Fusion