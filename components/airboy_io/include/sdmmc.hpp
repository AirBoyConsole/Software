#pragma once

#include <iostream>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "esp_check.h"
#include "esp_log.h"

namespace airboy {

#ifdef __cplusplus
extern "C" {
#endif

#define SDMMC_TAG "sdmmc"

/**
 * @brief sd card pinout struct
 */
typedef struct {
    uint8_t clk;  // 0xFF - end 
    uint8_t cmd;
    uint8_t d0;
    uint8_t d1;
    uint8_t d2;
    uint8_t d3;
}sdmmc_pinout_t;

class SDMMC 
{
    public:
        SDMMC(sdmmc_pinout_t *pinout, const char* mount_point = "/sdcard", uint8_t max_open_files = 32, bool mode_one_bit = true, int sdmmc_frequency = SDMMC_FREQ_HIGHSPEED, bool format_if_mount_failed = false);
        ~SDMMC();

        uint64_t card_size();
        uint64_t total_size();
        uint64_t free_size();
    private:
        sdmmc_card_t* sd_card;
        const char* mount_point;
};

#ifdef __cplusplus
}
#endif
}

