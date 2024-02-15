#include "game.h"
#include "resources.h"
#include "fmt/core.h"

namespace Game
{
    
    
void Engine::Character::Load(FSMgr::iFile* pfile)
{
    State = pfile->readU8();
    Direction = pfile->readU8();
    pfile->seek(2, 1);
    ClassID = pfile->readU8();
    MapCharID = pfile->readU8();
    field6_0x6 = pfile->readS16L();
    Tile.y = pfile->readS16L();
    Tile.x = pfile->readS16L();
    _field_0xc = pfile->readS16L();
    _field_0xe = pfile->readS16L();
    Frame = pfile->readU8();
    field15_0x11 = pfile->readU8();
    field_0x12 = pfile->readU8();
    for(auto &field : field17_0x13)
        field = pfile->readU8();
    Level = pfile->readU8();
    Gold = pfile->readS32L();
    FreePoints = pfile->readS32L();
    Exp = pfile->readS32L();
    Fehtovanie = pfile->readS16L();
    Trading = pfile->readS16L();
    Metkost = pfile->readS16L();
    PlotnickoeDelo = pfile->readS16L();
    Medicine = pfile->readS16L();
    Identification = pfile->readS16L();
    KuznechnoeDelo = pfile->readS16L();
    Armor = pfile->readS16L();
    Otravlenie = pfile->readS16L();
    pfile->seek(2, 1);
    CurrentLovkost = pfile->readS16L();
    BaseLovkost = pfile->readS16L();
    CurrentHarizm = pfile->readS16L();
    BaseHarizm = pfile->readS16L();
    HP = pfile->readS16L();
    CurrentSila = pfile->readS16L();
    BaseSila = pfile->readS16L();
    CurrentVinoslivost = pfile->readS16L();
    BaseVinoslivost = pfile->readS16L();
    
    for (size_t i = 0; i < 6; ++i)
        ArmorWeapons[i] = pfile->readS16L();
    
    for (size_t i = 0; i < 32; ++i)
        field74_0x72[i] = pfile->readU16L();
    
    for (size_t i = 0; i < 5; ++i)
        nnnn[i] = pfile->readU16L();

    pfile->seek(20, 1);
    field96_0xd0 = pfile->readS8();
    pfile->seek(5, 1);
    
    CharacterBase = pfile->readU8();
    FrameCount = pfile->readU8();
    paletteOffset = pfile->readS32L() / 512;
    pfile->seek(8, 1);
    field107_0xe4 = pfile->readS32L();
    POS.x = pfile->readS32L();
    field109_0xec = pfile->readS32L();
    POS.y = pfile->readS32L();
    field111_0xf4 = pfile->readS16L();
    field112_0xf6 = pfile->readS16L();
    pfile->seek(8, 1);
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
    field5_0x6 = pfile->readU8();       //6
    field6_0x7 = pfile->readU8();       //7
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
    unk1 = pfile->readS16L();
    unk2 = pfile->readS16L();
    MapID = pfile->readS16L();
    ItemID = pfile->readS16L();
    
    for(int16_t i = 0; i < 21; ++i)
        unk3[i] = pfile->readS16L();
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
    
    for(GS2 &it : GS2ARRAY)
        it.Load(pfile);
    
    for(int32_t i = 0; i < Characters.size(); ++i)
    {
        Character &it = Characters.at(i);
        it.CharIndex = i;
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