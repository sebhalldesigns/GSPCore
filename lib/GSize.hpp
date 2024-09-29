#ifndef GSIZE_HPP
#define GSIZE_HPP

struct GSize {
    float Width;
    float Height;

    GSize() = default;
    GSize(float width, float height): Width(width), Height(height) { }
};

#endif // GSIZE_HPP