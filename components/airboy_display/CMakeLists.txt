idf_build_get_property(target IDF_TARGET)

set(srcs "src/display.cpp"
         "src/ili9341_display.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES "soc" "driver" "esp_lcd")