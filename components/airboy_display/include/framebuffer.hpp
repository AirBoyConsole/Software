#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "status.hpp"
#include "vector2.hpp"

namespace arb {

#ifdef __cplusplus
extern "C" {
#endif

class FrameBuffer
{
public:
    FrameBuffer(Vector2i size, Vector2i offset);
    FrameBuffer(const FrameBuffer& other) = delete;
    FrameBuffer(const FrameBuffer&& other) = delete;

    ~FrameBuffer();

    FrameBuffer& operator=(const FrameBuffer& other) = delete;
    FrameBuffer& operator=(FrameBuffer&& other) = delete;

    uint16_t *buffer = nullptr;
    Vector2i size;
    Vector2i offset;
    SemaphoreHandle_t mutex;
    size_t lenght;
    status_t status;
};

#ifdef __cplusplus
}
#endif

}