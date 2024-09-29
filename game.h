#ifndef GAME_H
#define GAME_H

#include <map>
#include <bitset>
#include "common/common.h"
#include "simix/simix.h"
#include "resources.h"
#include "font.h"
#include "system.h"

//Stupid winapi
#ifdef PlaySound
#undef PlaySound
#endif



namespace Game
{
constexpr const int32_t BASECHR_CNT = 3;

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
constexpr const Common::Point MAPVIEW = Common::Point(MAPVIEWW, MAPVIEWH);

constexpr const int32_t TILEMAXW = 80;
constexpr const int32_t TILEMAXH = 160;

constexpr const Common::Rect MAPRECT = Common::Rect(SCREENRESX - MAPVIEWW, 0, SCREENRESX, MAPVIEWH);

constexpr const Common::Rect MSGRECT = Common::Rect(SCREENRESX - MAPVIEWW, MAPVIEWH, SCREENRESX, SCREENRESY);

//constexpr const Common::PointRect SRect_ARRAY_0046280c = Common::Rect(13, 13, 502, 361);
constexpr const Common::PointRect SRect_ARRAY_0046280c = Common::PointRect(80, 60, 502, 361);


constexpr const float PX = 1.0 / (float) SCREENRESX;
constexpr const float PY = 1.0 / (float) SCREENRESY;

constexpr const int32_t TXTHEIGHT = 15;

extern const Common::Point GScrOff;

constexpr const int32_t INVSIZE = 32;

constexpr const int32_t MAXPATHITER = 0x4000;

constexpr const int32_t SCREENSHIFT = 16;

constexpr const int32_t MAXSPEED = 5;

constexpr const int32_t SND_PER_CHAR = 8;
constexpr const int32_t SND_COMMON_CNT = 32;

constexpr const int32_t SND_AMBIENCE_CNT = 8;
constexpr const int32_t SND_AMBIENCE_FIRST = 0x100;


class Engine
{
public:
    typedef int32_t (Game::Engine::*TQuestFunc)();
public:
    enum KEYFN
    {
        KEYFN_ESC,
        
        KEYFN_F1,
        KEYFN_F2,
        KEYFN_F3,
        KEYFN_F4,
        KEYFN_F5,
        KEYFN_F6,
        KEYFN_F7,
        KEYFN_F8,
        KEYFN_F9,
        KEYFN_F10,
        KEYFN_F11,
        KEYFN_F12,
        
        KEYFN_TILDE,
        KEYFN_1,
        KEYFN_2,
        KEYFN_3,
        KEYFN_4,
        KEYFN_5,
        KEYFN_6,
        KEYFN_7,
        KEYFN_8,
        KEYFN_9,
        KEYFN_0,
        KEYFN_MINUS,
        KEYFN_ADD,
        KEYFN_BSLASH,
        KEYFN_BACKSPACE,
        
        KEYFN_TAB,
        KEYFN_Q,
        KEYFN_W,
        KEYFN_E,
        KEYFN_R,
        KEYFN_T,
        KEYFN_Y,
        KEYFN_U,
        KEYFN_I,
        KEYFN_O,
        KEYFN_P,
        KEYFN_BRKTL,
        KEYFN_BRKTR,
        
        KEYFN_CAPS,
        KEYFN_A,
        KEYFN_S,
        KEYFN_D,
        KEYFN_F,
        KEYFN_G,
        KEYFN_H,
        KEYFN_J,
        KEYFN_K,
        KEYFN_L,
        KEYFN_COLON,
        KEYFN_QUOTE,
        
        KEYFN_RETURN,
        
        KEYFN_SHIFT,
        
        KEYFN_Z,
        KEYFN_X,
        KEYFN_C,
        KEYFN_V,
        KEYFN_B,
        KEYFN_N,
        KEYFN_M,
        KEYFN_LESS,
        KEYFN_MORE,
        KEYFN_SLASH,
        
        KEYFN_CTRL,
        KEYFN_WIN,
        KEYFN_ALT,
        KEYFN_SPACE,
        KEYFN_MENU,
        
        KEYFN_PRNT,
        KEYFN_SCROLL,
        KEYFN_PAUSE,
        
        KEYFN_INSERT,
        KEYFN_HOME,
        KEYFN_PAGEUP,
        KEYFN_DELETE,
        KEYFN_END,
        KEYFN_PAGEDOWN,
        
        KEYFN_LEFT,
        KEYFN_RIGHT,
        KEYFN_UP,
        KEYFN_DOWN,
        
        KEYFN_NUMLOCK,
        KEYFN_NUMDIV,
        KEYFN_NUMMULT,
        KEYFN_NUMMINUS,
        KEYFN_NUM7,
        KEYFN_NUM8,
        KEYFN_NUM9,
        KEYFN_NUM4,
        KEYFN_NUM5,
        KEYFN_NUM6,
        KEYFN_NUMADD,
        KEYFN_NUM1,
        KEYFN_NUM2,
        KEYFN_NUM3,
        KEYFN_NUM0,
        KEYFN_NUMDEL,
        KEYFN_NUMRETURN,        
        
        KEYFN_MAX
    };
    
    enum MOUSEB
    {
        MOUSEB_L = (1 << 0),
        MOUSEB_R = (1 << 1),
        MOUSEB_3 = (1 << 2),
    };
    
    enum HOTKEY
    {
        HOTKEY_CYCLE,
        HOTKEY_DRAWWEAPON,
        HOTKEY_WPN1,
        HOTKEY_WPN2,
        HOTKEY_WPN3,
        HOTKEY_A,
        HOTKEY_SHIFTA,
        HOTKEY_F,
        HOTKEY_I,
        HOTKEY_GODMODE,
        HOTKEY_M,
        HOTKEY_Q,
        HOTKEY_CTRLS,
        HOTKEY_T,
        HOTKEY_SPEEDUP,
        HOTKEY_SPEEDDOWN,
        HOTKEY_BAG,
        HOTKEY_DBG_POINTS,
        HOTKEY_DBG_GOLD,
        HOTKEY_DBG_FULLMAP,
        
        HOTKEY_MAX
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
        PLSCREEN_1   = 1,
        PLSCREEN_2   = 2,
        PLSCREEN_3   = 3,
        PLSCREEN_4   = 4,
        PLSCREEN_MAP = 5,
        PLSCREEN_6   = 6,
        PLSCREEN_7   = 7,
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
        KHAR_ARMOR = 7,
        KHAR_N8 = 8,
        KHAR_N9 = 9,
    };
    
    enum CLASSID
    {
        CLASS_KUPETC = 2,
        CLASS_OHOTNIK = 3,
        CLASS_VOJD = 4,
        CLASS_VOIN = 5,
        
        CLASS_MASK = 7,
        
        CLASS_MIDMASK = 0x3F,
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
        
        ESLT_ARROWS = 12,
                
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
    
    enum VJOB
    {
        VJOB_NO = 0,
        VJOB_CHIEF = 1,
        VJOB_MEDIC = 2,
        VJOB_TRADER = 3,
        VJOB_SMITH = 4,
        VJOB_VOEVODA = 5
    };
    
    enum STORE
    {
        STORE_AWPN = (1 << 0), // armor and weapon
        STORE_ARROW = (1 << 1), // arrow slot
        STORE_TRINKET = (1 << 2), // accessories and trinkets
        STORE_INVENTORY = (1 << 3)
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
            std::array<int16_t, 20> Flames; // Flame frame id
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
        Common::PlaneArray<uint8_t, 160, 320> LightMap2;
        
        Common::Point MapLimits;
        
        Common::Point CheckFoot(Common::Point p) const
        {
            if (p.x < 0)
                p.x = 0;
            if (p.y < 0)
                p.y = 0;
            if (p.x > MapLimits.x * 2)
                p.x = MapLimits.x * 2;
            if (p.y > MapLimits.y * 2)
                p.y = MapLimits.y * 2;
            return p;
        }
    };
    
    struct ImagePlace
    {
        GFX::Image *Img = nullptr;
        GFX::PalImage *PalImg = nullptr;
        Common::Point DrawPlace;
        Common::Rect Limits;
        vec3f Fade;
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
        int32_t Index = -1;
        
        uint8_t State;
        int8_t Direction;
        uint8_t field2_0x2;
        uint8_t field_0x3;
        uint8_t ClassID;
        int32_t MapCharID;
        int16_t EnemyCharID;
        Common::Point Tile;
        Common::Point MoveTile;
        uint8_t Frame;
        uint8_t field15_0x11;
        uint8_t field_0x12;
        std::array<uint8_t, 32> field17_0x13;
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
        std::array<int16_t, INVSIZE> Inventory;
        std::array<int16_t, 5> Accessories;
        int16_t field76_0xbc;
        int16_t field78_0xbe;
        int16_t field80_0xc0;
        int16_t field82_0xc2;
        int16_t CurrentBrn;
        int16_t CurrentUdr;
        int16_t CurrentVer;
        int16_t field90_0xca;
        int16_t Arrows;
        int16_t field_0xce;
        int16_t field96_0xd0; //uint8_t
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
        
        Common::Point ViewPos;
        //int32_t field107_0xe4;
        //int32_t xPOS;
        Common::Point POS;
        //int32_t field109_0xec;
        //int32_t yPOS;
        Common::Point imgSize;
        //int16_t field111_0xf4;
        //int16_t field112_0xf6;
        uint16_t field113_0xf8;
        uint8_t field115_0xfa;
        uint8_t field116_0xfb;
        uint8_t field117_0xfc;
        uint8_t field118_0xfd;
        uint8_t field119_0xfe;
        uint8_t field120_0xff;
        
        void Load(FSMgr::iFile *pfile);
        
        int32_t GetMaxPartySize();
        
        std::string FmtCharName();
        std::string FmtLvlHP();
        std::string GetCharHint();
        
        inline void RecalcViewPos(Common::Point offset)
            { ViewPos = offset + POS + imgOffset; }
        
        void CopyDataFrom(const Character &chr);

        inline int32_t GetSoundId(int8_t SndId) const
        {
            if (ClassID & CLASS_BIT40)
                return ((ClassID & CLASS_MIDMASK) - 1) * SND_PER_CHAR + SndId + (SND_COMMON_CNT + BASECHR_CNT * SND_PER_CHAR);
            else
                return CharacterBase * SND_PER_CHAR + SndId + SND_COMMON_CNT;
        }

        inline static int32_t GetMonsterSoundId(uint8_t id, int8_t SndId)
        {
            return id * SND_PER_CHAR + SndId + (SND_COMMON_CNT + BASECHR_CNT * SND_PER_CHAR);
        }

        inline int32_t GetMonsterId() const
        {
            if (ClassID & CLASS_BIT40)
                return (ClassID & CLASS_MIDMASK) - 1;
            return -1;
        }
    };
    
    struct MapChar
    {
        int32_t Index = -1;
        int16_t CharacterIndex = 0;
        int32_t field1_0x2;
        int16_t field2_0x6;
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
            FLAG_UNIDENTIFIED = (1 << 0),
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
        
        ItemInfo& operator=(const ItemInfo& b)
        {
            TypeID = b.TypeID;
            BonusID = b.BonusID;
            SpecialID = b.SpecialID;
            InfoID = b.InfoID;
            Concentration = b.Concentration;
            Weight = b.Weight;
            Poison = b.Poison;
            Flags = b.Flags;
            unk1 = b.unk1;
            return *this;
        }
    };
    
    struct GS1
    {
        int32_t Index = -1;
        
        Common::Point Pos;
        Common::Point Tile;
        int16_t ImgID = 0;
        int16_t LootID = 0; // Reference id to _state.LootArray index
        int16_t unk2 = -1;
        int16_t MapID = 0;
        std::array<int16_t, 22> ItemID =  {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0}};
        
        void Load(FSMgr::iFile *pfile);
        GS1& operator=(const GS1& b)
        {
            Pos = b.Pos;
            Tile = b.Tile;
            ImgID = b.ImgID;
            LootID = b.LootID;
            unk2 = b.unk2;
            MapID = b.MapID;
            ItemID = b.ItemID;
            return *this;
        }
    };
    
    struct GS2
    {
        int32_t Index = -1;
        
        uint8_t field0_0x0;
        uint8_t field1_0x1;
        uint8_t field2_0x2;
        int8_t MapID;
        uint8_t TargetMapID;
        int16_t field5_0x5;
        int16_t field6_0x7;
        Common::Rect WarpZone;
        
        void Load(FSMgr::iFile *pfile);
    };
    
    struct Village
    {
        struct Job
        {
            uint8_t Direction = DIR_0;
            int16_t unk = 0;
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
        
        int32_t TimeSt[3];
        
        int32_t unk2 = 0;
        
        int16_t DoMedicPotionSlot6 = 0;
        int16_t DoMedicPotionSlot7 = 0;
        int16_t DoMedicPotionSlot5 = 0;
        
        std::array<int16_t, 25> MedicItems;
        
        //int16_t unk3 = 0;
        
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
    
    struct SpecialDmg
    {
        int32_t ID;
        int32_t Dmg;
        int32_t ClassID;
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
        
        GameState();
        
        void Load(FSMgr::iFile *pfile);
    };
    
    struct MixInfo
    {
        int16_t Potion1id;
        int16_t Potion2id;
        int16_t Result1id;
        int16_t Result2id;
        int16_t ExpPoints;
        int8_t MixType;
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
        uint8_t BonusKhar;
        bool BonusAdd;
        int16_t BonusValue;
    };
    
    struct BonusSet
    {
        int16_t Price;
        int8_t NeedsKhar;
        int8_t NeedsVal;
        Bonus Bonuses[3];
    };
    
    struct DlgTxt
    {
        int32_t Q1ID = -1;
        int32_t qsel = -1;
        int32_t q1txtid = -1;
        int32_t SoundID = -1;
        Common::Rect Rect;
    };
    
    struct TQuestState
    {
        int32_t State = 0;
        int32_t QuestID = -1;
        
        void Load(FSMgr::File *pfile);
    };
    
    struct TSq1
    {
        int32_t Index = -1;
        
        int32_t TxtId = 0;
        int32_t QwNext = 0;
        int32_t q4id = 0;
        int32_t SoundId = 0;
        int32_t unk1 = 0;
        int32_t unk2 = 0;
        int32_t unk3 = 0;
        
        void Load(FSMgr::File *pfile);
    };
    
    struct TSq5
    {
        int16_t id = -1;
        uint8_t Flags = 0;
    };
    
    struct THotKey
    {
        enum KMOD
        {
            KMOD_SHIFT = (1 << 0),
            KMOD_ALT   = (1 << 1),
            KMOD_CTRL  = (1 << 2),
            
            KMOD_MAX = (KMOD_SHIFT | KMOD_ALT | KMOD_CTRL) + 1
        };
        
        std::array<int16_t, KMOD_MAX> Funcs;
        
        //int16_t Key = -1; //usege for debug?
        
        THotKey()
        { Funcs.fill(-1); }
        
        void SetFunc(int16_t func, uint8_t mod = 0)
        {
            Funcs.at(mod) = func;
        }
    };
    
public:
    
    Engine()
    {
        for(int32_t i = 0; i < mapGS1.size(); ++i)
            mapGS1[i].Index = i;
        
        _cursors.fill(nullptr);
        
        quest_2.fill(0);

        SelectedCharacters.fill(nullptr);
    }
    
    bool Process();
    bool Update();
    
    void Init(int gfxmode);
    
    static int EventsWatcher(void *, SDL_Event *event);
    
    
    void CreateCursors();
    
    void Draw();
    
    void UpdateCursor();
    
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
    
    void ImgQueue1(GFX::Image *img, Common::Point pos, Common::Rect limits, vec3f fade = vec3f());
    void ImgQueue2(GFX::Image *img, Common::Point pos, Common::Rect limits, vec3f fade = vec3f());
    void ImgQueue1(GFX::PalImage *img, int32_t Pal, Common::Point pos, Common::Rect limits, vec3f fade = vec3f());
    void ImgQueue2(GFX::PalImage *img, int32_t Pal, Common::Point pos, Common::Rect limits, vec3f fade = vec3f());
    void TextQueue(const std::string &text, GFX::Font *font, Common::Point pos, Common::Rect limits = Common::Rect());
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
            return (float)(tilepos.x - _mainCharacter->Tile.x) * SDL_MIX_MAXVOLUME / 160;
    }
    
    int32_t ComputeVolume(Common::Point tilepos)
    {
        Common::Point tmp = FUN_00439ba0( _mainCharacter->Tile ) - FUN_00439ba0(tilepos);
        int32_t dist = tmp.Length<int32_t>();
        if (dist < 2049)
            return dist * -100 / 2048;
        else 
            return -100;
    }
    
    void DrawCharacterSprite(Character &ch);
    void FUN_004251d0(Character &ch);
    
    bool PlaceMob(Character *);
    
    Character *CalcMapChar(MapChar *pmchar);
    
    void FUN_0041c750(Character *pchar);
    int32_t GetCurrentWeight(Character *pchar, uint32_t ignore = 0);
    
    int32_t CheckKharUp(Character &pchar, int32_t param);
    
    
    int32_t FUN_0043976c(Common::Point my, Common::Point t);
    int32_t FUN_00411758(Character *ch1, Common::Point tilepos);
    bool FUN_0043ecba(Common::Point *out, Common::Point tilepos, int offset);
    void FUN_004143dc(Character *ch, int dir);
    
    inline Common::Point FUN_00439ba0(Common::Point tilepos) const
    {
        return Common::Point(tilepos.x * TileWh + (2 - (tilepos.y & 1)) * TileWhh, tilepos.y * TileHh + TileHh);
    }
    
    void GoToMapPos(int32_t mapid, int32_t x, int32_t y, int32_t p4)
    {
        _nextMapID = mapid;
        _mainMapChar->spwnTlRect.top = x;
        _mainMapChar->spwnTlRect.left = y;
        _mainMapChar->spwnTlRect.bottom = p4;
    }

    
    void FUN_004290d8();
    
    void FUN_00428f90(Common::Point pos);
    Common::Point FUN_00439bdc(Common::Point cam);
    Common::Point FUN_0043a000(Common::Point pos);
    
    void SetHintText(const std::string &txt);
    bool FUN_00416700(Village *vlg);
    
    void FUN_004292e4();
    void ResetMouseItemHold();
    bool FUN_0041e96c();
    
    void FUN_004118dc();
    void FUN_00436ad0(int32_t mapid);
    void FUN_00436a00();
    void FUN_00421170();
    void FUN_00421da4();
    void FUN_00439230(Character *pchar);
    bool FUN_0041f258(Character *pchar);
    void FUN_00421bb8(Character *pchar);
    void FUN_004110ec(Character *pchar);
    void MapObjectsUpdateFrames();
    vec3f FUN_0042c914(Character *pchar, ItemInfo *itm);
    int32_t FUN_0042c870(ItemInfo *itm);
    void FUN_0042f50c(Character *pchar, int32_t);
    void FUN_0041d0fc();
    void FUN_0041733c(Village::BldState *state);
    int32_t FUN_0041b70c(Character *pchar);
    void FUN_0041bdf0(MapChar *mchar);
    void FUN_00418510(Character *pchar, int32_t maxBonusGld, int32_t lvl);
    bool FUN_00412c30(MapChar *pchar);
    void FUN_00414ab4(Character *pchar);
    bool FUN_00416934(Character *pchar);
    void FUN_004105a0(Character *pchar);
    Character * FUN_004202a8(MapChar *mchar, Character *pchar);
    bool FUN_00413a84(Character *pchar);
    bool FUN_004142d4(Character *pchar, Common::Point pt, uint8_t p);
    
    bool FUN_00418364(Character *pchar, ItemInfo *pitm);
    void FUN_00421d24(Character *pchar, int32_t itmID);
    bool FUN_004104d4(Character *pchar, int32_t awid);
    int32_t FUN_0041b244(Character *pchar, int charId);
    int32_t GetArmor(Character *pchar);
    int32_t FUN_0041af48(Character *pchar, int32_t classID);
    int32_t FUN_0041b4cc(Character *pchar, int32_t fkt);
    
    int FUN_00413dc4(Character *pchar) { return 0; }
    
    int FUN_0043edb7(Character *pchar); //pathfind
    
    void FUN_00414078(Character *pchar);
    
    bool FUN_00413de4(Character *pchar);
    bool SkipBecauseOfHardCompute(Character *chr);
    bool FUN_0041f11c(MapChar *mchar, Character *pchar);
    bool FUN_0041f5d8(MapChar *mchar, Character *pchar);
    bool FUN_00410e84(Character *pchar);
    int32_t FUN_0041ec0c(Character *pchar);
    void FUN_0041ed8c(Character *pchar);
    bool FUN_0041e384(Character *pchar);
    Character * FUN_004203a0(MapChar *mchr1, MapChar *mchr2, Character *pchar);
    MapChar * FUN_0041fd74(MapChar *mchar);
    int32_t FUN_00439944(Common::Point p1, Common::Point p2);
    int32_t FUN_00439aa4(Common::Point p1, Common::Point p2);
    int32_t FUN_0041130c(Character *pchar, Common::Point t, int32_t chIndex);
    
    int32_t GetGroupMaxPlotnicLevel(MapChar *mchr);
    
    void FUN_004248a0(MapChar *mchr);
    void FUN_00416058(MapChar *mchr);
    int32_t FUN_0041b100(Character *ch1, Character *ch2);
    int32_t FUN_0041c480(Character *chr1, Character *chr2, int32_t param_3);
    void FUN_0041079c(Character *pchar, int32_t exp);
    bool FUN_0041bff8(Character *pchar);
    void FUN_00410728(Character *pchar);
    bool FUN_00415cd0(Character *pchar);
    bool FUN_004101e4(Character *pchar, Common::Point *ot);
    void FUN_004123bc(MapChar *mchr);
    
    Character * FUN_00420178(MapChar *mchar);
    void FUN_00410450(Character *pchar);
    bool FUN_00411fd0(Character *pchar);
    bool FUN_00412e00(Character *pchar);
    
    int FUN_0041165c(Common::Point p1, Common::Point p2);
    int FUN_00410ffc(Common::Point *pout, Common::Point p);
    void FUN_004391bc(Character *pchar, int dir);
    
    
    int32_t GetLootImage(const GS1 &loot) const;
    GS1* GetLootByTile(Common::Point tile);
    void FUN_004170a0(GS1 *loot);
    void FUN_0041093c(GS1 *loot);
    bool FUN_004138c8(Common::Point tile, int32_t itemId);
    
    void DrawSettingsScreen(int32_t);
    
    void ShutdownWithMsg(const std::string &msg);
    
    void OnEnterVillageUpdateTrader(Village *vlg);
    int32_t GetVillageCharacterJob(Character *pchar);
    Village * FUN_0043a1f8(int32_t mapId);
    
    bool FUN_00417268(Village *vlg, Character *pchr, int32_t iout, int32_t i1, int32_t i2);
    void UpdateVillageMedic(Village *vlg);
    void UpdateVillageSmith(Village *vlg);
    void UpdateVillageVoevoda(Village *vlg);
    
    
    bool CheckTraceImage(GFX::Image *img, Common::Point drawPos);
    bool CheckTraceImage(GFX::PalImage *img, Common::Point drawPos);
    
    void FUN_0042d574(Character *pchar, int32_t i);
    void DrawElmQueue();
    void FUN_004290ac(int elmid, int imgid);
    
    
    ItemInfo *AllocItem();
    int32_t GetItemWeight(const ItemInfo * ) const;
    
    std::string GetItemHint(const ItemInfo * ) const;
    std::string GetBonusValueString(const Bonus *b) const;
    bool FUN_0041db64(Character *pchar, ItemInfo *itm1, ItemInfo *itm2);
    bool FUN_00414e64(Character *pchar, ItemInfo *itm1, ItemInfo *itm2);
    bool FUN_00410010(Character *pchar, ItemInfo *itm1, ItemInfo *itm2);
    void MixPotions(Character *pchar, ItemInfo *itm1, ItemInfo *itm2);
    bool FUN_00414b3c(Character *pchar);
    void IdentifyCharacterItems(Character *pchar);
    bool FUN_0042054c(Character *pchar);
    bool FUN_004246f8(Character *pchar);
    
    
    int32_t FUN_0041ace0(Character *pchr, MapObject2 *parrow);
    int32_t FUN_0041c29c(Character *pchr, int32_t arrowDmg);
    
    bool TradeComplete(bool p);
    
    int32_t FUN_00434160(int32_t id);
    int32_t FUN_004351fc(int32_t id);
    int32_t FUN_00434b60(int32_t id);
    int32_t FUN_004359d4(int32_t id);
    int32_t FUN_00434c90(int32_t id);
    
    int32_t FUN_004323b0(int32_t classId);
    
    int32_t FUN_00432654(int32_t job);
    void FUN_00422778(Character *pchar);
    bool FUN_00436248(int32_t id);
    
    int32_t FUN_00432f70();
    int32_t FUN_00432fd0();
    int32_t FUN_004358b4();
    int32_t FUN_0043512c();
    int32_t FUN_0043499c();
    int32_t FUN_004340d4();
    int32_t FUN_004347d0();
    int32_t FUN_00434298();
    int32_t FUN_00435d6c();
    int32_t FUN_00434c44();
    int32_t FUN_00434a28();
    int32_t FUN_00434a6c();
    int32_t FUN_00434a94();
    int32_t FUN_00434abc();
    int32_t FUN_00434ae4();
    int32_t FUN_00434b0c();
    int32_t FUN_00434b34();
    int32_t FUN_00432820();
    int32_t FUN_00435740();
    int32_t FUN_004358e0();
    int32_t FUN_00435908();
    int32_t FUN_00435930();
    int32_t FUN_00435958();
    int32_t FUN_00435980();
    int32_t FUN_004359a8();
    int32_t FUN_004357e4();
    int32_t FUN_00435814();
    int32_t FUN_00435844();
    int32_t FUN_00435460();
    int32_t FUN_00433f18();
    int32_t FUN_00434954();
    int32_t FUN_004342cc();
    int32_t FUN_00434300();
    int32_t FUN_00434334();
    int32_t FUN_00434368();
    int32_t FUN_0043439c();
    int32_t FUN_004343d0();
    int32_t FUN_00434888();
    int32_t FUN_004348cc();
    int32_t FUN_00434628();
    int32_t FUN_0043408c();
    int32_t FUN_00434108();
    int32_t FUN_004348fc();
    int32_t FUN_004349d0();
    int32_t FUN_004346ac();
    int32_t FUN_00434788();
    int32_t FUN_00433e84();
    int32_t FUN_00434250();
    int32_t FUN_00434730();
    int32_t FUN_00433f54();
    int32_t FUN_00434438();
    int32_t FUN_00433edc();
    int32_t FUN_00433fac();
    int32_t FUN_0043400c();
    int32_t FUN_00434570();
    int32_t FUN_00434804();
    int32_t FUN_004353f4();
    int32_t FUN_00435100();
    int32_t FUN_00433af8();
    int32_t FUN_004352ec();
    int32_t FUN_00434d1c();
    int32_t FUN_00434da4();
    int32_t FUN_004339a8();
    int32_t FUN_004336e8();
    int32_t FUN_00433848();
    int32_t FUN_004338f8();
    int32_t FUN_004334b8();
    int32_t FUN_00433798();
    int32_t FUN_00433568();
    int32_t FUN_00433618();
    int32_t FUN_00435874();
    int32_t FUN_004351d8();
    int32_t FUN_0043516c();
    int32_t FUN_004351b4();
    int32_t FUN_00435190();
    int32_t FUN_0043536c();
    int32_t FUN_0043542c();
    int32_t FUN_00435334();
    int32_t FUN_004354a8();
    int32_t FUN_004354e0();
    int32_t FUN_00435568();
    int32_t FUN_00433368();
    int32_t FUN_00433410();
    int32_t FUN_00433464();
    int32_t FUN_0043321c();
    int32_t FUN_004333bc();
    int32_t FUN_00433270();
    int32_t FUN_004332c4();
    int32_t FUN_00435a84();
    int32_t FUN_00434e98();
    int32_t FUN_00433acc();
    int32_t FUN_00435514();
    int32_t FUN_004353bc();
    
    
    bool IsSelectedCharacter(Character *pchar);
    
    void SelectCharacter(Character *pchar);
    
    int32_t FUN_00417170(int val) { return 2 - ((System::rand() % 2 + val + 1) % 3); }
    
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
    
    int32_t FUN_0041aea0(int32_t val);
    int32_t FUN_0041b65c(int32_t val);
    
    
    //Input
    void PlayHandleKey(int16_t keyCode);    
    void PlayProcessMouse();
    int32_t FUN_004392f4();
    int32_t FUN_00439a28(Common::Point pt, const Common::PlaneVector<uint8_t> *mask);
    bool FUN_0041e500(int32_t slot);
    int16_t GetHotKeyFunc(const THotKey &hkey);
    int16_t GetHotKeyFunc(int16_t keyCode);
    void InitKeyMapping();
    void InitHotKeys();
    
    
    //UI
    void DrawTrade(int p);
    int32_t GetItemPrice(ItemInfo *itm);
    void DrawTradeInv(int inv, int p);
    void FillBkgRect(Common::Rect rect);
    void DrawMap();
    void UpdateMapImage();
    void DrawInventory(Character *chr, int btn, bool bkg = true);
    void DrawCharInfo();
    void DrawJournal();
    void DrawQuestScreen();
    
    void FUN_00425104(const ItemInfo *inf);
    void FUN_0041e210(Character *pchar, int32_t typ, int32_t itemid);
    bool FUN_00424918(Character *pchar, int32_t itemid);
    bool FUN_0041e778(int aidx);
    
    bool FUN_0041eb9c(std::array<int16_t, INVSIZE> *inv, int32_t p1, int32_t p2);
    
    int32_t FUN_0041ade4(Character *pchar);
    
    //Quest
    void FUN_0041f18c(TSq1 *qw, int32_t id);
    int FUN_004364f0(Character *pchar);
    int FUN_00435e78();
    void FUN_00435dbc(int32_t id);
    int32_t FUN_0043259c(int32_t funcid);
    std::string FUN_004363e8(int32_t id);
    
    //Sound
    void PlaySound(uint16_t SoundID, int Volume, int Pan, bool loop);
    void FUN_0043d1d0();
    void SoundPlaySpeech(int32_t id);
    bool FUN_00429c28(int32_t id);

    inline static int32_t GetMapSoundId(uint8_t mapID, uint8_t sndId)
    {
        return SND_AMBIENCE_FIRST + (mapID - 1) * SND_AMBIENCE_CNT + sndId;
    }
    
    void ChangeWeapon(int32_t wpnId);
    void FUN_0042179c();
    void FUN_004226c0();
    void FUN_00421698();
    void FUN_0042459c();
    void ShowHideCharInfo();
    
public:
    std::string _langLiter;
    Common::Point _mousePos;
    Common::Point _mouseMove;
    
    Common::Point _uiMousePos;
    
    Common::Point _mouseMapPos;
    Common::Point _tracePos;
    
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
    
    std::map<int16_t, int16_t> _KeyMap;
    std::array<int8_t, KEYFN_MAX> _KeyState;
    std::deque<int16_t> _KeyQueue;
    std::map<int16_t, THotKey> _HotKeys;
    int16_t _KeyPressed = 0;
    
    
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
    bool _bTransparentMenu = true;
    
    bool _svTransparency;
    bool _svDayNight;
    bool _svLightEffects;
    bool _svMusic;
    bool _svShadows;
    bool _svSound;
    
    
    
    static const std::map<uint8_t, vec3i> _dungeonMaps;
    static const std::array<int32_t, 13> _lightPhazes;
    
    bool _bLightEff = false;
    bool _bLightGround = false;
    
    int32_t _counter = 0;
    int32_t _speed = 0;
    
    
    Character *_mainCharacter = nullptr;
    bool _bGodMode = false;
    
    Character *CharInfoCharacter = nullptr;
    
    Character *_camLockChar = nullptr;
    
    Character *PInteractChar = nullptr;
    
    int32_t DAT_00a3e76c = 0;
    
    
    bool DWORD_00a3e758 = false;
    
    int32_t DAT_00a3e704 = 0;
    int32_t DisplayInvOfCharID2 = 1;
    int32_t DisplayInvOfCharID = 1;
    int _invButton = 0;
    
    Character *HardComputeUnit = nullptr;
    int32_t HardComputeCount = 0;
    
    int32_t int32_t_00a3e828 = 0;
    
    int32_t MsgTextTimeout = 0;
    std::string MsgText;
    
    uint32_t DWORD_00a3e74c = 0;
    std::array<uint8_t, 56> DAT_00a3e0a8;
    
    void *DAT_00a3e6a8 = nullptr;
    
    
    std::vector<DlgTxt> qOut1; 
    std::vector<DlgTxt> qOut2;
    
    int32_t qwsel1 = 0;
    int32_t qwsel2 = 0;
    
    std::array<TSq1, 8000> quest_1;
    std::array<uint16_t, 300> quest_2;
    std::array<TSq5, 6000> quest_4;
    std::array<TSq5, 6000> quest_5;
    std::map<int32_t, std::string> QuestTexts;
    
    std::array<TQuestState, 300> QuestsState;
    
    int32_t mapGS1Count = 0;
    std::array<GS1, 200> mapGS1;
    
    std::array<MapObject2, 100> MapObjects2;
    
    int32_t MapObjDrawRefCount1 = 0;
    int32_t MapObjDrawRefCount2 = 0;
    std::array<MapObjDrawRef, 2000> MapObjDraw;
    std::array<MapObjDrawRef, 2000> MapObjDrawHeap;
    
    std::array<int32_t, 512> MapObjUseCount;
    int32_t MapObjectsCount = 0;
    int32_t MapObjects2Count = 0;
    
    std::array<Character *, 10> SelectedCharacters;
    
    Character CharTmp;
    MapChar *_mainMapChar = nullptr;
    
    
    std::bitset<56> MapsOpened; //Remake it to bitfield and update map image only change
    
    int32_t _objectsToLoadCount = 0;
    std::array<int32_t, 512> _objectsToLoad;
    
    int32_t DAT_00a3e814 = 0;
    
    
    GameMap::Object *MouseOnObject = nullptr;
    Character *MouseOnCharacter = nullptr;
    
    Character *InfPchar = nullptr;
    int32_t InfTyp = -1;
    int32_t InfItemID = 0;
    std::array<int16_t, 6> InfSvArmrWpn;
    std::array<int16_t, INVSIZE> InfSvInv;
    std::array<int16_t, 5> InfSvAccess;
    
    std::array<int16_t, INVSIZE> int16_t_ARRAY_0083dc4c[4];
    std::array<int16_t, 4> int16_t_ARRAY_0083dd4c;
    
    int32_t DAT_00a3e850 = 0;
    int32_t DAT_00a3e854 = 0;
    
    
    
    Common::Point ViewTiles;
    
    int32_t ItemInvSlotWidth = 1;
    int32_t ScrlBtnWidth = 0;
    int32_t DAT_00a3e870 = 0;
    int32_t DAT_00a3e88c = 0;
    int32_t ScrlBtnHeight = 0;
    
    int32_t InvPos = 0;
    
    int32_t CursorAssume = 0;
    
    int32_t DAT_00a3e7a0 = -1;
    
    int32_t DWORD_00a3e7b4 = 0;
    int32_t DAT_00a3e7c4 = 0;
    
    int32_t SpeechBufferIndex = 0;
    
    
    int32_t DAT_00a3e7c8 = 0;
    int32_t DAT_00a3e7cc = 0;
    int32_t DAT_00a3e82c = 0;
    int32_t DAT_00a3e830 = 0;   
    
    
    
    
    std::vector<GFX::Image *> _menuImages;
    GFX::Image *_bkgImage = nullptr;
    
    GFX::Image *_mapImage = nullptr;
    
    std::array<SDL_Cursor *, 11> _cursors;
    int32_t _curCursor = -1;
    
    static const std::vector<Common::Rect> _mainMenuBoxes;
    static const std::vector<Common::Rect> _saveMenuBoxes; 
    static const std::vector<Common::Rect> _newGameMenuBoxes; 
    
    static const std::vector<Common::Rect> tagRECT_ARRAY_00465ab4;
    static const std::vector<Common::Rect> tagRECT_ARRAY_00465a54;
    static const std::vector<Common::Rect> ScreenBox_ARRAY_0045b0ac;
    
    
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
    static const std::array<Common::Point, 8> O45ad30;
    
    static const std::array<SpecialDmg, 18> SpecialDamages;
    
    static const uint8_t PathGo[8][8];
    
    static const std::array<MixInfo, 33> MixPotionsInfo;
    
    int32_t _saveMenuBtnID = -1;
    
    std::array<ImagePlace, 100> _imgQueue1;
    int32_t _imgQueue1Count = 0;
    std::array<ImagePlace, 100> _imgQueue2;
    int32_t _imgQueue2Count = 0;
    std::array<TextPlace, 100> _textQueue;
    int32_t _textQueueCount = 0;
    
    std::array<uint8_t, 7> DrawElm;
    
    std::array<GFX::Font *, 4> _Fonts;
    

    Simix::Mixer _mixer;
    
    
    bool _doQuit = false;
    
public:
    static Engine Instance;
};
    
}

static constexpr Game::Engine &GameInst = Game::Engine::Instance;

#endif /* GAME_H */

