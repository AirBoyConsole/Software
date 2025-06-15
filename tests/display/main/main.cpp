#include "esp_log.h"
#include <cmath>
#include "esp_timer.h"

#include "display.hpp"
#include "display_ili9341.hpp"
#include "renderer.hpp"
#include "full_renderer.hpp"
#include "vector2.hpp"
#include "draw_shapes.hpp"

static const char *TAG = "test";

void IRAM_ATTR test_task(void* arg)
{
    uint64_t* time_ptr = static_cast<uint64_t*>(arg);

    while(true)
    {
        ESP_LOGI(TAG, "time %llu", *time_ptr);
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    vTaskDelete(NULL);
}

extern "C" void app_main(void)
{
    arb::Display *display = new arb::DisplayILI9341;

    if (display->get_status())
    {
        ESP_LOGE(TAG, "error");
    }

    arb::FullRenderer render = arb::FullRenderer(*display);

    arb::ShapeRect rect(arb::Vector2i(80, 80), 0b0111001110000001);
    arb::ShapeRect rect_bg(arb::Vector2i(80, 80), 0b0010000011100100);

    int i = 0;
    static uint64_t delta = 0;
    uint64_t time = 0, oldtime = 0;

    xTaskCreatePinnedToCore(test_task, "test", 5192, &delta, 4, NULL, tskNO_AFFINITY);

    while(true)
    {
        oldtime = time;
        time = esp_timer_get_time();
        delta = (time - oldtime);

        if (render.begin_draw())
        {   
            render.clear(0b0010000011100100);

            if ((i ++) >= 300) i = 0;

            for (int j = 0; j < 1000; ++j)
            {
                render.draw_rect(rect, arb::Vector2i(i + j, 80));
            }
            

            render.end_draw();
        }

        
        //vTaskDelay(pdMS_TO_TICKS(100));
    }
}