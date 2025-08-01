
#ifndef FUSION_MATH_H
#define FUSION_MATH_H


namespace Fusion
{
    template <typename T>
    struct Vector2
    {
        T x;
        T y;
    };
    
    using Vector2f = Vector2<float>;
    using Vector2i = Vector2<int>;
    using Vector2u = Vector2<unsigned int>;

    template <typename T>
    struct Size
    {
        T width;
        T height;
    };

    using Sizef = Size<float>;
    using Sizei = Size<int>;
    using Sizeu = Size<unsigned int>;

    struct Rectangle
    {
        float x;
        float y;
        float width;
        float height;
    };
    
    struct Color
    {
        float r;
        float g;
        float b;
        float a;
    };
}

#endif 
