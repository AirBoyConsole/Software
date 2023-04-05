#include "generic_display.hpp"

namespace airboy 
{
    GenericDisplay::GenericDisplay(int height, int width)
    {
        this->height = height;
        this->width = width;
    }

    GenericDisplay::~GenericDisplay()
    {
        if (a_buffer != nullptr) delete [] a_buffer;
        if (b_buffer != nullptr) delete [] b_buffer;
    }

    bool GenericDisplay::lcd_trans_done_cb(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
    {
        BaseType_t woken = true;
        //ESP_LOGI("dupa", "koniec");
        return woken;
    }

    void GenericDisplay::init_framebuffer()
    {
        // TO DO: double buffering

        this->a_buffer = static_cast<uint16_t *>(heap_caps_malloc(this->width * this->height * sizeof(uint16_t), MALLOC_CAP_DMA));
        this->current_buffer = this->a_buffer;
    }
}
