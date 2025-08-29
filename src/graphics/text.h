
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
        Color GetColor() const { return m_Color; }
        std::string GetText() const { return m_Text; }
        void SetColor(Color color) { m_Color = color; }
        float GetSpacing() const { return m_Spacing; }
        float GetScale() const;
        const Font* GetFont() const { return m_Font; }

    private:

        virtual void UpdateGeometry() const override;
        const Font* m_Font = nullptr;
        std::string m_Text;
        float m_Spacing;
        Color m_Color;
        bool m_IsNeedMensured;
        Vector2f m_MeasuredText;
    };

} // namespace Fusion

#endif