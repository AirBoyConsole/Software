#pragma once

#include <iostream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_commands.h"
#include "driver/spi_master.h"

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_check.h"

#include "generic_display.hpp"

namespace airboy {

#define LCD_CMD_BITS    8
#define LCD_PARAM_BITS  8

#ifdef __cplusplus
extern "C" {
#endif

class ILI9341Display: public GenericDisplay 
{
    public:
        ILI9341Display(bus_cfg_t *config);

    private:
        void init_bus(bus_cfg_t *config);
};

#ifdef __cplusplus
}
#endif
}