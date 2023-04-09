#include "ili9341_display.hpp"
#include "driver/gpio.h"
#include <cstring>

namespace airboy 
{
    ILI9341Display::ILI9341Display(bus_cfg_t *config) : GenericDisplay(240, 320)
    {
        this->reset_pin = static_cast<gpio_num_t>(config->RST);
        gpio_num_t backlight = static_cast<gpio_num_t>(config->BL);
        gpio_num_t read = static_cast<gpio_num_t>(config->RD);

        gpio_config_t io_conf;
        memset(&io_conf, 0, sizeof(gpio_config_t));
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = GPIO_MODE_OUTPUT;
        io_conf.pin_bit_mask = ((1ULL << read) | (1ULL <<  this->reset_pin));
        ESP_ERROR_CHECK(gpio_config(&io_conf));

        gpio_set_level(read, 1);
        gpio_set_level(this->reset_pin, 1);

        ILI9341Display::init_bus(config);
        ILI9341Display::init_framebuffer();
        ILI9341Display::init_lcd();
        ILI9341Display::init_backlight(backlight, config->backlight);
    }

    void ILI9341Display::init_bus(bus_cfg_t *config)
    {
        spi_bus_config_t buscfg;
        memset(&buscfg, 0, sizeof(spi_bus_config_t));
        buscfg.sclk_io_num  = config->CLK;
        buscfg.mosi_io_num  = config->bus_bits[0];
        buscfg.quadwp_io_num = -1;
        buscfg.quadhd_io_num = -1;
        buscfg.data1_io_num = config->bus_bits[1];
        buscfg.data2_io_num = config->bus_bits[2];
        buscfg.data3_io_num = config->bus_bits[3];
        buscfg.data4_io_num = config->bus_bits[4];
        buscfg.data5_io_num = config->bus_bits[5];
        buscfg.data6_io_num = config->bus_bits[6];
        buscfg.data7_io_num = config->bus_bits[7];
		buscfg.max_transfer_sz = 240 * 320 * sizeof(uint16_t);
        buscfg.flags = SPICOMMON_BUSFLAG_OCTAL;

        ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

        esp_lcd_panel_io_spi_config_t io_config;
        memset(&io_config, 0, sizeof(esp_lcd_panel_io_spi_config_t));
        io_config.dc_gpio_num       = config->DC;
        io_config.cs_gpio_num       = config->CS;
        io_config.pclk_hz           = config->clock;
        io_config.lcd_cmd_bits      = 8;
        io_config.lcd_param_bits    = 8;
        io_config.spi_mode          = 3; // fastest mode
        io_config.trans_queue_depth = 10;
        io_config.flags.octal_mode  = 1;
        io_config.on_color_trans_done = this->lcd_trans_done_cb;

        ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &this->io));
    }

    void ILI9341Display::init_lcd()
    {
        gpio_set_level((gpio_num_t)14, 0);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level((gpio_num_t)14, 1);
        vTaskDelay(pdMS_TO_TICKS(10));

        esp_lcd_panel_io_tx_param(this->io, LCD_CMD_SLPOUT, NULL, 0);
        vTaskDelay(pdMS_TO_TICKS(100));

        int cmd = 0;
        while (vendor_specific_init[cmd].data_bytes != 0xff) 
        {
            esp_lcd_panel_io_tx_param(this->io, vendor_specific_init[cmd].cmd, vendor_specific_init[cmd].data, vendor_specific_init[cmd].data_bytes & 0x1F);
            cmd++;
        }

        vTaskDelay(pdMS_TO_TICKS(100));
        esp_lcd_panel_io_tx_param(this->io, LCD_CMD_DISPON, NULL, 0);
    }

    void ILI9341Display::drawFrame()
    {
        uint8_t temp[] = {0, 0, 0x01, 0x40};

        ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(this->io, 0x2A, &temp, 4));
        temp[2] = 0x0F;
        temp[3] = 0x0;
        ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(this->io, 0x2B, &temp, 4));
        // transfer frame buffer
        size_t len = 320 * 240 * 2;
        ESP_ERROR_CHECK(esp_lcd_panel_io_tx_color(this->io, 0x2C, this->a_buffer, len));
    }
}
