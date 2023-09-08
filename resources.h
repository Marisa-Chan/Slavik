#ifndef RESOURCES_H
#define RESOURCES_H

#include <inttypes.h>
#include <array>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>

#include "fsmgr.h"
#include "gfx.h"
#include "common/common.h"

namespace Game {

class Resources
{
public:
    struct CharacterSprites
    {
        std::array<GFX::Image *, 768> Images;
        short Frames[10][8]; //state dir
        short Seq[10][8][12]; //state dir frame
        Common::SPoint unk2[10][8][12];
        Common::SPoint unk3[10][8][12][2];
        
        void Load(FSMgr::iFile *pfile, const SDL_Color *pal);
    };
    
    struct GameMap
    {
        uint8_t TileMap[160][80][2]; //y x subtile
    };
public:
    Resources() {};
    ~Resources() {};

    bool Load();
    
    bool LoadGraphRes();
    
    static GFX::Image *LoadRL8BitImage(FSMgr::iFile *pfile, const SDL_Color *pal);
    static GFX::Image *LoadRL16BitImage(FSMgr::iFile *pfile);

public:
    
    static GameMap *LoadGameMap(int32_t mapID);
    
public:
    std::array<SDL_Color, 256 * 256 > Palettes;
    
    std::array<GFX::Image *, 11> Cursors;
    
    std::array<GFX::Image *, 512> Tiles;
    
    std::array<CharacterSprites, 3> CharacterBases;
    std::array<CharacterSprites, 24> CharacterEquip;
    
public:
    static Resources Res;
    
    static const std::array<uint8_t, 3> CharBasePal;
    static const std::array<uint8_t, 24> CharEquipPal;
};

static constexpr Resources &Res = Resources::Res;

}

#endif /* RESOURCES_H */

