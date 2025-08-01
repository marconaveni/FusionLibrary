#include "sprite.h"

namespace Fusion
{


    Sprite::Sprite(const Texture &texture)
        : Transformable()
        , m_Color(Color{1.0f,1.0f,1.0f,1.0f})
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


}