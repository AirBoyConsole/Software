#pragma once

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_attr.h"

//#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
//#include "esp_lcd_panel_vendor.h"
//#include "esp_lcd_panel_ops.h"
//#include "esp_lcd_panel_commands.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "esp_heap_caps.h"

#define DISPLAY_TAG "display"

namespace airboy {

#ifdef __cplusplus
extern "C" {
#endif

class FrameBuffer
{
public:
    FrameBuffer(uint16_t width, uint16_t height, uint16_t offset_x, uint16_t offset_y)
    {
        this->width = width;
        this->height = height;
        this->offset_x = offset_x;
        this->offset_y = offset_y;

        size = width * height * 2;
        buffer = static_cast<uint16_t *>(heap_caps_malloc(size, MALLOC_CAP_DMA));

        if (buffer == nullptr)
        {
            ESP_LOGE(DISPLAY_TAG, "dupa");
        }

        mutex = xSemaphoreCreateMutex();
    }

    ~FrameBuffer() 
    {
        heap_caps_free(buffer);
        vSemaphoreDelete(mutex);
    }

    uint16_t *buffer = nullptr;
    uint16_t width;
    uint16_t height;
    uint16_t offset_x;
    uint16_t offset_y;
    SemaphoreHandle_t mutex;
    size_t size;
};

class Display 
{
public:
    Display();
    virtual ~Display();

    int get_status() const;
    void add_buffer_queue(FrameBuffer* buffer);

protected:
    TaskHandle_t display_task_handle = nullptr;
    FrameBuffer *current_send_buffer = nullptr;
    FrameBuffer *current_queue_buffer = nullptr;
    int status = 0;

    static void IRAM_ATTR display_task(void* arg);
    static bool IRAM_ATTR lcd_trans_done_cb(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx);

private:
    QueueHandle_t buffer_queue = nullptr;
    

    virtual void IRAM_ATTR send_buffer() {};
};

#ifdef __cplusplus
}
#endif
}

