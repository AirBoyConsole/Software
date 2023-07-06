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

#define TAG "dupsko"

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "heap przed sd: %d", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));

    airboy::sdmmc_pinout_t sd_pinout{
        .clk = 41,
        .cmd = 40,
        .d0 = 42,
        .d1 = 44,
        .d2 = 43,
        .d3 = 39
    };

    airboy::SDMMC* sd_card = new airboy::SDMMC(&sd_pinout);

    ESP_LOGI(TAG, "heap po sd: %d", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));

    /*char buffer[128];

    std::fstream file;

    file.open("/sdcard/wavfile.wav", std::ios::in | std::ios::binary);

    if (!file.is_open())
    {
        ESP_LOGI("cos", "dupa");
        return;
    }

    file.seekg(44);

    for(int i = 0; i < 3; i++)
    {
        file.read(buffer, 128);

        for(int i = 0; i < 128; i++)
        {
            ESP_LOGI("cos", "value: %X", buffer[i]);
        }
    }*/
    

    airboy::InternalBus *internal_bus = new airboy::InternalBus();
    ESP_ERROR_CHECK(internal_bus->init_bus());

    airboy::audio_bus_cfg_t config;
    memset(&config, 0, sizeof(airboy::audio_bus_cfg_t));
    config.address = 0x6C;
    config.DOUT = 4;
    config.BCK = 5;
    config.LRCK = 3;

    /*ESP_LOGI(TAG, "test logi: %d", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));

    ESP_LOGI(TAG, "heap przded: %d", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));*/

    airboy::Audio *audio = airboy::LM4936Audio::create_instance(internal_bus->get_port(), config);

    /*ESP_LOGI(TAG, "heap po tworzeniu: %d", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));

    delete audio;

    ESP_LOGI(TAG, "heap po delete: %d", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));*/

    airboy::input_bus_cfg_t input_conf{
        .interrupt = 38,
        .addres_0 = 0x20,
        .addres_1 = 0x21
    };

    airboy::Input* input = new airboy::MCP23008Input(internal_bus->get_port(), &input_conf);

    audio->add_channel_wav();
    audio->add_channel_wav();

    audio->play("/sdcard/e.wav", 0, false, true);
    //audio->play("/sdcard/exp.wav", 1, false, true);

    while (true)
    {
        if (input->is_pressed(airboy::Buttons::BUTTON_DPAD_UP)) audio->pause_resume(0);
        if (input->is_pressed(airboy::Buttons::BUTTON_DPAD_DOWN)) audio->play("/sdcard/e.wav",0,true,false);
        if (input->is_pressed(airboy::Buttons::BUTTON_DPAD_LEFT)) audio->play("/sdcard/wayy.wav",0,true,true);
        if (input->is_pressed(airboy::Buttons::BUTTON_DPAD_RIGHT)) audio->play("/sdcard/exp.wav",1,true,false);

        if (input->is_pressed(airboy::Buttons::BUTTON_ACTION_X)) audio->set_volume(0, 0.1);
        if (input->is_pressed(airboy::Buttons::BUTTON_ACTION_Y)) audio->set_volume(0, 1.0);

        if (input->is_pressed(airboy::Buttons::BUTTON_ACTION_A)) 
        {
            ESP_LOGI(TAG, "heap przed delete: %d", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
            delete audio;
            ESP_LOGI(TAG, "heap po delete: %d", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
        }
        vTaskDelay(100);
    }
}