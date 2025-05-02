#include "framebuffer.hpp"

namespace airboy 
{
    FrameBuffer::FrameBuffer(uint16_t width, uint16_t height, uint16_t offset_x, uint16_t offset_y)
    {
        // fields initialization
        // -----------------------
        status = STATUS_OK;
        this->width = width;
        this->height = height;
        this->offset_x = offset_x;
        this->offset_y = offset_y;
        size = width * height;
        // -----------------------

        // buffer initialization
        // -----------------------
        buffer = static_cast<uint16_t *>(heap_caps_malloc(size * 2, MALLOC_CAP_DMA));
        if (buffer == nullptr)
            status = STATUS_NO_MEM;
        // -----------------------

        // mutex initialization
        // -----------------------
        mutex = xSemaphoreCreateBinary();
        if (mutex == nullptr)
            status = STATUS_NO_MEM;
        else
            xSemaphoreGive(mutex);
        // -----------------------
    }

    FrameBuffer::~FrameBuffer() 
    {
        heap_caps_free(buffer);
        vSemaphoreDelete(mutex);
    }
}