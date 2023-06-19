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

#include "input.hpp"
#include "mcp23008_input.hpp"
#include "display.hpp"
#include "ili9341_display.hpp"
#include "internal_bus.hpp"
#include "internal_eeprom.hpp"
#include "renderer.hpp"

namespace airboy {

#define ENGINE_TAG "Engine"

#ifdef __cplusplus
extern "C" {
#endif

class Engine 
{
public:
    Engine();
    ~Engine();

    virtual void setup() = 0;
    virtual void update(float delta) = 0;
    bool construct(int fps_target = 0);
    void run();

    void set_fps_target(int target);

protected:
    InternalEEprom *eeprom = nullptr;
    Input *input = nullptr;
    Display *display = nullptr;
    Renderer *renderer = nullptr;
    

private:
    void menu();

    InternalBus *internal_bus = nullptr;

    TickType_t fps_target;
    TickType_t fps_target_last_time;

};

#ifdef __cplusplus
}
#endif
}

/*void Renderer::darken_buffer(int darken_value)
    {
        for (int row = 0; row < 320; row++)
            for (int col = 0; col < 240; col++)
                #ifndef ENGINE_SPECIAL_OPTIMALIZATION
                    this->display.set_pixel_fast(row, col, (display.get_pixel(row, col)));
                #else
                    this->display.frame_buffer[display_size.x * pixel + pos.y] = color;
                #endif
                this->frame_buffer[i] = color;
    }*/
