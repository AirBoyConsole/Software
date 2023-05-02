#pragma once

#include <iostream>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_check.h"
#include "esp_log.h"

namespace airboy {

#ifdef __cplusplus
extern "C" {
#endif

#define BUS_TAG "Internal Bus"

class InternalBus 
{
    public:
        esp_err_t init_bus();
        i2c_port_t get_port();
    private:
        i2c_port_t bus_port;
};

#ifdef __cplusplus
}
#endif
}

