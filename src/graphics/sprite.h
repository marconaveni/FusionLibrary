
#ifndef SPRITE_H
#define SPRITE_H

#include "transformable.h"
#include "texture.h"
#include "vertex.h"

namespace Fusion
{
    class Sprite : public Transformable
    {
    public:
        friend class Renderer;

        explicit Sprite(const Texture &texture);

        explicit Sprite(const Texture &&texture) = delete;

        void SetTexture(const Texture &texture);

        void SetTexture(const Texture &&texture) = delete;

        const Texture *GetTexture() const { return m_Texture; }

    private:
        virtual void UpdateGeometry() const override; 
        const Texture *m_Texture;
        Color m_Color;
    };

}

#endif