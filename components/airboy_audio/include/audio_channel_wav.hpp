#pragma once

#include <iostream>
#include <fstream>
#include <cstring>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_check.h"

#include "audio_channel.hpp"
#include "audio.hpp"

namespace airboy {

#ifdef __cplusplus
extern "C" {
#endif

struct WavHeader_Struct
{
    //   RIFF Section    
    char RIFF_section[4];      // Letters "RIFF"
    uint32_t size;              // Size of entire file less 8
    char Riff_format[4];         // Letters "WAVE"
    
    //   Format Section    
    char format_section[4];    // letters "fmt"
    uint32_t format_size;        // Size of format section less 8
    uint16_t format_id;          // 1=uncompressed PCM
    uint16_t num_channels;       // 1=mono,2=stereo
    uint32_t sample_rate;        // 44100, 16000, 8000 etc.
    uint32_t byte_rate;          // =SampleRate * Channels * (BitsPerSample/8)
    uint16_t block_align;        // =Channels * (BitsPerSample/8)
    uint16_t bits_per_sample;     // 8,16,24 or 32

    // Data Section
    char data_section[4];      // The letters "data"
    uint32_t data_size;          // Size of the data that follows
};

class AudioChannelWav : public AudioChannel {
    public:
        AudioChannelWav(Audio* audio, unsigned int id);
        ~AudioChannelWav();

        void read_decode_file();
        bool read_header();

    private:
        WavHeader_Struct header;
        unsigned int file_bytes_left;

};

#ifdef __cplusplus
}
#endif

}

