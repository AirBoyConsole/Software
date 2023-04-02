#include <iostream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "generic_display.hpp"
#include "ili9341_display.hpp"

extern "C" void app_main(void)
{
    airboy::bus_cfg_t config{
        .bus_bits = {13, 12, 11, 10, 9, 8, 7, 6},
        .CLK = 16,
        .DC = 17,
        .CS = 18,
        .RST = 14,
        .RD = 15,
        .BL = 2,
        .clock = (40 * 1000 * 1000)
    };

    airboy::GenericDisplay *display = new airboy::ILI9341Display(&config);
}