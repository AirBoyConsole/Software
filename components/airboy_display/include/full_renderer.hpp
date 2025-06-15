#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "display.hpp"
#include "vector2.hpp"
#include "draw_shapes.hpp"
#include "renderer.hpp"
#include "framebuffer.hpp"

namespace arb {

#ifdef __cplusplus
extern "C" {
#endif

class FullRenderer : public Renderer
{
public:
    FullRenderer(const Display& display);

    bool begin_draw() override;
    void end_draw() override;
    void clear(uint16_t color) override;
    void draw_rect(ShapeRect& rect, Vector2i pos) override;
    void draw_bitmap(ShapeBitmap& bitmap, Vector2i pos) override;
    
private:
    FrameBuffer fbuffer;
};

#ifdef __cplusplus
}
#endif
}
