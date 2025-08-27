#include "transformable.h"

namespace Fusion
{
    Transformable::Transformable() : m_Source(Rectangle{0, 0, 0, 0}), m_Position(Rectangle{0, 0, 1, 1}), m_Origin(Vector2f{0}), m_Rotation(0)
    {
    }

    void Transformable::SetSource(Rectangle source)
    {
        m_Source = source;
        m_IsNeedTransform = true;
    }

    void Transformable::SetPosition(Vector2f position)
    {
        SetPosition(position.x, position.y);
    }

    void Transformable::SetPosition(float x, float y)
    {
        m_Position.x = x;
        m_Position.y = y;
        m_IsNeedTransform = true;
    }

    void Transformable::SetSize(Sizef size)
    {
        SetSize(size.width, size.height);
    }

    void Transformable::SetSize(float width, float height)
    {
        m_Position.width = width;
        m_Position.height = height;
        m_IsNeedTransform = true;
    }

    void Transformable::SetOrigin(Vector2f origin)
    {
        m_Origin = origin;
        m_IsNeedTransform = true;
    }

    void Transformable::SetRotation(float rotation)
    {
        m_Rotation = rotation;
        m_IsNeedTransform = true;
    }

    Vector2f Transformable::GetPosition() const
    {
        return Vector2f(m_Position.x, m_Position.y);
    }

    Sizef Transformable::GetSize() const
    {
        return Sizef(m_Position.width, m_Position.height);
    }

    const std::vector<Vertex>& Transformable::GetVertices() const
    {
        if (m_IsNeedTransform)
        {
            UpdateGeometry();
            m_IsNeedTransform = false;
        }
        return m_Vertices;
    }

} // namespace Fusion
