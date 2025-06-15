#include "full_renderer.hpp"

#include <algorithm>

namespace arb 
{
    FullRenderer::FullRenderer(const Display& display) : Renderer(display), fbuffer(display.get_size(), Vector2i::ZERO) {}

    bool FullRenderer::begin_draw()
    {
        if (xSemaphoreTake(fbuffer.mutex, pdMS_TO_TICKS(10)) == pdTRUE)
            return true;
        else
            return false;
    }

    void FullRenderer::end_draw() 
    {
        xSemaphoreGive(fbuffer.mutex);

        display.add_buffer_queue(&fbuffer);
    }

    void FullRenderer::clear(uint16_t color)
    {
        std::fill_n(fbuffer.buffer, fbuffer.lenght, color);
    }

    void FullRenderer::draw_rect(ShapeRect& rect, Vector2i pos)
    {
        int x0 = std::max(0, pos.x);
        int y0 = std::max(0, pos.y);
        int x1 = std::min(fbuffer.size.x, pos.x + rect.get_size().x);
        int y1 = std::min(fbuffer.size.y, pos.y + rect.get_size().y);

        if (x0 >= x1 || y0 >= y1) return;

        //This part was generated as optimised previous version
        uint16_t color16 = rect.get_color();
        uint32_t color32 = (color16 << 16) | color16;
        int width = x1 - x0;

        for (int y = y0; y < y1; ++y)
        {
            // get pointer to a row from buffer
            uint16_t* row16 = &fbuffer.buffer[y * fbuffer.size.x + x0];
            int n = width;

            if ((uintptr_t)row16 & 0x3)
            {
                *row16++ = color16;
                n--;
            }

            uint32_t* row32 = (uint32_t*)row16;
            while (n >= 2)
            {
                *row32++ = color32;
                n -= 2;
            }

            row16 = (uint16_t*)row32;
            if (n > 0)
            {
                *row16 = color16;
            }
        }
    }

    void FullRenderer::draw_bitmap(ShapeBitmap& bitmap, Vector2i pos)
    {

    }
}