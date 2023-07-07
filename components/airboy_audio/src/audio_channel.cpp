#include "audio_channel.hpp"

namespace airboy 
{
    AudioChannel::AudioChannel(Audio* audio, unsigned int id) : audio(audio), channel_id(id) {}
    AudioChannel::~AudioChannel() {}
}
