#include "resources.h"
#include "fsmgr.h"
#include "fmt/printf.h"
#include <vector>
#include <SDL2/SDL_video.h>


namespace Game {

Resources Resources::Res;

const std::array<uint8_t, 3> Resources::CharBasePal = { 74, 33, 39 };

const std::array<uint8_t, 24> Resources::CharEquipPal = { 2, 2, 2, 2, 4, 4, 
                                                          3, 3, 3, 4, 4, 4,
                                                          4, 4, 4, 4, 4, 4,
                                                          2, 2, 5, 5, 5, 5 };

void Resources::CharacterSprites::Load(FSMgr::iFile *pfile)
{
    size_t spos = pfile->tell();
    pfile->seek(4, 1); // Skip
    uint32_t dataSize = pfile->readU32L();
    
    for(int8_t k = 0; k < 10; ++k)
    {
        for(int8_t j = 0; j < 8; ++j)
        {
            SeqInfo &s = Seq[k][j];
            s.FrmCount = pfile->readS16L();
            
            for(int8_t i = 0; i < 12; ++i)
                s.FrameData[i].FrameID = pfile->readS16L();
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
                p.x = pfile->readS16L();
                p.y = pfile->readS16L();
            }
        }
    }
    
    std::vector<int32_t> sprHdr(768 * 2);
    for(size_t i = 0; i < 768; ++i)
    {
        sprHdr[i * 2] = pfile->readS32L();
        sprHdr[i * 2 + 1] = pfile->readS32L();
    }
        
    for(int8_t k = 0; k < 10; ++k)
    {
        for(int8_t j = 0; j < 8; ++j)
        {
            SeqInfo &s = Seq[k][j];
            for(int8_t i = 0; i < 12; ++i)
            {
                Common::Point &p1 = s.FrameData[i].ShdOffset;
                p1.x = pfile->readS16L();
                p1.y = pfile->readS16L();
                
                Common::Point &p2 = s.FrameData[i].ImgOffset;
                p2.x = pfile->readS16L();
                p2.y = pfile->readS16L();
            }
        }
    }
    
    size_t sprPos = pfile->tell();
    
    for(size_t i = 0; i < 768; ++i)
    {
        int32_t shdOff = sprHdr[i * 2];
        int32_t imgOff = sprHdr[i * 2 + 1];
        
        if (imgOff >= 0)
        {
            pfile->seek(sprPos + imgOff, 0);
            Images[i] = LoadRL8BitImage(pfile);
        }
        else
            Images[i] = nullptr;
        
        if (shdOff > 0)
        {
            pfile->seek(sprPos + shdOff, 0);
            Shadows[i] = LoadRL8BitShadow(pfile);
        }
        else
            Shadows[i] = nullptr;        
    }
    
    pfile->seek(spos + dataSize, 0);
}

void Resources::DynamicObject::Load(FSMgr::iFile *pfile)
{
    size_t spos = pfile->tell();
    pfile->seek(4, 1);
    uint32_t dataSize = pfile->readU32L();
    
    for(int8_t k = 0; k < 6; ++k)
    {
        for(int8_t j = 0; j < 8; ++j)
        {
            SeqInfo &s = Seq[k][j];
            s.FrmCount = pfile->readS16L();
            
            for(int8_t i = 0; i < 18; ++i)
                s.FrameData[i].FrameID = pfile->readS16L();
        }
    }
    
    std::vector<int32_t> sprHdr(512 * 2);
    for(size_t i = 0; i < 512; ++i)
    {
        sprHdr[i * 2] = pfile->readS32L();
        sprHdr[i * 2 + 1] = pfile->readS32L();
    }
    
    for(int8_t k = 0; k < 6; ++k)
    {
        for(int8_t j = 0; j < 8; ++j)
        {
            SeqInfo &s = Seq[k][j];
            for(int8_t i = 0; i < 18; ++i)
            {
                Common::Point &p1 = s.FrameData[i].ShdOffset;
                p1.x = pfile->readS16L();
                p1.y = pfile->readS16L();
                
                Common::Point &p2 = s.FrameData[i].ImgOffset;
                p2.x = pfile->readS16L();
                p2.y = pfile->readS16L();
            }
        }
    }
    
    size_t sprPos = pfile->tell();
    
    for(size_t i = 0; i < 512; ++i)
    {
        int32_t shdOff = sprHdr[i * 2];
        int32_t imgOff = sprHdr[i * 2 + 1];
        
        if (imgOff >= 0)
        {
            pfile->seek(sprPos + imgOff, 0);
            Images[i] = LoadRL8BitImage(pfile);
        }
        else
            Images[i] = nullptr;      
        
        if (shdOff > 0)
        {
            pfile->seek(sprPos + shdOff, 0);
            Shadows[i] = LoadRL8BitShadow(pfile);
        }
        else
            Shadows[i] = nullptr;  
    }
    
    pfile->seek(spos + dataSize, 0);
}

void Resources::SimpleObject::Load(FSMgr::iFile *pfile, const SDL_Color *palettes)
{
    size_t spos = pfile->tell();
    
    fld1 = pfile->readU32L();
    uint32_t dataSize = pfile->readU32L();
    
    int32_t numFlame = pfile->readS32L();
    
    NumFrames = pfile->readU32L();
    
    pfile->seek(4, 1);
    
    uint32_t palette = pfile->readU32L();
    
    FrameTime = pfile->readS32L();
    
    if (numFlame)
    {
        Flames.resize(numFlame);

        for(size_t i = 0; i < numFlame; ++i)
        {
            FlamePos &flame = Flames[i];
            flame.FlameID = pfile->readS8();
            flame.Position.x = pfile->readS16L();
            flame.Position.y = pfile->readS16L();
        }
    }
    
    pfile->seek((20 - numFlame) * 5, 1);
    
    std::vector<int32_t> sprHdr(8 * 2);
    for(size_t i = 0; i < 8; ++i)
    {
        sprHdr[i * 2] = pfile->readS32L();
        sprHdr[i * 2 + 1] = pfile->readS32L();
    }
    
    for(int8_t i = 0; i < 8; ++i)
    {
        Common::SPoint &p1 = unk[i][0];
        p1.x = pfile->readS16L();
        p1.y = pfile->readS16L();

        Common::SPoint &p2 = unk[i][1];
        p2.x = pfile->readS16L();
        p2.y = pfile->readS16L();
    }
    
    size_t sprPos = pfile->tell();
    
    for(size_t i = 0; i < 8; ++i)
    {
        int32_t sz = sprHdr[i * 2];
        int32_t soff = sprHdr[i * 2 + 1];
        
        if (soff >= 0 && sz > 0)
        {
            pfile->seek(sprPos + soff, 0);
            Images[i] = LoadRL8BitImage(pfile, palettes + (palette / 2));
            Shadows[i] = LoadRL8BitShadow(pfile);
        }
        else
            Images[i] = nullptr;        
    }
    
    pfile->seek(spos + dataSize, 0);
}

    
bool Resources::Load()
{
    if ( LoadGraphRes() &&
         LoadLightsRes() &&
         LoadObjectsRes() )
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
        
        Cursors.at(j) = LoadRL16BitImage(graph.get());
        
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
            Tiles.at(j) = LoadRL8BitImage(graph.get(), Palettes.data() + pal / 2);
        }
        else
            Tiles.at(j) = nullptr;
    }
    
    graph->seek(fpos + sz, 0);
    
    for(size_t i = 0; i < 3; ++i)
        CharacterBases.at(i).Load(graph.get());
    
    for(size_t i = 0; i < 24; ++i)
        CharacterEquip.at(i).Load(graph.get());
    
    
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
    FSMgr::File fobj = FSMgr::Mgr::ReadFile("objects.res");
    
    if (!fobj)
        return false;
    
    fobj->seek(8, 1); // skip offset and res size
    
    std::vector<int32_t> headers(512 * 2);
    for(size_t i = 0; i < 512; ++i)
    {
        headers[i * 2] = fobj->readS32L();
        headers[i * 2 + 1] = fobj->readS32L();
    }
    
    size_t spos = fobj->tell();
    
    for(size_t i = 0; i < 30; ++i)
    {
        if (headers[i * 2] >= 0)
        {
            fobj->seek(spos + headers[i * 2],0);
            DynObjects[i].Load(fobj.get());
        }
    }
    
    for(size_t i = 0; i < 482; ++i)
    {
        size_t j = 30 + i;
        if (headers[j * 2] >= 0)
        {
            fobj->seek(spos + headers[j * 2],0);
            SimpleObjects[i].Load(fobj.get(), Palettes.data());
        }
    }
    
    return true;
}

GFX::Image *Resources::LoadRL8BitImage(FSMgr::iFile *pfile, const SDL_Color *pal)
{
    short w = pfile->readS16L();
    short h = pfile->readS16L();
    
    /*std::vector<short> linesLength(h);
    for(int32_t i = 0; i < h; ++i)
        linesLength.at(i) = pfile->readS16L();*/
    pfile->seek(h * 2, 1); // skip
    
    GFX::Image *img = GFXDrawer.CreateImage({w, h});
    
    GFXDrawer.LockImage(img);
    for(int32_t y = 0; y < h; ++y)
    {
        uint32_t *px = (uint32_t *)((char *)img->SW->pixels + img->SW->pitch * y);
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
                    const SDL_Color &cl = pal[ pfile->readU8() ];
                    *px = SDL_MapRGBA(img->SW->format, cl.r, cl.g, cl.b, 255); 
                    px++;
                }
            }
        }
    }
    GFXDrawer.UnlockImage(img);
    
    return img;
}

GFX::PalImage *Resources::LoadRL8BitImage(FSMgr::iFile *pfile)
{
    short w = pfile->readS16L();
    short h = pfile->readS16L();
    
    /*std::vector<short> linesLength(h);
    for(int32_t i = 0; i < h; ++i)
        linesLength.at(i) = pfile->readS16L();*/
    pfile->seek(h * 2, 1); // skip
    
    GFX::PalImage *img = GFXDrawer.CreatePalImage({w, h});
    
    for(int32_t y = 0; y < h; ++y)
    {
        uint8_t *px = (uint8_t *)(img->SW.data() + img->SW.Width() * y);
        while(true)
        {
            uint8_t n = pfile->readU8();
            
            if (n == 0)
                break;
                        
            if (n & 0x80)
                px += (n & 0x7F) * 2;
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
                    px[0] = pfile->readU8();
                    px[1] = 255; 
                    px += 2;
                }
            }
        }
    }
    GFXDrawer.UpdatePalImage(img);
    
    return img;
}


GFX::PalImage *Resources::LoadRL8BitShadow(FSMgr::iFile *pfile)
{
    short w = pfile->readS16L();
    short h = pfile->readS16L();
    
    /*std::vector<short> linesLength(h);
    for(int32_t i = 0; i < h; ++i)
        linesLength.at(i) = pfile->readS16L();*/
    pfile->seek(h * 2, 1); // skip
    
    GFX::PalImage *img = GFXDrawer.CreatePalImage({w, h});
    
    for(int32_t y = 0; y < h; ++y)
    {
        uint8_t *px = (uint8_t *)(img->SW.data() + img->SW.Width() * y);
        while(true)
        {
            uint8_t n = pfile->readU8();
            
            if (n == 0)
                break;
                        
            if (n & 0x80)
                px += (n & 0x7F) * 2;
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

GFX::Image *Resources::LoadRL16BitImage(FSMgr::iFile *pfile)
{
    short w = pfile->readS16L();
    short h = pfile->readS16L();
    
    /*std::vector<short> linesLength(h);
    for(int32_t i = 0; i < h; ++i)
        linesLength.at(i) = pfile->readS16L();*/
    pfile->seek(h * 2, 1); // skip
    
    GFX::Image *img = GFXDrawer.CreateImage({w, h});
    
    GFXDrawer.LockImage(img);
    for(int32_t y = 0; y < h; ++y)
    {
        uint32_t *px = (uint32_t *)((char *)img->SW->pixels + img->SW->pitch * y);
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




}