#pragma once

#include <iostream>
#include <cstring>
#include <utility>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_check.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "display.hpp"
#include "camera.hpp"
#include "vector2.hpp"

namespace airboy {

#define RENDERER_TAG "renderer"

#ifdef __cplusplus
extern "C" {
#endif

// forward declaration
class Camera;

//renderer class
class Renderer 
{
public:
    Renderer(Display &display, Vector2i camera_size);
    ~Renderer();

    void draw_fill_rect(Vector2i pos, Vector2i size, uint16_t color);
    void draw_fill_rect_camera(Vector2i pos, Vector2i size, uint16_t color);
    void draw_bitmap(Vector2i pos, Vector2i size, const uint16_t* bitmap);
    void draw_bitmap_camera(Vector2i pos, Vector2i size, const uint16_t* bitmap);
    void draw_vline(Vector2i pos, int lenght, uint16_t color);
    void draw_hline(Vector2i pos, int lenght, uint16_t color);
    void draw_line(Vector2i start, Vector2i end, uint16_t color);
    void draw_vline_camera(Vector2i pos, int lenght, uint16_t color);
    void draw_hline_camera(Vector2i pos, int lenght, uint16_t color);
    void draw_line_camera(Vector2i start, Vector2i end, uint16_t color);

    Camera *camera = nullptr;

protected:
    std::pair<int, int> object_area_visible(int pos, int size, int cam_pos, int cam_size);
    std::pair<Vector2i, Vector2i> object_area_visible(Vector2i pos, Vector2i size, Vector2i cam_pos, Vector2i cam_size);

    Display &display;

private:
    void line_high(Vector2i start, Vector2i end, uint16_t color);
    void line_low(Vector2i start, Vector2i end, uint16_t color);
};




























#ifdef __cplusplus
}
#endif
}

