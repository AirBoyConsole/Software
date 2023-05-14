#include "display.hpp"

namespace airboy 
{
    TaskHandle_t Display::display_handle;

    Display::Display(int height, int width)
    {
        display_size.x = width;
        display_size.y = height;

        display_handle = xTaskGetCurrentTaskHandle();
    }

    Display::~Display()
    {
        if (frame_buffer != nullptr) delete [] frame_buffer;
    }

    bool Display::lcd_trans_done_cb(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
    {
        BaseType_t woken = true;
        vTaskResume(display_handle);
        return woken;
    }

    void Display::init_framebuffer()
    {
        this->frame_buffer = static_cast<uint16_t *>(heap_caps_malloc(display_size.x * display_size.y * sizeof(uint16_t), MALLOC_CAP_DMA));
    }

    void Display::init_backlight(gpio_num_t bl, uint32_t duty)
    {
        ledc_timer_config_t backlight_timer;
        memset(&backlight_timer, 0, sizeof(ledc_timer_config_t));
        backlight_timer.speed_mode       = LEDC_MODE,
        backlight_timer.timer_num        = LEDC_TIMER_0;
        backlight_timer.duty_resolution  = LEDC_TIMER_8_BIT;
        backlight_timer.freq_hz          = 5000;
        backlight_timer.clk_cfg          = LEDC_AUTO_CLK;
        ESP_ERROR_CHECK(ledc_timer_config(&backlight_timer));

        ledc_channel_config_t backlight_channel;
        memset(&backlight_channel, 0, sizeof(ledc_channel_config_t));
        backlight_channel.speed_mode     = LEDC_MODE;
        backlight_channel.channel        = LEDC_CHANNEL;
        backlight_channel.timer_sel      = LEDC_TIMER_0;
        backlight_channel.intr_type      = LEDC_INTR_DISABLE;
        backlight_channel.gpio_num       = bl;
        backlight_channel.duty           = duty;
        backlight_channel.hpoint         = 0;
        ESP_ERROR_CHECK(ledc_channel_config(&backlight_channel));
    }

    void Display::set_backlight_level(uint8_t value)
    {
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, value));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));

        //TO DO: send current backlight setting to eeprom
    }

    Vector2i Display::get_display_size()
    {
        return display_size;
    }
}
