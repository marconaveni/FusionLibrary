#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

#include <vector>

#include "fusion_math.h"
#include "vertex.h"

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

        Rectangle GetSource() const { return m_source; }
        Vector2f GetOrigin() const { return m_origin; }
        float GetRotation() const { return m_rotation; }
        Vector2f GetPosition() const;
        Sizef GetSize() const;
        virtual const std::vector<Vertex>& GetVertices() const;

    protected:

        Rectangle m_source;
        Rectangle m_position;
        Vector2f m_origin;
        float m_rotation;

        virtual void UpdateGeometry() const = 0;
        mutable bool m_isNeedTransform = true; // A flag de cache!
        mutable std::vector<Vertex> m_vertices;
    };

} // namespace Fusion

#endif