#include <iostream>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c.h"

#include "vector2.hpp"

#include "input.hpp"
#include "display.hpp"
#include "engine.hpp"
#include "math.hpp"

#define APP_TAG "App"

class Example : public airboy::Engine
{
public:
	void setup() override
	{
		// Called once at the start, so create things here
	}

	void update(float delta) override
	{
		ESP_LOGI(APP_TAG, "pressed value: %f", delta);
		this->display->clear_buffer();
	}
};

extern "C" void app_main(void)
{
    Example demo;
	if (demo.construct())
		demo.run();
	else ESP_LOGE(APP_TAG, "Construct error");
}