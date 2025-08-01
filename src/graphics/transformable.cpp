#include "transformable.h"

namespace Fusion
{
    Transformable::Transformable()
        : m_Source(Rectangle{0, 0, 10, 10}), m_Position(Rectangle{0, 0, 10, 10}), m_Origin(Vector2f{0}), m_Rotation(0)
    {
    }

    void Transformable::SetSource(Rectangle source)
    {
        m_Source = source;
    }

    void Transformable::SetPosition(Vector2f position)
    {
        SetPosition(position.x, position.y);
    }

    void Transformable::SetPosition(float x, float y)
    {
        m_Position.x = x;
        m_Position.y = y;
    }

    void Transformable::SetSize(Sizef size)
    {
        SetSize(size.width, size.height);
    }

    void Transformable::SetSize(float width, float height)
    {
        m_Position.width = width;
        m_Position.height = height;
    }

    void Transformable::SetOrigin(Vector2f origin)
    {
        m_Origin = origin;
    }

    void Transformable::SetRotation(float rotation)
    {
        m_Rotation = rotation;
    }

} // namespace Fusion
