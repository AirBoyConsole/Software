#include "internal_bus.hpp"

namespace airboy 
{
    esp_err_t InternalBus::init_bus()
    {
        this->bus_port = I2C_NUM_0;

        i2c_config_t conf;
        memset(&conf, 0, sizeof(i2c_config_t));
        conf.mode = I2C_MODE_MASTER;
        conf.sda_io_num = 48;
        conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
        conf.scl_io_num = 47;
        conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
        conf.master.clk_speed = 400000;

        i2c_param_config(this->bus_port, &conf);
        esp_err_t ret = i2c_driver_install(this->bus_port, conf.mode, 0, 0, 0);

        if (ret != ESP_OK) 
        {
            ESP_LOGE(BUS_TAG, "Internal bus configuration error");
            return ESP_FAIL;
        }

        return ESP_OK;
    }

    i2c_port_t InternalBus::get_port()
    {
        return this->bus_port;
    }
}
