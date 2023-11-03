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
    field5_0x5 = pfile->readU8();
    field6_0x6 = pfile->readS16L();
    tileY = pfile->readS16L();
    tileX = pfile->readS16L();
    _field_0xc = pfile->readS16L();
    _field_0xe = pfile->readS16L();
    Frame = pfile->readU8();
    field15_0x11 = pfile->readU8();
    field_0x12 = pfile->readU8();
    pfile->seek(32, 1);
    Level = pfile->readU8();
    Gold = pfile->readS32L();
    FreePoints = pfile->readS32L();
    Exp = pfile->readS32L();
    Fehtovanie = pfile->readU16L();
    Trading = pfile->readU16L();
    Metkost = pfile->readU16L();
    PlotnickoeDelo = pfile->readU16L();
    Medicine = pfile->readU16L();
    Identification = pfile->readU16L();
    KuznechnoeDelo = pfile->readU16L();
    pfile->seek(2, 1);
    Otravlenie = pfile->readU16L();
    pfile->seek(2, 1);
    CurrentLovkost = pfile->readU16L();
    BaseLovkost = pfile->readU16L();
    CurrentHarizm = pfile->readU16L();
    BaseHarizm = pfile->readU16L();
    HP = pfile->readU16L();
    CurrentSila = pfile->readU16L();
    BaseSila = pfile->readU16L();
    CurrentVinoslivost = pfile->readU16L();
    BaseVinoslivost = pfile->readU16L();
    
    for (size_t i = 0; i < 6; ++i)
        ArmorWeapons[i] = pfile->readS16L();
    
    for (size_t i = 0; i < 32; ++i)
        field74_0x72[i] = pfile->readU16L();
    
    for (size_t i = 0; i < 5; ++i)
        nnnn[i] = pfile->readU16L();

    pfile->seek(26, 1);
    
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
    /*unk = */ pfile->readU8();
}

void Engine::MapChar::Load(FSMgr::iFile* pfile)
{
    CharacterIndex = pfile->readS16L();
    pfile->seek(6, 1);
    field_0x8 = pfile->readS16L();
    pfile->seek(2, 1);
    field_0xc = pfile->readS16L();
    pfile->seek(2, 1);
    field_0x10 = pfile->readS16L();
    pfile->seek(2, 1);
    field_0x14 = pfile->readS16L();
    pfile->seek(5, 1);
    MapID = pfile->readU8();
    pfile->seek(4, 1);
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
    
void Engine::GameState::Load(FSMgr::iFile *pfile)
{
    for(ItemInfo &it : Items)
        it.Load(pfile);
    
    for(MapChar &it : MapChar_ARRAY)
        it.Load(pfile);
    
    pfile->seek(32000, 1);
    pfile->seek(4250, 1);
    
    
    for(int32_t i = 0; i < Characters.size(); ++i)
    {
        Character &it = Characters.at(i);
        it.CharIndex = i;
        it.Load(pfile);
    }
        
    
    pfile->seek(24100, 1);
}



void Engine::LoadGameState(int32_t stateID)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    FSMgr::File file = FSMgr::Mgr::ReadFile(fmt::format("GAME.{:d}", stateID));
    _state.Load(file.get());
}

}