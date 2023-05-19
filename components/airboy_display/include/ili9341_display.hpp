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
#include "driver/spi_master.h"

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_check.h"

#include "display.hpp"

namespace airboy {

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t data_bytes; // Length of data in above data array; 0xFF = end of cmds.
} lcd_init_cmd_t;

const lcd_init_cmd_t vendor_specific_init[] = {
    /* SW reset */
    {LCD_CMD_SWRESET, {0}, 0},
    /* 200 ms delay */
    {0x80, {250}, 1},
    /* Power contorl B, power control = 0, DC_ENA = 1 */
    //{0xCF, {0x00, 0xAA, 0XE0}, 3},
    /* Power on sequence control,
     * cp1 keeps 1 frame, 1st frame enable
     * vcl = 0, ddvdh=3, vgh=1, vgl=2
     * DDVDH_ENH=1
     */
    {0xED, {0x67, 0x03, 0X12, 0X81}, 4},
    /* Driver timing control A,
     * non-overlap=default +1
     * EQ=default - 1, CR=default
     * pre-charge=default - 1
     */
    {0xE8, {0x8A, 0x01, 0x78}, 3},
    /* Power control A, Vcore=1.6V, DDVDH=5.6V */
    {0xCB, {0x39, 0x2C, 0x00, 0x34, 0x02}, 5},
    /* Pump ratio control, DDVDH=2xVCl */
    {0xF7, {0x20}, 1},
    /* Driver timing control, all=0 unit */
    {0xEA, {0x00, 0x00}, 2},
    /* Power control 1, GVDD=4.75V */
    {0xC0, {0x23}, 1},
    /* Power control 2, DDVDH=VCl*2, VGH=VCl*7, VGL=-VCl*3 */
    {0xC1, {0x11}, 1},
    /* VCOM control 1, VCOMH=4.025V, VCOML=-0.950V */
    {0xC5, {0x43, 0x4C}, 2},
    /* VCOM control 2, VCOMH=VMH-2, VCOML=VML-2 */
    {0xC7, {0xA0}, 1},
    /* Memory access contorl, MX=MY=0, MV=1, ML=0, BGR=1, MH=0 */
    {0x36, {0xE8}, 1},
    /* Pixel format, 16bits/pixel for RGB/MCU interface */
    {0x3A, {0x55}, 1},  //*** INTERFACE PIXEL FORMAT: 0x66 -> 18 bit; 0x55 -> 16 bit
    /* Frame rate control, f=fosc, 70Hz fps */
    {0xB1, {0x00, 0x1B}, 2},
    /* Enable 3G, disabled */
    {0xF2, {0x00}, 1},
    /* Gamma set, curve 1 */
    {0x26, {0x01}, 1},
    /* Positive gamma correction */
    {0xE0, {0x1F, 0x36, 0x36, 0x3A, 0x0C, 0x05, 0x4F, 0X87, 0x3C, 0x08, 0x11, 0x35, 0x19, 0x13, 0x00}, 15},
    /* Negative gamma correction */
    {0xE1, {0x00, 0x09, 0x09, 0x05, 0x13, 0x0A, 0x30, 0x78, 0x43, 0x07, 0x0E, 0x0A, 0x26, 0x2C, 0x1F}, 15},
    /* Column address set, SC=0, EC=0xEF */
    {LCD_CMD_CASET, {0x00, 0x00, 0x00, 0xEF}, 4},
    /* Page address set, SP=0, EP=0x013F */
    {LCD_CMD_RASET, {0x00, 0x00, 0x01, 0x3f}, 4},
    /* Memory write */
    {LCD_CMD_RAMWR, {0}, 0},
    /* Entry mode set, Low vol detect disabled, normal display */
    {0xB7, {0x07}, 1},
    /* Display function control */
    {0xB6, {0x0A, 0x02}, 2},
    /* Sleep out */
    {LCD_CMD_SLPOUT, {0}, 0},
    /* Display on */
    {LCD_CMD_DISPON, {0}, 0},
    /* Invert colors */
	{LCD_CMD_INVON, {0}, 0},

    {0, {0}, 0xff},
};

class ILI9341Display : public Display 
{
    public:
        ILI9341Display(display_bus_cfg_t *config);
        void draw_frame();

    private:
        void init_bus(display_bus_cfg_t *config);
        void init_lcd(gpio_num_t reset);
        void set_draw_area();
};

#ifdef __cplusplus
}
#endif
}