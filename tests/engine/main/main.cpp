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
		this->renderer->draw_text(airboy::Vector2i(30, 0), 2, 0xFFFF, "no AA");
		this->renderer->draw_line(airboy::Vector2i(10, 20), airboy::Vector2i(110, 20), 0xFFFF);
		this->renderer->draw_line(airboy::Vector2i(10, 20), airboy::Vector2i(10, 200), 0xFFFF);
		this->renderer->draw_line(airboy::Vector2i(110, 20), airboy::Vector2i(110, 200), 0xFFFF);
		this->renderer->draw_line(airboy::Vector2i(10, 200), airboy::Vector2i(110, 200), 0xFFFF);
		this->renderer->draw_line(airboy::Vector2i(10, 20), airboy::Vector2i(110, 200), 0xFFFF);
		this->renderer->draw_line(airboy::Vector2i(10, 200), airboy::Vector2i(110, 20), 0xFFFF);
		//this->renderer->draw_fill_rect(airboy::Vector2i(10,10), airboy::Vector2i(55,55), 0xFFFF);
	}
};

extern "C" void app_main(void)
{
    Example demo;
	if (demo.construct(10))
		demo.run();
	else
		ESP_LOGE(APP_TAG, "Construct error");
}
