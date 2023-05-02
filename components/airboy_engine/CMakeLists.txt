idf_build_get_property(target IDF_TARGET)

set(srcs "src/engine.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES "airboy_display" "airboy_input" "airboy_internal")