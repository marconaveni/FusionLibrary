
#ifndef TEXT_H
#define TEXT_H

#include <string>
#include "transformable.h"
#include "font.h"

//#include "utf8.h"

namespace Fusion
{
    class Window;

    class Text : public Transformable
    {

    public:
        friend class Window;
      
        explicit Text(const Font &font);

        void SetFont(const Font &font);

        void SetText(std::string text);

        void SetScale(float scale);
        
        void SetSpacing(float spacing);

        Vector2f MeasureText(Window& window);
        
        Color GetColor() const { return m_Color; }
        
        std::string GetText() const { return m_Text; }

        void SetColor(Color color) { m_Color = color; }

        float GetSpacing() const { return m_Spacing; }


    private:
        const Font *m_Font;
        std::string m_Text;
        float m_Spacing;
        Color m_Color;
        bool m_IsNeedMensured;
        Vector2f m_MeasuredText;
    };



}

#endif