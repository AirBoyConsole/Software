#include "audio.hpp"

namespace airboy 
{
    TaskHandle_t Audio::audio_task_handle;

    Audio::Audio() {}

    Audio::~Audio() 
    {
        if (this->active_channel_count != 0) i2s_channel_disable(this->audio_chanel_handle);
        i2s_del_channel(this->audio_chanel_handle);
        vTaskDelete(this->audio_task_handle);

        for (auto iter = channels.begin(); iter != channels.end(); ++iter) delete *iter;
    }

    void Audio::play(const char *file_name, unsigned int channel_number, bool interrupt, bool repeat)
    {
        this->channels[channel_number]->is_playing = false;

        // check if chanell id exists
        if (this->channels.size() < channel_number) return;
        // if channel is playing and interrupt is not selected return 
        if ((this->channels[channel_number]->is_playing == true) && (interrupt == false)) return;
        // if interrupt is selected and another file was playing close that file
        if (this->channels[channel_number]->file.is_open()) this->channels[channel_number]->file.close();

        //open file and check whether it has been opened
        this->channels[channel_number]->file.open(file_name, std::ios::in | std::ios::binary);
        if (!this->channels[channel_number]->file.is_open()) return;

        // if header is not valid return
        if (!this->channels[channel_number]->read_header())
        {
            this->channels[channel_number]->file.close();
            return;
        } 

        // initialy fill audio buffer
        this->channels[channel_number]->read_decode_file();

        this->channels[channel_number]->is_playing = true;
        this->channels[channel_number]->paused = false;
        this->channels[channel_number]->repeat = repeat;

        if (active_channel_count == 0)  vTaskResume(this->audio_task_handle);
        active_channel_count++;
    }

    void Audio::pause_resume(unsigned int channel_number)
    {
        if (!this->channels[channel_number]->paused && this->channels[channel_number]->is_playing)
        {
            this->channels[channel_number]->paused = true;
            this->channels[channel_number]->is_playing = false;
        }
        else
        {
            this->channels[channel_number]->paused = false;
            this->channels[channel_number]->is_playing = true;
        }
    }

    void Audio::stop(unsigned int channel_number)
    {
        if (this->channels.size() < channel_number) return;

        channels[channel_number]->is_playing = false;
        active_channel_count--;

        if (active_channel_count == 0)  vTaskSuspend(this->audio_task_handle);
    }

    void Audio::pause_audio()
    {

    }

    void Audio::add_channel_wav()
    {
        this->channels.push_back(new AudioChannelWav(this, this->channels.size() + 1));
    }

    void IRAM_ATTR Audio::audio_task(void *arg)
    {
        Audio *audio = reinterpret_cast<Audio*>(arg);
        size_t bytes_written;

        vTaskSuspend(NULL);

        i2s_channel_enable(audio->audio_chanel_handle);

        while (true)
        {
            if (!audio->channels[0]->is_playing) 
            {
                char temp[] = {0, 0, 0, 0};
                i2s_channel_write(audio->audio_chanel_handle, temp, 4, &bytes_written, portMAX_DELAY);
                continue;
            }

            if (audio->channels[0]->buffer_bytes_left == 0) audio->channels[0]->read_decode_file();
            char* DataPtr = audio->channels[0]->channel_buffer + BUFFER_SIZE - audio->channels[0]->buffer_bytes_left;
            

            i2s_channel_write(audio->audio_chanel_handle, DataPtr, 4, &bytes_written, portMAX_DELAY);
            audio->channels[0]->buffer_bytes_left -= 4;
        }

        vTaskDelete(NULL);
    }
}
