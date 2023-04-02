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
        BaseType_t woken = false;
        return woken;
    }
}
