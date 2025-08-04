#include "text.h"
#include "window.h"
#include "fusion_utf8.h"

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
    }

    void Text::SetText(std::string text)
    {
        m_Text = text;
        m_IsNeedMensured = true;
    }
    void Text::SetScale(float scale)
    {
        m_Position.width = scale;
        m_Position.height = scale;
    }
    void Text::SetSpacing(float spacing)
    {
        m_Spacing = spacing;
    }
    Vector2f Text::MeasureText(Window &window)
    {
        if (m_IsNeedMensured)
        {
            m_MeasuredText = window.MeasureText(*this, m_Spacing);
            m_IsNeedMensured = false;
        }
        return m_MeasuredText;
    }

}