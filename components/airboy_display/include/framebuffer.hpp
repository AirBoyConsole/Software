#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "status.hpp"

namespace airboy {

#ifdef __cplusplus
extern "C" {
#endif

class FrameBuffer
{
public:
    FrameBuffer(uint16_t width, uint16_t height, uint16_t offset_x, uint16_t offset_y);
    ~FrameBuffer();

    uint16_t *buffer = nullptr;
    uint16_t width;
    uint16_t height;
    uint16_t offset_x;
    uint16_t offset_y;
    SemaphoreHandle_t mutex;
    size_t size;
    status_t status;
};

#ifdef __cplusplus
}
#endif

}