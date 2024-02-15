#ifndef GAME_H
#define GAME_H

#include <map>
#include "common/common.h"
#include "resources.h"
#include "font.h"



namespace Game
{
    
constexpr const int32_t TileW = 116;
constexpr const int32_t TileWh = TileW / 2;
constexpr const int32_t TileWhh = TileW / 4;
constexpr const int32_t TileH = 32;
constexpr const int32_t TileHh = TileH / 2;
constexpr const int32_t DayDuration = 21600;

constexpr const int32_t FrameTime = 60; //ms

constexpr const int32_t SCREENRESX = 800;
constexpr const int32_t SCREENRESY = 600;
constexpr const Common::Point SCREENRES = Common::Point(SCREENRESX, SCREENRESY);

constexpr const int32_t MAPVIEWW = 660;
constexpr const int32_t MAPVIEWH = 556;

constexpr const int32_t TILEMAXW = 80;
constexpr const int32_t TILEMAXH = 160;

constexpr const Common::Rect MAPRECT = Common::Rect(SCREENRESX - MAPVIEWW, 0, SCREENRESX, MAPVIEWH);


constexpr const float PX = 1.0 / (float) SCREENRESX;
constexpr const float PY = 1.0 / (float) SCREENRESY;

constexpr const int32_t TXTHEIGHT = 15;

extern const Common::Point GScrOff;


class Engine
{
public:
    enum KEYFN
    {
        KEYFN_MAPLEFT  = 0,
        KEYFN_MAPRIGHT = 1,
        KEYFN_MAPUP    = 2,
        KEYFN_MAPDOWN  = 3,
        
        KEYFN_SHIFT = 4,
        KEYFN_CTRL = 5,
        
        KEYFN_MAX
    };
    
    enum MOUSEB
    {
        MOUSEB_L = 1,
        MOUSEB_R = 2,
    };
    
    enum STATEMODE
    {
        STATEMD_PLAY = 0,
        STATEMD_MAINMENU = 1,
        STATEMD_NEWGAME = 2,
        STATEMD_QUIT = 3,
        STATEMD_MODE4 = 4,
        STATEMD_MODE5 = 5,
        STATEMD_SETTINGS = 6,
        STATEMD_MODE7 = 7,
        STATEMD_MODE8 = 8,
        STATEMD_MODE9 = 9,
        
        STATEMD_UNKNOWN = -1
    };
    
    enum PLAYSCREEN
    {
        PLSCREEN_0   = 0,
        PLSCREEN_MAP = 5,
    };
    
    enum MAINBTN
    {
        MAINBTN_RESUME = 0,
        MAINBTN_SAVE = 1,
        MAINBTN_LOAD = 2,
        MAINBTN_NEW = 3,
        MAINBTN_SETTINGS = 4,
        MAINBTN_EXIT = 5
    };
    
    enum BONUS
    {
        BONUS_KHAR = 0,
        BONUS_LOV  = 1,
        BONUS_KHAR2 = 2,
        BONUS_HP   = 3,
        BONUS_SIL  = 4,
        BONUS_VIN  = 5,
        BONUS_BRN  = 6,
        BONUS_UDR  = 7,
        BONUS_VER  = 8,
        
        BONUS_MAX
    };
    
    enum KHAR
    {
        KHAR_LEVEL = 0,
        KHAR_N1 = 1,
        KHAR_LOVKOST = 2,
        KHAR_HARIZMA = 3,
        KHAR_N4 = 4,
        KHAR_SILA = 5,
        KHAR_VINOSLIVOST = 6,
    };
    
    enum CLASSID
    {
        CLASS_KUPETC = 2,
        CLASS_OHOTNIK = 3,
        CLASS_VOJD = 4,
        CLASS_VOIN = 5,
        
        CLASS_MASK = 7,
        
        CLASS_BIT40 = 0x40,
        CLASS_BIT80 = 0x80,
        
    };
    
    enum EQSLOT
    {
        ESLT_0 = 0,
        ESLT_1 = 1,
        ESLT_2 = 2,
        ESLT_3 = 3,
        ESLT_4 = 4,
        ESLT_5 = 5,
                
        EQSLOT_UNK = -1,
    };
    
    enum CHSTATE
    {
        CHSTATE_0 = 0,
        CHSTATE_1 = 1,
        CHSTATE_2 = 2,
        CHSTATE_3 = 3,
        CHSTATE_4 = 4,
        CHSTATE_5 = 5,
        CHSTATE_6 = 6,
        CHSTATE_7 = 7,
        CHSTATE_8 = 8,
        CHSTATE_9 = 9,
    };
    
    enum DIRECTION
    {
        DIR_0 = 0, 
        DIR_1 = 1, 
        DIR_2 = 2,
        DIR_3 = 3,
        DIR_4 = 4,
        DIR_5 = 5,
        DIR_6 = 6,
        DIR_7 = 7,
    };
    
public:
    
    
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
            int32_t Index = -1; //In vector
            
            
            int32_t ObjId = -1; //SimpleObject ID
            std::array<uint8_t, 20> Flames; // Flame frame id
            int16_t FrameTimeLeft = 0;
            Common::Point Pos;
            uint8_t unk3 = 0;
            uint8_t CurrentFrame = 0;
        };
        
        struct Cell
        {
            uint8_t Flags = 0;
            int32_t ID = 0;
        };
        
        uint8_t TileMap[160][80][2]; //y x subtile
        Common::PlaneArray<Cell, 160, 320> FootMap;
        
        std::vector<Decoration> Decorations;
        
        std::vector<Object> MapObjects;
        
        Common::PlaneArray<uint8_t, 80, 160> LightMap;
        Common::PlaneArray<uint8_t, 80, 160> LightMap2;
        
        Common::Point MapLimits;
    };
    
    struct ImagePlace
    {
        GFX::Image *Img = nullptr;
        GFX::PalImage *PalImg = nullptr;
        Common::Point DrawPlace;
        Common::Rect Limits;
        int32_t Pal = -1;
    };
    
    struct TextPlace
    {
        std::string Text;
        GFX::Font *Font = nullptr;
        Common::Point DrawPlace;
        Common::Rect Limits;
    };
    
    struct Character
    {
        int32_t CharIndex = -1;
        uint8_t State;
        int8_t Direction;
        uint8_t field2_0x2;
        uint8_t field_0x3;
        uint8_t ClassID;
        int32_t MapCharID;
        int16_t field6_0x6;
        Common::Point Tile;
        int16_t _field_0xc;
        int16_t _field_0xe;
        uint8_t Frame;
        uint8_t field15_0x11;
        uint8_t field_0x12;
        std::array<uint8_t, 32> field17_0x13;
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
        int16_t Fehtovanie;
        int16_t Trading;
        int16_t Metkost;
        int16_t PlotnickoeDelo;
        int16_t Medicine;
        int16_t Identification;
        int16_t KuznechnoeDelo;
        int16_t Armor;
        int16_t Otravlenie;
        uint8_t field62_0x52;
        uint8_t field63_0x53;
        int16_t CurrentLovkost;
        int16_t BaseLovkost;
        int16_t CurrentHarizm;
        int16_t BaseHarizm;
        int16_t HP;
        int16_t CurrentSila;
        int16_t BaseSila;
        int16_t CurrentVinoslivost;
        int16_t BaseVinoslivost;
        std::array<int16_t, 6> ArmorWeapons;
        std::array<uint16_t, 32> field74_0x72;
        std::array<uint16_t, 5> nnnn;
        uint8_t field76_0xbc;
        uint8_t field77_0xbd;
        uint8_t field78_0xbe;
        uint8_t field79_0xbf;
        uint8_t field80_0xc0;
        uint8_t field81_0xc1;
        uint8_t field82_0xc2;
        uint8_t field83_0xc3;
        int16_t CurrentBrn;
        int16_t CurrentUdr;
        int16_t CurrentVer;
        uint8_t field90_0xca;
        uint8_t field91_0xcb;
        uint8_t field92_0xcc;
        uint8_t field93_0xcd;
        int16_t field_0xce;
        int8_t field96_0xd0;
        uint8_t Flags;
        int8_t field98_0xd2;
        int8_t field99_0xd3;
        uint8_t NameID;
        uint8_t NickID;
        uint8_t CharacterBase;
        uint8_t FrameCount;
        int32_t paletteOffset;
        int32_t pFrame;
        Common::Point shdOffset;
        Common::Point imgOffset;
        Common::Point wpnOffset;
        int32_t field107_0xe4;
        //int32_t xPOS;
        Common::Point POS;
        int32_t field109_0xec;
        //int32_t yPOS;
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
        
        int32_t GetMaxPartySize();
    };
    
    struct MapChar
    {
        int32_t Index = -1;
        int16_t CharacterIndex = 0;
        int32_t field1_0x2;
        uint8_t field5_0x6;
        uint8_t field6_0x7;
        Common::Rect spwnTlRect;
        Common::Rect someRect;
        uint8_t unk1;
        uint8_t unk2;
        uint8_t unk3;
        uint8_t MapID;
        uint8_t GroupSize;
        uint8_t unk4;
        uint8_t unk5;
        uint8_t unk6;
        
        void Load(FSMgr::iFile *pfile);
    };
    
    struct ItemInfo
    {
        enum FLAGS
        {
            FLAG_IDENTIFIED = (1 << 0),
        };
        
        int32_t Index = -1;
        
        int8_t TypeID = -1;
        int8_t BonusID = -1;
        int8_t SpecialID = -1;
        int8_t InfoID = -1;
        float Concentration = 0.0;
        float Weight = 0.0;
        int16_t Poison = 0;
        uint8_t Flags = 0;
        uint8_t unk1 = 0;
        
        void Load(FSMgr::iFile *pfile);
    };
    
    struct GS1
    {
        int32_t Index = -1;
        
        Common::Point Pos;
        Common::Point Tile;
        int16_t ImgID = 0;
        int16_t unk1 = 0;
        int16_t unk2 = -1;
        int16_t MapID = 0;
        int16_t ItemID = 0;
        int16_t unk3[21];
        
        void Load(FSMgr::iFile *pfile);
    };
    
    struct GS2
    {
        uint8_t field0_0x0;
        uint8_t field1_0x1;
        uint8_t field2_0x2;
        int8_t MapID;
        uint8_t field4_0x4;
        int16_t field5_0x5;
        int16_t field6_0x7;
        int16_t left;
        int16_t up;
        int16_t right;
        int16_t bottom;
        
        void Load(FSMgr::iFile *pfile);
    };
    
    struct Village
    {
        struct Job
        {
            uint8_t Direction = DIR_0;
            uint8_t unk = 0;
            int16_t CharID = 0;
            Common::Point Tile;
            
            void Load(FSMgr::iFile *pfile);
        };
        
        struct BldState
        {
            int8_t BuildInfoID = 0;
            int8_t State = 0;
            int16_t ObjID = 0;
            int16_t unk = 0;
            int16_t BldLeft = 0;
            
            void Load(FSMgr::iFile *pfile);
        };
        
        int32_t Index = -1;
        
        uint8_t Bld1Num = 0;
        uint8_t Bld2Num = 0;
        uint8_t ChiefCharId = 0;
        uint8_t MapID = 0;
        
        int32_t UpdateMedicTimer = 0;
        int32_t SmithUpdateTimer = 0;
        int32_t VoevodaUpdateTimer = 0;
        int32_t GoldTribute = 0;
        int32_t GoldTributeTimer = 0;
        
        std::array<BldState, 119> BuildingState;
        std::array<Job, 5> Jobs;
        
        int32_t unk1 = 0;
        
        int32_t TimeSt1 = 0;
        int32_t TimeSt2 = 0;
        int32_t TimeSt3 = 0;
        
        int32_t unk2 = 0;
        
        int16_t DoMedicPotionSlot6 = 0;
        int16_t DoMedicPotionSlot7 = 0;
        int16_t DoMedicPotionSlot5 = 0;
        
        std::array<int16_t, 24> MedicItems;
        
        int16_t unk3 = 0;
        
        std::array<int16_t, 32> TraderItems;
        std::array<int16_t, 23> SmithItems;
        
        uint8_t Flags = 0;
        
        uint8_t unk4 = 0;
        int8_t SmithItemsCount = 0;
        
        Village() {
            MedicItems.fill(0);
            TraderItems.fill(0);
            SmithItems.fill(0);
        }
        
        void Load(FSMgr::iFile *pfile);
    };
    
    struct WeapArmorItemInfo
    {
        int32_t ID;
        int32_t Dmg;
        int32_t unk1;
        int32_t unk2;
        int32_t unk3;
        int32_t unk4;
        int32_t unk5;
        int32_t unk6;
        int32_t unk7;
        int32_t LootImgID;
        int32_t SprImage;
    };
    
    struct AccessorieItemInfo
    {
        int32_t ID;
        int16_t BonusID;
        int32_t Weight;
        int32_t ImgID;
        int32_t LootImgID;
    };
    
    struct BldInfo
    {
        int32_t ID;
        int16_t NeededPlotnicLvl;
        int16_t GraphObjectOffset;
        int16_t BuildPeriod;
        int16_t BurnPeriod2;
        int16_t __unk3;
        int16_t __unk4;
        int16_t __unk5;
    };
    
    
    struct GameState
    {
        std::array<ItemInfo, 8192> Items;
        std::array<MapChar, 200> MapChar_ARRAY;
        
        std::array<GS1, 500> GS1ARRAY;
        int32_t GS1ARRAYCount = 0;
        
        std::array<GS2, 250> GS2ARRAY;
        int32_t GS2ARRAYCount = 0;
        
        std::array<Character, 2000> Characters;
        
        std::array<Village, 20> VillageState;
        
        void Load(FSMgr::iFile *pfile);
    };
    

    struct MapObject2
    {
        Common::FPoint p1;
        Common::FPoint p2;
        Common::Point p3;
        int16_t CharID;
        int16_t Dmg;
        int16_t Poison;
        uint8_t dmg2;
        uint8_t unk4;
        uint8_t SpecialID;
        uint8_t InfoID;
    };
    
    struct MapObjDrawRef
    {
        enum TYPE
        {
            TYPE_UNK  = 0,
            TYPE_OBJ2 = 1,
            TYPE_OBJ1 = 2,
            TYPE_CHAR = 4
        };
        
        int8_t Type = TYPE_UNK;
        Common::Point Pos;
        
        GameMap::Object *MOBJ1 = nullptr;
        MapObject2 *MOBJ2 = nullptr;
        Character *CHAR = nullptr;
        int32_t NextObjInHeap = 0;
    };
    
    
    struct Bonus
    {
        uint8_t BonusID;
        uint8_t BonusType;
        int16_t BonusValue;
    };
    
    struct BonusSet
    {
        int16_t unk1;
        int8_t unk3;
        int8_t unk4;
        Bonus Bonuses[3];
    };
    
public:
    bool Process();
    bool Update();
    
    void Init(int gfxmode);
    
    static int EventsWatcher(void *, SDL_Event *event);
    
    void Draw();
    
    void DrawGame();
    
    
    bool LoadMap(int32_t mapID, int32_t param = 0);
    void DrawGroundAndObj();
    void DrawObjects();
    
    void LoadGameState(int32_t stateID);
    
    static GameMap *LoadGameMap(int32_t mapID);
    
    vec3f CalculateLight();
    bool StartPlayMovie(const std::string &movie);
    
    void SetMusicOn(bool en);
    
    void PlayChangeScreen(int32_t screen);
    
    
    int32_t GetMouseOnScreenBox(Common::Point point, const std::vector<Common::Rect> &boxes);
    
    void ImgQueue1(GFX::Image *img, Common::Point pos, Common::Rect limits);
    void ImgQueue2(GFX::Image *img, Common::Point pos, Common::Rect limits);
    void ImgQueue1(GFX::PalImage *img, int32_t Pal, Common::Point pos, Common::Rect limits);
    void ImgQueue2(GFX::PalImage *img, int32_t Pal, Common::Point pos, Common::Rect limits);
    void TextQueue(const std::string &text, GFX::Font *font, Common::Point pos, Common::Rect limits);
    int32_t PlaceTextWidth(const std::string &text, GFX::Font *font, Common::Point pos, int32_t width);
    std::string GetStrToken(const std::string &text);
    
    void UpdateMainMenu();
    void Update7();
    void Update8();
    void Update9();
    
    void UpdateGame();
    
    void LoadUsedObjects();
    
    
    
    void ProcessCamera();
    
    
    
    void OnMovieEnd();
    
    bool LoadMainMenuImages();
    void MainMenuDraw(int32_t highlight);

    void FreeMenuImages();
    
    void PlaySound(int, int, int, int) 
    {printf("Incomplete %s\n", __PRETTY_FUNCTION__);};
    
    bool LoadINTR();
    
    void LoadSAVE() 
    {printf("Incomplete %s\n", __PRETTY_FUNCTION__);};
    
    bool LoadNEWH();
    void DrawNewGameMenu(int32_t);
    void UpdateNewGameMenu();
    void DrawNewGameText(int32_t);
    void DrawNewGameChar();
    
    void SaveLoadMenuDraw(bool isSave)
    {printf("Incomplete %s\n", __PRETTY_FUNCTION__);};
    
    
    
    int32_t ComputePan(Common::Point tilepos)
    {
        if (_mainCharacter->Tile.x == tilepos.x)
            return 0;
        else 
            return (float)(tilepos.x - _mainCharacter->Tile.x) * 10000.0 * 0.00625;
    }
    
    int32_t ComputeVolume(Common::Point tilepos)
    {
        Common::Point tmp = FUN_00439ba0( _mainCharacter->Tile ) - FUN_00439ba0(tilepos);
        int32_t dist = tmp.Length<int32_t>();
        if (dist < 2049)
            return dist * -10000 / 2048;
        else 
            return -10000;
    }
    
    void DrawCharacterSprite(Character &ch);
    bool PlaceMob(Character *);
    
    Character *CalcMapChar(MapChar *pmchar);
    
    void FUN_0041c750(Character *param_1);
    
    int32_t CheckKharUp(Character &pchar, int32_t param);
    
    
    int32_t FUN_00411758(Character *ch1, Common::Point tilepos);
    bool FUN_0043ecba(Common::Point *out, Common::Point tilepos, int offset);
    void FUN_004143dc(Character *ch, int dir);
    
    Common::Point FUN_00439ba0(Common::Point tilepos)
    {
        return Common::Point(tilepos.x * TileWh + (2 - (tilepos.y & 1)) * TileWhh, tilepos.y * TileHh + TileHh);
    }
    
    void FUN_004290d8();
    
    void FUN_00428f90(Common::Point pos);
    Common::Point FUN_00439bdc(Common::Point cam);
    Common::Point FUN_0043a000(Common::Point pos);
    
    void FUN_004292e4();
    void FUN_00429194(int32_t);
    
    void FUN_004118dc();
    void FUN_00436ad0(int32_t);
    void FUN_00436a00();
    void FUN_00414ab4(Character *pchar);
    
    void FUN_00414078(Character *pchar);
    
    
    int32_t GetLootImage(const GS1 &loot) const;
    GS1* GetLootByTile(Common::Point tile);
    
    void DrawSettingsScreen(int32_t);
    
    void ShutdownWithMsg(const std::string &msg);
    
    void OnEnterVillageUpdateTrader(Village *vlg);
    
    
    bool CheckMouseOnImage(GFX::Image *img, Common::Point drawPos);
    
    
    ItemInfo *AllocItem();
    
    int32_t FUN_00417170(int val) { return 2 - ((rand() % 2 + val + 1) % 3); }
    
    int32_t FUN_004171d4(int val) 
    {
        if (val < 4)
            return 3;
        else if (val < 7)
            return 6;
        else if (val < 10)
            return 8;
        return 10;
    }
    
    int32_t FUN_00417220(int val)
    {
        switch((val + 1) & 3)
        {
            case 0:
                return 2;
            case 1:
                return 1;
            default:
                return 0;
        }
        return 0;
    }

    
public:
    std::string _langLiter;
    Common::Point _mousePos;
    Common::Point _mouseMove;
    
    Common::Point _mouseMapPos;
    
    uint32_t _mousePress = 0;
    uint32_t _mouseDown = 0;
    uint32_t _mouseDownPrev = 0;
    
    Common::Point _mouseCurPos;
    
    
    Common::Point _screenSize;
    
    Common::Point _camera;
    Common::Point _some;
    Common::Point _camMax;
    Common::Point _gameViewport;
    Common::Point _viewStartPos;
    
    Common::Point _outOff;
    
    float _zoom = 1.0;
    Common::Rect _visibleTiles;
    
    GameState _state;
    int32_t _stateMode = STATEMD_UNKNOWN;
    int32_t _nextStateMode = STATEMD_UNKNOWN;
    
    int32_t _playScreenID = PLSCREEN_0;
    
    std::map<int16_t, int8_t> _KeyMap;
    std::array<int8_t, KEYFN_MAX> _KeyState;
    
    Village *CurrentVillage = nullptr;
    
    GameMap *_currentMap = nullptr;
    int32_t _currentMapID = -1;
    int32_t _nextMapID = 0;
    
    
    vec3f _ambientColor;
    vec3f _lightColor;
    
    bool _isPlayingVideo = false;
    bool _isGameStarted = false;
    
    bool _bConfTransparency = true;
    bool _bConfDayNight = true;
    bool _bConfLightEffects = true;
    bool _bConfMusic = true;
    bool _bConfShadows = true;
    bool _bConfSound = true;
    
    bool _svTransparency;
    bool _svDayNight;
    bool _svLightEffects;
    bool _svMusic;
    bool _svShadows;
    bool _svSound;
    
    static const std::map<uint8_t, vec3i> _mapsLights;
    static const std::array<int32_t, 13> _lightPhazes;
    
    bool _bLightEff = false;
    bool _bLightGround = false;
    
    int32_t _counter = 0;
    
    
    Character *_mainCharacter = nullptr;
    bool _bGodMode = false;
    
    Character *CharInfoCharacter = nullptr;
    
    Character *_camLockChar = nullptr;
    
    int32_t DWORD_00a3e758 = 0;
    
    int32_t DAT_00a3e704 = 0;
    int32_t DisplayInvOfCharID2 = 1;
    int32_t DisplayInvOfCharID = 1;
    int32_t DAT_00a3e690 = 0;
    int32_t DAT_00a3e84c = 0;
    uint32_t DAT_00a3e74c = 0;
    std::array<uint8_t, 56> DAT_00a3e0a8;
    
    void *DAT_00a3e6a8 = nullptr;
    
    
    int32_t mapGS1Count = 0;
    std::array<GS1, 200> mapGS1;
    
    std::array<MapObject2, 100> MapObjects2;
    
    int32_t MapObjDrawRefCount1 = 0;
    int32_t MapObjDrawRefCount2 = 0;
    std::array<MapObjDrawRef, 2000> MapObjDraw;
    std::array<MapObjDrawRef, 2000> MapObjDrawHeap;
    
    std::array<int32_t, 512> MapObjUseCount;
    int32_t MapObjectsCount = 0;
    
    std::array<Character *, 10> SelectedCharacters;
    
    Character CharTmp;
    MapChar *_mainMapChar = nullptr;
    
    
    
    int32_t _objectsToLoadCount = 0;
    std::array<int32_t, 512> _objectsToLoad;
    
    
    GameMap::Object *MouseOnObject = nullptr;
    
    
    Common::Point ViewTiles;
    int32_t ViewTilesHoriz = 0;
    
    std::vector<GFX::Image *> _menuImages;
    GFX::Image *_bkgImage = nullptr;
    
    static const std::vector<Common::Rect> _mainMenuBoxes;
    static const std::vector<Common::Rect> _saveMenuBoxes; 
    static const std::vector<Common::Rect> _newGameMenuBoxes; 
    
    static const int8_t EqLookUp1[8][6];
    static const int8_t EqLookUp2[4][8][6];
    static const int8_t EqLookUp3[4][8][6];
    
    static const std::array<WeapArmorItemInfo, 70> ArmorWeaponInfo;
    static const std::array<AccessorieItemInfo, 9> AcessoriesInfo;
    static const std::array<AccessorieItemInfo, 14> AlchemiesInfo;
    static const std::array<AccessorieItemInfo, 54> MiscItemsInfo;
    
    static const std::array<BonusSet, 74> BonusesInfo;
    
    static const std::array<BldInfo, 42> BuildingInfo;
    
    static const Common::Point PlaceDOff[2][8][12];
    
    static const int FlameAnims[7][2];    
    
    int32_t _saveMenuBtnID = -1;
    
    std::array<ImagePlace, 100> _imgQueue1;
    int32_t _imgQueue1Count = 0;
    std::array<ImagePlace, 100> _imgQueue2;
    int32_t _imgQueue2Count = 0;
    std::array<TextPlace, 100> _textQueue;
    int32_t _textQueueCount = 0;
    
    std::array<GFX::Font *, 4> _Fonts;
    
    bool _doQuit = false;
    
public:
    static Engine Instance;
};
    
}

static constexpr Game::Engine &GameInst = Game::Engine::Instance;

#endif /* GAME_H */

