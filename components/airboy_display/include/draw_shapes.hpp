#pragma once

#include "vector2.hpp"

namespace arb {

#ifdef __cplusplus
extern "C" {
#endif

class ShapeRect 
{
public:
    ShapeRect(Vector2i size, uint16_t color);

    Vector2i get_size() const;
    uint16_t get_color() const;

private:
    Vector2i size;
    uint16_t color;
};


class ShapeBitmap
{
public:
    ShapeBitmap(Vector2i size, const uint16_t *pixels);

    Vector2i get_size() const;
    const uint16_t* get_pixels() const;
private:
    Vector2i size;
    const uint16_t *pixels;
};



#ifdef __cplusplus
}
#endif
}
