#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "audio.hpp"
#include "lm4936_audio.hpp"
#include "internal_bus.hpp"
#include "sdmmc.hpp"
#include "input.hpp"
#include "mcp23008_input.hpp"

#define TAG "audio_test"

extern "C" void app_main(void)
{
    airboy::sdmmc_pinout_t sd_pinout{
        .clk = 41,
        .cmd = 40,
        .d0 = 42,
        .d1 = 44,
        .d2 = 43,
        .d3 = 39
    };

    airboy::SDMMC* sd_card = new airboy::SDMMC(&sd_pinout);

    airboy::InternalBus *internal_bus = new airboy::InternalBus();
    ESP_ERROR_CHECK(internal_bus->init_bus());

    airboy::audio_bus_cfg_t config;
    memset(&config, 0, sizeof(airboy::audio_bus_cfg_t));
    config.address = 0x6C;
    config.DOUT = 4;
    config.BCK = 5;
    config.LRCK = 3;

    airboy::Audio *audio = airboy::LM4936Audio::create_instance(internal_bus->get_port(), config);

    airboy::input_bus_cfg_t input_conf{
        .interrupt = 38,
        .addres_0 = 0x20,
        .addres_1 = 0x21
    };

    airboy::Input* input = new airboy::MCP23008Input(internal_bus->get_port(), &input_conf);

    audio->add_channel_wav();
    audio->add_channel_wav();
    audio->add_channel_wav();
    audio->add_channel_wav();

    audio->play("/sdcard/e.wav", 0, false, true);
    audio->play("/sdcard/exp.wav", 1, true, true);
    audio->play("/sdcard/wayy.wav", 2, true, true);
    //audio->play("/sdcard/wayy.wav", 3, true, true);

    unsigned int chanlle = 0;

    while (true)
    {
        ESP_LOGI(TAG, "%u",chanlle);
        if (input->is_pressed(airboy::Buttons::BUTTON_DPAD_UP)) chanlle++;
        if (input->is_pressed(airboy::Buttons::BUTTON_DPAD_DOWN)) chanlle--;
        if (input->is_pressed(airboy::Buttons::BUTTON_DPAD_LEFT)) audio->stop(chanlle);
        if (input->is_pressed(airboy::Buttons::BUTTON_DPAD_RIGHT)) audio->play("/sdcard/exp.wav",chanlle,true,false);

        if (input->is_pressed(airboy::Buttons::BUTTON_ACTION_X)) audio->set_volume(0, 0.1);
        if (input->is_pressed(airboy::Buttons::BUTTON_ACTION_Y)) audio->set_volume(0, 1.0);

        if (input->is_pressed(airboy::Buttons::BUTTON_ACTION_A)) 
        {
            char* stats = "";
            vTaskGetRunTimeStats(stats);

            ESP_LOGI(TAG, "%s",stats);
        }

        vTaskDelay(100);
    }
}