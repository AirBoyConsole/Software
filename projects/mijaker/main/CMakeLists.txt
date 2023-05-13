idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES "airboy_engine")

add_compile_definitions(ENGINE_SPECIAL_OPTIMALIZATION)