#pragma once

#include <iostream>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_commands.h"

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_attr.h"

#include "vector2.hpp"

namespace airboy {
    
#define LCD_HOST SPI2_HOST
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL            LEDC_CHANNEL_0

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
    uint8_t bus_bits[8];
    uint8_t CLK;
    uint8_t DC;
    uint8_t CS;
    uint8_t RST;
    uint8_t RD;
    uint8_t BL;
    int clock;
    uint8_t backlight;
}display_bus_cfg_t;

class Display {
    public:
        Display(int height, int width);
        ~Display();

        virtual void draw_frame() = 0;

        void set_backlight_level(uint8_t value);
        void clear_buffer(uint16_t color);
        void clear_buffer();
        void clear_rect(int x, int y, int w, int h, uint16_t color);
        void set_pixel(int x, int y, uint16_t color);
        void set_pixel_fast(int x, int y, uint16_t color);
        Vector2i get_display_size();

#ifdef ENGINE_SPECIAL_OPTIMALIZATION
        uint16_t *frame_buffer = nullptr;
#endif
        
    protected:
        virtual void init_bus(display_bus_cfg_t *config) = 0;
        virtual void init_lcd(gpio_num_t reset) = 0;

        static bool lcd_trans_done_cb(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx);

        void init_framebuffer();
        void init_backlight(gpio_num_t bl, uint32_t duty);
        
        static TaskHandle_t display_handle;
        esp_lcd_panel_io_handle_t io;
        Vector2i display_size;

#ifndef ENGINE_SPECIAL_OPTIMALIZATION
        uint16_t *frame_buffer = nullptr;
#endif
};

inline void Display::clear_buffer(uint16_t color)
{
    for (int i = 0; i < 240 * 320; i++) this->frame_buffer[i] = color;
}

inline void Display::clear_buffer()
{
    memset(this->frame_buffer, 0, display_size.y * display_size.x * 2);
}

inline void Display::clear_rect(int x, int y, int w, int h, uint16_t color)
{
    for (int row = y; row < y + h; row++)
		for (int col = x; col < x + w; col++)
			this->frame_buffer[display_size.x * row + col] = color;
}

inline void Display::set_pixel(int x, int y, uint16_t color)
{
    if ((x < 0) || (x > display_size.x) || (y < 0) || (y > display_size.y)) return;
    this->frame_buffer[display_size.x * y + x] = color;
}

inline void Display::set_pixel_fast(int x, int y, uint16_t color)
{
    this->frame_buffer[display_size.x * y + x] = color;
}

#ifdef __cplusplus
}
#endif
}

