#include "text.h"

#include <cmath>

#include "fusion_utf8.h"

namespace Fusion
{
    Text::Text()
        : m_spacing(0), m_color(Color{1.0f, 1.0f, 1.0f, 1.0f}), m_isNeedMensured(true), m_measuredText({0.0f, 0.0f})
    {
    }

    Text::Text(const Font& font)
        : m_spacing(0), m_color(Color{1.0f, 1.0f, 1.0f, 1.0f}), m_isNeedMensured(true), m_measuredText({0.0f, 0.0f})
    {
        SetFont(font);
    }

    void Text::SetFont(const Font& font)
    {
        m_font = &font;
        m_isNeedMensured = true;
        m_isNeedTransform = true;
    }

    void Text::SetText(std::string text)
    {
        m_text = text;
        m_isNeedMensured = true;
        m_isNeedTransform = true;
    }
    void Text::SetScale(float scale)
    {
        m_position.width = scale;
        m_position.height = scale;
        m_isNeedMensured = true;
        m_isNeedTransform = true;
    }
    void Text::SetSpacing(float spacing)
    {
        m_spacing = spacing;
        m_isNeedMensured = true;
        m_isNeedTransform = true;
    }
    Vector2f Text::MeasureText()
    {
        if (!m_isNeedMensured)
        {
            return m_measuredText;
        }

        m_isNeedMensured = false;

        if (m_text.empty() || m_font == nullptr)
        {
            m_measuredText = {0.0f, 0.0f};
            return m_measuredText;
        }

        float maxWidth = 0.0f;                       // Armazena a largura da linha mais longa
        float currentLineWidth = 0.0f;               // Largura da linha que estamos medindo
        float totalHeight = m_font->GetLineHeight(); // Começa com a altura de uma linha

        size_t i = 0;
        while (i < m_text.length())
        {
            int32_t codepoint = Fusion::Utf8::DecodeNext(m_text, i);
            if (codepoint == 0)
            {
                break;
            }

            // Se encontramos uma quebra de linha
            if (codepoint == '\n')
            {
                // Remove o espaçamento extra do final da última linha
                if (currentLineWidth > 0)
                {
                    currentLineWidth -= m_spacing;
                }
                // Compara a largura da linha que acabamos de medir com a máxima
                maxWidth = std::max(maxWidth, currentLineWidth);
                currentLineWidth = 0.0f;                // Reseta a largura para a próxima linha
                totalHeight += m_font->GetLineHeight(); // Adiciona a altura de mais uma linha
                continue;
            }

            auto itGlyph = m_font->GetCharData().find(codepoint);
            if (itGlyph == m_font->GetCharData().end())
            {
                codepoint = '?';
                itGlyph = m_font->GetCharData().find(codepoint);
                if (itGlyph == m_font->GetCharData().end())
                    continue;
            }

            stbtt_packedchar pc = itGlyph->second;

            // Adiciona o avanço do caractere e o espaçamento à largura da linha atual
            currentLineWidth += pc.xadvance + m_spacing;
        }

        if (currentLineWidth > 0)
        {
            currentLineWidth -= m_spacing; // remove o ultimo espaçamento
        }
        // Após o loop, faz uma última verificação para a última linha do texto
        maxWidth = std::max(maxWidth, currentLineWidth);

        // Aplica a escala final ao resultado
        const float scale = GetScale();
        m_measuredText.x = maxWidth * scale;
        m_measuredText.y = totalHeight * scale;

        return m_measuredText;
    }

    void Text::UpdateGeometry() const
    {
        m_vertices.clear(); // Limpa o cache antigo

        if (!m_font)
        {
            return;
        }
        

        glm::vec3 pivot = glm::vec3(m_position.x + m_origin.x, m_position.y + m_origin.y, 0.0f);

        // 2. Cria a matriz de rotação que gira em torno do pivô
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, pivot);
        transform = glm::rotate(transform, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::translate(transform, -pivot);

        // Converte a cor da engine para glm::vec4
        glm::vec4 glmColor = {m_color.r, m_color.g, m_color.b, m_color.a};

        float xpos = m_position.x;
        float ypos = m_position.y + m_font->GetTopToBaseline();

        size_t i = 0;
        while (i < m_text.length())
        {

            int32_t codepoint = Fusion::Utf8::DecodeNext(m_text, i);
            if (codepoint == 0)
            {
                break;
            }

            if (codepoint == '\n')
            {
                ypos += m_font->GetLineHeight(); // Avança para a próxima linha
                xpos = m_position.x;             // Reseta para o início da linha
                continue;                        // Pula para a próxima iteração
            }

            auto itGlyph = m_font->GetCharData().find(codepoint);
            if (itGlyph == m_font->GetCharData().end())
            {
                codepoint = '?';
                itGlyph = m_font->GetCharData().find(codepoint);
                if (itGlyph == m_font->GetCharData().end())
                {
                    continue;
                }
            }

            stbtt_packedchar pc = itGlyph->second; // Atribuição direta!

            stbtt_aligned_quad q;
            float temp_x = xpos;
            float temp_y = ypos;
            stbtt_GetPackedQuad(&pc, m_font->GetAtlasSize().width, m_font->GetAtlasSize().height, 0, &temp_x, &temp_y,
                                &q, 0);

            xpos = temp_x;
            xpos += m_spacing; // gera espaçamento adicionar parametro depois

            glm::vec4 p1 = {q.x0, q.y0, 0.0f, 1.0f}; // Top-left
            glm::vec4 p2 = {q.x1, q.y0, 0.0f, 1.0f}; // Top-right
            glm::vec4 p3 = {q.x1, q.y1, 0.0f, 1.0f}; // Bottom-right
            glm::vec4 p4 = {q.x0, q.y1, 0.0f, 1.0f}; // Bottom-left

            const float scale = GetScale();
            glm::vec3 scale_pivot = glm::vec3(m_position.x, m_position.y, 0.0f);
            p1 = glm::translate(glm::mat4(1.0f), scale_pivot) * glm::scale(glm::mat4(1.0f), {scale, scale, 1.0f}) *
                 glm::translate(glm::mat4(1.0f), -scale_pivot) * p1;
            p2 = glm::translate(glm::mat4(1.0f), scale_pivot) * glm::scale(glm::mat4(1.0f), {scale, scale, 1.0f}) *
                 glm::translate(glm::mat4(1.0f), -scale_pivot) * p2;
            p3 = glm::translate(glm::mat4(1.0f), scale_pivot) * glm::scale(glm::mat4(1.0f), {scale, scale, 1.0f}) *
                 glm::translate(glm::mat4(1.0f), -scale_pivot) * p3;
            p4 = glm::translate(glm::mat4(1.0f), scale_pivot) * glm::scale(glm::mat4(1.0f), {scale, scale, 1.0f}) *
                 glm::translate(glm::mat4(1.0f), -scale_pivot) * p4;

            p1 = transform * p1;
            p2 = transform * p2;
            p3 = transform * p3;
            p4 = transform * p4;

            glm::vec2 uv1 = {q.s0, q.t0}; // UV Top-left
            glm::vec2 uv2 = {q.s1, q.t0}; // UV Top-right
            glm::vec2 uv3 = {q.s1, q.t1}; // UV Bottom-right
            glm::vec2 uv4 = {q.s0, q.t1}; // UV Bottom-left

            m_vertices.push_back({glm::vec3(p4), glmColor, uv4}); // Vértice 0: Bottom-left
            m_vertices.push_back({glm::vec3(p3), glmColor, uv3}); // Vértice 1: Bottom-right
            m_vertices.push_back({glm::vec3(p2), glmColor, uv2}); // Vértice 2: Top-right
            m_vertices.push_back({glm::vec3(p1), glmColor, uv1}); // Vértice 3: Top-left
        }
    }

    float Text::GetScale() const
    {
        return std::max(m_position.width, m_position.height);
    }
} // namespace Fusion