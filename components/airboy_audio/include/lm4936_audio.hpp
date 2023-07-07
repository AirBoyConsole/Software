#pragma once

#include <iostream>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/i2s_std.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "esp_check.h"

#include "audio.hpp"

namespace airboy {

#ifdef __cplusplus
extern "C" {
#endif

class LM4936Audio : public Audio 
{
    public:
        ~LM4936Audio();

        /**
         * @brief create instance of an Audio object
         * 
         * @return pointer to object or nullptr if object can't be created
         */ 
        static Audio* create_instance(i2c_port_t port, audio_bus_cfg_t &conf);

    private:
        LM4936Audio(i2c_port_t port, audio_bus_cfg_t &conf);
        void init_audio();
        esp_err_t register_write(uint8_t data);

        i2c_port_t port;
        uint8_t chip_address;
};

#ifdef __cplusplus
}
#endif
}