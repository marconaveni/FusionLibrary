#include "transformable.h"

namespace Fusion
{
    Transformable::Transformable() : m_source(Rectangle{0, 0, 0, 0}), m_position(Rectangle{0, 0, 1, 1}), m_origin(Vector2f{0}), m_rotation(0)
    {
    }

    void Transformable::SetSource(Rectangle source)
    {
        m_source = source;
        m_isNeedTransform = true;
    }

    void Transformable::SetPosition(Vector2f position)
    {
        SetPosition(position.x, position.y);
    }

    void Transformable::SetPosition(float x, float y)
    {
        m_position.x = x;
        m_position.y = y;
        m_isNeedTransform = true;
    }

    void Transformable::SetSize(Sizef size)
    {
        SetSize(size.width, size.height);
    }

    void Transformable::SetSize(float width, float height)
    {
        m_position.width = width;
        m_position.height = height;
        m_isNeedTransform = true;
    }

    void Transformable::SetOrigin(Vector2f origin)
    {
        m_origin = origin;
        m_isNeedTransform = true;
    }

    void Transformable::SetRotation(float rotation)
    {
        m_rotation = rotation;
        m_isNeedTransform = true;
    }

    Vector2f Transformable::GetPosition() const
    {
        return Vector2f(m_position.x, m_position.y);
    }

    Sizef Transformable::GetSize() const
    {
        return Sizef(m_position.width, m_position.height);
    }

    const std::vector<Vertex>& Transformable::GetVertices() const
    {
        if (m_isNeedTransform)
        {
            UpdateGeometry();
            m_isNeedTransform = false;
        }
        return m_vertices;
    }

} // namespace Fusion
