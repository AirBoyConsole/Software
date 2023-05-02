#include "mcp23008_input.hpp"

namespace airboy 
{
    MCP23008Input::MCP23008Input(i2c_port_t port, input_bus_cfg_t *conf) : Input()
    {
        this->chip_0_address = conf->addres_0;
        this->chip_1_address = conf->addres_1;

        this->port = port;
        this->interrupt = static_cast<gpio_num_t>(conf->interrupt);
        init_input();
    }

    esp_err_t MCP23008Input::register_read(uint8_t chip_address, uint8_t reg_addr, uint8_t *data)
    {
        //set the register to read
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (chip_address << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write_byte(cmd, reg_addr, true);
        i2c_master_stop(cmd);

        esp_err_t ret = i2c_master_cmd_begin(this->port, cmd, 1000 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        if (ret != ESP_OK) 
        {
            ESP_LOGE(INPUT_TAG, "Error one reading register %d", reg_addr);
            return ESP_FAIL;
        }

        //read the register
        cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (chip_address << 1) | I2C_MASTER_READ, true);
        i2c_master_read_byte(cmd, data, I2C_MASTER_NACK);
        i2c_master_stop(cmd);

        ret = i2c_master_cmd_begin(this->port, cmd, 1000 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        if (ret != ESP_OK) 
        {
            ESP_LOGE(INPUT_TAG, "Error two reading register %d", reg_addr);
            return ESP_FAIL;
        }

        return ESP_OK;
    }

    esp_err_t MCP23008Input::register_write(uint8_t chip_address, uint8_t reg_addr, uint8_t data)
    {
        //write the register
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (chip_address << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write_byte(cmd, reg_addr, true);
        i2c_master_write_byte(cmd, data, true);
        i2c_master_stop(cmd);

        esp_err_t ret = i2c_master_cmd_begin(this->port, cmd, 1000 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        if (ret != ESP_OK) 
        {
            ESP_LOGE(INPUT_TAG, "Error writing register at address %d", reg_addr);
            return ESP_FAIL;
        }

        return ESP_OK;
    }

    void MCP23008Input::init_input()
    {
        // config interrupt pin
        gpio_config_t io_conf;
        memset(&io_conf, 0, sizeof(gpio_config_t));
        io_conf.intr_type = GPIO_INTR_NEGEDGE;
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pin_bit_mask = (1ULL << this->interrupt);
        ESP_ERROR_CHECK(gpio_config(&io_conf));
        
        // initialize interrupt
        gpio_install_isr_service(0);
        gpio_isr_handler_add(this->interrupt, input_interrupt, NULL);

        // initialize expanders registers from predefined array
        uint8_t cmd = 0;
        while(input_vendor_specific_init[cmd].address != 0xff)
        {
            ESP_ERROR_CHECK(register_write(this->chip_0_address, input_vendor_specific_init[cmd].address, input_vendor_specific_init[cmd].data));
            ESP_ERROR_CHECK(register_write(this->chip_1_address, input_vendor_specific_init[cmd].address, input_vendor_specific_init[cmd].data));
            cmd++;
        }

        // read input to reset interrupt
        read_input();

        // create task that handles input
        xTaskCreatePinnedToCore(input_task, "get-input", 2048, this, 10, &input_handle, tskNO_AFFINITY);
    }
}
