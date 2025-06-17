#include "simix.h"
#include "fmt/printf.h"

#include "music_ogg.h"

namespace Simix
{


void Mixer::PlayMusic(const std::string &path)
{
    if (_music)
        FreeMusic();
    
    _musicFile = FSMgr::Mgr::ReadFile(path);
    if (_musicFile)
    {
        SDL_LockAudioDevice(_devid);
        _music = new MixOggMusic(_spec.format, &_musicFile);
        SDL_UnlockAudioDevice(_devid);
    }
}


void Mixer::PlayMusic(int musicType, FSMgr::File *file, int32_t spos, int32_t sz)
{
    if (_music)
        FreeMusic();
    

    if (file)
    {
        SDL_LockAudioDevice(_devid);
        switch(musicType)
        {
            case MUSICTYPE_OGG:
                _music = new MixOggMusic(_spec.format, file, spos, sz);
                break;
            
            default:
                break;
        }
        
        SDL_UnlockAudioDevice(_devid);
    }
}

void Mixer::FreeMusic()
{
    if (_music)
    {
        SDL_LockAudioDevice(_devid);
        delete _music;

        _music = nullptr;

        _musicFile = nullptr;

        SDL_UnlockAudioDevice(_devid);
    }
}





void Mixer::MixingMusic(uint8_t *dst, int32_t dstlen)
{
    if (_musicStop)
        return;

    float speedf = (float)_music->freq / (float)_spec.freq;
    int32_t outpos = 0;
    while(outpos < dstlen)
    {
        if (_music->blkq.empty())
            break;
        
        auto& sblk = _music->blkq.front();
        
        uint32_t srcSz = sblk.size() - _music->blkpos;

        uint32_t writed = MixData(dst + outpos, dstlen - outpos, 
                                          sblk.data() + _music->blkpos, &srcSz, &_music->smplpos, 
                                          _spec.format, _music->chn, speedf, _musicVolume, _musicPan);

        outpos += writed;
        _music->blkpos += srcSz;
        _music->streamed += srcSz;

        if (_music->blkpos >= sblk.size())
        {
            _music->datalen -= sblk.size();
            _music->blkpos -= sblk.size();
            _music->blkq.pop_front();
        }
    }
}

void Mixer::QueueMusic()
{
    if (_musicStop)
        return;
    
    SDL_LockAudioDevice(_devid);
    if (_music && !_music->queueEnd)
    {
        const int32_t seclen = (SDL_AUDIO_BITSIZE(_spec.format) >> 3) * _music->freq * _music->chn;
        while (_music->datalen < seclen / 2)
        {
            if (!_music->Read(seclen / 10))
                break;
        }            
    }

    SDL_UnlockAudioDevice(_devid);
}

bool Mixer::IsMusicEnd() const
{
    if (!_music)
        return true;
    
    return _music->queueEnd && _music->blkq.empty();
}


void Mixer::StopMusic(bool stop)
{
    _musicStop = stop;
}

void Mixer::VolumeMusic(int32_t db)
{
    _musicVolume = DCBToVol(db);
}

}