#include "simix.h"
#include <algorithm>
#include <SDL2/SDL.h>

namespace Simix
{

/*from SDL*/

static const uint8_t mix8[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
    0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
    0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24,
    0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A,
    0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45,
    0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
    0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B,
    0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
    0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71,
    0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C,
    0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92,
    0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D,
    0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8,
    0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3,
    0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE,
    0xBF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9,
    0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4,
    0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA,
    0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5,
    0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

/* The volume ranges from 0 - 128 */
#define ADJUST_VOLUME(s, v)     ((s) = ((s) * (v)) / SDL_MIX_MAXVOLUME)
#define ADJUST_VOLUME_U8(s, v)  ((s) = ((((s) - 128) * (v)) / SDL_MIX_MAXVOLUME) + 128)
#define ADJUST_VOLUME_U16(s, v) ((s) = ((((s) - 32768) * (v)) / SDL_MIX_MAXVOLUME) + 32768)

#define GETFINTPOS(v) (v >> 8)
#define GETFRACPOS(v) (v & 0xFF)


//Hacky internal func
inline bool CheckEnd(MixChannel *chn, uint32_t *pos, uint32_t *fpos, uint32_t *stpos, uint32_t *endpos, uint32_t realEnd)
{
    if (*pos < *endpos)
        return false;

    if (chn->loop == 0)
    {
        chn->active = false;
        return true;
    }

    *fpos = chn->smplpos = GETFRACPOS(*fpos);

    if (chn->loopStart > 0)
        *stpos = chn->loopStart;
    else
        *stpos = 0;

    *pos = *stpos;

    if (chn->loop > 0)
    {
        chn->loop--;
        if (chn->loop == 0 && chn->afterloop)
            *endpos = realEnd;
    }

    return false;
}

void Mixer::MixBufferChannel(Uint8 *dst, int32_t dstlen, MixChannel *chn)
{
    MixBuffer *buf = chn->data;

    const float fspeedfactor = ((float)buf->freq / (float)_spec.freq) * chn->pitch;
    const uint32_t speedfactor = fspeedfactor * 256.0f;

    const int32_t lvlm = chn->pan > 0 ? chn->volume *  chn->pan / 128 : chn->volume;
    const int32_t rvlm = chn->pan < 0 ? chn->volume * -chn->pan / 128 : chn->volume;

    const int32_t SampleSize = ( SDL_AUDIO_BITSIZE(_spec.format) / 8 );
    const int32_t FullSampleSize = SampleSize * buf->channels;
    const uint32_t srclen = buf->size & ~(FullSampleSize - 1);

    uint32_t endpos = srclen ;

    if (chn->loop != 0 && chn->loopEnd > 0)
        endpos = chn->loopEnd;

    uint32_t fpos = chn->smplpos;
    uint32_t stpos = chn->pos;    

    if (buf->channels >= 2)
    {
        switch(_spec.format)
        {
            case AUDIO_U8:
            {
                dstlen /= 2;

                while (dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const uint8_t *src = (uint8_t *)(buf->data.data() + pos);
                    uint8_t lsmpl = src[0];
                    uint8_t rsmpl = src[1];
                    ADJUST_VOLUME_U8(rsmpl, rvlm);
                    ADJUST_VOLUME_U8(lsmpl, lvlm);

                    *dst = mix8[*dst + lsmpl];
                    ++dst;
                    *dst = mix8[*dst + rsmpl];
                    ++dst;

                    fpos += speedfactor;
                }
                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            }
            break;

            case AUDIO_S8:
            {
                int8_t *dst8 = (int8_t *)dst;

                dstlen /= 2;

                while (dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const int8_t *src = (int8_t *)(buf->data.data() + pos);
                    int8_t lsmpl = src[0];
                    int8_t rsmpl = src[1];
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst8 = CLAMP<int16_t>((int16_t)*dst8 + (int16_t)lsmpl, SDL_MIN_SINT8, SDL_MAX_SINT8);
                    ++dst8;
                    *dst8 = CLAMP<int16_t>((int16_t)*dst8 + (int16_t)rsmpl, SDL_MIN_SINT8, SDL_MAX_SINT8);
                    ++dst8;
                    
                    fpos += speedfactor;
                }
                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            }
            break;

            case AUDIO_S16LSB:
            {
                int16_t *dst16 = (int16_t *)dst;
                
                dstlen /= 4;

                while(dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const int16_t *src = (int16_t *)(buf->data.data() + pos);

                    int32_t lsmpl = SDL_SwapLE16(src[0]);
                    int32_t rsmpl = SDL_SwapLE16(src[1]);
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst16 = SDL_SwapLE16( CLAMP<int32_t>((int32_t)SDL_SwapLE16(*dst16) + lsmpl, SDL_MIN_SINT16, SDL_MAX_SINT16) );
                    ++dst16;
                    *dst16 = SDL_SwapLE16( CLAMP<int32_t>((int32_t)SDL_SwapLE16(*dst16) + rsmpl, SDL_MIN_SINT16, SDL_MAX_SINT16) );
                    ++dst16;

                    fpos += speedfactor;
                }

                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            } 
            break;

            case AUDIO_S16MSB:
            {
                int16_t *dst16 = (int16_t *)dst;
                
                dstlen /= 4;

                while(dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const int16_t *src = (int16_t *)(buf->data.data() + pos);

                    int32_t lsmpl = SDL_SwapBE16(src[0]);
                    int32_t rsmpl = SDL_SwapBE16(src[1]);
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst16 = SDL_SwapBE16( CLAMP<int32_t>((int32_t)SDL_SwapBE16(*dst16) + lsmpl, SDL_MIN_SINT16, SDL_MAX_SINT16) );
                    ++dst16;
                    *dst16 = SDL_SwapBE16( CLAMP<int32_t>((int32_t)SDL_SwapBE16(*dst16) + rsmpl, SDL_MIN_SINT16, SDL_MAX_SINT16) );
                    ++dst16;

                    fpos += speedfactor;
                }

                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            } 
            break;

            case AUDIO_U16LSB:
            {
                uint16_t *dst16 = (uint16_t *)dst;
                
                dstlen /= 4;

                while(dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const uint16_t *src = (uint16_t *)(buf->data.data() + pos);

                    int32_t lsmpl = SDL_SwapLE16(src[0]);
                    int32_t rsmpl = SDL_SwapLE16(src[1]);
                    ADJUST_VOLUME_U16(rsmpl, rvlm);
                    ADJUST_VOLUME_U16(lsmpl, lvlm);

                    *dst16 = SDL_SwapLE16( CLAMP<int32_t>(((int32_t)SDL_SwapLE16(*dst16) + lsmpl) - 32768 * 2,
                                                            SDL_MIN_SINT16, SDL_MAX_SINT16) + 32768);
                    ++dst16;
                    *dst16 = SDL_SwapLE16( CLAMP<int32_t>(((int32_t)SDL_SwapLE16(*dst16) + rsmpl) - 32768 * 2,
                                                            SDL_MIN_SINT16, SDL_MAX_SINT16) + 32768 );
                    ++dst16;

                    fpos += speedfactor;
                }

                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            } 
            break;

            case AUDIO_U16MSB:
            {
                uint16_t *dst16 = (uint16_t *)dst;
                
                dstlen /= 4;

                while(dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;

                    const uint16_t *src = (uint16_t *)(buf->data.data() + pos);

                    int32_t lsmpl = SDL_SwapBE16(src[0]);
                    int32_t rsmpl = SDL_SwapBE16(src[1]);
                    ADJUST_VOLUME_U16(rsmpl, rvlm);
                    ADJUST_VOLUME_U16(lsmpl, lvlm);

                    *dst16 = SDL_SwapBE16( CLAMP<int32_t>(((int32_t)SDL_SwapBE16(*dst16) + lsmpl) - 32768 * 2,
                                                            SDL_MIN_SINT16, SDL_MAX_SINT16) + 32768);
                    ++dst16;
                    *dst16 = SDL_SwapBE16( CLAMP<int32_t>(((int32_t)SDL_SwapBE16(*dst16) + rsmpl) - 32768 * 2,
                                                            SDL_MIN_SINT16, SDL_MAX_SINT16) + 32768 );
                    ++dst16;

                    fpos += speedfactor;
                }

                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            } 
            break;

            case AUDIO_S32LSB:
            {
                int32_t *dst32 = (int32_t *)dst;
                
                dstlen /= 8;

                while(dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;

                    const int32_t *src = (int32_t *)(buf->data.data() + pos);
                    
                    int64_t lsmpl = SDL_SwapLE32(src[0]);
                    int64_t rsmpl = SDL_SwapLE32(src[1]);
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst32 = SDL_SwapLE32( CLAMP<int64_t>(((int64_t)SDL_SwapLE32(*dst32) + lsmpl), SDL_MIN_SINT32, SDL_MAX_SINT32));
                    ++dst32;
                    *dst32 = SDL_SwapLE32( CLAMP<int64_t>(((int64_t)SDL_SwapLE32(*dst32) + rsmpl), SDL_MIN_SINT32, SDL_MAX_SINT32));
                    ++dst32;

                    fpos += speedfactor;
                }

                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            } 
            break;

            case AUDIO_S32MSB:
            {
                int32_t *dst32 = (int32_t *)dst;
                
                dstlen /= 8;

                while(dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const int32_t *src = (int32_t *)(buf->data.data() + pos);

                    int64_t lsmpl = SDL_SwapBE32(src[0]);
                    int64_t rsmpl = SDL_SwapBE32(src[1]);
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst32 = SDL_SwapBE32( CLAMP<int64_t>(((int64_t)SDL_SwapBE32(*dst32) + lsmpl), SDL_MIN_SINT32, SDL_MAX_SINT32));
                    ++dst32;
                    *dst32 = SDL_SwapBE32( CLAMP<int64_t>(((int64_t)SDL_SwapBE32(*dst32) + rsmpl), SDL_MIN_SINT32, SDL_MAX_SINT32));
                    ++dst32;

                    fpos += speedfactor;
                }

                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            } 
            break;

            case AUDIO_F32LSB:
            {
                float *fdst32 = (float *)dst;
                
                dstlen /= 8;

                const float fmaxvolume = 1.0f / ((float)SDL_MIX_MAXVOLUME);
                /* !!! FIXME: are these right? */
                const double max_audioval = 3.402823466e+38F;
                const double min_audioval = -3.402823466e+38F;

                while(dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const float *src = (float *)(buf->data.data() + pos);

                    double lsmpl = SDL_SwapFloatLE(src[0]) * (float)lvlm * fmaxvolume;
                    double rsmpl = SDL_SwapFloatLE(src[1]) * (float)rvlm * fmaxvolume;

                    *fdst32 = SDL_SwapFloatLE( CLAMP<double>(((double)SDL_SwapFloatLE(*fdst32) + lsmpl), min_audioval, max_audioval));
                    ++fdst32;
                    *fdst32 = SDL_SwapFloatLE( CLAMP<double>(((double)SDL_SwapFloatLE(*fdst32) + rsmpl), min_audioval, max_audioval));
                    ++fdst32;

                    fpos += speedfactor;
                }

                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            } 
            break;

            case AUDIO_F32MSB:
            {
                float *fdst32 = (float *)dst;
                
                dstlen /= 8;

                const float fmaxvolume = 1.0f / ((float)SDL_MIX_MAXVOLUME);
                /* !!! FIXME: are these right? */
                const double max_audioval = 3.402823466e+38F;
                const double min_audioval = -3.402823466e+38F;

                while(dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const float *src = (float *)(buf->data.data() + pos);
                    
                    double lsmpl = SDL_SwapFloatBE(src[0]) * (float)lvlm * fmaxvolume;
                    double rsmpl = SDL_SwapFloatBE(src[1]) * (float)rvlm * fmaxvolume;

                    *fdst32 = SDL_SwapFloatBE( CLAMP<double>(((double)SDL_SwapFloatBE(*fdst32) + lsmpl), min_audioval, max_audioval));
                    ++fdst32;
                    *fdst32 = SDL_SwapFloatBE( CLAMP<double>(((double)SDL_SwapFloatBE(*fdst32) + rsmpl), min_audioval, max_audioval));
                    ++fdst32;

                    fpos += speedfactor;
                }

                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            } 
            break;
            
            default:
                //printf("No mixer for format %x\n", _spec.format);
                break;
        }
    }
    else
    {
        switch(_spec.format)
        {
            case AUDIO_U8:
            {
                uint8_t lsmpl, rsmpl;
                
                dstlen /= 2;

                while (dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos);

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const uint8_t *src = (uint8_t *)(buf->data.data() + pos);

                    lsmpl = rsmpl = src[ 0 ];
                    ADJUST_VOLUME_U8(rsmpl, rvlm);
                    ADJUST_VOLUME_U8(lsmpl, lvlm);

                    *dst = mix8[*dst + lsmpl];
                    ++dst;
                    *dst = mix8[*dst + rsmpl];
                    ++dst;

                    fpos += speedfactor;
                }
                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos);
            }
            break;

            case AUDIO_S8:
            {
                int8_t *dst8 = (int8_t *)dst;
                int8_t lsmpl, rsmpl;
                
                dstlen /= 2;

                while (dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos);

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const int8_t *src = (int8_t *)(buf->data.data() + pos);
                    
                    lsmpl = rsmpl = src[ 0 ];
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst8 = CLAMP<int16_t>((int16_t)*dst8 + (int16_t)lsmpl, SDL_MIN_SINT8, SDL_MAX_SINT8);
                    ++dst8;
                    *dst8 = CLAMP<int16_t>((int16_t)*dst8 + (int16_t)rsmpl, SDL_MIN_SINT8, SDL_MAX_SINT8);
                    ++dst8;
                    
                    fpos += speedfactor;
                }
                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos);
            }
            break;

            case AUDIO_S16LSB:
            {
                int32_t lsmpl, rsmpl;

                int16_t *dst16 = (int16_t *)dst;
                
                dstlen /= 4;

                while(dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const int16_t *src = (int16_t *)(buf->data.data() + pos);

                    lsmpl = rsmpl = SDL_SwapLE16(src[0]);
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst16 = SDL_SwapLE16( CLAMP<int32_t>((int32_t)SDL_SwapLE16(*dst16) + lsmpl, SDL_MIN_SINT16, SDL_MAX_SINT16) );
                    ++dst16;
                    *dst16 = SDL_SwapLE16( CLAMP<int32_t>((int32_t)SDL_SwapLE16(*dst16) + rsmpl, SDL_MIN_SINT16, SDL_MAX_SINT16) );
                    ++dst16;

                    fpos += speedfactor;
                }

                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            } 
            break;

            case AUDIO_S16MSB:
            {
                int32_t lsmpl, rsmpl;

                int16_t *dst16 = (int16_t *)dst;
                
                dstlen /= 4;

                while(dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const int16_t *src = (int16_t *)(buf->data.data() + pos);

                    lsmpl = rsmpl = SDL_SwapBE16(src[0]);
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst16 = SDL_SwapBE16( CLAMP<int32_t>((int32_t)SDL_SwapBE16(*dst16) + lsmpl, SDL_MIN_SINT16, SDL_MAX_SINT16) );
                    ++dst16;
                    *dst16 = SDL_SwapBE16( CLAMP<int32_t>((int32_t)SDL_SwapBE16(*dst16) + rsmpl, SDL_MIN_SINT16, SDL_MAX_SINT16) );
                    ++dst16;

                    fpos += speedfactor;
                }

                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            } 
            break;

            case AUDIO_U16LSB:
            {
                int32_t lsmpl, rsmpl;

                uint16_t *dst16 = (uint16_t *)dst;
                
                dstlen /= 4;

                while(dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const uint16_t *src = (uint16_t *)(buf->data.data() + pos);
                    
                    lsmpl = rsmpl = SDL_SwapLE16(src[0]);
                    ADJUST_VOLUME_U16(rsmpl, rvlm);
                    ADJUST_VOLUME_U16(lsmpl, lvlm);

                    *dst16 = SDL_SwapLE16( CLAMP<int32_t>(((int32_t)SDL_SwapLE16(*dst16) + lsmpl) - 32768 * 2,
                                                            SDL_MIN_SINT16, SDL_MAX_SINT16) + 32768);
                    ++dst16;
                    *dst16 = SDL_SwapLE16( CLAMP<int32_t>(((int32_t)SDL_SwapLE16(*dst16) + rsmpl) - 32768 * 2,
                                                            SDL_MIN_SINT16, SDL_MAX_SINT16) + 32768 );
                    ++dst16;

                    fpos += speedfactor;
                }

                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            } 
            break;

            case AUDIO_U16MSB:
            {
                int32_t lsmpl, rsmpl;

                uint16_t *dst16 = (uint16_t *)dst;
                
                dstlen /= 4;

                while(dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const uint16_t *src = (uint16_t *)(buf->data.data() + pos);

                    lsmpl = rsmpl = SDL_SwapBE16(src[0]);
                    ADJUST_VOLUME_U16(rsmpl, rvlm);
                    ADJUST_VOLUME_U16(lsmpl, lvlm);

                    *dst16 = SDL_SwapBE16( CLAMP<int32_t>(((int32_t)SDL_SwapBE16(*dst16) + lsmpl) - 32768 * 2,
                                                            SDL_MIN_SINT16, SDL_MAX_SINT16) + 32768);
                    ++dst16;
                    *dst16 = SDL_SwapBE16( CLAMP<int32_t>(((int32_t)SDL_SwapBE16(*dst16) + rsmpl) - 32768 * 2,
                                                            SDL_MIN_SINT16, SDL_MAX_SINT16) + 32768 );
                    ++dst16;

                    fpos += speedfactor;
                }

                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            } 
            break;

            case AUDIO_S32LSB:
            {
                int64_t lsmpl, rsmpl;

                int32_t *dst32 = (int32_t *)dst;
                
                dstlen /= 8;

                while(dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const int32_t *src = (int32_t *)(buf->data.data() + pos);
                    
                    lsmpl = rsmpl = SDL_SwapLE32(src[0]);
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst32 = SDL_SwapLE32( CLAMP<int64_t>(((int64_t)SDL_SwapLE32(*dst32) + lsmpl), SDL_MIN_SINT32, SDL_MAX_SINT32));
                    ++dst32;
                    *dst32 = SDL_SwapLE32( CLAMP<int64_t>(((int64_t)SDL_SwapLE32(*dst32) + rsmpl), SDL_MIN_SINT32, SDL_MAX_SINT32));
                    ++dst32;

                    fpos += speedfactor;
                }

                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            } 
            break;

            case AUDIO_S32MSB:
            {
                int64_t lsmpl, rsmpl;

                int32_t *dst32 = (int32_t *)dst;
                
                dstlen /= 8;

                while(dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const int32_t *src = (int32_t *)(buf->data.data() + pos);
                    
                    lsmpl = rsmpl = SDL_SwapBE32(src[0]);
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst32 = SDL_SwapBE32( CLAMP<int64_t>(((int64_t)SDL_SwapBE32(*dst32) + lsmpl), SDL_MIN_SINT32, SDL_MAX_SINT32));
                    ++dst32;
                    *dst32 = SDL_SwapBE32( CLAMP<int64_t>(((int64_t)SDL_SwapBE32(*dst32) + rsmpl), SDL_MIN_SINT32, SDL_MAX_SINT32));
                    ++dst32;

                    fpos += speedfactor;
                }

                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            } 
            break;

            case AUDIO_F32LSB:
            {
                double lsmpl, rsmpl;

                float *fdst32 = (float *)dst;
                
                dstlen /= 8;

                const float fmaxvolume = 1.0f / ((float)SDL_MIX_MAXVOLUME);
                /* !!! FIXME: are these right? */
                const double max_audioval = 3.402823466e+38F;
                const double min_audioval = -3.402823466e+38F;

                while(dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const float *src = (float *)(buf->data.data() + pos);
                    
                    lsmpl = rsmpl = SDL_SwapFloatLE(src[0]);

                    rsmpl *= (float)rvlm * fmaxvolume;
                    lsmpl *= (float)lvlm * fmaxvolume;

                    *fdst32 = SDL_SwapFloatLE( CLAMP<double>(((double)SDL_SwapFloatLE(*fdst32) + lsmpl), min_audioval, max_audioval));
                    ++fdst32;
                    *fdst32 = SDL_SwapFloatLE( CLAMP<double>(((double)SDL_SwapFloatLE(*fdst32) + rsmpl), min_audioval, max_audioval));
                    ++fdst32;

                    fpos += speedfactor;
                }

                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            } 
            break;

            case AUDIO_F32MSB:
            {
                double lsmpl, rsmpl;

                float *fdst32 = (float *)dst;
                
                dstlen /= 8;

                const float fmaxvolume = 1.0f / ((float)SDL_MIX_MAXVOLUME);
                /* !!! FIXME: are these right? */
                const double max_audioval = 3.402823466e+38F;
                const double min_audioval = -3.402823466e+38F;

                while(dstlen--)
                {
                    uint32_t pos = stpos + GETFINTPOS(fpos) * FullSampleSize;

                    if (CheckEnd(chn, &pos, &stpos, &fpos, &endpos, srclen))
                        break;
                    
                    const float *src = (float *)(buf->data.data() + pos);

                    lsmpl = rsmpl = SDL_SwapFloatBE(src[0]);

                    rsmpl *= (float)rvlm * fmaxvolume;
                    lsmpl *= (float)lvlm * fmaxvolume;

                    *fdst32 = SDL_SwapFloatBE( CLAMP<double>(((double)SDL_SwapFloatBE(*fdst32) + lsmpl), min_audioval, max_audioval));
                    ++fdst32;
                    *fdst32 = SDL_SwapFloatBE( CLAMP<double>(((double)SDL_SwapFloatBE(*fdst32) + rsmpl), min_audioval, max_audioval));
                    ++fdst32;

                    fpos += speedfactor;
                }

                chn->smplpos = GETFRACPOS(fpos);
                chn->pos = stpos + GETFINTPOS(fpos) * FullSampleSize;
            } 
            break;
            
            default:
                //printf("No mixer for format %x\n", _spec.format);
                break;
        }
    }
}



uint32_t Mixer::MixData(uint8_t *dst, uint32_t dstlen, const uint8_t *_src, uint32_t *srclen, uint8_t *ssmplpos,
                    SDL_AudioFormat format, int chan, float fspeedfactor, uint8_t volume, int pan)
{
    const uint32_t speedfactor = fspeedfactor * 256.0f;
    const int32_t SampleSize = ( SDL_AUDIO_BITSIZE(format) / 8 );
    const int32_t FullSampleSize = SampleSize * chan;

    const int32_t lvlm = pan > 0 ? volume *  pan / 128 : volume;
    const int32_t rvlm = pan < 0 ? volume * -pan / 128 : volume;    

    *srclen &= ~(FullSampleSize - 1);

    const uint32_t endpos = *srclen;

    uint32_t fpos = *ssmplpos;

    uint32_t writed = 0;

    if (chan >= 2)
    {
        switch(format)
        {
            case AUDIO_U8:
            {
                dstlen /= 2;

                while (dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const uint8_t *src = (uint8_t *)(_src + pos);
                    
                    uint8_t lsmpl = src[0];
                    uint8_t rsmpl = src[1];
                    ADJUST_VOLUME_U8(rsmpl, rvlm);
                    ADJUST_VOLUME_U8(lsmpl, lvlm);

                    *dst = mix8[*dst + lsmpl];
                    ++dst;
                    *dst = mix8[*dst + rsmpl];
                    ++dst;

                    writed += 2;
                    fpos += speedfactor;
                }
            }
            break;

            case AUDIO_S8:
            {
                int8_t *dst8 = (int8_t *)dst;
                
                dstlen /= 2;

                while (dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const int8_t *src = (int8_t *)(_src + pos);
                    
                    int8_t lsmpl = src[0];
                    int8_t rsmpl = src[1];
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst8 = CLAMP<int16_t>((int16_t)*dst8 + (int16_t)lsmpl, SDL_MIN_SINT8, SDL_MAX_SINT8);
                    ++dst8;
                    *dst8 = CLAMP<int16_t>((int16_t)*dst8 + (int16_t)rsmpl, SDL_MIN_SINT8, SDL_MAX_SINT8);
                    ++dst8;

                    writed += 2;
                    fpos += speedfactor;
                }
            }
            break;

            case AUDIO_S16LSB:
            {
                int16_t *dst16 = (int16_t *)dst;
                
                dstlen /= 4;

                while(dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const int16_t *src = (int16_t *)(_src + pos);
                    
                    int32_t lsmpl = SDL_SwapLE16(src[0]);
                    int32_t rsmpl = SDL_SwapLE16(src[1]);
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst16 = SDL_SwapLE16( CLAMP<int32_t>((int32_t)SDL_SwapLE16(*dst16) + lsmpl, SDL_MIN_SINT16, SDL_MAX_SINT16) );
                    ++dst16;
                    *dst16 = SDL_SwapLE16( CLAMP<int32_t>((int32_t)SDL_SwapLE16(*dst16) + rsmpl, SDL_MIN_SINT16, SDL_MAX_SINT16) );
                    ++dst16;

                    writed += 4;
                    fpos += speedfactor;
                }
            } 
            break;

            case AUDIO_S16MSB:
            {
                int16_t *dst16 = (int16_t *)dst;
                
                dstlen /= 4;

                while(dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const int16_t *src = (int16_t *)(_src + pos);

                    int32_t lsmpl = SDL_SwapBE16(src[0]);
                    int32_t rsmpl = SDL_SwapBE16(src[1]);
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst16 = SDL_SwapBE16( CLAMP<int32_t>((int32_t)SDL_SwapBE16(*dst16) + lsmpl, SDL_MIN_SINT16, SDL_MAX_SINT16) );
                    ++dst16;
                    *dst16 = SDL_SwapBE16( CLAMP<int32_t>((int32_t)SDL_SwapBE16(*dst16) + rsmpl, SDL_MIN_SINT16, SDL_MAX_SINT16) );
                    ++dst16;

                    writed += 4;
                    fpos += speedfactor;
                }
            } 
            break;

            case AUDIO_U16LSB:
            {
                uint16_t *dst16 = (uint16_t *)dst;
                
                dstlen /= 4;

                while(dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const uint16_t *src = (uint16_t *)(_src + pos);
                    
                    int32_t lsmpl = SDL_SwapLE16(src[0]);
                    int32_t rsmpl = SDL_SwapLE16(src[1]);
                    ADJUST_VOLUME_U16(rsmpl, rvlm);
                    ADJUST_VOLUME_U16(lsmpl, lvlm);

                    *dst16 = SDL_SwapLE16( CLAMP<int32_t>(((int32_t)SDL_SwapLE16(*dst16) + lsmpl) - 32768 * 2,
                                                            SDL_MIN_SINT16, SDL_MAX_SINT16) + 32768);
                    ++dst16;
                    *dst16 = SDL_SwapLE16( CLAMP<int32_t>(((int32_t)SDL_SwapLE16(*dst16) + rsmpl) - 32768 * 2,
                                                            SDL_MIN_SINT16, SDL_MAX_SINT16) + 32768 );
                    ++dst16;

                    writed += 4;
                    fpos += speedfactor;
                }
            } 
            break;

            case AUDIO_U16MSB:
            {
                uint16_t *dst16 = (uint16_t *)dst;
                
                dstlen /= 4;

                while(dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const uint16_t *src = (uint16_t *)(_src + pos);

                    int32_t lsmpl = SDL_SwapBE16(src[0]);
                    int32_t rsmpl = SDL_SwapBE16(src[1]);
                    ADJUST_VOLUME_U16(rsmpl, rvlm);
                    ADJUST_VOLUME_U16(lsmpl, lvlm);

                    *dst16 = SDL_SwapBE16( CLAMP<int32_t>(((int32_t)SDL_SwapBE16(*dst16) + lsmpl) - 32768 * 2,
                                                            SDL_MIN_SINT16, SDL_MAX_SINT16) + 32768);
                    ++dst16;
                    *dst16 = SDL_SwapBE16( CLAMP<int32_t>(((int32_t)SDL_SwapBE16(*dst16) + rsmpl) - 32768 * 2,
                                                            SDL_MIN_SINT16, SDL_MAX_SINT16) + 32768 );
                    ++dst16;

                    writed += 4;
                    fpos += speedfactor;
                }
            } 
            break;

            case AUDIO_S32LSB:
            {
                int32_t *dst32 = (int32_t *)dst;
                
                dstlen /= 8;

                while(dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const int32_t *src = (int32_t *)(_src + pos);
                    
                    int64_t lsmpl = SDL_SwapLE32(src[0]);
                    int64_t rsmpl = SDL_SwapLE32(src[1]);
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst32 = SDL_SwapLE32( CLAMP<int64_t>(((int64_t)SDL_SwapLE32(*dst32) + lsmpl), SDL_MIN_SINT32, SDL_MAX_SINT32));
                    ++dst32;
                    *dst32 = SDL_SwapLE32( CLAMP<int64_t>(((int64_t)SDL_SwapLE32(*dst32) + rsmpl), SDL_MIN_SINT32, SDL_MAX_SINT32));
                    ++dst32;

                    writed += 8;
                    fpos += speedfactor;
                }
            } 
            break;

            case AUDIO_S32MSB:
            {
                int32_t *dst32 = (int32_t *)dst;
                
                dstlen /= 8;

                while(dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const int32_t *src = (int32_t *)(_src + pos);
                    
                    int64_t lsmpl = SDL_SwapBE32(src[0]);
                    int64_t rsmpl = SDL_SwapBE32(src[1]);
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst32 = SDL_SwapBE32( CLAMP<int64_t>(((int64_t)SDL_SwapBE32(*dst32) + lsmpl), SDL_MIN_SINT32, SDL_MAX_SINT32));
                    ++dst32;
                    *dst32 = SDL_SwapBE32( CLAMP<int64_t>(((int64_t)SDL_SwapBE32(*dst32) + rsmpl), SDL_MIN_SINT32, SDL_MAX_SINT32));
                    ++dst32;

                    writed += 8;
                    fpos += speedfactor;
                }
            } 
            break;

            case AUDIO_F32LSB:
            {
                float *fdst32 = (float *)dst;
                
                dstlen /= 8;

                const float fmaxvolume = 1.0f / ((float)SDL_MIX_MAXVOLUME);
                /* !!! FIXME: are these right? */
                const double max_audioval = 3.402823466e+38F;
                const double min_audioval = -3.402823466e+38F;

                while(dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const float *src = (float *)(_src + pos);
                    
                    double lsmpl = SDL_SwapFloatLE(src[0]) * (float)lvlm * fmaxvolume;
                    double rsmpl = SDL_SwapFloatLE(src[1]) * (float)rvlm * fmaxvolume;

                    *fdst32 = SDL_SwapFloatLE( CLAMP<double>(((double)SDL_SwapFloatLE(*fdst32) + lsmpl), min_audioval, max_audioval));
                    ++fdst32;
                    *fdst32 = SDL_SwapFloatLE( CLAMP<double>(((double)SDL_SwapFloatLE(*fdst32) + rsmpl), min_audioval, max_audioval));
                    ++fdst32;

                    writed += 8;
                    fpos += speedfactor;
                }
            } 
            break;

            case AUDIO_F32MSB:
            {
                float *fdst32 = (float *)dst;
                
                dstlen /= 8;

                const float fmaxvolume = 1.0f / ((float)SDL_MIX_MAXVOLUME);
                /* !!! FIXME: are these right? */
                const double max_audioval = 3.402823466e+38F;
                const double min_audioval = -3.402823466e+38F;

                while(dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const float *src = (float *)(_src + pos);

                    double lsmpl = SDL_SwapFloatBE(src[0]) * (float)lvlm * fmaxvolume;
                    double rsmpl = SDL_SwapFloatBE(src[1]) * (float)rvlm * fmaxvolume;

                    *fdst32 = SDL_SwapFloatBE( CLAMP<double>(((double)SDL_SwapFloatBE(*fdst32) + lsmpl), min_audioval, max_audioval));
                    ++fdst32;
                    *fdst32 = SDL_SwapFloatBE( CLAMP<double>(((double)SDL_SwapFloatBE(*fdst32) + rsmpl), min_audioval, max_audioval));
                    ++fdst32;

                    writed += 8;
                    fpos += speedfactor;
                }
            } 
            break;
            
            default:
                //printf("No mixer for format %x\n", _spec.format);
                break;
        }
    }
    else
    {
        switch(format)
        {
            case AUDIO_U8:
            {
                uint8_t lsmpl, rsmpl;
                
                dstlen /= 2;

                while (dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const uint8_t *src = (uint8_t *)(_src + pos);
                    
                    lsmpl = rsmpl = src[ 0 ];
                    ADJUST_VOLUME_U8(rsmpl, rvlm);
                    ADJUST_VOLUME_U8(lsmpl, lvlm);

                    *dst = mix8[*dst + lsmpl];
                    ++dst;
                    *dst = mix8[*dst + rsmpl];
                    ++dst;

                    writed += 2;
                    fpos += speedfactor;
                }
            }
            break;

            case AUDIO_S8:
            {
                int8_t *dst8 = (int8_t *)dst;
                int8_t lsmpl, rsmpl;
                
                dstlen /= 2;

                while (dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const int8_t *src = (int8_t *)(_src + pos);
                    
                    lsmpl = rsmpl = src[ 0 ];
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst8 = CLAMP<int16_t>((int16_t)*dst8 + (int16_t)lsmpl, SDL_MIN_SINT8, SDL_MAX_SINT8);
                    ++dst8;
                    *dst8 = CLAMP<int16_t>((int16_t)*dst8 + (int16_t)rsmpl, SDL_MIN_SINT8, SDL_MAX_SINT8);
                    ++dst8;
                    
                    writed += 4;
                    fpos += speedfactor;
                }
            }
            break;

            case AUDIO_S16LSB:
            {
                int32_t lsmpl, rsmpl;

                int16_t *dst16 = (int16_t *)dst;
                
                dstlen /= 4;

                while(dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const int16_t *src = (int16_t *)(_src + pos);

                    lsmpl = rsmpl = SDL_SwapLE16(src[0]);
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst16 = SDL_SwapLE16( CLAMP<int32_t>((int32_t)SDL_SwapLE16(*dst16) + lsmpl, SDL_MIN_SINT16, SDL_MAX_SINT16) );
                    ++dst16;
                    *dst16 = SDL_SwapLE16( CLAMP<int32_t>((int32_t)SDL_SwapLE16(*dst16) + rsmpl, SDL_MIN_SINT16, SDL_MAX_SINT16) );
                    ++dst16;

                    writed += 4;
                    fpos += speedfactor;
                }
            } 
            break;

            case AUDIO_S16MSB:
            {
                int32_t lsmpl, rsmpl;

                int16_t *dst16 = (int16_t *)dst;
                
                dstlen /= 4;

                while(dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const int16_t *src = (int16_t *)(_src + pos);
                    
                    lsmpl = rsmpl = SDL_SwapBE16(src[0]);
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst16 = SDL_SwapBE16( CLAMP<int32_t>((int32_t)SDL_SwapBE16(*dst16) + lsmpl, SDL_MIN_SINT16, SDL_MAX_SINT16) );
                    ++dst16;
                    *dst16 = SDL_SwapBE16( CLAMP<int32_t>((int32_t)SDL_SwapBE16(*dst16) + rsmpl, SDL_MIN_SINT16, SDL_MAX_SINT16) );
                    ++dst16;

                    writed += 4;
                    fpos += speedfactor;
                }
            } 
            break;

            case AUDIO_U16LSB:
            {
                int32_t lsmpl, rsmpl;

                uint16_t *dst16 = (uint16_t *)dst;
                
                dstlen /= 4;

                while(dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;

                    const uint16_t *src = (uint16_t *)(_src + pos);
                    
                    lsmpl = rsmpl = SDL_SwapLE16(src[0]);
                    ADJUST_VOLUME_U16(rsmpl, rvlm);
                    ADJUST_VOLUME_U16(lsmpl, lvlm);

                    *dst16 = SDL_SwapLE16( CLAMP<int32_t>(((int32_t)SDL_SwapLE16(*dst16) + lsmpl) - 32768 * 2,
                                                            SDL_MIN_SINT16, SDL_MAX_SINT16) + 32768);
                    ++dst16;
                    *dst16 = SDL_SwapLE16( CLAMP<int32_t>(((int32_t)SDL_SwapLE16(*dst16) + rsmpl) - 32768 * 2,
                                                            SDL_MIN_SINT16, SDL_MAX_SINT16) + 32768 );
                    ++dst16;

                    writed += 4;
                    fpos += speedfactor;
                }
            } 
            break;

            case AUDIO_U16MSB:
            {
                int32_t lsmpl, rsmpl;

                uint16_t *dst16 = (uint16_t *)dst;
                
                dstlen /= 4;

                while(dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const uint16_t *src = (uint16_t *)(_src + pos);

                    lsmpl = rsmpl = SDL_SwapBE16(src[0]);
                    ADJUST_VOLUME_U16(rsmpl, rvlm);
                    ADJUST_VOLUME_U16(lsmpl, lvlm);

                    *dst16 = SDL_SwapBE16( CLAMP<int32_t>(((int32_t)SDL_SwapBE16(*dst16) + lsmpl) - 32768 * 2,
                                                            SDL_MIN_SINT16, SDL_MAX_SINT16) + 32768);
                    ++dst16;
                    *dst16 = SDL_SwapBE16( CLAMP<int32_t>(((int32_t)SDL_SwapBE16(*dst16) + rsmpl) - 32768 * 2,
                                                            SDL_MIN_SINT16, SDL_MAX_SINT16) + 32768 );
                    ++dst16;

                    writed += 4;
                    fpos += speedfactor;
                }
            } 
            break;

            case AUDIO_S32LSB:
            {
                int64_t lsmpl, rsmpl;

                int32_t *dst32 = (int32_t *)dst;
                
                dstlen /= 8;

                while(dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const int32_t *src = (int32_t *)(_src + pos);
                    
                    lsmpl = rsmpl = SDL_SwapLE32(src[0]);
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst32 = SDL_SwapLE32( CLAMP<int32_t>(((int64_t)SDL_SwapLE32(*dst32) + lsmpl), SDL_MIN_SINT32, SDL_MAX_SINT32));
                    ++dst32;
                    *dst32 = SDL_SwapLE32( CLAMP<int32_t>(((int64_t)SDL_SwapLE32(*dst32) + rsmpl), SDL_MIN_SINT32, SDL_MAX_SINT32));
                    ++dst32;

                    writed += 8;
                    fpos += speedfactor;
                }
            } 
            break;

            case AUDIO_S32MSB:
            {
                int64_t lsmpl, rsmpl;

                int32_t *dst32 = (int32_t *)dst;
                
                dstlen /= 8;

                while(dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const int32_t *src = (int32_t *)(_src + pos);
                    
                    lsmpl = rsmpl = SDL_SwapBE32(src[0]);
                    ADJUST_VOLUME(rsmpl, rvlm);
                    ADJUST_VOLUME(lsmpl, lvlm);

                    *dst32 = SDL_SwapBE32( CLAMP<int32_t>(((int64_t)SDL_SwapBE32(*dst32) + lsmpl), SDL_MIN_SINT32, SDL_MAX_SINT32));
                    ++dst32;
                    *dst32 = SDL_SwapBE32( CLAMP<int32_t>(((int64_t)SDL_SwapBE32(*dst32) + rsmpl), SDL_MIN_SINT32, SDL_MAX_SINT32));
                    ++dst32;

                    writed += 8;
                    fpos += speedfactor;
                }
            } 
            break;

            case AUDIO_F32LSB:
            {
                double lsmpl, rsmpl;

                float *fdst32 = (float *)dst;
                
                dstlen /= 8;

                const float fmaxvolume = 1.0f / ((float)SDL_MIX_MAXVOLUME);
                /* !!! FIXME: are these right? */
                const double max_audioval = 3.402823466e+38F;
                const double min_audioval = -3.402823466e+38F;

                while(dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const float *src = (float *)(_src + pos);
                    
                    lsmpl = rsmpl = SDL_SwapFloatLE(src[0]);

                    rsmpl *= (float)rvlm * fmaxvolume;
                    lsmpl *= (float)lvlm * fmaxvolume;

                    *fdst32 = SDL_SwapFloatLE( CLAMP<double>(((double)SDL_SwapFloatLE(*fdst32) + lsmpl), min_audioval, max_audioval));
                    ++fdst32;
                    *fdst32 = SDL_SwapFloatLE( CLAMP<double>(((double)SDL_SwapFloatLE(*fdst32) + rsmpl), min_audioval, max_audioval));
                    ++fdst32;

                    writed += 8;
                    fpos += speedfactor;
                }
            } 
            break;

            case AUDIO_F32MSB:
            {
                double lsmpl, rsmpl;

                float *fdst32 = (float *)dst;
                
                dstlen /= 8;

                const float fmaxvolume = 1.0f / ((float)SDL_MIX_MAXVOLUME);
                /* !!! FIXME: are these right? */
                const double max_audioval = 3.402823466e+38F;
                const double min_audioval = -3.402823466e+38F;

                while(dstlen--)
                {
                    uint32_t pos = GETFINTPOS(fpos) * FullSampleSize;

                    if (pos >= endpos)
                        break;
                    
                    const float *src = (float *)(_src + pos);

                    lsmpl = rsmpl = SDL_SwapFloatBE(src[0]);

                    rsmpl *= (float)rvlm * fmaxvolume;
                    lsmpl *= (float)lvlm * fmaxvolume;

                    *fdst32 = SDL_SwapFloatBE( CLAMP<double>(((double)SDL_SwapFloatBE(*fdst32) + lsmpl), min_audioval, max_audioval));
                    ++fdst32;
                    *fdst32 = SDL_SwapFloatBE( CLAMP<double>(((double)SDL_SwapFloatBE(*fdst32) + rsmpl), min_audioval, max_audioval));
                    ++fdst32;

                    writed += 8;
                    fpos += speedfactor;
                }
            } 
            break;
            
            default:
                //printf("No mixer for format %x\n", _spec.format);
                break;
        }
    }

    *ssmplpos = GETFRACPOS(fpos);
    *srclen = GETFINTPOS(fpos) * FullSampleSize;

    return writed;
}



}