#pragma once

#include <iostream>
#include <cstring>
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

class Renderer 
{
public:
    Renderer(Display *display, Vector2i camera_size);
    ~Renderer();

    void draw_fill_rect(Vector2i pos, Vector2i size, uint16_t color);
    void draw_fill_rect_camera(Vector2i pos, Vector2i size, uint16_t color);

protected:
    Camera *camera = nullptr;
    Display *display = nullptr;

    Vector2i display_size;
    

};

#ifdef __cplusplus
}
#endif
}

