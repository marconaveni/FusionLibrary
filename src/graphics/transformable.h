#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

#include "fusion_math.h"

namespace Fusion
{

    class Transformable
    {
    public:
        Transformable();
        ~Transformable() = default;
        void SetSource(Rectangle source);
        void SetPosition(Vector2f position);
        void SetPosition(float x, float y);
        void SetSize(Sizef size);
        void SetSize(float width, float height);
        void SetOrigin(Vector2f origin);
        void SetRotation(float rotation);

        Rectangle GetSource() const { return m_Source; }
        Vector2f GetOrigin() const { return m_Origin; }
        float GetRotation() const { return m_Rotation; }
        Vector2f GetPosition() const;
        Sizef GetSize() const;

    protected:
        Rectangle m_Source;
        Rectangle m_Position;
        Vector2f m_Origin;
        float m_Rotation;
    };

} // namespace Fusion

#endif