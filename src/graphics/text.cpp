#include "text.h"


namespace Fusion
{
    Text::Text(const Font &font)
        : m_Color(Color{1.0f,1.0f,1.0f,1.0f})
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
    }
}