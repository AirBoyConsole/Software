#include "draw_shapes.hpp"

namespace arb 
{
    ShapeRect::ShapeRect(Vector2i size, uint16_t color)
    {
        this->size = size;
        this->color = color;
    }

    Vector2i ShapeRect::get_size() const
    {
        return size;
    }

    uint16_t ShapeRect::get_color() const
    {
        return color;
    }

    ShapeBitmap::ShapeBitmap(Vector2i size, const uint16_t* pixels)
    {
        this->size = size;
        this->pixels = pixels;
    }

    Vector2i ShapeBitmap::get_size() const
    {
        return size;
    }

    const uint16_t* ShapeBitmap::get_pixels() const
    {
        return pixels;
    }
}