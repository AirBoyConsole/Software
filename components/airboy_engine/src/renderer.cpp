#include "renderer.hpp"

namespace airboy 
{
    Renderer::Renderer(Display *display, Vector2i camera_size) 
    {
        this->display = display;
        this->camera = new Camera(camera_size);
        this->display_size = display->get_display_size();
    }

    Renderer::~Renderer() {}

    void Renderer::draw_fill_rect(Vector2i pos, Vector2i size, uint16_t color)
    {
        if ((pos.x < 0) || (pos.y < 0) || (size.x > this->display_size.x) || (size.y > this->display_size.y)) return;

        for (int row = pos.y; row < pos.y + size.y; row++)
            for (int col = pos.x; col < pos.x + size.x; col++)
                this->display->set_pixel_fast(col, row, color);
    }

    void Renderer::draw_fill_rect_camera(Vector2i pos, Vector2i size, uint16_t color)
    {
        /*int x, y, h, w;

        x = max(r->x, viewport->x);
        w = min((r->w + r->x), (viewport->w + viewport->x));
        if (x > w) return;
        w -= x;
            
        y = max(r->y, viewport->y);
        h = min((r->h + r->y), (viewport->h + viewport->y));
        if (y > h) return;
        h -= y;

        x -= viewport->x;
        y -= viewport->y;

        for (int row = y; row < y + h; row++)
            for (int col = x; col < x + w; col++)
                set_pixel(col, row, r->color);*/
    }
    
}
