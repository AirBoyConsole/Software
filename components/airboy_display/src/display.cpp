#include "display.hpp"

namespace arb 
{
    Display::Display(Vector2i size)
    {
        status = STATUS_OK;

        this->size = size;

        buffer_queue = xQueueCreate(10, sizeof(FrameBuffer *));
        if (buffer_queue == nullptr)
            status = STATUS_NO_MEM;
    }

    Display::~Display()
    {
        vQueueDelete(buffer_queue);
    }

    int Display::get_status() const
    {
        return status;
    }

    Vector2i Display::get_size() const
    {
        return size;
    }

    void Display::add_buffer_queue(FrameBuffer* buffer) const
    {
        if (xSemaphoreTake(buffer->mutex, pdMS_TO_TICKS(10)) == pdTRUE)
            if (xQueueSend(buffer_queue, &buffer, pdMS_TO_TICKS(10)) != pdTRUE)
                xSemaphoreGive(buffer->mutex);
    }

    void IRAM_ATTR Display::display_task(void* arg)
    {
        Display *display = reinterpret_cast<Display*>(arg);

        if (display != nullptr)
        {
            if (display->status == 0)
            {
                while(true)
                {
                    if(xQueueReceive(display->buffer_queue, &display->current_queue_buffer, portMAX_DELAY))
                    {
                        display->send_buffer();
                    }
                }
            }
        }

        // pointer given is not valid or display was not created correctly or a break occured in task loop
        // delete this task
        vTaskDelete(NULL);
    }

    bool IRAM_ATTR Display::lcd_trans_done_cb(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
    {
        BaseType_t woken = pdFALSE;

        FrameBuffer **buffer = static_cast<FrameBuffer**>(user_ctx);
        xSemaphoreGiveFromISR((*buffer)->mutex, &woken);

        return (woken == pdTRUE);
    }
}
