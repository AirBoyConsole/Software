#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "esp_heap_caps.h"
#include "esp_lcd_panel_io.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_attr.h"

#include "status.hpp"
#include "framebuffer.hpp"

#define DISPLAY_TAG "display"

namespace airboy {

#ifdef __cplusplus
extern "C" {
#endif

class Display 
{
public:
    /**
    * @brief The following macro is used to determine the recommended size of the
    */
    Display();
    virtual ~Display();

    int get_status() const;
    void add_buffer_queue(FrameBuffer* buffer);

protected:
    TaskHandle_t display_task_handle = nullptr;
    FrameBuffer *current_send_buffer = nullptr;
    FrameBuffer *current_queue_buffer = nullptr;
    status_t status;

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

