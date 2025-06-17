#ifndef SIMIX_MUSIC_H
#define SIMIX_MUSIC_H

#include "simix.h"

#define OV_EXCLUDE_STATIC_CALLBACKS
#include <vorbis/vorbisfile.h>

namespace Simix
{

class MixOggMusic: public MixMusic
{
private:
    OggVorbis_File m_vorbis;   
    FSMgr::File *f = nullptr; 
    uint8_t *data = nullptr;
    int32_t dataSize = -1;
    int32_t startPos = 0;
    int32_t pos = 0;

    int oggword = 2;
    int oggbendian = 0;
    int oggsigned = 0;

public:
    // Create OGG stream from file, from current position 
    MixOggMusic(SDL_AudioFormat fmt, FSMgr::File *file, int32_t startPos = 0, int32_t size = 0);
    virtual ~MixOggMusic();

    virtual bool Read(int32_t blksize) override;

public:
    static size_t ovread(void* ptr, size_t size, size_t nmemb, void* data);
    static int ovseek(void* data, ogg_int64_t offset, int whence);
    static long ovtell(void* data);

    static ov_callbacks ovcallbacks;
};

};
#endif