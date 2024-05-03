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

constexpr const Common::Rect MSGRECT = Common::Rect(SCREENRESX - MAPVIEWW, MAPVIEWH, SCREENRESX, SCREENRESY);

//constexpr const Common::PointRect SRect_ARRAY_0046280c = Common::Rect(13, 13, 502, 361);
constexpr const Common::PointRect SRect_ARRAY_0046280c = Common::PointRect(80, 60, 502, 361);


constexpr const float PX = 1.0 / (float) SCREENRESX;
constexpr const float PY = 1.0 / (float) SCREENRESY;

constexpr const int32_t TXTHEIGHT = 15;

extern const Common::Point GScrOff;

constexpr const int32_t INVSIZE = 32;

constexpr const int32_t MAXPATHITER = 0x4000;

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
        
        Common::Point ViewPos;
        //int32_t field107_0xe4;
        //int32_t xPOS;
        Common::Point POS;
        //int32_t field109_0xec;
        //int32_t yPOS;
        Common::Point imgSize;
        //int16_t field111_0xf4;
        //int16_t field112_0xf6;
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
        
        std::string FmtCharName();
        std::string FmtLvlHP();
        std::string GetCharHint();
        
        inline void RecalcViewPos(Common::Point offset)
            { ViewPos = offset + POS + imgOffset; }
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
    };
    
public:
    
    Engine()
    {
        for(int32_t i = 0; i < mapGS1.size(); ++i)
            mapGS1[i].Index = i;
    }
    
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
    
    void ImgQueue1(GFX::Image *img, Common::Point pos, Common::Rect limits, vec3f fade = vec3f());
    void ImgQueue2(GFX::Image *img, Common::Point pos, Common::Rect limits, vec3f fade = vec3f());
    void ImgQueue1(GFX::PalImage *img, int32_t Pal, Common::Point pos, Common::Rect limits, vec3f fade = vec3f());
    void ImgQueue2(GFX::PalImage *img, int32_t Pal, Common::Point pos, Common::Rect limits, vec3f fade = vec3f());
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
    int32_t GetCurrentWeight(Character *pchar);
    
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
    
    void FUN_0042f9b8(const std::string &txt);
    bool FUN_00416700(Village *vlg);
    
    void FUN_004292e4();
    void FUN_00429194(int32_t);
    bool FUN_0041e96c();
    
    void FUN_004118dc();
    void FUN_00436ad0(int32_t);
    void FUN_00436a00();
    void FUN_00421170();
    void FUN_00421da4();
    void FUN_00439230(Character *pchar);
    bool FUN_0041f258(Character *pchar);
    void FUN_00421bb8(Character *pchar);
    void FUN_004110ec(Character *pchar);
    int FUN_00434c90(int32_t );    
    void MapObjectsUpdateFrames();
    void FUN_0042d574(Character *pchar);
    vec3f FUN_0042c914(Character *pchar, ItemInfo *itm);
    int32_t FUN_0042c870(ItemInfo *itm);
    void FUN_0042f50c(Character *pchar, int32_t);
    void FUN_0041d0fc();
    void FUN_0041733c(Village::BldState *state);
    int32_t FUN_0041b70c(Character *pchar);
    void FUN_0041bdf0(MapChar *mchar);
    void FUN_00418510(Character *pchar, int32_t w, int32_t lvl);
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
    
    void DrawSettingsScreen(int32_t);
    
    void ShutdownWithMsg(const std::string &msg);
    
    void OnEnterVillageUpdateTrader(Village *vlg);
    int32_t GetVillageCharacterJob(Character *pchar);
    Village * FUN_0043a1f8(int32_t mapId);
    
    void UpdateVillageMedic(Village *vlg);
    void UpdateVillageSmith(Village *vlg);
    void UpdateVillageVoevoda(Village *vlg);
    
    
    bool CheckTraceImage(GFX::Image *img, Common::Point drawPos);
    bool CheckTraceImage(GFX::PalImage *img, Common::Point drawPos);
    
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
    
    
    //Input
    void PlayHandleKey(int16_t keyCode);    
    void PlayProcessMouse();
    int32_t FUN_004392f4();
    int32_t FUN_00439a28(Common::Point pt, const Common::PlaneVector<uint8_t> *mask);
    bool FUN_0041e500(int32_t slot);
    
    
    //UI
    void FUN_00431d70(int p);
    void FillBkgRect(Common::Rect rect);
    void DrawMap();
    void UpdateMapImage();
    void DrawInventory(Character *chr, int btn, bool bkg = true);
    void DrawCharInfo();
    void DrawJournal();
    
    void FUN_00425104(const ItemInfo *inf);
    void FUN_0041e210(Character *pchar, int32_t typ, int32_t itemid);
    bool FUN_00424918(Character *pchar, int32_t itemid);
    bool FUN_0041e778(int aidx);
    
    
    //Sound
    void FUN_0043d1d0();

    
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
    
    std::map<int16_t, int8_t> _KeyMap;
    std::array<int8_t, KEYFN_MAX> _KeyState;
    std::deque<int16_t> _KeyQueue;
    int16_t _KeyCode = 0;
    
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
    
    bool DWORD_00a3e758 = false;
    
    int32_t DAT_00a3e704 = 0;
    int32_t DisplayInvOfCharID2 = 1;
    int32_t DisplayInvOfCharID = 1;
    
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
    
    
    std::array<int8_t, 56> MapsOpened; //Remake it to bitfield and update map image only change
    
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
    
    
    
    Common::Point ViewTiles;
    
    int32_t ItemInvSlotWidth = 1;
    int32_t ScrlBtnWidth = 0;
    int32_t DAT_00a3e870 = 0;
    int32_t DAT_00a3e88c = 0;
    int32_t ScrlBtnHeight = 0;
    
    int32_t InvPos = 0;
    
    int32_t DAT_00a3e790 = 0;
    
    int32_t DAT_00a3e7a0 = -1;
    
    int32_t DWORD_00a3e7b4 = 0;
    
    std::vector<GFX::Image *> _menuImages;
    GFX::Image *_bkgImage = nullptr;
    
    GFX::Image *_mapImage = nullptr;
    
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
    
    std::array<TQuestState, 300> QuestsState;
    
    bool _doQuit = false;
    
public:
    static Engine Instance;
};
    
}

static constexpr Game::Engine &GameInst = Game::Engine::Instance;

#endif /* GAME_H */

