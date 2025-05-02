#include "esp_log.h"

#include "display.hpp"
#include "display_ili9341.hpp"
#include "framebuffer.hpp"

static const char *TAG = "display";

extern "C" void app_main(void)
{
    airboy::FrameBuffer buffer(320, 120, 0, 0);
    airboy::FrameBuffer buffer2(320, 120, 0, 120);

    ESP_LOGE(TAG, "size: %d", buffer.size);

    for (int i = 0; i < buffer.size; i++)
    {
        buffer.buffer[i] = 0;
    }
    for (int i = 0; i < buffer2.size; i++)
    {
        buffer2.buffer[i] = 0xF00F;
    }

    airboy::Display *display = new airboy::DisplayILI9341;

    if (display->get_status())
    {
        ESP_LOGE(TAG, "error");
    }
    
    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(100));

        for (int i = 0; i < 50; i++)
        {
            buffer.buffer[30*320 + 30+ i] = 0x0FF0;
        } 

        display->add_buffer_queue(&buffer);
        display->add_buffer_queue(&buffer2);
    }
}