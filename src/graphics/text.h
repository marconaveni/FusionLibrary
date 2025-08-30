
#ifndef TEXT_H
#define TEXT_H

#include <string>

#include "font.h"
#include "transformable.h"
#include "vertex.h"

namespace Fusion
{

    class Text : public Transformable
    {

    public:

        friend class Window;

        Text();
        explicit Text(const Font& font);
        void SetFont(const Font& font);
        void SetText(std::string text);
        void SetScale(float scale);
        void SetSpacing(float spacing);
        Vector2f MeasureText();
        Color GetColor() const { return m_color; }
        std::string GetText() const { return m_text; }
        void SetColor(Color color) { m_color = color; }
        float GetSpacing() const { return m_spacing; }
        float GetScale() const;
        const Font* GetFont() const { return m_font; }

    private:

        virtual void UpdateGeometry() const override;
        const Font* m_font = nullptr;
        std::string m_text;
        float m_spacing;
        Color m_color;
        bool m_isNeedMensured;
        Vector2f m_measuredText;
    };

} // namespace Fusion

#endif