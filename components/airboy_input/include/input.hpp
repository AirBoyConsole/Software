#pragma once

#include <iostream>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_check.h"
#include "esp_log.h"

namespace airboy {

#define INPUT_TAG "Input"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Configuration struct
 */
typedef struct{
    uint8_t interrupt;
    uint8_t addres_0;
    uint8_t addres_1;
}input_bus_cfg_t;

/**
 * @brief Enum of button id
 */
enum Buttons {
	BUTTON_DPAD_UP	       = 0b0100000000000000,
	BUTTON_DPAD_DOWN       = 0b0001000000000000,
	BUTTON_DPAD_LEFT       = 0b1000000000000000,
	BUTTON_DPAD_RIGHT      = 0b0000100000000000,
	BUTTON_SELECT          = 0b0000000100000000,
	BUTTON_AIRBOY          = 0b0000001000000000,
	BUTTON_START           = 0b0000010000000000,
	BUTTON_BUMPER_LEFT     = 0b0010000000000000,
	BUTTON_BUMPER_RIGHT    = 0b0000000001000000,
	BUTTON_ACTION_A        = 0b0000000000100000,
	BUTTON_ACTION_B        = 0b0000000010000000,
	BUTTON_ACTION_Y        = 0b0000000000000010,
	BUTTON_ACTION_X        = 0b0000000000000001,
};

class Input 
{
    public:
        Input();

        /**
         * @brief Reads the input on demand
         */
        void read_input();
        
        /**
         * @brief Sets the previous buttons value (used only by engine class)
         */
        void reset_previous();

        /**
         * @brief block input for read time. Should be used with unblock_input
         */
        void block_input();

        /**
         * @brief unblock input. Should be used with block_input
         */
        void unblock_input();

        /**
         * @brief get the interrupt pin
         * 
         * @return interrupt pin value
         */
        gpio_num_t get_interrupt_pin();

        /**
         * @brief get the raw value of inputs
         * 
         * @return raw value of inputs
         */
        uint16_t get_raw();

        /**
         * @brief check if the button is pressed
         * @param[in] button id
         * 
         * @return true if pressed, false if released
         */
        bool is_pressed(Buttons button);

        /**
         * @brief check if the button was pressed on current frame
         * @param[in] button id
         * 
         * @return true if pressed
         */
        bool is_just_pressed(Buttons button);

        /**
         * @brief check if the button was released on current frame
         * @param[in] button id
         * 
         * @return true if released
         */
        bool is_just_released(Buttons button);

    protected:
        static void IRAM_ATTR input_task(void* arg);
        static void input_interrupt(void* args);
        virtual void init_input() = 0;
        virtual esp_err_t register_write(uint8_t chip_address, uint8_t reg_addr, uint8_t data) = 0;
        virtual esp_err_t register_read(uint8_t chip_address, uint8_t reg_addr, uint8_t *data) = 0;

        static TaskHandle_t input_handle;
        SemaphoreHandle_t input_semaphore;
        i2c_port_t port;
        gpio_num_t interrupt;
        uint8_t chip_0_address;
        uint8_t chip_1_address;
        uint16_t buttons_value;
        uint16_t prev_buttons_value;
};

#ifdef __cplusplus
}
#endif
}

