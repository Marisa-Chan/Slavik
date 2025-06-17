#include "music_ogg.h"

namespace Simix
{

MixOggMusic::MixOggMusic(SDL_AudioFormat fmt, FSMgr::File *file, int32_t spos, int32_t size)
: f(file)
{
    pos = 0;
    if (!spos)
        startPos = (*f)->tell();
    else
        startPos = spos;

    if (!size)
    {
        (*f)->seek(0, SEEK_END);
        dataSize = (*f)->tell() - startPos;
    }
    else
        dataSize = size;
    
    (*f)->seek(startPos, SEEK_SET);

    int status = ov_open_callbacks(this, &m_vorbis, NULL, 0, ovcallbacks);

    vorbis_info *inf = ov_info(&m_vorbis, -1);

    chn = inf->channels;
    freq = inf->rate;      

    oggsigned = SDL_AUDIO_ISSIGNED(fmt);
    oggword = SDL_AUDIO_BITSIZE(fmt) >> 3;
    oggbendian = SDL_AUDIO_ISBIGENDIAN(fmt);

    queueEnd = false;
}


MixOggMusic::~MixOggMusic()
{
    ov_clear(&m_vorbis);

    if (data)
        delete[] data;
}


bool MixOggMusic::Read(int32_t blksize)
{
    if (queueEnd)
        return false;

    if (blksize == 0)
        blksize = chn * freq * oggword / 10;

    blkq.emplace_back();

    auto &blk = blkq.back();
    blk.resize(blksize);

    int len = ov_read(&m_vorbis, (char *)blk.data(), blk.size(), oggbendian, oggword, oggsigned, NULL);

    if (len <= 0)
    {
        queueEnd = true;    
        blkq.pop_back();
        return false;
    }

    if (len < blk.size())
        blk.resize(len);
    
    datalen += len;
    return true;
}

size_t MixOggMusic::ovread(void* ptr, size_t size, size_t nmemb, void* data)
{
    MixOggMusic *stream = static_cast<MixOggMusic *>(data);
    if (stream->data)
    {
        size_t readsz = size * nmemb;
        if (readsz > stream->dataSize - stream->pos)
            readsz = stream->dataSize - stream->pos;
        
        memcpy(ptr, stream->data + stream->pos, readsz);

        stream->pos += readsz;
        return readsz;
    }
    else if (stream->f->operator bool())
    {
        size_t want = size * nmemb;
        if (want > stream->dataSize - stream->pos)
            want = stream->dataSize - stream->pos;
        
        size_t readsz = (* stream->f)->read(ptr, want);
        stream->pos += readsz;
        return readsz;
    }

    return -1;
}

int MixOggMusic::ovseek(void* data, ogg_int64_t offset, int whence)
{
    MixOggMusic *stream = static_cast<MixOggMusic *>(data);
    if (stream->data)
    {
        int32_t want = 0;
        if (whence == SEEK_SET)
            want = offset;
        else if (whence == SEEK_CUR)
            want = stream->pos + offset;
        else if (whence == SEEK_END)
            want = stream->dataSize + offset;
        
        if (want < 0 || want > stream->dataSize)
            return -1;
        
        stream->pos = want;
        return 0;
    }
    else if (stream->f)
    {
        int32_t want = 0;
        if (whence == SEEK_SET)
            want = offset;
        else if (whence == SEEK_CUR)
            want = stream->pos + offset;
        else if (whence == SEEK_END)
            want = stream->dataSize + offset;
        
        if (want < 0 || want > stream->dataSize)
            return -1;
        
        stream->pos = want;

        return (*stream->f)->seek(stream->startPos + want, SEEK_SET);
    }

    return -1;
}

long MixOggMusic::ovtell(void* data)
{
    MixOggMusic *stream = static_cast<MixOggMusic *>(data);
    return stream->pos;
}

ov_callbacks MixOggMusic::ovcallbacks = {MixOggMusic::ovread, MixOggMusic::ovseek, NULL, MixOggMusic::ovtell};


}; // namespace Simix
