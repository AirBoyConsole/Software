#include <iostream>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c.h"

#include "internal_eeprom.hpp"
#include "internal_bus.hpp"

#define APP_TAG "App"

extern "C" void app_main(void)
{
    airboy::InternalBus *internal_bus = new airboy::InternalBus();
    ESP_ERROR_CHECK(internal_bus->init_bus());

    airboy::InternalEEprom *eeprom = new airboy::InternalEEprom(internal_bus->get_port(), 0x50);
}