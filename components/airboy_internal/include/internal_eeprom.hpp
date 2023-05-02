#pragma once

#include <iostream>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_check.h"
#include "esp_log.h"

#include "input.hpp"
#include "generic_display.hpp"

#define EEPROM_TAG "eeprom"

namespace airboy {

#ifdef __cplusplus
extern "C" {
#endif

class InternalEEprom 
{
    public:
        InternalEEprom(i2c_port_t port, uint8_t chip_address);


        void get_input_config(input_bus_cfg_t* conf);
        uint8_t get_input_type();
        void get_display_config(display_bus_cfg_t* conf);
        uint8_t get_display_type();
        //TODO: get_audio_config();

    private:
        esp_err_t write_byte(uint8_t chip_address, uint16_t mem_address, uint8_t data);
        esp_err_t write(uint8_t chip_address, uint16_t start_mem_address, uint8_t* data, size_t size);
        esp_err_t read_byte(uint8_t chip_address, uint16_t mem_address, uint8_t *data);
        esp_err_t read(uint8_t chip_address, uint16_t start_mem_address, uint8_t* data, size_t size);

        void config_eeprom();

        i2c_port_t port;
        uint8_t page_size;
        uint32_t chip_size;
        uint8_t chip_address;
};

#ifdef __cplusplus
}
#endif

}

