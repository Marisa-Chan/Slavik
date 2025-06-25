#include "resources.h"
#include "fsmgr.h"
#include "fmt/printf.h"
#include <vector>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_rwops.h>
#include <SDL2/SDL_image.h>

namespace Game {

Resources Resources::Res;

const std::array<uint8_t, 3> Resources::CharBasePal = { 74, 33, 39 };

const std::array<uint8_t, 24> Resources::CharEquipPal = { 2, 2, 2, 2, 4, 4, 
                                                          3, 3, 3, 4, 4, 4,
                                                          4, 4, 4, 4, 4, 4,
                                                          2, 2, 5, 5, 5, 5 };

void Resources::CharacterSprites::Load(FSMgr::File *pfile)
{
    size_t spos = (*pfile)->tell();
    (*pfile)->seek(4, 1); // Skip
    uint32_t dataSize = (*pfile)->readU32L();
    
    for(int8_t k = 0; k < 10; ++k)
    {
        for(int8_t j = 0; j < 8; ++j)
        {
            SeqInfo &s = Seq[k][j];
            s.FrmCount = (*pfile)->readS16L();
            
            for(int8_t i = 0; i < 12; ++i)
                s.FrameData[i].FrameID = (*pfile)->readS16L();
        }
    }
    
    for(int8_t k = 0; k < 10; ++k)
    {
        for(int8_t j = 0; j < 8; ++j)
        {
            SeqInfo &s = Seq[k][j];
            for(int8_t i = 0; i < 12; ++i)
            {
                Common::Point &p = s.FrameData[i].WpnOffset;
                p.x = (*pfile)->readS16L();
                p.y = (*pfile)->readS16L();
            }
        }
    }
    
    std::vector<int32_t> sprHdr(768 * 2);
    for(size_t i = 0; i < 768; ++i)
    {
        sprHdr[i * 2] = (*pfile)->readS32L();
        sprHdr[i * 2 + 1] = (*pfile)->readS32L();
    }
        
    for(int8_t k = 0; k < 10; ++k)
    {
        for(int8_t j = 0; j < 8; ++j)
        {
            SeqInfo &s = Seq[k][j];
            for(int8_t i = 0; i < 12; ++i)
            {
                Common::Point &p1 = s.FrameData[i].ShdOffset;
                p1.x = (*pfile)->readS16L();
                p1.y = (*pfile)->readS16L();
                
                Common::Point &p2 = s.FrameData[i].ImgOffset;
                p2.x = (*pfile)->readS16L();
                p2.y = (*pfile)->readS16L();
            }
        }
    }
    
    size_t sprPos = (*pfile)->tell();
    
    for(size_t i = 0; i < 768; ++i)
    {
        int32_t shdOff = sprHdr[i * 2];
        int32_t imgOff = sprHdr[i * 2 + 1];
        
        if (imgOff >= 0)
        {
            (*pfile)->seek(sprPos + imgOff, 0);
            Images[i] = LoadRL8BitImage(pfile);
        }
        else
            Images[i] = nullptr;
        
        if (shdOff > 0)
        {
            (*pfile)->seek(sprPos + shdOff, 0);
            Shadows[i] = LoadRL8BitShadow(pfile);
        }
        else
            Shadows[i] = nullptr;        
    }
    
    (*pfile)->seek(spos + dataSize, 0);
}

void Resources::DynamicObject::LoadInfo(FSMgr::File *pfile)
{
    size_t spos = (*pfile)->tell();
    (*pfile)->seek(4, 1);
    uint32_t dataSize = (*pfile)->readU32L();
    
    for(int8_t k = 0; k < 6; ++k)
    {
        for(int8_t j = 0; j < 8; ++j)
        {
            SeqInfo &s = Seq[k][j];
            s.FrmCount = (*pfile)->readS16L();
            
            for(int8_t i = 0; i < 18; ++i)
                s.FrameData[i].FrameID = (*pfile)->readS16L();
        }
    }
    
    for(size_t i = 0; i < 512; ++i)
    {
        ShdOffsets[i] = (*pfile)->readS32L();
        ImgOffsets[i] = (*pfile)->readS32L();
    }
    
    for(int8_t k = 0; k < 6; ++k)
    {
        for(int8_t j = 0; j < 8; ++j)
        {
            SeqInfo &s = Seq[k][j];
            for(int8_t i = 0; i < 18; ++i)
            {
                Common::Point &p1 = s.FrameData[i].ShdOffset;
                p1.x = (*pfile)->readS16L();
                p1.y = (*pfile)->readS16L();
                
                Common::Point &p2 = s.FrameData[i].ImgOffset;
                p2.x = (*pfile)->readS16L();
                p2.y = (*pfile)->readS16L();
            }
        }
    }
    
    DataOffset = (*pfile)->tell();

    Flags |= FLAG_INFO_LOADED;

    (*pfile)->seek(spos + dataSize, 0);
}

void Resources::DynamicObject::LoadData(FSMgr::File *pfile)
{
    if ((Flags & FLAG_INFO_LOADED) == 0)
        return;
    if (Flags & FLAG_DATA_LOADED)
        return;
    
    for(size_t i = 0; i < 512; ++i)
    {
        if (ImgOffsets[i] >= 0)
        {
            (*pfile)->seek(DataOffset + ImgOffsets[i], 0);
            Images[i] = LoadRL8BitImage(pfile);
        }
        else
            Images[i] = nullptr;      
        
        if (ShdOffsets[i] > 0)
        {
            (*pfile)->seek(DataOffset + ShdOffsets[i], 0);
            Shadows[i] = LoadRL8BitShadow(pfile);
        }
        else
            Shadows[i] = nullptr;  
    }

    Flags |= FLAG_DATA_LOADED;
}

void Resources::SimpleObject::LoadInfo(FSMgr::File *pfile)
{
    size_t spos = (*pfile)->tell();
    
    Flags = (*pfile)->readU32L() & (~(FLAG_DATA_LOADED | FLAG_INFO_LOADED));
    uint32_t dataSize = (*pfile)->readU32L();
    
    int32_t numFlame = (*pfile)->readS32L();
    
    NumFrames = (*pfile)->readU32L();
    
    (*pfile)->seek(4, 1);
    
    PaletteOffset = (*pfile)->readU32L() / 2;
    
    FrameTime = (*pfile)->readS32L();
    
    if (numFlame)
    {
        Flames.resize(numFlame);

        for(size_t i = 0; i < numFlame; ++i)
        {
            FlamePos &flame = Flames[i];
            flame.Index = i;
            flame.FlameID = (*pfile)->readS8();
            flame.Position.x = (*pfile)->readS16L();
            flame.Position.y = (*pfile)->readS16L();
        }
    }
    
    (*pfile)->seek((20 - numFlame) * 5, 1);
    
    for(size_t i = 0; i < 8; ++i)
    {
        DataImgOffsets[i].ShdOffset = (*pfile)->readS32L();
        DataImgOffsets[i].ImgOffset = (*pfile)->readS32L();
    }
    
    for(int8_t i = 0; i < 8; ++i)
    {
        Common::Point &p1 = Offsets.at(i).ShdOffset;
        p1.x = (*pfile)->readS16L();
        p1.y = (*pfile)->readS16L();

        Common::Point &p2 = Offsets.at(i).Offset;
        p2.x = (*pfile)->readS16L();
        p2.y = (*pfile)->readS16L();
    }
    
    DataOffset = (*pfile)->tell();
    
    Flags |= FLAG_INFO_LOADED;

    (*pfile)->seek(spos + dataSize, 0);
}


void Resources::SimpleObject::LoadData(FSMgr::File *pfile, const SDL_Color *palettes)
{
    if ((Flags & FLAG_INFO_LOADED) == 0)
        return;
    if (Flags & FLAG_DATA_LOADED)
        return;
    
    for(size_t i = 0; i < 8; ++i)
    {
        ImgShdOffset &entry = DataImgOffsets.at(i);
        
        if (entry.ImgOffset >= 0)
        {
            (*pfile)->seek(DataOffset + entry.ImgOffset, 0);
            Images[i] = LoadRL8BitImage(pfile, palettes + PaletteOffset);
        }
        else
            Images[i] = nullptr;     

        if (entry.ShdOffset >= 0)
        {
            (*pfile)->seek(DataOffset + entry.ShdOffset, 0);
            Shadows[i] = LoadRL8BitShadow(pfile);
        }
        else 
            Shadows[i] = nullptr;
    }

    Flags |= FLAG_DATA_LOADED;
}

    
bool Resources::Load(Simix::Mixer *mx)
{
    if ( LoadGraphRes() &&
         LoadLightsRes() &&
         LoadObjectsRes() &&
         LoadFlames() &&
         LoadMask() &&
         LoadSounds(mx) &&
         LoadMusicInfo() )
        return true;
    
    return false;
}

bool Resources::LoadGraphRes()
{
    FSMgr::File graph = FSMgr::Mgr::ReadFile("graph.res");
    
    if (!graph)
        return false;
    
    // Load palettes
    for(int32_t j = 0; j < 256 * 256; ++j)
    {
        uint16_t clr = graph->readU16L();
        SDL_Color &cl = Palettes.At(j);
        cl.b = GFX::Lkp5To8[ (clr >> 0) & 0x1f ];
        cl.g = GFX::Lkp5To8[ (clr >> 5) & 0x1f ];
        cl.r = GFX::Lkp5To8[ (clr >> 10) & 0x1f ];
    }
    
    // Load cursors
    for(int32_t j = 0; j < 11; ++j)
    {
        graph->seek(4, 1); // Skip 4
        uint32_t nextOff = graph->readU32L();
        nextOff += graph->tell();
        
        CursorImages.at(j) = LoadRL16BitImage(&graph);
        
        graph->seek(nextOff, 0);
    }
    
    // Load tiles
    graph->seek(4, 1); // Skip 4
    uint32_t sz = graph->readU32L();
    
    std::vector<int32_t> tilesInfo(512 * 2);
    for(int32_t &v : tilesInfo)
        v = graph->readS32L();
    
    size_t fpos = graph->tell();
    
    for(int32_t j = 0; j < 512; ++j)
    {
        int32_t off = tilesInfo.at(j * 2);
        int32_t pal = tilesInfo.at(j * 2 + 1);
        if (off >= 0 && pal >= 0)
        {
            graph->seek(fpos + off, 0);
            Tiles.at(j) = LoadRL8BitImage(&graph, Palettes.data() + pal / 2);
        }
        else
            Tiles.at(j) = nullptr;
    }
    
    graph->seek(fpos + sz, 0);
    
    for(size_t i = 0; i < 3; ++i)
        CharacterBases.at(i).Load(&graph);
    
    for(size_t i = 0; i < 24; ++i)
        CharacterEquip.at(i).Load(&graph);
    
    
    return true;
}

bool Resources::LoadLightsRes()
{
    std::vector<uint8_t> LightLookup(100);
    for (size_t i = 0; i < 100; ++i)
        LightLookup[i] = 255.0 / 100.0 * (float)i;
    
    FSMgr::File lights = FSMgr::Mgr::ReadFile("lights.res");
    if (!lights)
        return false;
    
    Light1 = GFXDrawer.CreateLight({64, 64});
    lights->read(Light1->SW.data(), 64 * 64);
    for(uint8_t &val : Light1->SW)
        val = LightLookup.at(val);
    GFXDrawer.UpdateLight(Light1);
    
    Light2 = GFXDrawer.CreateLight({64, 43});
    lights->read(Light2->SW.data(), 64 * 43);
    for(uint8_t &val : Light2->SW)
        val = LightLookup.at(val);
    GFXDrawer.UpdateLight(Light2);
    
    for(size_t i = 0; i < 19; ++i)
    {
        TilesLight[i] = GFXDrawer.CreateLight({114, 64});
        lights->read(TilesLight[i]->SW.data(), 114 * 64);
        for(uint8_t &val : TilesLight[i]->SW)
            val = LightLookup.at(val);
        GFXDrawer.UpdateLight(TilesLight[i]);
    }
    
    return true;
}

bool Resources::LoadObjectsRes()
{
    ObjectsResFile = FSMgr::Mgr::ReadFile("objects.res");
    
    if (!ObjectsResFile)
        return false;
    
    ObjectsResFile->seek(8, 1); // skip offset and res size

    std::vector<FileEntry> entries(512); //Use vector because it will alloc not on stack
    for(size_t i = 0; i < 512; ++i)
    {
        entries[i].Offset = ObjectsResFile->readS32L();
        entries[i].Size = ObjectsResFile->readS32L();
    }
    
    size_t offset = ObjectsResFile->tell();
    
    for(size_t i = 0; i < 30; ++i)
    {
        if (entries[i].Offset >= 0)
        {
            ObjectsResFile->seek(offset + entries[i].Offset, 0);
            DynObjects[i].LoadInfo(&ObjectsResFile);
        }
    }
    
    for(size_t i = 0; i < 482; ++i)
    {
        size_t j = 30 + i;
        if (entries[j].Offset >= 0)
        {
            ObjectsResFile->seek(offset + entries[j].Offset, 0);
            SimpleObjects[i].LoadInfo(&ObjectsResFile);
        }
    }
    
    return true;
}

bool Resources::LoadFlames()
{
    FSMgr::File fobj = FSMgr::Mgr::ReadFile("flames.res");
    
    if (!fobj)
        return false;
    
    for(GFX::Image *&flame : Flames)
    {
        fobj->seek(4, 1);
        int32_t sz = fobj->readS32L();
        size_t pos = fobj->tell();
        flame = LoadFlameImage(&fobj); 
        fobj->seek(pos + sz, 0);
    }
    
    return true;
}

GFX::Image *Resources::LoadRL8BitImage(FSMgr::File *pfile, const SDL_Color *pal)
{
    short w = (*pfile)->readS16L();
    short h = (*pfile)->readS16L();
    
    /*std::vector<short> linesLength(h);
    for(int32_t i = 0; i < h; ++i)
        linesLength.at(i) = (*pfile)->readS16L();*/
    (*pfile)->seek(h * 2, 1); // skip
    
    GFX::Image *img = GFXDrawer.CreateImage({w, h});
    
    GFXDrawer.LockImage(img);
    for(int32_t y = 0; y < h; ++y)
    {
        uint32_t *px = (uint32_t *)((char *)img->SW->pixels + img->SW->pitch * y);
        while(true)
        {
            uint8_t n = (*pfile)->readU8();
            
            if (n == 0)
                break;
                        
            if (n & 0x80)
                px += n & 0x7F;
                /*for (int32_t i = 0; i < n; ++i)
                    {
                        const SDL_Color &cl = Palettes[ palOffset + (*pfile)->readU8() ];
                        *px = SDL_MapRGBA(); 
                        px++;
                    }*/
            else
            {
                for (int32_t i = 0; i < n; ++i)
                {
                    const SDL_Color &cl = pal[ (*pfile)->readU8() ];
                    *px = SDL_MapRGBA(img->SW->format, cl.r, cl.g, cl.b, 255); 
                    px++;
                }
            }
        }
    }
    GFXDrawer.UnlockImage(img);
    
    return img;
}

GFX::PalImage *Resources::LoadRL8BitImage(FSMgr::File *pfile)
{
    short w = (*pfile)->readS16L();
    short h = (*pfile)->readS16L();
    
    /*std::vector<short> linesLength(h);
    for(int32_t i = 0; i < h; ++i)
        linesLength.at(i) = (*pfile)->readS16L();*/
    (*pfile)->seek(h * 2, 1); // skip
    
    GFX::PalImage *img = GFXDrawer.CreatePalImage({w, h});
    
    for(int32_t y = 0; y < h; ++y)
    {
        uint16_t *px = (img->SW.data() + img->SW.Width() * y);
        while(true)
        {
            uint8_t n = (*pfile)->readU8();
            
            if (n == 0)
                break;
                        
            if (n & 0x80)
                px += (n & 0x7F);
                /*for (int32_t i = 0; i < n; ++i)
                    {
                        const SDL_Color &cl = Palettes[ palOffset + (*pfile)->readU8() ];
                        *px = SDL_MapRGBA(); 
                        px++;
                    }*/
            else
            {
                for (int32_t i = 0; i < n; ++i)
                {
                    *px = 0xFF00 | (*pfile)->readU8();
                    px++;
                }
            }
        }
    }
    GFXDrawer.UpdatePalImage(img);
    
    return img;
}


GFX::PalImage *Resources::LoadRL8BitShadow(FSMgr::File *pfile)
{
    short w = (*pfile)->readS16L();
    short h = (*pfile)->readS16L();
    
    /*std::vector<short> linesLength(h);
    for(int32_t i = 0; i < h; ++i)
        linesLength.at(i) = (*pfile)->readS16L();*/
    (*pfile)->seek(h * 2, 1); // skip
    
    GFX::PalImage *img = GFXDrawer.CreatePalImage({w, h});
    
    for(int32_t y = 0; y < h; ++y)
    {
        uint8_t *px = (uint8_t *)(img->SW.data() + img->SW.Width() * y);
        while(true)
        {
            uint8_t n = (*pfile)->readU8();
            
            if (n == 0)
                break;
                        
            if (n & 0x80)
                px += (n & 0x7F) * 2;
                /*for (int32_t i = 0; i < n; ++i)
                    {
                        const SDL_Color &cl = Palettes[ palOffset + (*pfile)->readU8() ];
                        *px = SDL_MapRGBA(); 
                        px++;
                    }*/
            else
            {
                for (int32_t i = 0; i < n; ++i)
                {
                    px[0] = 0;
                    px[1] = 255; 
                    px += 2;
                }
            }
        }
    }
    GFXDrawer.UpdatePalImage(img);
    
    return img;
}

GFX::Image *Resources::LoadRL16BitImage(FSMgr::File *pfile)
{
    short w = (*pfile)->readS16L();
    short h = (*pfile)->readS16L();
    
    /*std::vector<short> linesLength(h);
    for(int32_t i = 0; i < h; ++i)
        linesLength.at(i) = (*pfile)->readS16L();*/
    (*pfile)->seek(h * 2, 1); // skip
    
    GFX::Image *img = GFXDrawer.CreateImage({w, h});
    
    GFXDrawer.LockImage(img);
    for(int32_t y = 0; y < h; ++y)
    {
        uint32_t *px = (uint32_t *)((char *)img->SW->pixels + img->SW->pitch * y);
        while(true)
        {
            uint8_t n = (*pfile)->readU8();
            
            if (n == 0)
                break;
            
            if (n & 0x80)
                px += n & 0x7F;
                /*for (int32_t i = 0; i < n; ++i)
                    {
                        const SDL_Color &cl = Palettes[ palOffset + (*pfile)->readU8() ];
                        *px = SDL_MapRGBA(); 
                        px++;
                    }*/
            else
            {
                for (int32_t i = 0; i < n; ++i)
                {
                    uint16_t clr = (*pfile)->readU16L();
                    *px = SDL_MapRGBA(img->SW->format, GFX::Lkp5To8[ (clr >> 10) & 0x1f ]
                                                      , GFX::Lkp5To8[ (clr >> 5) & 0x1f ]
                                                      , GFX::Lkp5To8[ (clr >> 0) & 0x1f ]
                                                      , 255); 
                    px++;
                }
            }
        }
    }
    GFXDrawer.UnlockImage(img);
    
    return img;
}

GFX::Image *Resources::LoadFlameImage(FSMgr::File* pfile)
{
    short w = (*pfile)->readS16L();
    short h = (*pfile)->readS16L();
    
    /*std::vector<short> linesLength(h);
    for(int32_t i = 0; i < h; ++i)
        linesLength.at(i) = (*pfile)->readS16L();*/
    (*pfile)->seek(h * 2, 1); // skip
    
    GFX::Image *img = GFXDrawer.CreateImage({w, h});
    
    GFXDrawer.LockImage(img);
    for(int32_t y = 0; y < h; ++y)
    {
        uint32_t *px = (uint32_t *)((char *)img->SW->pixels + img->SW->pitch * y);
        while(true)
        {
            uint8_t n = (*pfile)->readU8();
            
            if (n == 0)
                break;
            
            if (n & 0x80)
                px += n & 0x7F;
                /*for (int32_t i = 0; i < n; ++i)
                    {
                        const SDL_Color &cl = Palettes[ palOffset + (*pfile)->readU8() ];
                        *px = SDL_MapRGBA(); 
                        px++;
                    }*/
            else
            {
                for (int32_t i = 0; i < n; ++i)
                {
                    uint16_t clr = (*pfile)->readU16L();
                    int r = GFX::Lkp5To8[ (clr >> 10) & 0x1f ];
                    int g = GFX::Lkp5To8[ (clr >> 5) & 0x1f ];
                    int b = GFX::Lkp5To8[ (clr >> 0) & 0x1f ];
                    int a = (2 * r + g + b) / 4;
                    *px = SDL_MapRGBA(img->SW->format, r * a / 255, g * a / 255, b * a / 255, a);
                    px++;
                }
            }
        }
    }
    GFXDrawer.UnlockImage(img);
    
    return img;
}




static Sint64 mysizefunc(SDL_RWops * context)
{
    FSMgr::File *fil = (FSMgr::File *)context->hidden.unknown.data2;

    size_t curr = (*fil)->tell();
    (*fil)->seek(0, SEEK_END);
    size_t sz = (*fil)->tell();
    (*fil)->seek(curr, SEEK_SET);
    return sz;
}

static Sint64 myseekfunc(SDL_RWops *context, Sint64 offset, int whence)
{
    FSMgr::File *fil = (FSMgr::File *)context->hidden.unknown.data2;
    switch (whence)
    {
    case RW_SEEK_SET:
        if ( (*fil)->seek(offset, SEEK_SET) != 0 )
            return -1;
        break;
    case RW_SEEK_CUR:
        if ( (*fil)->seek(offset, SEEK_CUR) != 0 )
            return -1;
        break;
    case RW_SEEK_END:
        if ( (*fil)->seek(offset, SEEK_END) != 0 )
            return -1;
        break;
    default:
        SDL_SetError("Can't seek of this kind of whence");
        return -1;
    }
    return (*fil)->tell();
}

static size_t myreadfunc(SDL_RWops *context, void *ptr, size_t size, size_t maxnum)
{
    FSMgr::File *fil = (FSMgr::File *)context->hidden.unknown.data2;
    return (*fil)->read(ptr, size * maxnum) / size;
}

static size_t mywritefunc(SDL_RWops *context, const void *ptr, size_t size, size_t num)
{
    FSMgr::File *fil = (FSMgr::File *)context->hidden.unknown.data2;
    return (*fil)->write(ptr, size * num) / size;
}

static int myclosefunc(SDL_RWops *context)
{
    //FSMgr::File *fil = (FSMgr::File *)context->hidden.unknown.data2;
    //delete fil;

    SDL_FreeRW(context);
    return 0;
}

static SDL_RWops * MyCustomRWop(FSMgr::File *fil)
{
    SDL_RWops *c = SDL_AllocRW();

    if (c == NULL)
        return NULL;

    c->size = mysizefunc;
    c->seek = myseekfunc;
    c->read = myreadfunc;
    c->write = mywritefunc;
    c->close = myclosefunc;
    c->type = 0xF00BA91;
    c->hidden.unknown.data2 = fil;
    return c;
}

Common::PlaneVector<uint8_t>* Resources::LoadMask(FSMgr::File *pfile)
{
    SDL_RWops *rwops = MyCustomRWop(pfile);
    if (!rwops)
        return nullptr;
    
    SDL_Surface *loaded = IMG_Load_RW(rwops, 1);
    
    if (!loaded)
    {
        printf("%s\n", SDL_GetError());
        return nullptr;
    }
    
    if (loaded->format->BytesPerPixel != 1)
    {
        SDL_FreeSurface(loaded);
        return nullptr;
    }
    
    SDL_LockSurface(loaded);

    Common::PlaneVector<uint8_t> *mask = new Common::PlaneVector<uint8_t>(loaded->w, loaded->h);
    
    for(int32_t y = 0; y < loaded->h; ++y)
    {
        for(int32_t x = 0; x < loaded->w; ++x)
            mask->At(x, y) = *((uint8_t *)loaded->pixels + y * loaded->pitch + x);
    }

    SDL_UnlockSurface(loaded);
    SDL_FreeSurface(loaded);
    
    return mask;
}

bool Resources::LoadMask()
{
    FSMgr::File f = FSMgr::Mgr::ReadFile("res/mask.png");
    if (!f)
        return false;
    
    ScreenMask = LoadMask(&f);
    if (!ScreenMask)
        return false;
    
    f = FSMgr::Mgr::ReadFile("res/mapmask.png");
    if (!f)
        return false;
    
    MapMask = LoadMask(&f);
    if (!MapMask)
        return false;
    
    return true;
}

bool Resources::LoadSounds(Simix::Mixer *mx)
{
    SoundResFile = FSMgr::Mgr::ReadFile("sounds.res");
    
    if (!SoundResFile)
        return false;

    SoundResFile->seek(8, 0);

    for(size_t i = 0; i < 1000; ++i)
    {
        int32_t spos = SoundResFile->readS32L();
        int32_t ssize = SoundResFile->readS32L();

        if ( spos >= 0 && ssize > 1 )
            SoundResEntries[i] = FileEntry(spos, ssize);
    }

    SoundResOffset = SoundResFile->tell();

    // Load main sounds
    for(int i = 0; i < 32; ++i)
    {
        auto it = SoundResEntries.find(i);
        if (it != SoundResEntries.end())
        {
            FileEntry &entr = it->second;
            SoundResFile->seek(SoundResOffset + entr.Offset, 0);

            std::vector<uint8_t> buf(entr.Size);
            SoundResFile->read(buf.data(), entr.Size);

            Sounds[i] = mx->LoadSample(buf.data(), entr.Size, 22050, AUDIO_S16LSB, 1);
        }
    }

    // Load char base models sound
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 8; ++j)
        {
            const int32_t soundId = 32 + i * 8 + j;
            auto it = SoundResEntries.find(soundId);
            if (it != SoundResEntries.end())
            {
                FileEntry &entr = it->second;
                SoundResFile->seek(SoundResOffset + entr.Offset, 0);

                std::vector<uint8_t> buf(entr.Size);
                SoundResFile->read(buf.data(), entr.Size);

                Sounds[soundId] = mx->LoadSample(buf.data(), entr.Size, 22050, AUDIO_S16LSB, 1);
            }
        }
    }

    return true;   
}

bool Resources::LoadMusicInfo()
{
    MusicResFile = FSMgr::Mgr::ReadFile("musics.res");

    if (!MusicResFile)
        return false;

    MusicResFile->seek(8, 0);

    size_t MusicResOffset = MusicResFile->tell() + MUSIC_NUM * 8;

    for(size_t i = 0; i < MUSIC_NUM; ++i)
    {
        int32_t spos = MusicResFile->readS32L() + MusicResOffset;
        int32_t ssize = MusicResFile->readS32L();

        if ( spos >= 0 && ssize > 1 )
            MusicResEntries[i] = FileEntry(spos, ssize);
    }

    return true;
}


void Resources::LoadDynSound(Simix::Mixer *mx, int32_t soundId)
{
    auto it = SoundResEntries.find(soundId);
    if (it != SoundResEntries.end())
    {
        FileEntry &entr = it->second;
        SoundResFile->seek(SoundResOffset + entr.Offset, 0);

        std::vector<uint8_t> buf(entr.Size);
        SoundResFile->read(buf.data(), entr.Size);

        DynSounds[soundId] = mx->LoadSample(buf.data(), entr.Size, 22050, AUDIO_S16LSB, 1);
    }
}

void Resources::UnloadDynSounds(Simix::Mixer *mx)
{
    for(auto it = DynSounds.begin(); it != DynSounds.end(); it = DynSounds.erase(it))
        mx->UnloadSample(it->second);
}

void Resources::UnloadSounds(Simix::Mixer *mx)
{
    for(auto it = Sounds.begin(); it != Sounds.end(); it = Sounds.erase(it))
        mx->UnloadSample(it->second);
    
    for(auto it = DynSounds.begin(); it != DynSounds.end(); it = DynSounds.erase(it))
        mx->UnloadSample(it->second);
}


bool Resources::PlayMusic(Simix::Mixer *mx, int32_t id)
{
    mx->FreeMusic();

    if (id < 0 || id > MUSIC_NUM)
        return false;
    
    FileEntry &entry = MusicResEntries.at(id);
    mx->PlayMusic(Simix::MUSICTYPE_OGG, &MusicResFile, entry.Offset, entry.Size);
    return true;
}


bool Resources::LoadObjectData(uint32_t id)
{
    //printf("---------- LOADOBJECT %d \n", id);
    if (id < 30)
    {
        if ((DynObjects.at(id).Flags & DynamicObject::FLAG_INFO_LOADED) == 0)
            return false;
        if (DynObjects.at(id).Flags & DynamicObject::FLAG_DATA_LOADED)
            return true;
        
        DynObjects.at(id).LoadData(&ObjectsResFile);
    } 
    else if (id < 512)
    {
        uint32_t sId = id - 30;
        if ((SimpleObjects.at(sId).Flags & SimpleObject::FLAG_INFO_LOADED) == 0)
            return false;
        if (SimpleObjects.at(sId).Flags & SimpleObject::FLAG_DATA_LOADED)
            return true;

        SimpleObjects.at(sId).LoadData(&ObjectsResFile, Palettes.data());

        SimpleObjects.at(sId).Flags &= ~SimpleObject::FLAG_LOADING;
    }
    else
        return false;
    
    return true;
}

}