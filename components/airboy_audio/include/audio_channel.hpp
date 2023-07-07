#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_check.h"

#define CHANNEL_TAG "channel"

// when lot of files are being played from slow medum (sd card) buffer should be bigger, the internal memory is faster.
// the buffer can be smaller if sample rate is lower
// if you hear distortion increase this value
#define BUFFER_SIZE 2048

namespace airboy {

#ifdef __cplusplus
extern "C" {
#endif
class Audio;
class AudioChannel {
    public:
        AudioChannel(Audio* audio, unsigned int id);
        virtual ~AudioChannel();

        /**
         * @brief function is called when audio buffer is empty and new samples must be read
         */
        virtual void read_decode_file() = 0;
        /**
         * @brief function is called when the file is opened
         * 
         * @return true if header is ok false if something's wrong
         */    
        virtual bool read_header() = 0; 

        // current state of the channel
        enum class State {
            PLAY,
            PAUSE,
            STOP
        };
        State state = State::STOP;

        // channel music file
        std::fstream file;

        // channel flags
        bool stereo;
        bool repeat = false;

        // channel volume
        float volume = 1.0;
        
        // channel buffer
        unsigned int buffer_bytes_left = 0;
        char channel_buffer[BUFFER_SIZE];
        
    protected:
        // pointer to audio stystem that created the channel
        Audio* audio;
        // channel number in audio->channels vector
        unsigned int channel_id;
};

#ifdef __cplusplus
}
#endif
}

