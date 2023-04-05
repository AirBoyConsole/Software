#include "ili9341_display.hpp"
#include "driver/gpio.h"
#include <cstring>

namespace airboy 
{
    ILI9341Display::ILI9341Display(bus_cfg_t *config) : GenericDisplay(240, 320)
    {
        esp_rom_gpio_pad_select_gpio(GPIO_NUM_2);
        esp_rom_gpio_pad_select_gpio(GPIO_NUM_15);
        gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
        gpio_set_direction(GPIO_NUM_15, GPIO_MODE_OUTPUT);
        gpio_set_level(GPIO_NUM_2, 0);
        gpio_set_level(GPIO_NUM_15, 1);

        ILI9341Display::init_bus(config);
        ILI9341Display::init_framebuffer();

        //test code
        esp_lcd_panel_handle_t panel_handle = NULL;
        esp_lcd_panel_dev_config_t panel_config;
        memset(&panel_config, 0, sizeof(esp_lcd_panel_dev_config_t));
        panel_config.reset_gpio_num = config->RST;
        panel_config.rgb_endian = LCD_RGB_ENDIAN_BGR;
        panel_config.bits_per_pixel = 16;

        for (int pixel = 0; pixel < 320 *240 *2; pixel++)
        {
            this->a_buffer[pixel] = 0x9999;
        }

        ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(this->io, &panel_config, &panel_handle));
        esp_lcd_panel_reset(panel_handle);
        esp_lcd_panel_init(panel_handle);
        esp_lcd_panel_swap_xy(panel_handle, true);
        esp_lcd_panel_invert_color(panel_handle, true);
    
        for (uint8_t y = 0; y < this->height; y += 48)
        {
            esp_lcd_panel_draw_bitmap(panel_handle, 0, y, this->width, y + 48, &(this->a_buffer[y * this->width]));
        }    


        //ILI9341Display::init_lcd();
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
        io_config.pclk_hz           = 10 * 100 * 100;//config->clock;
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
        esp_lcd_panel_io_tx_param(this->io, LCD_CMD_SLPOUT, NULL, 0);
        vTaskDelay(pdMS_TO_TICKS(100));

        int cmd = 0;
        while (vendor_specific_init[cmd].data_bytes != 0xff) 
        {
            esp_lcd_panel_io_tx_param(this->io, vendor_specific_init[cmd].cmd, vendor_specific_init[cmd].data, vendor_specific_init[cmd].data_bytes & 0x1F);
            cmd++;
        }
    }

    void ILI9341Display::drawFrame()
    {
        uint8_t temp[] = {0, 0, 0x01, 0,40};

        ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(this->io, 0x2A, &temp, 4));
        temp[2] = 0;
        temp[3] = 0xF0;
        ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(this->io, 0x2B, &temp, 4));
        // transfer frame buffer
        size_t len = 320 * 240 * 2;
        ESP_ERROR_CHECK(esp_lcd_panel_io_tx_color(this->io, 0x2C, this->a_buffer, len));
    }
}