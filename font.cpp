#include <complex>

#include "font.h"
#include "gfx.h"


namespace GFX {
    
    
Font *CDrawer::LoadFont(const std::string &fname)
{
    FSMgr::File f = FSMgr::Mgr::ReadFile(fname);
    if (!f)
        return nullptr;
    
    Font *fnt = new Font();
    
    int32_t firstChar = f->readU8();
    int32_t charCount = f->readU8();
    fnt->Width = f->readU16L();
    
    std::array<SDL_Surface *, 256> tiles;
    tiles.fill(nullptr);
    
    int32_t maxW = 0;
    int32_t maxH = 0;
    
    for(int32_t i = firstChar; i < firstChar + charCount - 1; ++i)
    {
        uint32_t pos = f->readU32L();
        if (pos)
        {
            fnt->Present.at( i >> 5 ) |= 1 << (i & 0x1F);
            size_t back = f->tell();
            f->seek(pos, 0);
            tiles.at(i) = Font::LoadRL16BitImage(f.get());  
            
            maxW += tiles.at(i)->w;
            maxH += tiles.at(i)->h;
            
            f->seek(back, 0);
        }
    }
    
    maxW /= (charCount - 1);
    maxH /= (charCount - 1);
    
    int32_t charSZ = maxW;
    if (charSZ < maxH)
        charSZ = maxH;
    
    int32_t texSide = GFX::PowerOf2( (int32_t)(sqrtf(charCount) + 0.99) * (charSZ + 1) );
    int32_t charSZSQ = GFX::PowerOf2(charSZ);
    
    /* Compute texture */
    Common::Point size(texSide, texSide);
    bool complete = false;
    while (!complete)
    {
        fnt->SW = GFXDrawer.CreateSWSurface(size);
        Common::Point pos(0, 0);
        int32_t hg = 0;
        
        for(int32_t i = 0; i < 256; ++i)
        {
            SDL_Surface *img = tiles.at(i);
            if (img)
            {
                if (pos.x + img->w >= size.x)
                {
                    pos.x = 0;
                    pos.y += hg;
                    hg = 0;
                }
                if (pos.y + img->h >= size.y)
                {
                    SDL_FreeSurface(fnt->SW);
                    fnt->SW = nullptr;
                    size += Common::Point(charSZSQ, charSZSQ);
                    complete = false;
                    break;
                }
                    
                if (hg < img->h)
                    hg = img->h;
                
                Common::PointRect rect(pos.x, pos.y, img->w, img->h);
                fnt->SWTiles.at(i) = rect;
                fnt->HWTiles.at(i) = Common::FRect((float)pos.x / (float)size.x,
                                                   (float)pos.y / (float)size.y,
                                                   (float)(pos.x + img->w) / (float)size.x,
                                                   (float)(pos.y + img->h) / (float)size.y );

                SDL_Rect sdlrect = rect;                    
                SDL_BlitSurface(img, NULL, fnt->SW, &sdlrect);
                
                pos.x += img->w;
                complete = true;
            }
        }    
    }
    
    for(SDL_Surface *img : tiles)
    {
        if (img)
            SDL_FreeSurface(img);
    }      
    
    if (!fnt->SW)
    {
        delete fnt;
        return nullptr;
    }
    

    glGenTextures(1, &fnt->HW);
    _state.Tex = fnt->HW;
    ApplyStates();
            
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (fnt->SW->format->format == _pixfmt->format)
    {
        SDL_LockSurface(fnt->SW);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fnt->SW->w, fnt->SW->h, 0, _glPixfmt, _glPixtype, fnt->SW->pixels);
        SDL_UnlockSurface(fnt->SW);
    }
    else
    {
        SDL_Surface *conv = ConvertSDLSurface(fnt->SW, _pixfmt);

        SDL_LockSurface(conv);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fnt->SW->w, fnt->SW->h, 0, _glPixfmt, _glPixtype, conv->pixels);
        SDL_UnlockSurface(conv);

        SDL_FreeSurface(conv);
    }
    
    return fnt;    
}

SDL_Surface *Font::LoadRL16BitImage(FSMgr::iFile *pfile)
{
    short w = pfile->readS16L();
    short h = pfile->readS16L();
    
    /*std::vector<short> linesLength(h);
    for(int32_t i = 0; i < h; ++i)
        linesLength.at(i) = pfile->readS16L();*/
    pfile->seek(h * 2, 1); // skip
    
    SDL_Surface *img = GFXDrawer.CreateSWSurface({w, h});
    
    SDL_LockSurface(img);
    for(int32_t y = 0; y < h; ++y)
    {
        uint32_t *px = (uint32_t *)((char *)img->pixels + img->pitch * y);
        while(true)
        {
            uint8_t n = pfile->readU8();
            
            if (n == 0)
                break;
            
            if (n & 0x80)
                px += n & 0x7F;
                /*for (int32_t i = 0; i < n; ++i)
                    {
                        const SDL_Color &cl = Palettes[ palOffset + pfile->readU8() ];
                        *px = SDL_MapRGBA(); 
                        px++;
                    }*/
            else
            {
                for (int32_t i = 0; i < n; ++i)
                {
                    uint16_t clr = pfile->readU16L();
                    *px = SDL_MapRGBA(img->format, GFX::Lkp5To8[ (clr >> 10) & 0x1f ]
                                                 , GFX::Lkp5To8[ (clr >> 5) & 0x1f ]
                                                 , GFX::Lkp5To8[ (clr >> 0) & 0x1f ]
                                                 , 255); 
                    px++;
                }
            }
        }
    }
    SDL_UnlockSurface(img);
    
    return img;
}
    
void CDrawer::DrawText(const std::string &txt, Font *fnt, Common::Point outpos)
{
    _state.Tex = fnt->HW;
    _state.Pal = -1;
    _state.DrawMode = 0;
    ApplyStates();

    const char *pos = txt.c_str();
    while(pos < txt.c_str() + txt.size())
    {
        uint32_t chrsz = Font::Utf8CharLen(*pos);
        uint32_t uchr = 0;
        
        if (chrsz)
        {
            if ( (pos[0] & 0x80) == 0 )
                uchr = pos[0];
            else if ( (pos[0] & 0xE0) == 0xC0 )
            {
                uchr = (pos[0] & 0x1F) << 6;
                uchr |= (pos[1] & 0x3F);
            }
            else if ( (pos[0] & 0xF0) == 0xE0 )
            {
                uchr = (pos[0] & 0xF) << 12;
                uchr |= (pos[1] & 0x3F) << 6;
                uchr |= (pos[2] & 0x3F);
            }
            else if ( (pos[0] & 0xF0) == 0xF0 )
            {
                uchr = (pos[0] & 0xF) << 18;
                uchr |= (pos[1] & 0x3F) << 12;
                uchr |= (pos[2] & 0x3F) << 6;
                uchr |= (pos[3] & 0x3F);
            }
            
            pos += chrsz;
            
            //unicode to cp866
            int32_t chid = -1;
            if (uchr < 128)
                chid = uchr;
            else if (uchr >= 0x410 && uchr < 0x440)
                chid = uchr - 0x410 + 0x80;
            else if (uchr >= 0x440 && uchr < 0x450)
                chid = uchr - 0x440 + 0xE0;
            else if (uchr == 0x401)
                chid = 0xf0;
            else if (uchr == 0x451)
                chid = 0xf1;
            
            if (chid != -1 && fnt->IsPresent(chid))
            {
                Common::Point charDem = fnt->SWTiles.at(chid).Size();
                
                Common::FRect out(outpos.x, outpos.y, outpos.x + charDem.x, outpos.y + charDem.y);
                Common::FRect &texTile = fnt->HWTiles.at(chid);
                
                outpos.x += charDem.x;
                
                std::array<TVertex, 4> vtx = {
                    TVertex( vec3f(out.left,  out.top,    0.0), tUtV(texTile.left, texTile.top) ),
                    TVertex( vec3f(out.left,  out.bottom, 0.0), tUtV(texTile.left, texTile.bottom) ),
                    TVertex( vec3f(out.right, out.bottom, 0.0), tUtV(texTile.right, texTile.bottom) ),
                    TVertex( vec3f(out.right, out.top,    0.0), tUtV(texTile.right, texTile.top) )
                };
                
                DrawVtxQuad(vtx);
                
            }
            else
                outpos.x += fnt->Width;
        }
        else
            pos++;
    }
}

//SDL2 code
#define UTF8_IsLeadByte(c)     ((c) >= 0xC0 && (c) <= 0xF4)
#define UTF8_IsTrailingByte(c) ((c) >= 0x80 && (c) <= 0xBF)

uint32_t Font::Utf8CharLen(uint8_t chr)
{
    if (chr >= 0xC0 && chr <= 0xDF) {
        return 2;
    } else if (chr >= 0xE0 && chr <= 0xEF) {
        return 3;
    } else if (chr >= 0xF0 && chr <= 0xF4) {
        return 4;
    }

    return 1;
}

uint32_t Font::Utf8GetUnicodeChar(const char *chr)
{
    uint32_t uchr = 0;
    
    if ( (chr[0] & 0x80) == 0 )
        uchr = chr[0];
    else if ( (chr[0] & 0xE0) == 0xC0 )
    {
        uchr = (chr[0] & 0x1F) << 6;
        uchr |= (chr[1] & 0x3F);
    }
    else if ( (chr[0] & 0xF0) == 0xE0 )
    {
        uchr = (chr[0] & 0xF) << 12;
        uchr |= (chr[1] & 0x3F) << 6;
        uchr |= (chr[2] & 0x3F);
    }
    else if ( (chr[0] & 0xF0) == 0xF0 )
    {
        uchr = (chr[0] & 0xF) << 18;
        uchr |= (chr[1] & 0x3F) << 12;
        uchr |= (chr[2] & 0x3F) << 6;
        uchr |= (chr[3] & 0x3F);
    }
    return uchr;
}

int32_t Font::StringWidth(const std::string &text)
{
    int32_t width = 0;
    const char *pos = text.c_str();
    while(pos < text.c_str() + text.size())
    {
        uint32_t chrsz = Font::Utf8CharLen(*pos);
        
        if (chrsz)
        {
            uint32_t uchr = Font::Utf8GetUnicodeChar(pos);
            
            pos += chrsz;
            
            //unicode to cp866
            int32_t chid = -1;
            if (uchr < 128)
                chid = uchr;
            else if (uchr >= 0x410 && uchr < 0x440)
                chid = uchr - 0x410 + 0x80;
            else if (uchr >= 0x440 && uchr < 0x450)
                chid = uchr - 0x440 + 0xE0;
            else if (uchr == 0x401)
                chid = 0xf0;
            else if (uchr == 0x451)
                chid = 0xf1;
            
            if (chid != -1 && IsPresent(chid))
                width += SWTiles.at(chid).w;
            else
                width += Width;
        }
        else
            pos++;
    }
    return width;
}
    
}
