#include "text.h"
#include "fusion_utf8.h"
#include <cmath>

namespace Fusion
{
    Text::Text(const Font &font)
        : m_Spacing(0), m_Color(Color{1.0f, 1.0f, 1.0f, 1.0f}), m_IsNeedMensured(true), m_MeasuredText(0)
    {
        SetFont(font);
    }

    void Text::SetFont(const Font &font)
    {
        m_Font = &font;
        m_IsNeedMensured = true;
        m_IsNeedTransform = true;
    }

    void Text::SetText(std::string text)
    {
        m_Text = text;
        m_IsNeedMensured = true;
        m_IsNeedTransform = true;
    }
    void Text::SetScale(float scale)
    {
        m_Position.width = scale;
        m_Position.height = scale;
        m_IsNeedMensured = true;
        m_IsNeedTransform = true;
    }
    void Text::SetSpacing(float spacing)
    {
        m_Spacing = spacing;
        m_IsNeedMensured = true;
        m_IsNeedTransform = true;
    }
    Vector2f Text::MeasureText()
    {
        if (m_IsNeedMensured)
        {
            m_IsNeedMensured = false;

            if (m_Text.empty())
            {
                return Vector2f{0};
            }

            float xpos = 0.0f;
            float ypos = 0.0f;

            // Variáveis para rastrear a caixa delimitadora vertical do texto
            float minY = 0.0f, maxY = 0.0f;

            size_t i = 0;
            const float scale = GetScale();
            while (i < m_Text.length())
            {
                int32_t codepoint = Fusion::Utf8::DecodeNext(m_Text, i);

                if (codepoint == 0)
                {
                    break;
                }

                auto itGlyph = m_Font->GetCharData().find(codepoint);
                if (itGlyph == m_Font->GetCharData().end())
                {
                    codepoint = '?';
                    itGlyph = m_Font->GetCharData().find(codepoint);
                    if (itGlyph == m_Font->GetCharData().end())
                    {
                        continue;
                    }
                }

                stbtt_packedchar pc = itGlyph->second;
                stbtt_aligned_quad q;

                // Esta função calcula a posição do caractere (q) e atualiza o xpos com o avanço.
                // É a mesma função usada para renderizar, mas aqui só nos importam as métricas.
                stbtt_GetPackedQuad(&pc, m_Font->GetAtlasSize().width, m_Font->GetAtlasSize().height, 0, &xpos, &ypos, &q, 0);

                xpos += m_Spacing;

                // Atualiza a altura máxima baseada no caractere mais alto e mais baixo
                minY = std::min(minY, q.y0 * scale);
                maxY = std::max(maxY, q.y1 * scale);
            }

            xpos -= m_Spacing; // Remove o espaçamento final

            m_MeasuredText.x = xpos * scale;  // A largura final é a posição final de x
            m_MeasuredText.y = (maxY - minY); // A altura é a diferença entre o ponto mais alto e o mais baixo
        }

        return m_MeasuredText;
    }

    void Text::UpdateGeometry() const
    {
        m_Vertices.clear(); // Limpa o cache antigo

        glm::vec3 pivot = glm::vec3(m_Position.x + m_Origin.x, m_Position.y + m_Origin.y, 0.0f);

        // 2. Cria a matriz de rotação que gira em torno do pivô
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, pivot);
        transform = glm::rotate(transform, glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::translate(transform, -pivot);

        // Converte a cor da engine para glm::vec4
        glm::vec4 glmColor = {m_Color.r, m_Color.g, m_Color.b, m_Color.a};

        float xpos = m_Position.x;
        float ypos = m_Position.y + m_Font->GetTopToBaseline();

        float x = m_Font->GetTopToBaseline();

        size_t i = 0;
        while (i < m_Text.length())
        {

            int32_t codepoint = Fusion::Utf8::DecodeNext(m_Text, i);
            if (codepoint == 0)
            {
                break;
            }

            auto itGlyph = m_Font->GetCharData().find(codepoint);
            if (itGlyph == m_Font->GetCharData().end())
            {
                codepoint = '?';
                itGlyph = m_Font->GetCharData().find(codepoint);
                if (itGlyph == m_Font->GetCharData().end())
                {
                    continue;
                }
            }

            stbtt_packedchar pc = itGlyph->second; // Atribuição direta!

            stbtt_aligned_quad q;
            stbtt_GetPackedQuad(&pc, m_Font->GetAtlasSize().width, m_Font->GetAtlasSize().height, 0, &xpos, &ypos, &q, 0);

            xpos += m_Spacing; // gera espaçamento adicionar parametro depois

            glm::vec4 p1 = {q.x0, q.y0, 0.0f, 1.0f}; // Top-left
            glm::vec4 p2 = {q.x1, q.y0, 0.0f, 1.0f}; // Top-right
            glm::vec4 p3 = {q.x1, q.y1, 0.0f, 1.0f}; // Bottom-right
            glm::vec4 p4 = {q.x0, q.y1, 0.0f, 1.0f}; // Bottom-left

            const float scale = GetScale();
            glm::vec3 scale_pivot = glm::vec3(m_Position.x, m_Position.y, 0.0f);
            p1 = glm::translate(glm::mat4(1.0f), scale_pivot) * glm::scale(glm::mat4(1.0f), {scale, scale, 1.0f}) * glm::translate(glm::mat4(1.0f), -scale_pivot) * p1;
            p2 = glm::translate(glm::mat4(1.0f), scale_pivot) * glm::scale(glm::mat4(1.0f), {scale, scale, 1.0f}) * glm::translate(glm::mat4(1.0f), -scale_pivot) * p2;
            p3 = glm::translate(glm::mat4(1.0f), scale_pivot) * glm::scale(glm::mat4(1.0f), {scale, scale, 1.0f}) * glm::translate(glm::mat4(1.0f), -scale_pivot) * p3;
            p4 = glm::translate(glm::mat4(1.0f), scale_pivot) * glm::scale(glm::mat4(1.0f), {scale, scale, 1.0f}) * glm::translate(glm::mat4(1.0f), -scale_pivot) * p4;

            p1 = transform * p1;
            p2 = transform * p2;
            p3 = transform * p3;
            p4 = transform * p4;

            glm::vec2 uv1 = {q.s0, q.t0}; // UV Top-left
            glm::vec2 uv2 = {q.s1, q.t0}; // UV Top-right
            glm::vec2 uv3 = {q.s1, q.t1}; // UV Bottom-right
            glm::vec2 uv4 = {q.s0, q.t1}; // UV Bottom-left

            m_Vertices.push_back({glm::vec3(p4), glmColor, uv4}); // Vértice 0: Bottom-left
            m_Vertices.push_back({glm::vec3(p3), glmColor, uv3}); // Vértice 1: Bottom-right
            m_Vertices.push_back({glm::vec3(p2), glmColor, uv2}); // Vértice 2: Top-right
            m_Vertices.push_back({glm::vec3(p1), glmColor, uv1}); // Vértice 3: Top-left
        }
    }

    float Text::GetScale() const
    {
        return std::max(m_Position.width, m_Position.height);
    }
}