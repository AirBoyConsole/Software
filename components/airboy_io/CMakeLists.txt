idf_build_get_property(target IDF_TARGET)

set(srcs "src/sdmmc.cpp"
          "src/spiflash.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES "soc" "fatfs")