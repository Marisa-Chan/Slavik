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
        std::array<GFX::PalImage *, 768> Images;
        std::array<GFX::PalImage *, 768> Shadows;

        struct FrameInfo
        {
            int16_t FrameID;
            Common::Point WpnOffset;
            Common::Point ImgOffset;
            Common::Point ShdOffset;
        };
    
        struct SeqInfo
        {
            int16_t FrmCount;
            std::array<FrameInfo, 12> FrameData;
        };
        
        SeqInfo Seq[10][8];  //state dir
        
        void Load(FSMgr::iFile *pfile);
    };
    
    struct DynamicObject
    {
        std::array<GFX::PalImage *, 512> Images;
        std::array<GFX::PalImage *, 512> Shadows;
        
        struct FrameInfo
        {
            int16_t FrameID;
            Common::Point ImgOffset;
            Common::Point ShdOffset;
        };
    
        struct SeqInfo
        {
            int16_t FrmCount;
            std::array<FrameInfo, 18> FrameData;
        };
        
        SeqInfo Seq[6][8];  //state dir
        
        void Load(FSMgr::iFile *pfile);
    };
    
    struct SimpleObject
    {
        struct FlamePos
        {
            int32_t FlameID;
            Common::Point Position;
        };
        
        int32_t fld1 = 0;
        
        std::array<GFX::Image *,8> Images;
        std::array<GFX::PalImage *,8> Shadows;
        
        //int32_t NumFlames;
        std::vector<FlamePos> Flames;
        
        int32_t NumFrames;
        int32_t FrameTime;
        
        Common::SPoint unk[8][2];
        
        void Load(FSMgr::iFile *pfile, const SDL_Color *palettes);
    };
    
    
public:
    Resources() {};
    ~Resources() {};

    bool Load();
    
    bool LoadGraphRes();
    bool LoadLightsRes();
    bool LoadObjectsRes();
    
    static GFX::Image *LoadRL8BitImage(FSMgr::iFile *pfile, const SDL_Color *pal);
    static GFX::PalImage *LoadRL8BitImage(FSMgr::iFile *pfile);
    static GFX::PalImage *LoadRL8BitShadow(FSMgr::iFile *pfile);
    static GFX::Image *LoadRL16BitImage(FSMgr::iFile *pfile);
   
    
public:
    Common::PlaneArray<SDL_Color, 256, 256> Palettes;
    
    std::array<GFX::Image *, 11> Cursors;
    
    std::array<GFX::Image *, 512> Tiles;
    
    std::array<CharacterSprites, 3> CharacterBases;
    std::array<CharacterSprites, 24> CharacterEquip;
    
    
    GFX::Light * Light1 = nullptr;
    GFX::Light * Light2 = nullptr;
    std::array<GFX::Light *, 19> TilesLight;
    
    std::array<DynamicObject, 30> DynObjects;
    std::array<SimpleObject, 482> SimpleObjects;
    
public:
    static Resources Res;
    
    static const std::array<uint8_t, 3> CharBasePal;
    static const std::array<uint8_t, 24> CharEquipPal;
};

static constexpr Resources &Res = Resources::Res;

}

#endif /* RESOURCES_H */

