#include <iostream>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c.h"

#include "input.hpp"
#include "mcp23008_input.hpp"
#include "generic_display.hpp"
#include "ili9341_display.hpp"
#include "engine.hpp"

#define APP_TAG "App"

class Example : public airboy::Engine
{
public:
	bool setup() override
	{
		// Called once at the start, so create things here
		return true;
	}
	bool update(float delta) override
	{
		ESP_LOGI(APP_TAG, "pressed value: %d", input->is_just_pressed(airboy::Buttons::BUTTON_DPAD_UP));

		return true;
	}
};

extern "C" void app_main(void)
{
    Example demo;
	if (demo.construct())
		demo.run();
	else ESP_LOGE(APP_TAG, "Construct error");
}