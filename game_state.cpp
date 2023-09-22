#include "game.h"
#include "resources.h"
#include "fmt/core.h"

namespace Game
{
    
    int aaa = 0;
    
void Engine::Character::Load(FSMgr::iFile* pfile)
{
    State = pfile->readU8();
    Direction = pfile->readU8();
    pfile->seek(2, 1);
    ClassID = pfile->readU8();
    pfile->seek(3, 1);
    field8_0x8 = pfile->readS16L();
    field9_0xa = pfile->readS16L();
    _field_0xc = pfile->readS16L();
    _field_0xe = pfile->readS16L();
    Frame = pfile->readU8();
    pfile->seek(34, 1);
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
        ArmorWeapons[i] = pfile->readU16L();
    
    for (size_t i = 0; i < 32; ++i)
        field74_0x72[i] = pfile->readU16L();
    
    for (size_t i = 0; i < 5; ++i)
        nnnn[i] = pfile->readU16L();

    pfile->seek(26, 1);
    
    CharacterBase = pfile->readU8();
    FrameCount = pfile->readU8();
    paletteOffset = pfile->readS32L();
    pfile->seek(8, 1);
    field107_0xe4 = pfile->readS32L();
    xPOS = pfile->readS32L();
    field109_0xec = pfile->readS32L();
    yPOS = pfile->readS32L();
    field111_0xf4 = pfile->readS16L();
    field112_0xf6 = pfile->readS16L();
    pfile->seek(8, 1);
    
    aaa += 1;
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
    
void Engine::GameState::Load(FSMgr::iFile *pfile)
{
    pfile->seek(0x20000, 1);
    
    for(MapChar &it : MapChar_ARRAY)
        it.Load(pfile);
    
    pfile->seek(32000, 1);
    pfile->seek(4250, 1);
    
    for(Character &it : Characters)
        it.Load(pfile);
    
    pfile->seek(24100, 1);
}



void Engine::LoadGameState(int32_t stateID)
{
    FSMgr::File file = FSMgr::Mgr::ReadFile(fmt::format("GAME.{:d}", stateID));
    _state.Load(file.get());
}

}