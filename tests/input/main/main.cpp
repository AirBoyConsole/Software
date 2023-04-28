#include <iostream>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c.h"

#include "input.hpp"
#include "mcp23008_input.hpp"

#define INPUT_I2C_PORT I2C_NUM_0
#define TAG "test_input"

extern "C" void app_main(void)
{
    i2c_config_t conf;
    memset(&conf, 0, sizeof(i2c_config_t));
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = 48;
    conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
    conf.scl_io_num = 47;
    conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
    conf.master.clk_speed = 100000;

    i2c_param_config(INPUT_I2C_PORT, &conf);
    ESP_ERROR_CHECK(i2c_driver_install(INPUT_I2C_PORT, conf.mode, 0, 0, 0));

    airboy::Input *input = new airboy::MCP23008Input(38, INPUT_I2C_PORT, 0x20, 0x21);

    while (true)
    {
        ESP_LOGI(TAG, "pressed value: %d", input->is_pressed(airboy::Buttons::BUTTON_DPAD_UP));
        ESP_LOGI(TAG, "just_pressed value: %d", input->is_just_pressed(airboy::Buttons::BUTTON_DPAD_UP));
        ESP_LOGI(TAG, "just_released value: %d", input->is_just_released(airboy::Buttons::BUTTON_DPAD_UP));

        input->reset_previous();
        vTaskDelay(5);
    }
}