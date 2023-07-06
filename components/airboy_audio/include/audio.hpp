#pragma once

#include <vector>
#include <iostream>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/i2s_std.h"
#include "esp_log.h"
#include "esp_check.h"

#include "audio_channel.hpp"
#include "audio_channel_wav.hpp"

#define AUDIO_TAG "Audio"

namespace airboy {

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Configuration struct
 */
typedef struct{
    uint8_t DOUT;
    uint8_t BCK;
    uint8_t LRCK;
    uint8_t address;
}audio_bus_cfg_t;

typedef struct{
    int16_t left;
    int16_t right;
}sample16_t;

class Audio {
    public:
        Audio(audio_bus_cfg_t &conf);
        virtual ~Audio();

        void play(const char* file_name, unsigned int channel_number, bool interrupt = false, bool repeat = false);
        void pause_resume(unsigned int channel_number);
        void stop(unsigned int channel_number);
        void pause_audio();
        void set_volume(unsigned int channel_number, float volume);

        void add_channel_wav();

        void set_sample_rate(unsigned int new_sample_rate);
        unsigned int get_sample_rate();

    protected:
        static void IRAM_ATTR audio_task(void* arg);
        virtual void init_audio(audio_bus_cfg_t &conf) = 0;

        std::vector<AudioChannel*> channels;
        unsigned int active_channel_count = 0;
        unsigned int sample_rate = 16000;

        i2s_chan_handle_t audio_chanel_handle;
        static TaskHandle_t audio_task_handle;
        static bool instance;    
};

#ifdef __cplusplus
}
#endif
}

