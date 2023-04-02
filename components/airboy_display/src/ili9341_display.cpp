#include "ili9341_display.hpp"

namespace airboy 
{
    ILI9341Display::ILI9341Display(bus_cfg_t *config) : GenericDisplay(240, 320)
    {
        init_bus(config);
    }

    void ILI9341Display::init_bus(bus_cfg_t *config)
    {
        spi_bus_config_t buscfg;
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
		buscfg.max_transfer_sz = this->height * this->width * sizeof(uint16_t);
        buscfg.flags = SPICOMMON_BUSFLAG_OCTAL;

        ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

        esp_lcd_panel_io_spi_config_t io_config;
        io_config.dc_gpio_num       = config->DC;
        io_config.cs_gpio_num       = config->CS;
        io_config.pclk_hz           = this->clock;
        io_config.lcd_cmd_bits      = LCD_CMD_BITS;
        io_config.lcd_param_bits    = LCD_PARAM_BITS;
        io_config.spi_mode          = 3; // fastest mode
        io_config.trans_queue_depth = 10;
        io_config.flags.octal_mode  = 1;
        io_config.on_color_trans_done = this->lcd_trans_done_cb;

        ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &this->io));
    }
}
