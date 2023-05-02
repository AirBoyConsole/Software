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

#include "input.hpp"
#include "mcp23008_input.hpp"
#include "generic_display.hpp"
#include "ili9341_display.hpp"
#include "internal_bus.hpp"
#include "internal_eeprom.hpp"

namespace airboy {

#define ENGINE_TAG "Engine"

#ifdef __cplusplus
extern "C" {
#endif

class Engine 
{
public:
    Engine();
    ~Engine();

    virtual bool setup() = 0;
    virtual bool update(float delta) = 0;
    bool construct();
    void run();

protected:
    Input *input = nullptr;
    GenericDisplay *display = nullptr;
    InternalEEprom *eeprom = nullptr;

private:
    InternalBus *internal_bus = nullptr;
    

};

#ifdef __cplusplus
}
#endif
}

