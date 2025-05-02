idf_build_get_property(target IDF_TARGET)

set(srcs)

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES "soc" "driver")