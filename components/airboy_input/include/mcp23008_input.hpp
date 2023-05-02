#pragma once

#include <iostream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_check.h"
#include "esp_log.h"

#include "input.hpp"

namespace airboy {

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief one init command struct
 */
typedef struct {
    uint8_t address;  // 0xFF - end 
    uint8_t data;
} input_init_cmd_t;

/**
 * @brief array of init commands
 */
const input_init_cmd_t input_vendor_specific_init[] = {
    /* IODIR */
    {0x00, 0xFF},
    /* IPOL */
    {0x01, 0xFF},
    /* GPINTEN */
    {0x02, 0xFF},
    /* INTCON */
    {0x04, 0x00},
    /* GPPU */
    {0x06, 0xFF},
    /* IOCON */
    {0x05, 0B00101000},
    /* end */
    {0xFF, 0x00}
};

class MCP23008Input : public Input 
{
    public:
        MCP23008Input(i2c_port_t port, input_bus_cfg_t *conf);

    private:
        void init_input();        
        esp_err_t register_write(uint8_t chip_address, uint8_t reg_addr, uint8_t data);
        esp_err_t register_read(uint8_t chip_address, uint8_t reg_addr, uint8_t *data);
};

#ifdef __cplusplus
}
#endif
}