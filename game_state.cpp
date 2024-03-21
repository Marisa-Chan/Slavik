#include "game.h"
#include "resources.h"
#include "fmt/core.h"

namespace Game
{
    
    
void Engine::Character::Load(FSMgr::iFile* pfile)
{
    State = pfile->readU8();        //0
    Direction = pfile->readU8();    //1
    field2_0x2 = pfile->readU8();   //2
    field_0x3 = pfile->readU8();    //3
    ClassID = pfile->readU8();      //4
    MapCharID = pfile->readU8();    //5
    EnemyCharID = pfile->readS16L(); //6
    Tile.y = pfile->readS16L();     //8
    Tile.x = pfile->readS16L();     //a
    MoveTile.y = pfile->readS16L(); //c
    MoveTile.x = pfile->readS16L(); //e
    Frame = pfile->readU8();        //10
    field15_0x11 = pfile->readU8(); //11
    field_0x12 = pfile->readU8();   //12
    for(auto &field : field17_0x13) //13
        field = pfile->readU8();    //+20
    Level = pfile->readU8();        //33
    Gold = pfile->readS32L();       //34
    FreePoints = pfile->readS32L(); //38
    Exp = pfile->readS32L();        //3c
    Fehtovanie = pfile->readS16L(); //40
    Trading = pfile->readS16L();    //42
    Metkost = pfile->readS16L();    //44
    PlotnickoeDelo = pfile->readS16L(); //46
    Medicine = pfile->readS16L();   //48
    Identification = pfile->readS16L(); //4a
    KuznechnoeDelo = pfile->readS16L(); //4c
    Armor = pfile->readS16L();      //4e
    Otravlenie = pfile->readS16L(); //50
    field62_0x52 = pfile->readU8(); //52
    field63_0x53 = pfile->readU8(); //53
    CurrentLovkost = pfile->readS16L(); //54
    BaseLovkost = pfile->readS16L();    //56
    CurrentHarizm = pfile->readS16L();  //58
    BaseHarizm = pfile->readS16L();     //5a
    HP = pfile->readS16L();             //5c
    CurrentSila = pfile->readS16L();    //5e
    BaseSila = pfile->readS16L();       //60
    CurrentVinoslivost = pfile->readS16L(); //62
    BaseVinoslivost = pfile->readS16L();    //64
    
    for (size_t i = 0; i < 6; ++i)      //66
        ArmorWeapons[i] = pfile->readS16L(); //+c
    
    for (size_t i = 0; i < 32; ++i)     //72
        Inventory[i] = pfile->readU16L(); //+40
    
    for (size_t i = 0; i < 5; ++i)      //b2
        Accessories[i] = pfile->readU16L(); //+a

    field76_0xbc = pfile->readS16L(); //bc
    field78_0xbe = pfile->readS16L(); //be
    field80_0xc0 = pfile->readS16L(); //c0
    field82_0xc2 = pfile->readS16L(); //c2
    CurrentBrn = pfile->readS16L(); //c4
    CurrentUdr = pfile->readS16L(); //c6
    CurrentVer = pfile->readS16L(); //c8
    field90_0xca = pfile->readS16L(); //ca
    Arrows = pfile->readS16L();     //cc
    field_0xce = pfile->readS16L(); //ce
    field96_0xd0 = pfile->readS8(); //d0
    Flags = pfile->readU8();        //d1
    field98_0xd2 = pfile->readS8(); //d2
    field99_0xd3 = pfile->readS8(); //d3
    NameID = pfile->readU8();       //d4
    NickID = pfile->readU8();       //d5    
    CharacterBase = pfile->readU8();    //d6
    FrameCount = pfile->readU8();       //d7
    paletteOffset = pfile->readS32L() / 512;    //d8
    pfile->seek(8, 1);                  //dc
    ViewPos.x = pfile->readS32L();      //e4
    POS.x = pfile->readS32L();          //e8
    ViewPos.y = pfile->readS32L();      //ec
    POS.y = pfile->readS32L();          //f0
    imgSize.x = pfile->readS16L();  //f4
    imgSize.y = pfile->readS16L();  //f6
    pfile->seek(8, 1);                  //f8
}

int32_t Engine::Character::GetMaxPartySize()
{
    static const int32_t PartySizeCharm [5][10] =
    {{15, 21, 28, 36, 45, 55, 65, 74, 83, 95},
     {15, 21, 28, 36, 45, 55, 65, 74, 83, 95},
     {17, 23, 30, 38, 47, 57, 67, 76, 85, 97},
     {15, 21, 28, 36, 45, 55, 65, 74, 83, 95},
     {20, 28, 36, 45, 55, 65, 74, 83, 95, 110}};
    
    int32_t partySize = 0;
    for (; partySize < 10; ++partySize)
    {
        if (CurrentHarizm < PartySizeCharm[(ClassID & CLASS_MASK) - 1][partySize])
            break;
    }
    
    if (partySize < 10)
        partySize++;
    
    return partySize;
}


void Engine::ItemInfo::Load(FSMgr::iFile* pfile)
{
    TypeID = pfile->readS8();
    BonusID = pfile->readS8();
    SpecialID = pfile->readS8();
    InfoID = pfile->readS8();
    Concentration = pfile->readFloatL();
    Weight = pfile->readFloatL();
    Poison = pfile->readS16L();
    Flags = pfile->readU8();
    unk1 = pfile->readU8();
}

void Engine::MapChar::Load(FSMgr::iFile* pfile)
{
    CharacterIndex = pfile->readS16L(); //0
    field1_0x2 = pfile->readS32L();     //2
    field2_0x6 = pfile->readS16L();     //6
    spwnTlRect.top = pfile->readS16L(); //8
    someRect.top = pfile->readS16L();   //a
    spwnTlRect.bottom = pfile->readS16L(); // c
    someRect.bottom = pfile->readS16L();   // e
    spwnTlRect.left = pfile->readS16L();   // 10
    someRect.left = pfile->readS16L();     // 12
    spwnTlRect.right = pfile->readS16L();  // 14
    someRect.right = pfile->readS16L();    // 16
    unk1 = pfile->readU8();                // 18
    unk2 = pfile->readU8();                // 19
    unk3 = pfile->readU8();                // 1a
    MapID = pfile->readU8();               // 1b
    GroupSize = pfile->readU8();           // 1c
    unk4 = pfile->readU8();                // 1d
    unk5 = pfile->readU8();                // 1e
    unk6 = pfile->readU8();                // 1f
}

void Engine::GS1::Load(FSMgr::iFile* pfile)
{
    Pos.x = pfile->readS32L();
    Pos.y = pfile->readS32L();
    Tile.y = pfile->readS16L();
    Tile.x = pfile->readS16L();
    ImgID = pfile->readS16L();
    LootID = pfile->readS16L();
    unk2 = pfile->readS16L();
    MapID = pfile->readS16L();
    for(int16_t i = 0; i < 22; ++i)
        ItemID[i] = pfile->readS16L();
}

void Engine::GS2::Load(FSMgr::iFile* pfile)
{
    field0_0x0 = pfile->readU8();
    field1_0x1 = pfile->readU8();
    field2_0x2 = pfile->readU8();
    MapID = pfile->readS8();
    field4_0x4 = pfile->readU8();
    field5_0x5 = pfile->readS16L();
    field6_0x7 = pfile->readS16L();
    left = pfile->readS16L();
    up = pfile->readS16L();
    right = pfile->readS16L();
    bottom = pfile->readS16L();
}


void Engine::Village::Job::Load(FSMgr::iFile* pfile)
{
    Direction = pfile->readU8();
    unk = pfile->readU8();
    CharID = pfile->readS16L();
    Tile.y = pfile->readS16L();
    Tile.x = pfile->readS16L();
}

void Engine::Village::BldState::Load(FSMgr::iFile* pfile)
{
    BuildInfoID = pfile->readS8();
    State = pfile->readS8();
    ObjID = pfile->readS16L();
    unk = pfile->readS16L();
    BldLeft = pfile->readS16L();
}

void Engine::Village::Load(FSMgr::iFile* pfile)
{
    Bld1Num = pfile->readU8();
    Bld2Num = pfile->readU8();
    ChiefCharId = pfile->readU8();
    MapID = pfile->readU8();
        
    UpdateMedicTimer = pfile->readS32L();
    SmithUpdateTimer = pfile->readS32L();
    VoevodaUpdateTimer = pfile->readS32L();
    GoldTribute = pfile->readS32L();
    GoldTributeTimer = pfile->readS32L();
    
    for (BldState &bld : BuildingState)
        bld.Load(pfile);
    
    for (Job &job : Jobs)
        job.Load(pfile);
    
    unk1 = pfile->readS32L();
        
    TimeSt1 = pfile->readS32L();
    TimeSt2 = pfile->readS32L();
    TimeSt3 = pfile->readS32L();

    unk2 = pfile->readS32L();
        
    DoMedicPotionSlot6 = pfile->readS16L();
    DoMedicPotionSlot7 = pfile->readS16L();
    DoMedicPotionSlot5 = pfile->readS16L();
        
    for (int16_t &item : MedicItems)
        item = pfile->readS16L();
        
    unk3 = pfile->readS16L();

    for (int16_t &item : TraderItems)
        item = pfile->readS16L();
    
    for (int16_t &item : SmithItems)
        item = pfile->readS16L();

    Flags = pfile->readU8();

    unk4 = pfile->readU8();
    SmithItemsCount = pfile->readS8();
}
    
void Engine::GameState::Load(FSMgr::iFile *pfile)
{
    for(int32_t i = 0; i < Items.size(); ++i)
    {
        ItemInfo &it = Items.at(i);
        it.Index = i;
        it.Load(pfile);
    }
    
    for(int32_t i = 0; i < MapChar_ARRAY.size(); ++i)
    {
        MapChar &it = MapChar_ARRAY.at(i);
        it.Index = i;
        it.Load(pfile);
    }
    
    for(int32_t i = 0; i < GS1ARRAY.size(); ++i)
    {
        GS1 &it = GS1ARRAY.at(i);
        it.Index = i;
        it.Load(pfile);
    }
    
    for(int32_t i = 0; i < GS2ARRAY.size(); ++i)
    {
        GS2 &it = GS2ARRAY.at(i);
        it.Index = i;
        it.Load(pfile);
    }
    
    for(int32_t i = 0; i < Characters.size(); ++i)
    {
        Character &it = Characters.at(i);
        it.Index = i;
        it.Load(pfile);
    }

    for(int32_t i = 0; i < VillageState.size(); ++i)
    {
        Village &it = VillageState.at(i);
        it.Index = i;
        it.Load(pfile);
    }
}



void Engine::LoadGameState(int32_t stateID)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    FSMgr::File file = FSMgr::Mgr::ReadFile(fmt::format("GAME.{:d}", stateID));
    
    _state.Load(file.get());
    
    _state.GS1ARRAYCount = 0;
    for( GS1 &obj : _state.GS1ARRAY )
    {
        if (obj.MapID == 0 )
            break;
        
        obj.ImgID = GetLootImage(obj);
        _state.GS1ARRAYCount++;
    }
    
    _state.GS2ARRAYCount = 0;
    for ( GS2 &obj : _state.GS2ARRAY )
    {
        if (obj.field4_0x4 == 0)
            break;
        
        _state.GS2ARRAYCount++;
    }
}

}