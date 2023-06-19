#include "renderer.hpp"
#include "text.hpp"

namespace airboy 
{
    Renderer::Renderer(Display &display, Vector2i camera_size) 
    : display(display)
    {
        //this->display = display;
        this->camera = new Camera(camera_size, false, 0.15);
    }

    Renderer::~Renderer() {}

    std::pair<int, int> Renderer::object_area_visible(int pos, int size, int cam_pos, int cam_size)
    {
        int start, lenght;
        start = std::max(pos, cam_pos);
        lenght = std::min((size + pos), (cam_size + cam_pos));
        lenght -= start;
        start -= cam_pos;

        return std::make_pair(start, lenght);
    }

    std::pair<Vector2i, Vector2i> Renderer::object_area_visible(Vector2i pos, Vector2i size, Vector2i cam_pos, Vector2i cam_size)
    {
        Vector2i out_pos, out_size;

        out_pos.x = std::max(pos.x, cam_pos.x);
        out_size.x = std::min((size.x + pos.x), (cam_size.x + cam_pos.x));
        out_size.x -= out_pos.x;
        out_pos.x -= cam_pos.x;

        out_pos.y = std::max(pos.y, cam_pos.y);
        out_size.y = std::min((size.y + pos.y), (cam_size.y + cam_pos.y));
        out_size.y -= out_pos.y;
        out_pos.y -= cam_pos.y;

        return std::make_pair(out_pos, out_size);
    }

    void Renderer::line_high(Vector2i start, Vector2i end, uint16_t color)
    {
        Vector2i display_size = display.get_display_size();
        int dx, dy, xi, D, x;

        dx = end.x - start.x;
        dy = end.y - start.y;
        xi = 1;
        if (dx < 0) 
        {
            xi = -1;
            dx = -dx;
        }

        D = (2 * dx) - dy;
        x = start.x;

        for (int y = start.y; y <= end.y; y++) 
        {
            #ifndef ENGINE_SPECIAL_OPTIMALIZATION
                this->display.set_pixel_fast(x, y, color);
            #else
                this->display.frame_buffer[display_size.x * y + x] = color;
            #endif

            if ( D > 0 ) 
            {
                x = x + xi;
                D = D + (2 * (dx - dy));
            } else {
                D = D + 2 * dx;
            }
        }
    }

    void Renderer::line_low(Vector2i start, Vector2i end, uint16_t color)
    {
        Vector2i display_size = display.get_display_size();
        int dx, dy, yi, D, y;

        dx = end.x - start.x;
        dy = end.y - start.y;
        yi = 1;
        if (dy < 0) 
        {
            yi = -1;
            dy = -dy;
        }

        D = (2 * dy) - dx;
        y = start.y;

        for (int x = start.x; x <= end.x; x++) 
        {
            #ifndef ENGINE_SPECIAL_OPTIMALIZATION
                this->display.set_pixel_fast(x, y, color);
            #else
                this->display.frame_buffer[display_size.x * y + x] = color;
            #endif

            if ( D > 0 ) 
            {
                y = y + yi;
                D = D + (2 * (dy - dx));
            } else {
                D = D + 2 * dy;
            }
        }
    }


    void Renderer::draw_fill_rect(Vector2i pos, Vector2i size, uint16_t color)
    {
        Vector2i display_size = display.get_display_size();

        if ((pos.x < 0) || (pos.y < 0) || (pos.x + size.x >= display_size.x) || (pos.y + size.y >= display_size.y)) return;

        for (int row = pos.y; row < pos.y + size.y; row++)
            for (int col = pos.x; col < pos.x + size.x; col++)
        
                #ifndef ENGINE_SPECIAL_OPTIMALIZATION
                    this->display.set_pixel_fast(col, row, color);
                #else
                    this->display.frame_buffer[display_size.x * row + col] = color;
                #endif
    }

    void Renderer::draw_fill_rect_camera(Vector2i pos, Vector2i size, uint16_t color)
    {
        #ifdef ENGINE_SPECIAL_OPTIMALIZATION
            Vector2i display_size = display.get_display_size();
        #endif

        std::pair<Vector2i, Vector2i> axis = object_area_visible(pos, size, camera->position , camera->size);
        if ((axis.second.x <= 0) || ((axis.second.y <= 0))) return;

        for (int row = axis.first.y; row < axis.first.y + axis.second.y; row++)
            for (int col = axis.first.x; col < axis.first.x + axis.second.x; col++)
                #ifndef ENGINE_SPECIAL_OPTIMALIZATION
                    this->display.set_pixel_fast(col, row, color);
                #else
                    this->display.frame_buffer[display_size.x * row + col] = color;
                #endif
    }

    void Renderer::draw_bitmap(Vector2i pos, Vector2i size, const uint16_t* bitmap)
    {
        Vector2i display_size = display.get_display_size();

        if ((pos.x < 0) || (pos.y < 0) || (pos.x + size.x >= display_size.x) || (pos.y + size.y >= display_size.y)) return;

        int offset = 0;
        for (int row = pos.y; row < pos.y + size.y; row++)
            for (int col = pos.x; col < pos.x + size.x; col++)
            {
                if (bitmap[offset] == 0xFFFF)
                {
                    offset++;
                    continue;
                }

                #ifndef ENGINE_SPECIAL_OPTIMALIZATION
                    this->display.set_pixel_fast(col, row, bitmap[offset]);
                #else
                    this->display.frame_buffer[display_size.x * col + row] = bitmap[offset];
                #endif
                    
                offset++;
            }
    }

    /*void Renderer::draw_bitmap(Vector2i pos, Vector2i size, int scale, const uint16_t* bitmap)
    {
        Vector2i display_size = display.get_display_size();

        if ((pos.x < 0) || (pos.y < 0) || (pos.x + (size.x * scale) >= display_size.x) || (pos.y + (size.y * scale) >= display_size.y)) return;

        int offset = 0;
        for (int row = pos.y; row < pos.y + size.y * scale; row += scale)
        {
            for(int j = 0, temp_offset = offset; j < scale; j++, offset = temp_offset)
            {
                for (int col = pos.x; col < pos.x + size.x * scale; col += scale)
                {
                    if (bitmap[offset] == 0xFFFF)
                    {
                        offset++;
                        continue;
                    }

                    for (int i = 0; i < scale; i++)
                        #ifndef ENGINE_SPECIAL_OPTIMALIZATION
                            this->display.set_pixel_fast(col + i, row + j, bitmap[offset]);
                        #else
                            this->display.frame_buffer[display_size.x * (col + i) + row + j] = bitmap[offset];
                        #endif
                        
                    offset++;
                }
            }
        }
            
                
    }*/

    void Renderer::draw_bitmap_camera(Vector2i pos, Vector2i size, const uint16_t* bitmap)
    {
        #ifdef ENGINE_SPECIAL_OPTIMALIZATION
            Vector2i display_size = display.get_display_size();
        #endif

        std::pair<Vector2i, Vector2i> axis = object_area_visible(pos, size, camera->position , camera->size);
        if ((axis.second.x <= 0) || ((axis.second.y <= 0))) return;

        int x, y, offset;
        x = size.x - axis.second.x;
        y = size.y - axis.second.y;

        if (axis.first.y == 0) offset = size.x * y;
        else offset = 0;
        
        for (int row = axis.first.y; row < axis.first.y + axis.second.y; row++)
        {
            if (axis.first.x == 0) offset += x;

            for (int col = axis.first.x; col < axis.first.x + axis.second.x; col++)
            {
                if (bitmap[offset] == 0xFFFF)
                {
                    offset++;
                    continue;
                }

                #ifndef ENGINE_SPECIAL_OPTIMALIZATION
                    this->display.set_pixel_fast(col, row, bitmap[offset]);
                #else
                    this->display.frame_buffer[display_size.x * col + row] = bitmap[offset];
                #endif
                    
                offset++;
            }

            if (axis.first.x != 0) offset += x;
        }
    }

    void Renderer::draw_vline(Vector2i pos, int lenght, uint16_t color)
    {
        Vector2i display_size = display.get_display_size();

        if ((pos.x < 0) || (pos.y < 0) || (pos.y + lenght >= display_size.y)) return;

        for (int pixel = pos.y; pixel < pos.y + lenght; pixel++)
            #ifndef ENGINE_SPECIAL_OPTIMALIZATION
                this->display.set_pixel_fast(pos.x, pixel, color);
            #else
                this->display.frame_buffer[display_size.x * pos.x + pixel] = color;
            #endif
    } 

    void Renderer::draw_hline(Vector2i pos, int lenght, uint16_t color)
    {
        Vector2i display_size = display.get_display_size();

        if ((pos.x < 0) || (pos.y < 0) || (pos.x + lenght >= display_size.x)) return;

        for (int pixel = pos.x; pixel < pos.x + lenght; pixel++)
            #ifndef ENGINE_SPECIAL_OPTIMALIZATION
                this->display.set_pixel_fast(pixel, pos.y, color);
            #else
                this->display.frame_buffer[display_size.x * pixel + pos.y] = color;
            #endif
    }

    void Renderer::draw_line(Vector2i start, Vector2i end, uint16_t color)
    {
        Vector2i display_size = display.get_display_size();

        if ((start.x < 0) || (start.y < 0) || (start.x >= display_size.x) || (start.y >= display_size.y)) return;
        if ((end.x < 0) || (end.y < 0) || (end.x >= display_size.x) || (end.y >= display_size.y)) return;

        if (abs(end.y - start.y) < abs(end.x - start.x))
            if (start.x > end.x)
                line_low(end, start, color);
            else
                line_low(start, end, color);
        else
            if (start.y > end.y)
                line_high(end, start, color);
            else
                line_high(start, end, color);
    }

    void Renderer::draw_vline_camera(Vector2i pos, int lenght, uint16_t color)
    {

    }

    void Renderer::draw_hline_camera(Vector2i pos, int lenght, uint16_t color)
    {

    }

    void Renderer::draw_line_camera(Vector2i start, Vector2i end, uint16_t color)
    {

    }

    void Renderer::draw_text(Vector2i pos, int scale, uint16_t color, const char *str)
    {
        Vector2i display_size = display.get_display_size();

        const int fontw = 6,
            fonth = 8;

        if (pos.y + fonth * scale > display_size.y || pos.y < 0 || pos.x < 0 || scale < 1)
            return;

        int len = strlen(str),
            index = 0;

        for (int i = 0; i < len; i++) 
        { 
            if (pos.x + i * fontw * scale > display_size.x) return;
            if (str[i] < ' ') return;

            index = str[i] - ' ';

            for (int ty = 0; ty < fonth * scale; ty++) 
            {
                for (int tx = 0; tx < fontw * scale; tx++) 
                {
                    if (defaultFont[index][fontw * (int)(ty/scale) + (int)(tx/scale)] == 1)
						display.set_pixel_fast(pos.x + i * fontw * scale + tx, pos.y + ty, color);
                }
			}
		}
	}
}
