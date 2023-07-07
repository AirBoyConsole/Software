#pragma once
/*
 *
 * NOT TESTED YET!!!!
 * 
 */

#include <iostream>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_vfs_fat.h"
#include "wear_levelling.h"
#include "esp_check.h"
#include "esp_log.h"

#define SPIFLASH_TAG "spiflash"

namespace airboy {

#ifdef __cplusplus
extern "C" {
#endif

class SPIFlash 
{
    public:
        SPIFlash(const char* mount_point = "/spiflash", uint8_t max_open_files = 6, bool format_on_fail = false, const char * partition_label = "ffat");
        ~SPIFlash();

        bool format(bool full_wipe = 0, const char* partition_label = "ffat");
        uint64_t total_size();
        uint64_t free_size();
    private:
        const esp_partition_t *check_ffat_partition(const char* label);

        wl_handle_t wl_handle;
        const char* mount_point;
        
};

#ifdef __cplusplus
}
#endif

}

