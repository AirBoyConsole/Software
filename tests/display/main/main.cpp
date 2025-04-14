#include "esp_log.h"

#include "display.hpp"
#include "display_ili9341.hpp"

static const char *TAG = "display";

extern "C" void app_main(void)
{
    airboy::FrameBuffer buffer(32, 32, 0, 0);

    for (int i = 0; i < buffer.size; i++)
    {
        buffer.buffer = 0;
    }

    airboy::Display *display = new airboy::DisplayILI9341;

    if (display->get_status())
    {
        ESP_LOGE(TAG, "error");
    }

    display->add_buffer_queue(&buffer);

    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}