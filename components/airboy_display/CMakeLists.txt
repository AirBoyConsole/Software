idf_build_get_property(target IDF_TARGET)

set(srcs "src/display.cpp" "src/display_ili9341.cpp" "src/framebuffer.cpp" "src/draw_shapes.cpp" "src/renderer.cpp" "src/full_renderer.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES "soc" "driver" "esp_lcd" "airboy_error" "airboy_math")