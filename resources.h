#ifndef RESOURCES_H
#define RESOURCES_H

#include <inttypes.h>
#include <array>
#include <map>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>

#include "fsmgr.h"
#include "gfx.h"
#include "common/common.h"
#include "simix/simix.h"

namespace Game {

class Resources
{
public:
    constexpr static const int16_t MUSIC_NUM = 9;
public:

    struct FileEntry
    {
        int32_t Offset = 0;
        int32_t Size = 0;

        FileEntry() = default;
        FileEntry(int32_t Off, int32_t sz)
         : Offset(Off), Size(sz) {};
    };
 
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
        
        void Load(FSMgr::File *pfile);
    };
    
    struct DynamicObject
    {
        enum
        {
            FLAG_INFO_LOADED = (1 << 8),
            FLAG_DATA_LOADED = (1 << 9)
        };

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

        uint16_t Flags = 0;
        
        bool Loaded = false;
        
        int32_t Size = 0;

        std::array<GFX::PalImage *, 512> Images;
        std::array<GFX::PalImage *, 512> Shadows;
        
        SeqInfo Seq[6][8];  //state dir

        // Where All data placed
        int32_t DataOffset = -1;

        std::array<int32_t, 512> ShdOffsets;
        std::array<int32_t, 512> ImgOffsets;
        
        void LoadInfo(FSMgr::File *pfile);
        void LoadData(FSMgr::File *pfile);
    };
    
    struct SimpleObject
    {
        enum 
        {
            FLAG_LOADING = (1 << 7),
            FLAG_INFO_LOADED = (1 << 8),
            FLAG_DATA_LOADED = (1 << 9),
        };

        struct FlamePos
        {
            int32_t Index = -1;
                    
            int32_t FlameID; //Animation seq id
            Common::Point Position;
        };
        
        struct FrameInfo
        {
            Common::Point ShdOffset;
            Common::Point Offset;
        };

        struct ImgShdOffset
        {
            int32_t ImgOffset = -1;
            int32_t ShdOffset = -1;
        };
        
        uint32_t Flags = 0;
        
        std::array<GFX::Image *,8> Images;
        std::array<GFX::PalImage *,8> Shadows;
        
        //int32_t NumFlames;
        std::vector<FlamePos> Flames;
        
        int32_t NumFrames = 0;
        int32_t FrameTime = 0;

        int32_t PaletteOffset = 0;
        
        std::array<FrameInfo, 8> Offsets;

        // Where All data placed
        int32_t DataOffset = -1;

        std::array<ImgShdOffset, 8> DataImgOffsets;
        
        void LoadInfo(FSMgr::File *pfile);
        void LoadData(FSMgr::File *pfile, const SDL_Color *palettes);

        bool IsDataLoaded() const { return (Flags & FLAG_DATA_LOADED) != 0; }
    };
    
    
public:
    Resources() {};
    ~Resources() {};

    bool Load(Simix::Mixer *mx);
    
    bool LoadGraphRes();
    bool LoadLightsRes();
    bool LoadObjectsRes();
    bool LoadFlames();
    bool LoadMask();
    bool LoadSounds(Simix::Mixer *mx);
    bool LoadMusicInfo();

    void LoadDynSound(Simix::Mixer *mx, int32_t soundId);
    void UnloadDynSounds(Simix::Mixer *mx);
    
    static GFX::Image *LoadRL8BitImage(FSMgr::File *pfile, const SDL_Color *pal);
    static GFX::PalImage *LoadRL8BitImage(FSMgr::File *pfile);
    static GFX::PalImage *LoadRL8BitShadow(FSMgr::File *pfile);
    static GFX::Image *LoadRL16BitImage(FSMgr::File *pfile);
    static GFX::Image *LoadFlameImage(FSMgr::File *pfile);
    static Common::PlaneVector<uint8_t> *LoadMask(FSMgr::File *pfile);


    void UnloadSounds(Simix::Mixer *mx);

    bool PlayMusic(Simix::Mixer *mx, int32_t id);

    bool LoadObjectData(uint32_t id);
    
public:
    Common::PlaneArray<SDL_Color, 256, 256> Palettes;
    
    std::array<GFX::Image *, 11> CursorImages;
    
    std::array<GFX::Image *, 512> Tiles;
    
    std::array<CharacterSprites, 3> CharacterBases;
    std::array<CharacterSprites, 24> CharacterEquip;
    
    
    GFX::Light * Light1 = nullptr;
    GFX::Light * Light2 = nullptr;
    std::array<GFX::Light *, 19> TilesLight;
    
    std::array<DynamicObject, 30> DynObjects;
    std::array<SimpleObject, 482> SimpleObjects;
    
    std::array<GFX::Image *,74> Flames;
    
    Common::PlaneVector<uint8_t> *ScreenMask = nullptr;
    Common::PlaneVector<uint8_t> *MapMask = nullptr;

    std::map<uint16_t, Simix::MixBuffer *> Sounds;
    std::map<uint16_t, Simix::MixBuffer *> DynSounds;

private:
    FSMgr::File SoundResFile;
    std::map<uint16_t, FileEntry> SoundResEntries;
    size_t SoundResOffset = 0;

    FSMgr::File MusicResFile;
    std::array<FileEntry, MUSIC_NUM> MusicResEntries;   
    
    FSMgr::File ObjectsResFile;

public:
    static Resources Res;
    
    static const std::array<uint8_t, 3> CharBasePal;
    static const std::array<uint8_t, 24> CharEquipPal;
};

static constexpr Resources &Res = Resources::Res;

}

#endif /* RESOURCES_H */

