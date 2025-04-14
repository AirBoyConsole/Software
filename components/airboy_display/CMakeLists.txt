idf_build_get_property(target IDF_TARGET)

set(srcs "src/display.cpp" "src/display_ili9341.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES "soc" "driver" "esp_lcd")