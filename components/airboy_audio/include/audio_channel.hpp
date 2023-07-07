#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_check.h"

#define CHANNEL_TAG "channel"
#define BUFFER_SIZE 4096
#define BYTES_TO_SEND 4

namespace airboy {

#ifdef __cplusplus
extern "C" {
#endif
class Audio;
class AudioChannel {
    public:
        AudioChannel(Audio* audio, unsigned int id);
        virtual ~AudioChannel();

        virtual void read_decode_file() = 0;    
        virtual bool read_header() = 0; 

        enum class State {
            PLAY,
            PAUSE,
            STOP
        };

        State state = State::STOP;

        std::fstream file;
        bool stereo;

        float volume = 1.0;
        
        bool repeat = false;
        
        unsigned int buffer_bytes_left = 0;
        char channel_buffer[BUFFER_SIZE];
        
    protected:
        Audio* audio;
        unsigned int channel_id;
};

#ifdef __cplusplus
}
#endif
}

