idf_build_get_property(target IDF_TARGET)

set(srcs "src/engine.cpp" "src/renderer.cpp" "src/camera.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES "esp_timer" "airboy_display" "airboy_input" "airboy_internal" "airboy_math")