#ifndef GRECT_HPP
#define GRECT_HPP

struct GRect
{
    int X;
    int Y;
    int Width;
    int Height;

    GRect() = default;
    GRect(int x, int y, int width, int height) : X(x), Y(y), Width(width), Height(height) {}
};

#endif // GRECT_HPP