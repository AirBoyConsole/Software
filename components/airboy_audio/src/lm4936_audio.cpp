#include "lm4936_audio.hpp"

namespace airboy 
{
    bool Audio::instance;

    LM4936Audio::LM4936Audio(i2c_port_t port, audio_bus_cfg_t &conf) : Audio(conf)
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
        ESP_ERROR_CHECK(register_write(0x9F));
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
