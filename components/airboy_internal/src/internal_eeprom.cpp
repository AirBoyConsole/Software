#include "internal_eeprom.hpp"

namespace airboy 
{   
    InternalEEprom::InternalEEprom(i2c_port_t port, uint8_t chip_address)
    {
        this->chip_address = chip_address;
        this->port = port;
   
        config_eeprom();
    }

    esp_err_t InternalEEprom::write_byte(uint8_t chip_address, uint16_t mem_address, uint8_t data) 
    {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (chip_address << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK);
        i2c_master_write_byte(cmd, mem_address >> 8, I2C_MASTER_ACK);
        i2c_master_write_byte(cmd, mem_address & 0xFF, I2C_MASTER_ACK);
        i2c_master_write_byte(cmd, data, I2C_MASTER_ACK);
        i2c_master_stop(cmd);

        esp_err_t ret = i2c_master_cmd_begin(this->port, cmd, 1000 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        if (ret != ESP_OK) 
        {
            ESP_LOGE(EEPROM_TAG, "Error writing to mem address %d", mem_address);
            return ESP_FAIL;
        }

        return ret;
    }

    esp_err_t InternalEEprom::write(uint8_t chip_address, uint16_t start_mem_address, uint8_t* data, size_t size) 
    {
        esp_err_t ret;

        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (chip_address << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK);
        i2c_master_write_byte(cmd, start_mem_address >> 8, I2C_MASTER_ACK);
        i2c_master_write_byte(cmd, start_mem_address & 0xFF, I2C_MASTER_ACK);

        size_t bytes_remaining = size;
        unsigned int current_address = start_mem_address;
        int first_write_size = ((this->page_size - 1) - start_mem_address % (this->page_size - 1)) + 1;
        if (start_mem_address % (this->page_size - 1) == 0 && start_mem_address != 0) first_write_size = 1;

        if (bytes_remaining <= first_write_size) 
        {
            i2c_master_write(cmd, data, bytes_remaining, I2C_MASTER_NACK);
        } else {
            i2c_master_write(cmd, data, first_write_size, I2C_MASTER_NACK);
            bytes_remaining -= first_write_size;
            current_address += first_write_size;
            i2c_master_stop(cmd);

            ret = i2c_master_cmd_begin(this->port, cmd, 1000 / portTICK_PERIOD_MS);
            i2c_cmd_link_delete(cmd);

            if (ret != ESP_OK) 
            {
                ESP_LOGE(EEPROM_TAG, "Error writing to mem address %d", current_address);
                return ESP_FAIL;
            }

            while (bytes_remaining > 0) 
            {
                cmd = i2c_cmd_link_create();
                vTaskDelay(20/portTICK_PERIOD_MS);

                i2c_master_start(cmd);
                i2c_master_write_byte(cmd, (chip_address << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK);
                i2c_master_write_byte(cmd, current_address >> 8, I2C_MASTER_ACK);
                i2c_master_write_byte(cmd, current_address & 0xFF, I2C_MASTER_ACK);

                if (bytes_remaining <= this->page_size) 
                {
                    i2c_master_write(cmd, data + (size-bytes_remaining), bytes_remaining, I2C_MASTER_NACK);
                    bytes_remaining = 0;
                } else {
                    i2c_master_write(cmd, data + (size-bytes_remaining), this->page_size, I2C_MASTER_NACK);
                    bytes_remaining -= this->page_size;
                    current_address += this->page_size;
                }
                i2c_master_stop(cmd);

                ret = i2c_master_cmd_begin(this->port, cmd, 1000 / portTICK_PERIOD_MS);
                i2c_cmd_link_delete(cmd);

                if (ret != ESP_OK) 
                {
                    ESP_LOGE(EEPROM_TAG, "Error writing to mem address %d", current_address);
                    return ESP_FAIL;
                }
            }
        }

        return ESP_OK;
    }

    esp_err_t InternalEEprom::read_byte(uint8_t chip_address, uint16_t mem_address, uint8_t* data) 
    {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (chip_address << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK);
        i2c_master_write_byte(cmd, mem_address << 8, I2C_MASTER_ACK);
        i2c_master_write_byte(cmd, mem_address & 0xFF, I2C_MASTER_ACK);
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (chip_address << 1) | I2C_MASTER_READ, I2C_MASTER_ACK);

        uint8_t temp;
        i2c_master_read_byte(cmd, &temp, I2C_MASTER_NACK);
        i2c_master_stop(cmd);
        esp_err_t ret = i2c_master_cmd_begin(this->port, cmd, 1000 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        if (ret != ESP_OK) 
        {
            ESP_LOGE(EEPROM_TAG, "Error reading from mem address %d", mem_address);
            return ESP_FAIL;
        }

        *data = temp;
        return ret;
    }

    esp_err_t InternalEEprom::read(uint8_t chip_address, uint16_t start_mem_address, uint8_t* data, size_t size) 
    {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (chip_address << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK);
        i2c_master_write_byte(cmd, start_mem_address << 8, I2C_MASTER_ACK);
        i2c_master_write_byte(cmd, start_mem_address & 0xFF, I2C_MASTER_ACK);
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (chip_address << 1) | I2C_MASTER_READ, I2C_MASTER_ACK);

        if (size > 1) {
            i2c_master_read(cmd, data, size - 1, I2C_MASTER_ACK);
        }
        i2c_master_read_byte(cmd, data + size - 1, I2C_MASTER_NACK);
        i2c_master_stop(cmd);
        esp_err_t ret = i2c_master_cmd_begin(this->port, cmd, 1000 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        if (ret != ESP_OK) 
        {
            ESP_LOGE(EEPROM_TAG, "Error reading from mem address %d", start_mem_address);
            return ESP_FAIL;
        }

        return ret;
    }

    void InternalEEprom::config_eeprom()
    {
        uint8_t temp[4];

        ESP_ERROR_CHECK(read(this->chip_address, 0x0000, temp, 4));

        //TODO: replace with function 4 bytes to int
        this->chip_size = temp[3];
        this->chip_size += temp[2] << 8;
        this->chip_size += temp[1] << 16;
        this->chip_size += temp[0] << 24;

        ESP_ERROR_CHECK(read_byte(this->chip_address, 0x0005, &temp[0]));
        this->page_size = temp[0];
    }

    void InternalEEprom::get_input_config(input_bus_cfg_t *conf)
    {
        uint8_t data[3];
        read(this->chip_address, 0x0026, data, 3);

        conf->interrupt = data[0];
        conf->addres_0 = data[1];
        conf->addres_1 = data[2];
    }

    uint8_t InternalEEprom::get_input_type()
    {
        uint8_t out;
        ESP_ERROR_CHECK(read_byte(this->chip_address, 0x0025, &out));
        return out;
    }

    void InternalEEprom::get_display_config(display_bus_cfg_t *conf)
    {
        uint8_t data[19];
        read(this->chip_address, 0x0012, data, 19);

        conf->bus_bits[0] = data[0];
        conf->bus_bits[1] = data[1];
        conf->bus_bits[2] = data[2];
        conf->bus_bits[3] = data[3];
        conf->bus_bits[4] = data[4];
        conf->bus_bits[5] = data[5];
        conf->bus_bits[6] = data[6];
        conf->bus_bits[7] = data[7];
        conf->CLK = data[8];
        conf->DC = data[9];
        conf->CS = data[10];
        conf->RST = data[11];
        conf->RD = data[12];
        conf->BL = data[13];

        conf->clock = data[17];
        conf->clock += data[16] << 8;
        conf->clock += data[15] << 16;
        conf->clock += data[14] << 24;

        conf->backlight = data[18];

        ESP_LOGI(EEPROM_TAG, "value: %d", conf->clock);
    }

    uint8_t InternalEEprom::get_display_type()
    {
        uint8_t out;
        ESP_ERROR_CHECK(read_byte(this->chip_address, 0x0011, &out));
        return out;
    }
}
