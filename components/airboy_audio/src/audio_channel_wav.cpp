#include "audio_channel_wav.hpp"

namespace airboy 
{
    AudioChannelWav::AudioChannelWav(Audio* audio, unsigned int id) : AudioChannel(audio, id) {}
    AudioChannelWav::~AudioChannelWav() {}

    void AudioChannelWav::read_decode_file()
    {
        // check if any bytes left in file
        if (this->file_bytes_left == 0)
        {
            //if the repeat flag is set, play the file again, otherwise stop the channel
            if (repeat)
            {
                
                this->file_bytes_left = this->header.data_size;
                this->file.seekg(44);
            }
            else
            {
                this->audio->stop(this->channel_id);
            }
        }

        //if there are less bytes left in the file than the buffer size read all bytes left, if not read whole buffer
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
        //wav header is 44 bytes long
        this->file.read((char *) &this->header, 44);

        // must be 16 bit stereo or mono file
        if (!(this->header.num_channels == 2 || this->header.num_channels == 1)) return false;
        if (this->header.bits_per_sample != 16) return false;

        // set the channel mode to mono or stereo
        if (this->header.num_channels == 1) this->stereo = false;
        else this->stereo = true;

        // set audio sample rate to file sample rate
        audio->set_sample_rate(this->header.sample_rate);
        //audio system sample rate must be the same as file sample rate
        if (audio->get_sample_rate() != this->header.sample_rate) return false;

        this->file_bytes_left = this->header.data_size;

        return true;
    }
}
