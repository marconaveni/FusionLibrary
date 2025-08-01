
#ifndef SPRITE_H
#define SPRITE_H

#include "transformable.h"
#include "texture.h"

namespace Fusion
{
    class Sprite : public Transformable
    {
    public:
        friend class Window;

        explicit Sprite(const Texture &texture);

        explicit Sprite(const Texture &&texture) = delete;

        void SetTexture(const Texture &texture);

        void SetTexture(const Texture &&texture) = delete;

    private:
        const Texture *m_Texture;
        Color m_Color;
    };

}

#endif