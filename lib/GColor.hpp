#ifndef GCOLOR_HPP
#define GCOLOR_HPP

struct GColor {
    float Red;
    float Green;
    float Blue;
    float Alpha;

    GColor() = default;
    GColor(float red, float green, float blue, float alpha) : Red(red), Green(green), Blue(blue), Alpha(alpha) {}
};

#endif // GCOLOR_HPP