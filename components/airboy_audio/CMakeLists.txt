idf_build_get_property(target IDF_TARGET)

set(srcs "src/audio.cpp"
         "src/lm4936_audio.cpp"
         "src/audio_channel.cpp"
         "src/audio_channel_wav.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES "soc" "driver" )