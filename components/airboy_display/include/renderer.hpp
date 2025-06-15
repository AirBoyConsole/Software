#pragma once

#include "display.hpp"
#include "vector2.hpp"
#include "draw_shapes.hpp"

namespace arb {

#ifdef __cplusplus
extern "C" {
#endif

class Renderer 
{
public:
    Renderer(const Display& display);

    virtual bool begin_draw() = 0;
    virtual void end_draw() = 0;
    virtual void clear(uint16_t color) = 0;
    virtual void draw_rect(ShapeRect& rect, Vector2i pos) = 0;
    virtual void draw_bitmap(ShapeBitmap& bittmap, Vector2i pos) = 0;

protected:
    const Display& display;
};

#ifdef __cplusplus
}
#endif
}
