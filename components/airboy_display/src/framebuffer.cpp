#include "framebuffer.hpp"

namespace arb 
{
    FrameBuffer::FrameBuffer(Vector2i size, Vector2i offset)
    {
        // fields initialization
        // -----------------------
        status = STATUS_OK;
        this->size = size;
        this->offset = offset;
        lenght = size.x * size.y;
        // -----------------------

        // buffer initialization
        // -----------------------
        buffer = static_cast<uint16_t *>(heap_caps_malloc(lenght * 2, MALLOC_CAP_DMA));
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