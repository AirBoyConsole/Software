idf_build_get_property(target IDF_TARGET)

set(srcs "src/internal_bus.cpp"
          "src/internal_eeprom.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES "soc" "driver" "airboy_display" "airboy_input")