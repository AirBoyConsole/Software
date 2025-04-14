#include "display_ili9341.hpp"

#include "display.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/idf_additions.h"

#include "esp_mac.h"

namespace airboy 
{
    DisplayILI9341::DisplayILI9341()
    {
        gpio_config_t io_conf = {};
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = GPIO_MODE_OUTPUT;
        io_conf.pin_bit_mask = ((1ULL << GPIO_NUM_15) | (1ULL <<  GPIO_NUM_14) | (1ULL <<  GPIO_NUM_2));
        gpio_config(&io_conf);

        gpio_set_level(GPIO_NUM_15, true);
        gpio_set_level(GPIO_NUM_14, true);
        gpio_set_level(GPIO_NUM_2, false);

        init_bus();

        gpio_set_level(GPIO_NUM_14, false);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(GPIO_NUM_14, true);
        vTaskDelay(pdMS_TO_TICKS(10));

        esp_lcd_panel_io_tx_param(this->io, LCD_CMD_SLPOUT, NULL, 0);

        int cmd = 0;
        while (vendor_specific_init[cmd].data_bytes != 0xff) 
        {
            esp_lcd_panel_io_tx_param(this->io, vendor_specific_init[cmd].cmd, vendor_specific_init[cmd].data, vendor_specific_init[cmd].data_bytes & 0x1F);
            cmd++;
        }

        esp_lcd_panel_io_tx_param(this->io, LCD_CMD_DISPON, NULL, 0);

        // the task must be created after all other inintializations
        xTaskCreatePinnedToCore(display_task, "display", 5192, this, 6, &display_task_handle, tskNO_AFFINITY);
        if (display_task_handle == nullptr)
            status = 1;
    }

    DisplayILI9341::~DisplayILI9341()
    {
        // the task must be destroyed first
        if (display_task_handle)
            vTaskDelete(display_task_handle);
    }

    void IRAM_ATTR DisplayILI9341::send_buffer()
    {
        vTaskDelay(pdMS_TO_TICKS(10));
        uint8_t col_address[] = 
        {
            static_cast<uint8_t>((current_queue_buffer->offset_x >> 8) & 0xFF),
            static_cast<uint8_t>(current_queue_buffer->offset_x & 0xFF),
            static_cast<uint8_t>(((current_queue_buffer->width - 1) >> 8) & 0xFF),
            static_cast<uint8_t>((current_queue_buffer->width - 1) & 0xFF)
        };

        esp_lcd_panel_io_tx_param(this->io, 0x2A, &col_address, 4);

        vTaskDelay(pdMS_TO_TICKS(10));
        uint8_t row_address[] = 
        {
            static_cast<uint8_t>((current_queue_buffer->offset_y >> 8) & 0xFF),
            static_cast<uint8_t>(current_queue_buffer->offset_y & 0xFF),
            static_cast<uint8_t>(((current_queue_buffer->height - 1) >> 8) & 0xFF),
            static_cast<uint8_t>((current_queue_buffer->height - 1) & 0xFF)
        };

        esp_lcd_panel_io_tx_param(this->io, 0x2B, &row_address, 4);

        current_send_buffer = current_queue_buffer;

        vTaskDelay(pdMS_TO_TICKS(10));
        esp_lcd_panel_io_tx_color(this->io, 0x2C, current_queue_buffer->buffer, current_queue_buffer->size);

    }

    void DisplayILI9341::init_bus()
    {
        spi_bus_config_t buscfg = {};
        buscfg.sclk_io_num   = 16;
        buscfg.mosi_io_num   = 13;
        buscfg.quadwp_io_num = -1;
        buscfg.quadhd_io_num = -1;
        buscfg.data1_io_num  = 12;
        buscfg.data2_io_num  = 11;
        buscfg.data3_io_num  = 10;
        buscfg.data4_io_num  = 9;
        buscfg.data5_io_num  = 8;
        buscfg.data6_io_num = 7;
        buscfg.data7_io_num = 6;
		buscfg.max_transfer_sz = 240 * 320 * sizeof(uint16_t);
        buscfg.flags = SPICOMMON_BUSFLAG_OCTAL;

        ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

        esp_lcd_panel_io_spi_config_t io_config = {};
        io_config.dc_gpio_num       = 17;
        io_config.cs_gpio_num       = 18;
        io_config.pclk_hz           = 1000000;
        io_config.lcd_cmd_bits      = 8;
        io_config.lcd_param_bits    = 8;
        io_config.spi_mode          = 0; // fastest mode
        io_config.trans_queue_depth = 20;
        io_config.flags.octal_mode  = 1;
        io_config.on_color_trans_done = lcd_trans_done_cb;
        io_config.user_ctx = &current_send_buffer;

        ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI2_HOST, &io_config, &this->io));
    }

}
