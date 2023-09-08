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

void Resources::CharacterSprites::Load(FSMgr::iFile *pfile,  const SDL_Color *pal)
{
    size_t spos = pfile->tell();
    pfile->seek(4, 1); // Skip
    uint32_t dataSize = pfile->readU32L();
    
    for(int8_t k = 0; k < 10; ++k)
    {
        for(int8_t j = 0; j < 8; ++j)
        {
            Frames[k][j] = pfile->readS16L();
            
            for(int8_t i = 0; i < 12; ++i)
                Seq[k][j][i] = pfile->readS16L();
        }
    }
    
    for(int8_t k = 0; k < 10; ++k)
    {
        for(int8_t j = 0; j < 8; ++j)
        {
            for(int8_t i = 0; i < 12; ++i)
            {
                Common::SPoint &p = unk2[k][j][i];
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
            for(int8_t i = 0; i < 12; ++i)
            {
                Common::SPoint &p1 = unk3[k][j][i][0];
                p1.x = pfile->readS16L();
                p1.y = pfile->readS16L();
                
                Common::SPoint &p2 = unk3[k][j][i][1];
                p2.x = pfile->readS16L();
                p2.y = pfile->readS16L();
            }
        }
    }
    
    size_t sprPos = pfile->tell();
    
    for(size_t i = 0; i < 768; ++i)
    {
        int32_t sz = sprHdr[i * 2];
        int32_t soff = sprHdr[i * 2 + 1];
        
        if (soff >= 0 && sz > 0)
        {
            pfile->seek(sprPos + soff, 0);
            Images[i] = LoadRL8BitImage(pfile, pal);
        }
        else
            Images[i] = nullptr;        
    }
    
    pfile->seek(spos + dataSize, 0);
}

    
bool Resources::Load()
{
    if ( LoadGraphRes() )
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
        SDL_Color &cl = Palettes.at(j);
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
        CharacterBases.at(i).Load(graph.get(), Palettes.data() + CharBasePal.at(i) * 256);
    
    for(size_t i = 0; i < 24; ++i)
        CharacterEquip.at(i).Load(graph.get(), Palettes.data() + CharEquipPal.at(i) * 256);
    
    
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


Resources::GameMap *Resources::LoadGameMap(int32_t mapID)
{
    FSMgr::File f = FSMgr::Mgr::ReadFile(fmt::format("KONUNG.{:d}", mapID));
    if (!f)
        return nullptr;
    
    GameMap *map = new GameMap();
    for(size_t j = 0; j < 160; ++j)
    {
        for(size_t i = 0; i < 80; ++i)
        {
            map->TileMap[j][i][0] = f->readU8();
            map->TileMap[j][i][1] = f->readU8();
        }
    }
    
    return map;
}

}