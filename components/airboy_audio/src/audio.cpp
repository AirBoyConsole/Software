#include "audio.hpp"

namespace airboy 
{
    TaskHandle_t Audio::audio_task_handle;

    Audio::Audio(audio_bus_cfg_t &conf) 
    {
        i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);

        i2s_new_channel(&chan_cfg, &this->audio_chanel_handle, NULL);

        i2s_std_config_t std_cfg;
        memset(&std_cfg, 0, sizeof(i2s_std_config_t));
        std_cfg.clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(16000);
        std_cfg.slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO);
        std_cfg.slot_cfg.data_bit_width = I2S_DATA_BIT_WIDTH_16BIT;
        std_cfg.slot_cfg.slot_bit_width = I2S_SLOT_BIT_WIDTH_AUTO;
        std_cfg.slot_cfg.bit_shift = true;
        std_cfg.gpio_cfg.mclk = I2S_GPIO_UNUSED;
        std_cfg.gpio_cfg.bclk = static_cast<gpio_num_t>(conf.BCK),
        std_cfg.gpio_cfg.ws = static_cast<gpio_num_t>(conf.LRCK),
        std_cfg.gpio_cfg.dout = static_cast<gpio_num_t>(conf.DOUT),
        std_cfg.gpio_cfg.din = I2S_GPIO_UNUSED,
        std_cfg.gpio_cfg.invert_flags = {
            .mclk_inv = false,
            .bclk_inv = false,
            .ws_inv = false,
        },

        i2s_channel_init_std_mode(this->audio_chanel_handle, &std_cfg);

        xTaskCreatePinnedToCore(audio_task, "audio", 8192, this, 14, &this->audio_task_handle, 1);
    }

    Audio::~Audio() 
    {
        if (this->active_channel_count != 0) i2s_channel_disable(this->audio_chanel_handle);
        i2s_del_channel(this->audio_chanel_handle);
        vTaskDelete(this->audio_task_handle);

        for (auto iter = channels.begin(); iter != channels.end(); ++iter) delete *iter;

        Audio::instance = false;
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

        if (active_channel_count == 0)  vTaskResume(this->audio_task_handle);
        active_channel_count++;

        this->channels[channel_number]->paused = false;
        this->channels[channel_number]->repeat = repeat;
        this->channels[channel_number]->is_playing = true;
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

        if (active_channel_count == 0) 
        {
            i2s_channel_disable(this->audio_chanel_handle);
            vTaskSuspend(this->audio_task_handle);
        } 
    }

    void Audio::pause_audio()
    {
        
    }

    void Audio::set_volume(unsigned int channel_number, float volume)
    {
        if (volume < 0.0) return;

        channels[channel_number]->volume = volume;
    }

    void Audio::add_channel_wav()
    {
        this->channels.push_back(new AudioChannelWav(this, this->channels.size()));
    }

    void Audio::set_sample_rate(unsigned int new_sample_rate)
    {
        if (active_channel_count != 0) return;
        if (new_sample_rate < 8000) return;

        i2s_std_clk_config_t conf = I2S_STD_CLK_DEFAULT_CONFIG(new_sample_rate);

        i2s_channel_reconfig_std_clock(this->audio_chanel_handle, &conf);
        this->sample_rate = new_sample_rate;
    }

    unsigned int Audio::get_sample_rate()
    {
        return this->sample_rate;
    }

    void IRAM_ATTR Audio::audio_task(void *arg)
    {
        Audio *audio = reinterpret_cast<Audio*>(arg);
        size_t bytes_written;
        int32_t sample_left, sample_right;
        sample16_t* stereo_sample;
        int16_t* mono_sample;
        sample16_t mixed_sample;

        vTaskSuspend(NULL);

        i2s_channel_enable(audio->audio_chanel_handle);

        while (true)
        {
            sample_left = sample_right = 0;
            for (unsigned int i = 0; i < audio->channels.size(); i++)
            {
                if (audio->channels[i]->buffer_bytes_left == 0) audio->channels[i]->read_decode_file();
                if (!audio->channels[i]->is_playing) continue; 

                if (audio->channels[i]->stereo)
                {
                    stereo_sample = reinterpret_cast<sample16_t*>(audio->channels[i]->channel_buffer + BUFFER_SIZE - audio->channels[i]->buffer_bytes_left);

                    stereo_sample->left *= audio->channels[i]->volume;
                    stereo_sample->right *= audio->channels[i]->volume;

                    int32_t a = sample_left, b = sample_right;
                    sample_left = a + stereo_sample->left - ((a * stereo_sample->left) >> 0x10);
                    sample_right = b + stereo_sample->right - ((b * stereo_sample->right) >> 0x10);

                    mixed_sample.left = static_cast<int16_t>(sample_left);
                    mixed_sample.right = static_cast<int16_t>(sample_right);

                    audio->channels[i]->buffer_bytes_left -= 4;
                }
                else
                {
                    mono_sample = reinterpret_cast<int16_t*>(audio->channels[i]->channel_buffer + BUFFER_SIZE - audio->channels[i]->buffer_bytes_left);

                    *mono_sample *= audio->channels[i]->volume;

                    int32_t a = sample_left;
                    sample_left = a + (*mono_sample) - ((a * (*mono_sample)) >> 0x10);
                    sample_right = sample_left;

                    mixed_sample.left = static_cast<int16_t>(sample_left);
                    mixed_sample.right = static_cast<int16_t>(sample_left);

                    audio->channels[i]->buffer_bytes_left -= 2;
                }
            }

            i2s_channel_write(audio->audio_chanel_handle, &mixed_sample, 4, &bytes_written, portMAX_DELAY);
        }

        vTaskDelete(NULL);
    }
}
