idf_build_get_property(target IDF_TARGET)

set(srcs "src/generic_display.cpp"
         "src/ili9341_display.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    PRIV_INCLUDE_DIRS "private_include"
                    REQUIRES "soc" "driver" "esp_lcd")