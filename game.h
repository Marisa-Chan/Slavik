#ifndef GAME_H
#define GAME_H

#include <map>
#include "common/common.h"
#include "resources.h"


namespace Game
{
    
class Engine
{
public:
    enum KEYFN
    {
        KEYFN_MAPLEFT  = 0,
        KEYFN_MAPRIGHT = 1,
        KEYFN_MAPUP    = 2,
        KEYFN_MAPDOWN  = 3,
        
        KEYFN_MAX
    };
    
    struct GameMap
    {
        struct Decoration
        {
            Decoration(int32_t id, Common::Point pos)
            : TileID(id), Position(pos) {};
            
            int32_t TileID = -1;
            Common::Point Position;
        };
        
        struct Object
        {
            int32_t ObjId = -1;
            std::array<uint8_t, 20> Flames;
            int16_t FrameTimeLeft = 0;
            int16_t unk1 = 0;
            int16_t unk2 = 0;
            uint8_t unk3 = 0;
            uint8_t CurrentFrame = 0;
        };
        
        uint8_t TileMap[160][80][2]; //y x subtile
        Common::PlaneArray<uint16_t, 160, 320> FootMap;
        
        std::vector<Decoration> Decorations;
        
        std::vector<Object> MapObjects;
        
        Common::PlaneArray<uint8_t, 80, 160> LightMap;
        
        Common::Point MapLimits;
    };
    
public:
    struct Character
    {
        uint8_t State;
        uint8_t Direction;
        uint8_t field2_0x2;
        uint8_t field3_0x3;
        uint8_t ClassID;
        uint8_t field5_0x5;
        uint8_t field6_0x6;
        uint8_t field7_0x7;
        int16_t field8_0x8;
        int16_t field9_0xa;
        int16_t _field_0xc;
        int16_t _field_0xe;
        uint8_t Frame;
        uint8_t field15_0x11;
        uint8_t field16_0x12;
        uint8_t field17_0x13;
        uint8_t field18_0x14;
        uint8_t field19_0x15;
        uint8_t field20_0x16;
        uint8_t field21_0x17;
        uint8_t field22_0x18;
        uint8_t field23_0x19;
        uint8_t field24_0x1a;
        uint8_t field25_0x1b;
        uint8_t field26_0x1c;
        uint8_t field27_0x1d;
        uint8_t field28_0x1e;
        uint8_t field29_0x1f;
        uint8_t field30_0x20;
        uint8_t field31_0x21;
        uint8_t field32_0x22;
        uint8_t field33_0x23;
        uint8_t field34_0x24;
        uint8_t field35_0x25;
        uint8_t field36_0x26;
        uint8_t field37_0x27;
        uint8_t field38_0x28;
        uint8_t field39_0x29;
        uint8_t field40_0x2a;
        uint8_t field41_0x2b;
        uint8_t field42_0x2c;
        uint8_t field43_0x2d;
        uint8_t field44_0x2e;
        uint8_t field45_0x2f;
        uint8_t field46_0x30;
        uint8_t field47_0x31;
        uint8_t field48_0x32;
        uint8_t Level;
        int32_t Gold;
        int32_t FreePoints;
        int32_t Exp;
        uint16_t Fehtovanie;
        uint16_t Trading;
        uint16_t Metkost;
        uint16_t PlotnickoeDelo;
        uint16_t Medicine;
        uint16_t Identification;
        uint16_t KuznechnoeDelo;
        uint16_t field60_0x4e;
        uint16_t Otravlenie;
        uint8_t field62_0x52;
        uint8_t field63_0x53;
        uint16_t CurrentLovkost;
        uint16_t BaseLovkost;
        uint16_t CurrentHarizm;
        uint16_t BaseHarizm;
        uint16_t HP;
        uint16_t CurrentSila;
        uint16_t BaseSila;
        uint16_t CurrentVinoslivost;
        uint16_t BaseVinoslivost;
        uint16_t ArmorWeapons[6];
        uint16_t field74_0x72[32];
        uint16_t nnnn[5];
        uint8_t field76_0xbc;
        uint8_t field77_0xbd;
        uint8_t field78_0xbe;
        uint8_t field79_0xbf;
        uint8_t field80_0xc0;
        uint8_t field81_0xc1;
        uint8_t field82_0xc2;
        uint8_t field83_0xc3;
        uint8_t field84_0xc4;
        uint8_t field85_0xc5;
        uint8_t field86_0xc6;
        uint8_t field87_0xc7;
        uint8_t field88_0xc8;
        uint8_t field89_0xc9;
        uint8_t field90_0xca;
        uint8_t field91_0xcb;
        uint8_t field92_0xcc;
        uint8_t field93_0xcd;
        uint8_t field94_0xce;
        uint8_t field95_0xcf;
        uint8_t field96_0xd0;
        uint8_t field97_0xd1;
        uint8_t field98_0xd2;
        uint8_t field99_0xd3;
        uint8_t field100_0xd4;
        uint8_t field101_0xd5;
        uint8_t CharacterBase;
        uint8_t FrameCount;
        int32_t paletteOffset;
        void * pSpriteHdr;
        uint16_t * pGraphUnk3;
        int32_t field107_0xe4;
        int32_t xPOS;
        int32_t field109_0xec;
        int32_t yPOS;
        int16_t field111_0xf4;
        int16_t field112_0xf6;
        uint8_t field113_0xf8;
        uint8_t field114_0xf9;
        uint8_t field115_0xfa;
        uint8_t field116_0xfb;
        uint8_t field117_0xfc;
        uint8_t field118_0xfd;
        uint8_t field119_0xfe;
        uint8_t field120_0xff;
        
        void Load(FSMgr::iFile *pfile);
    };
    
    struct MapChar
    {
        int16_t CharacterIndex;
        uint8_t field1_0x2;
        uint8_t field2_0x3;
        uint8_t field3_0x4;
        uint8_t field4_0x5;
        uint8_t field5_0x6;
        uint8_t field6_0x7;
        int16_t field_0x8;
        uint8_t field9_0xa;
        uint8_t field10_0xb;
        int16_t field_0xc;
        uint8_t field13_0xe;
        uint8_t field14_0xf;
        int16_t field_0x10;
        uint8_t field17_0x12;
        uint8_t field18_0x13;
        int16_t field_0x14;
        uint8_t field21_0x16;
        uint8_t field22_0x17;
        uint8_t field23_0x18;
        uint8_t field24_0x19;
        uint8_t field25_0x1a;
        uint8_t MapID;
        uint8_t field27_0x1c;
        uint8_t field28_0x1d;
        uint8_t field29_0x1e;
        uint8_t field30_0x1f;
        
        void Load(FSMgr::iFile *pfile);
    };
    
    struct GameState
    {
        //UnkItemStruct [2000];
        std::array<MapChar, 200> MapChar_ARRAY;
        //DAT_0079efdc
        //DAT_007aa2dc
        std::array<Character, 2000> Characters;
        //DAT_00835b18
        
        void Load(FSMgr::iFile *pfile);
    };
    
public:
    bool Process();
    
    void Init(int gfxmode);
    
    static int EventsWatcher(void *, SDL_Event *event);
    
    void Draw();
    
    void LoadMap(int32_t mapID);
    void DrawMap();
    
    void LoadGameState(int32_t stateID);
    
    static GameMap *LoadGameMap(int32_t mapID);
    
public:
    Common::Point _camera;
    float _zoom = 1.0;
    Common::Rect _visibleTiles;
    
    GameState _state;
    
    std::map<int16_t, int8_t> _KeyMap;
    std::array<int8_t, KEYFN_MAX> _KeyState;
    
    GameMap *_currentMap = nullptr;
    int32_t _currentMapID = -1;
    
public:
    static Engine Instance;
};
    
}

static constexpr Game::Engine &GameInst = Game::Engine::Instance;

#endif /* GAME_H */

