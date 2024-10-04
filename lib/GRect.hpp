#ifndef GRECT_HPP
#define GRECT_HPP

struct GRect
{
    float X;
    float Y;
    float Width;
    float Height;

    GRect() = default;
    GRect(float x, float y, float width, float height) : X(x), Y(y), Width(width), Height(height) {}
};

#endif // GRECT_HPP