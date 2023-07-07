#include "sdmmc.hpp"

namespace airboy 
{
    SDMMC::SDMMC(sdmmc_pinout_t *pinout, const char* mount_point, uint8_t max_open_files, bool mode_one_bit, int sdmmc_frequency, bool format_if_mount_failed) : mount_point(mount_point)
    {
        esp_vfs_fat_sdmmc_mount_config_t mount_config;
        memset(&mount_config, 0, sizeof(esp_vfs_fat_sdmmc_mount_config_t));
        mount_config.format_if_mount_failed = format_if_mount_failed;
        mount_config.max_files = max_open_files;
        mount_config.allocation_unit_size = 32 * 1024; //32KB

        sdmmc_host_t host = SDMMC_HOST_DEFAULT();
        host.max_freq_khz = sdmmc_frequency;

        sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
        if (mode_one_bit)   
        {
            slot_config.width = 1;
            slot_config.clk = static_cast<gpio_num_t>(pinout->clk);
            slot_config.cmd = static_cast<gpio_num_t>(pinout->cmd);
            slot_config.d0 = static_cast<gpio_num_t>(pinout->d0);
        }
        else
        {
            slot_config.width = 4;
            slot_config.clk = static_cast<gpio_num_t>(pinout->clk);
            slot_config.cmd = static_cast<gpio_num_t>(pinout->cmd);
            slot_config.d0 = static_cast<gpio_num_t>(pinout->d0);
            slot_config.d1 = static_cast<gpio_num_t>(pinout->d1);
            slot_config.d2 = static_cast<gpio_num_t>(pinout->d2);
            slot_config.d3 = static_cast<gpio_num_t>(pinout->d3);
        }

        slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;                

        esp_err_t ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &this->sd_card);
    
        if (ret != ESP_OK) 
        {
            ESP_LOGE(SDMMC_TAG, "Failed to initialize SD card.");
            if (ret == ESP_FAIL) 
            {
                ESP_LOGE(SDMMC_TAG, "Failed to mount filesystem.");
            } 
            else 
            {
                ESP_LOGE(SDMMC_TAG, "Failed to initialize the card (%s).", esp_err_to_name(ret));
            }
        }
    }

    SDMMC::~SDMMC()
    {
        esp_vfs_fat_sdcard_unmount(mount_point, this->sd_card);
    }

    uint64_t SDMMC::card_size()
    {
        if(!this->sd_card)  return 0;
        return this->sd_card->csd.capacity * this->sd_card->csd.sector_size;
    }

    uint64_t SDMMC::total_size()
    {
        FATFS* fs;
        DWORD fre_clust;

        auto res = f_getfree(this->mount_point, &fre_clust, &fs);
        if (res != FR_OK) return 0;

        return static_cast<uint64_t>((fs->n_fatent - 2) * fs->csize);;
    }

    uint64_t SDMMC::free_size()
    {
        FATFS* fs;
        DWORD fre_clust;

        auto res = f_getfree(this->mount_point, &fre_clust, &fs);
        if (res != FR_OK) return 0;

        return static_cast<uint64_t>(fre_clust * fs->csize);
    }
}
