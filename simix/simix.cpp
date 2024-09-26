#include "simix.h"
#include <SDL2/SDL.h>

namespace Simix
{

bool Mixer::Init(int32_t channels, int32_t freq, int32_t buffsz)
{
    if (!SDL_WasInit(SDL_INIT_AUDIO)) {
        if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
            return false;
        }
    }

    SDL_AudioSpec desired;
    desired.callback = CB_MixChannels;
    desired.userdata = this;
    desired.channels = 2;
    desired.format = AUDIO_S16SYS;
    desired.freq = freq;
    desired.samples = buffsz;
    desired.padding = 0;
    desired.silence = 0;
    desired.size = 0;

    _devid = SDL_OpenAudioDevice(0, 0, &desired, &_spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE | SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);


    _channels.resize(channels);

    SDL_PauseAudioDevice(_devid, 0);

    _opened = true;
    return true;
}

MixBuffer *Mixer::LoadSample(void *samples, uint32_t len, uint32_t freq, uint16_t format, uint32_t channels)
{
    const int32_t sampleSize = ( SDL_AUDIO_BITSIZE(format) / 8 ) * channels;
    const int32_t waveSize = len & ~(sampleSize - 1);
    MixBuffer *sample = nullptr;

    if (format != _spec.format ||
        channels > 2)
    {
        //Convert samples format
        SDL_AudioCVT wavecvt;
        int32_t outchan = channels;
        if (channels > 2)
            outchan = 2;
        SDL_BuildAudioCVT(&wavecvt, format, channels, freq, _spec.format, channels, freq);

        wavecvt.len = waveSize;

        sample = new MixBuffer(wavecvt.len * wavecvt.len_mult);
        wavecvt.buf = (Uint8 *)sample->data.data();

        memcpy(wavecvt.buf, samples, wavecvt.len);
        
        SDL_ConvertAudio(&wavecvt);
        sample->data.resize(wavecvt.len_cvt);
        sample->channels = outchan;
        sample->size = wavecvt.len_cvt;
    }
    else
    {
        sample = new MixBuffer(waveSize);
        memcpy(sample->data.data(), samples, waveSize);
        sample->channels = channels;
        sample->size = waveSize;
    }

    sample->freq = freq;
    return sample;
}

int32_t Mixer::PlaySample(MixBuffer *buf, int volume, int pan, float pitch)
{
    SDL_LockAudioDevice(_devid);
    const int32_t chId = AllocChannel();
    if (chId != -1)
    {    
        MixChannel &channel = _channels.at(chId);
        channel.pos = 0;
        channel.smplpos = 0;
        channel.loop = 0;
        channel.loopEnd = -1;
        channel.loopStart = -1;
        channel.afterloop = false;
        channel.data = buf;
        channel.volume = CLAMP<int32_t>(volume, 0, SDL_MIX_MAXVOLUME);
        channel.pan = CLAMP<int32_t>(pan, -SDL_MIX_MAXVOLUME, SDL_MIX_MAXVOLUME);
        channel.pitch = CLAMP<float>(pitch, 0.01, 10.0);
    }
    SDL_UnlockAudioDevice(_devid);
    return chId;    
}

void Mixer::UnloadSample(MixBuffer *buf)
{
    SDL_LockAudioDevice(_devid);
    for(int32_t i = 0; i < _channels.size(); ++i)
    {
        if (_channels[i].data == buf)
        {
            _channels[i].active = false;
            _channels[i].data = nullptr;
        }
    }
    SDL_UnlockAudioDevice(_devid);

    delete buf;
}


void SDLCALL Mixer::CB_MixChannels(void *udata, Uint8 *stream, int len)
{
    Mixer *mx = (Mixer *)udata;

    memset(stream, 0, len);
    
    for(int32_t i = 0; i < mx->_channels.size(); ++i)
    {
        MixChannel &channel = mx->_channels[i];
        if (!channel.active)
            continue;

        mx->MixBufferChannel(stream, len, &channel);
    }
}

int32_t Mixer::AllocChannel()
{
    for(int32_t i = 0; i < _channels.size(); ++i)
    {
        if (_channels[i].active == false)
        {
            _channels[i].active = true;
            return i;
        }
    }

    return -1;
}



}