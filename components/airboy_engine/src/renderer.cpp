#include "renderer.hpp"
#include "text.hpp"

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
        if ((pos.x < 0) || (pos.y < 0) || (size.x > this->display_size.x) || (size.y > this->display_size.y))
			return;

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


    void Renderer::draw_text(Vector2i pos, int scale, uint16_t color, const char *str)
    {
        const int fontw = 6,
            fonth = 8;

        if (pos.y + fonth * scale > this->display_size.y || pos.y < 0 || pos.x < 0 || scale < 1)
            return;

        int len = strlen(str),
            index = 0;

        for (int i = 0; i < len; i++) {
            if (pos.x + i * fontw * scale > this->display_size.x)
                return;

            if (str[i] < ' ')
                return;

            index = str[i] - ' ';

            for (int ty = 0; ty < fonth * scale; ty++) {
                for (int tx = 0; tx < fontw * scale; tx++) {
                    if (defaultFont[index][fontw * (int)(ty/scale) + (int)(tx/scale)] == 1)
						this->display->set_pixel_fast(pos.x + i * fontw * scale + tx, pos.y + ty, color);
                }
			}
		}
	}


	void Renderer::draw_line(Vector2i point1, Vector2i point2, uint16_t color)
	{
        if ((point1.x < 0) || (point1.y < 0) || (point1.x > this->display_size.x) || (point1.y > this->display_size.y))
			return;

        if ((point2.x < 0) || (point2.y < 0) || (point2.x > this->display_size.x) || (point2.y > this->display_size.y))
			return;

		Vector2i delta = Vector2i(point2.x - point1.x, point2.y - point1.y);

		int steps = abs(delta.x) > abs(delta.y) ? 
			abs(delta.x) : 
			abs(delta.y);

		Vector2f increment = Vector2f(delta.x / (float) steps, delta.y / (float) steps),
				 pixel = Vector2f(point1.x, point1.y);

		for (int i = 0; i < steps; i++) {
			this->display->set_pixel_fast(roundf(pixel.x), roundf(pixel.y), color);
			pixel.x += increment.x;
			pixel.y += increment.y;
		}

		this->display->set_pixel_fast(roundf(pixel.x), roundf(pixel.y), color);
	}
}
