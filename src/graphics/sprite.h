
#ifndef SPRITE_H
#define SPRITE_H

#include "texture.h"
#include "transformable.h"
#include "vertex.h"

namespace Fusion
{
    class Sprite : public Transformable
    {
    public:

        friend class Renderer;

        Sprite();
        explicit Sprite(const Texture& texture);
        explicit Sprite(const Texture&& texture) = delete;
        void SetTexture(const Texture& texture);
        void SetTexture(const Texture&& texture) = delete;
        const Texture* GetTexture() const { return m_texture; }
        void SetColor(Color color) { m_color = color; }

    private:

        virtual void UpdateGeometry() const override;
        const Texture* m_texture = nullptr;
        Color m_color;
    };

} // namespace Fusion

#endif