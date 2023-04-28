idf_build_get_property(target IDF_TARGET)

set(srcs "src/input.cpp"
         "src/mcp23008_input.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES "soc" "driver")