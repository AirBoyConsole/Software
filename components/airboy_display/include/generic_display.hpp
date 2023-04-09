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
}bus_cfg_t;

class GenericDisplay {
    public:
        GenericDisplay(int height, int width);
        ~GenericDisplay();

        static bool lcd_trans_done_cb(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx);

        void init_framebuffer();
        void init_backlight(gpio_num_t bl, uint32_t duty);
        void set_backlight_level(uint8_t value);

        virtual void init_bus(bus_cfg_t *config) = 0;
        virtual void init_lcd() = 0;
        virtual void drawFrame() = 0;

        int height;
        int width;
        uint8_t bl_level;
        gpio_num_t reset_pin;
        esp_lcd_panel_io_handle_t io;
        uint16_t *a_buffer = nullptr;
        uint16_t *b_buffer = nullptr;
        uint16_t *current_buffer = nullptr;
};

#ifdef __cplusplus
}
#endif
}

