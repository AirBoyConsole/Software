idf_build_get_property(target IDF_TARGET)

set(srcs "src/vector2.cpp" "src/math.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include")