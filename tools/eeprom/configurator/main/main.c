#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_check.h"
#include "esp_log.h"

#define BUS_PORT I2C_NUM_0
#define TAG "EEPROM"

esp_err_t eeprom_write_byte(uint8_t chip_address, uint16_t mem_address, uint8_t data) 
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (chip_address << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, mem_address >> 8, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, mem_address & 0xFF, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, data, I2C_MASTER_ACK);
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(BUS_PORT, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Error writing to mem address %d", mem_address);
        return ESP_FAIL;
    }

    return ret;
}

esp_err_t eeprom_read_byte(uint8_t chip_address, uint16_t mem_address, uint8_t* data) 
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
        esp_err_t ret = i2c_master_cmd_begin(BUS_PORT, cmd, 1000 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        if (ret != ESP_OK) 
        {
            ESP_LOGE(TAG, "Error reading from mem address %d", mem_address);
            return ESP_FAIL;
        }

        *data = temp;
        return ret;
    }

uint8_t configuration[] = {
    0x00, 0x00, 0x40, 0x00, //eeprom size
    0x40,                   //eeprom page size
    0x0A,                   //eeprom write delay
    0x32, 0x31, 0x41,       //revision string (21A) - 2.1A
    0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //info on occupied blocks
    0x00,                                           //lcd type 0 = ili9341
    0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, //lcd bus pinout
    0x10, 0x11, 0x12, 0x0E, 0x0F, 0x02,             //lcd controll pinout
    0x03, 0x93, 0x87, 0x00,                         //lcd clock
    0x00,                   //backlight level
    0x00,                   //input type 0 = mcp23008
    0x26,                   //interrupt pin
    0x20, 0x21,             //expanders addresses
    0x00,                   //audio configuration 0 = pcm5100a + lm4936
    0x03, 0x04, 0x05,       //audio bus pinout
    0xC6,                   //amp address
    0x99,                   //audio volume
    0x01,                   //battery check pin

};

void app_main(void)
{
    i2c_config_t conf;
    memset(&conf, 0, sizeof(i2c_config_t));
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = 48;
    conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
    conf.scl_io_num = 47;
    conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
    conf.master.clk_speed = 400000;

    i2c_param_config(BUS_PORT, &conf);
    ESP_ERROR_CHECK(i2c_driver_install(BUS_PORT, conf.mode, 0, 0, 0));

    ESP_LOGI(TAG, "done %d", sizeof(configuration));

    for (int i = 0; i < sizeof(configuration); i++)
    {
        eeprom_write_byte(0x50, i, configuration[i]);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    for (int i = 0; i < sizeof(configuration); i++)
    {
        uint8_t data_out[1];
        eeprom_read_byte(0x50, i, data_out);
        ESP_LOGI(TAG, "data at addtress %d = %d", i, data_out[0]);
    }

    ESP_LOGI(TAG, "done");
}