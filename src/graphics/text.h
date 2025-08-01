
#ifndef TEXT_H
#define TEXT_H

#include <string>
#include "transformable.h"
#include "font.h"

namespace Fusion
{

    class Text : public Transformable
    {

    public:
        friend class Window;
      
        explicit Text(const Font &font);

        void SetFont(const Font &font);

        void SetText(std::string text);


    private:
        const Font *m_Font;
        std::string m_Text;
        Color m_Color;
    };



}

#endif