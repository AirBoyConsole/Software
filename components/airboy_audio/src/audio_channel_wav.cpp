#include "audio_channel_wav.hpp"

namespace airboy 
{
    AudioChannelWav::AudioChannelWav(Audio* audio, unsigned int id) : AudioChannel(audio, id) {}
    AudioChannelWav::~AudioChannelWav() {}

    void AudioChannelWav::read_decode_file()
    {
        //ESP_LOGI(CHANNEL_TAG, "bytes left: %u", this->file_bytes_left);
        if (this->file_bytes_left == 0)
        {
            if (repeat)
            {
                this->file_bytes_left = this->header.data_size;
                this->file.seekg(44);
                ESP_LOGI(CHANNEL_TAG, "audio loop");
            }
            else
            {
                this->audio->stop(this->channel_id);
                ESP_LOGI(CHANNEL_TAG, "audio stop");
            }
        }

        if (this->file_bytes_left < BUFFER_SIZE)
        {
            this->file.read(channel_buffer, this->file_bytes_left);
            this->buffer_bytes_left = this->file_bytes_left;
            this->file_bytes_left = 0;
        }
        else
        {
            this->file.read(channel_buffer, BUFFER_SIZE);
            this->buffer_bytes_left = BUFFER_SIZE;
            this->file_bytes_left -= BUFFER_SIZE;
        }
    }

    bool AudioChannelWav::read_header()
    {
        this->file.read((char *) &this->header, 44);
        ESP_LOGI(CHANNEL_TAG, "header check");

        // must be 16 bit stereo file
        if (this->header.num_channels != 2) return false;
        if (this->header.bits_per_sample != 16) return false;

        this->file_bytes_left = this->header.data_size;
        ESP_LOGI(CHANNEL_TAG, "file size %d", this->file_bytes_left);

        return true;
    }
}
