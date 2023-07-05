#include "lm4936_audio.hpp"

namespace airboy 
{
    bool Audio::instance;

    LM4936Audio::LM4936Audio(i2c_port_t port, audio_bus_cfg_t &conf)
    {
        this->chip_address = conf.address;
        this->port = port;

        init_audio(conf);
    }

    LM4936Audio::~LM4936Audio() {}

    Audio* LM4936Audio::create_instance(i2c_port_t port, audio_bus_cfg_t &conf)
    {
        if (LM4936Audio::instance)
        {
            ESP_LOGE(AUDIO_TAG, "cannot create more than one instance of audio");
            return nullptr;
        } 

        LM4936Audio::instance = true;
        Audio *audio = new LM4936Audio(port, conf);
        return audio;
    }

    void LM4936Audio::init_audio(audio_bus_cfg_t &conf)
    {
        ESP_ERROR_CHECK(register_write(0x05));
        ESP_ERROR_CHECK(register_write(0x93));

        i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);

        i2s_new_channel(&chan_cfg, &this->audio_chanel_handle, NULL);

        i2s_std_config_t std_cfg;
        memset(&std_cfg, 0, sizeof(i2s_std_config_t));
        std_cfg.clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(48000);
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

    esp_err_t LM4936Audio::register_write(uint8_t data)
    {
        //write the register
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (this->chip_address << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write_byte(cmd, data, true);
        i2c_master_stop(cmd);

        esp_err_t ret = i2c_master_cmd_begin(this->port, cmd, 1000 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        if (ret != ESP_OK) 
        {
            ESP_LOGE(AUDIO_TAG, "Error writing register");
            return ESP_FAIL;
        }

        return ESP_OK;
    }
}
