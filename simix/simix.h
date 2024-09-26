#ifndef SIMIX_H
#define SIMIX_H

#include <inttypes.h>
#include <vector>

#include <SDL2/SDL_audio.h>

namespace Simix
{

struct MixBuffer
{
    MixBuffer(int32_t len)
    {
        data.resize(len);
    }

    std::vector<uint8_t> data;
    uint32_t size = 0;
    uint32_t freq = 0;
    int channels = 1;
};


struct MixChannel
{
    bool active = false;

    int volume = SDL_MIX_MAXVOLUME;
    int pan = 0;
    
    int32_t loop = 0;
    int32_t loopStart = -1;
    int32_t loopEnd = -1;
    bool afterloop = false;

    MixBuffer *data = nullptr;
    int32_t pos = 0;
    uint8_t smplpos = 0;
    float pitch = 1.0f;
};

template<class T> inline constexpr const T& CLAMP(const T& v, const T& lo, const T& hi)
{ 
    if (v < lo) return lo; 
    else if (v > hi) return hi; 
    return v;
}

class Mixer
{    
public:
    bool Init(int32_t channels, int32_t freq = 48000, int32_t buffsz = 1024);

    MixBuffer *LoadSample(void *samples, uint32_t len, uint32_t freq, uint16_t format, uint32_t channels = 1);
    int32_t PlaySample(MixBuffer *buf, int volume = SDL_MIX_MAXVOLUME, int pan = 0, float pitch = 1.0);
    void UnloadSample(MixBuffer *buf);

    static void SDLCALL CB_MixChannels(void *udata, Uint8 *stream, int len);

    void MixBufferChannel(Uint8 *dst, int32_t dstlen, MixChannel *chn);

protected:
    int32_t AllocChannel();
    

protected:
    SDL_AudioDeviceID _devid = 0;
    SDL_AudioSpec _spec;

    std::vector<MixChannel> _channels;

    bool _opened = false;
};

int32_t DCBToVol(int32_t db);


};
#endif /* SIMIX_H */