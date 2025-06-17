#ifndef SIMIX_H
#define SIMIX_H

#include <inttypes.h>
#include <vector>
#include <queue>
#include <string>
#include "fsmgr.h"

#include <SDL2/SDL_audio.h>

namespace Simix
{

enum MUSICTYPE
{
    MUSICTYPE_OGG = 1
};


class MixMusic
{
public:
    virtual ~MixMusic() = default;

    virtual bool Read(int32_t blksize = 0) = 0;

public:
    std::deque< std::vector<uint8_t> > blkq;
    int32_t datalen = 0; //All blocks size in queue

    uint32_t blkpos = 0; //Position in the block
    uint8_t smplpos = 0; //Position in sample (for resampling)
    
    bool queueEnd = false; //Queue all data

    int chn = 2; //Number of channels
    int freq = 44100; //Music freq

    uint32_t streamed = 0; //Number of bytes streamed
};



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
    ~Mixer();

    bool Init(int32_t channels, int32_t freq = 48000, int32_t buffsz = 1024);
    void Close();

    MixBuffer *LoadSample(void *samples, uint32_t len, uint32_t freq, uint16_t format, uint32_t channels = 1);
    int32_t PlaySample(MixBuffer *buf, int volume = SDL_MIX_MAXVOLUME, int pan = 0, float pitch = 1.0);
    void UnloadSample(MixBuffer *buf);

    static void SDLCALL CB_MixChannels(void *udata, Uint8 *stream, int len);

    void MixBufferChannel(Uint8 *dst, int32_t dstlen, MixChannel *chn);

    static uint32_t MixData(uint8_t *dst, uint32_t dstlen, const uint8_t *src, uint32_t *srclen, uint8_t *smplpos, SDL_AudioFormat format, int chan, float fspeedfactor, uint8_t volume, int pan = 0);

    void PlayMusic(const std::string &path);
    void PlayMusic(int musicType, FSMgr::File *file, int32_t spos = 0, int32_t sz = 0);
    void FreeMusic();

    void MixingMusic(uint8_t *dst, int32_t dstlen);

    void QueueMusic();

    void StopMusic(bool stop);

    /* set music volume in db 0 - 100*/
    void VolumeMusic(int32_t db);

    bool Update();

    bool IsMusicEnd() const;

    static int _UpdateThread(void *data);


protected:
    int32_t AllocChannel();
    

protected:
    SDL_AudioDeviceID _devid = 0;
    SDL_AudioSpec _spec;

    std::vector<MixChannel> _channels;

    bool _opened = false;
    bool _closing = false;

    MixMusic * _music = nullptr;
    FSMgr::File _musicFile = nullptr;
    int _musicVolume = 128;
    int _musicPan = 0;
    bool _musicStop = false;

    SDL_Thread *_updateThread;
};

int32_t DCBToVol(int32_t db);


};
#endif /* SIMIX_H */