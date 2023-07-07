#include "spiflash.hpp"

namespace airboy 
{   
    SPIFlash::SPIFlash(const char* mount_point, uint8_t max_open_files, bool format_on_fail, const char * partition_label)
    {
        this->mount_point = mount_point;

        // check if a partition with the specified label exists
        if (!check_ffat_partition(partition_label)) ESP_LOGE(SPIFLASH_TAG, "partition label not found");

        esp_vfs_fat_mount_config_t conf = 
        {
            .format_if_mount_failed = format_on_fail,
            .max_files = max_open_files,
            .allocation_unit_size = CONFIG_WL_SECTOR_SIZE,
            .disk_status_check_enable = false
        };

        esp_err_t err = esp_vfs_fat_spiflash_mount_rw_wl(mount_point, partition_label, &conf, &this->wl_handle);

        if(err != ESP_OK)
        {
            ESP_LOGE(SPIFLASH_TAG, "Mounting FFat partition failed! Error: %d", err);
        }
    }

    SPIFlash::~SPIFlash()
    {
        esp_vfs_fat_spiflash_unmount_rw_wl(mount_point, this->wl_handle);
    }

    bool SPIFlash::format(bool full_wipe, const char* partition_label)
    {
        esp_err_t result;

        // check if partition isn't mounted
        if(this->wl_handle != WL_INVALID_HANDLE)
        {
            ESP_LOGE(SPIFLASH_TAG, "Already Mounted!");
            return false;
        }

        wl_handle_t temp_handle;
        // Attempt to mount to see if there is already data
        const esp_partition_t *ffat_partition = check_ffat_partition(partition_label);
        if (!ffat_partition)
        {
            ESP_LOGE(SPIFLASH_TAG, "partition label not found");
            return false;
        } 
        result = wl_mount(ffat_partition, &temp_handle);

        if (result == ESP_OK) 
        {
            // Wipe disk- quick just wipes the FAT. Full zeroes the whole disk
            uint32_t wipe_size = full_wipe ? wl_size(temp_handle) : 16384;
            wl_erase_range(temp_handle, 0, wipe_size);
            wl_unmount(temp_handle);
        } else {
            return false;
            ESP_LOGE(SPIFLASH_TAG, "wl_mount failed!");
        }

        // Now do a mount with format_if_fail (which it will)
        esp_vfs_fat_mount_config_t conf = 
        {
            .format_if_mount_failed = true,
            .max_files = 1,
            .allocation_unit_size = CONFIG_WL_SECTOR_SIZE,
            .disk_status_check_enable = false
        };

        result = esp_vfs_fat_spiflash_mount_rw_wl("/format_ffat", partition_label, &conf, &temp_handle);
        if (result != ESP_OK)
        {
            return false;
            ESP_LOGE(SPIFLASH_TAG, "esp_vfs_fat_spiflash_mount failed!");
        }
        esp_vfs_fat_spiflash_unmount_rw_wl("/format_ffat", temp_handle);
        
        
        return true;
    }

    uint64_t SPIFlash::total_size()
    {
        FATFS* fs;
        DWORD fre_clust;

        auto res = f_getfree(this->mount_point, &fre_clust, &fs);
        if (res != FR_OK) return 0;

        return static_cast<uint64_t>((fs->n_fatent - 2) * fs->csize);;
    }

    uint64_t SPIFlash::free_size()
    {
        FATFS* fs;
        DWORD fre_clust;

        auto res = f_getfree(this->mount_point, &fre_clust, &fs);
        if (res != FR_OK) return 0;

        return static_cast<uint64_t>(fre_clust * fs->csize);
    }

    const esp_partition_t* SPIFlash::check_ffat_partition(const char* label)
    {
        const esp_partition_t* ck_part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, label);
        if (!ck_part) return nullptr;
        return ck_part;
    }
}
