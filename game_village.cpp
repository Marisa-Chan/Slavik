#include "game.h"

namespace Game {


void Engine::OnEnterVillageUpdateTrader(Village *vlg)
{
    int iVar19 = (_state.MapChar_ARRAY.at(vlg->ChiefCharId).unk5 & 3);
    
    int maxItm = (rand() % 3) + 1;
    for (int i = 0; i < 2; ++i)
    {
        if (maxItm && vlg->MedicItems[i * 14] == 0) 
        {
            ItemInfo *itm = AllocItem();
            if (!itm)
                return;
            
            maxItm--;
            itm->TypeID = 9;
            itm->InfoID = 3;
            
            vlg->MedicItems[i * 14] = itm->Index;
        }
    }
    
    for (int i = 0; i < 3; ++i)
    {
        if (vlg->TraderItems[i] == 0 && (rand() % 100) > 70 )
        {
            ItemInfo *itm = AllocItem();
            if (!itm)
                return;
            
            vlg->TraderItems[i] = itm->Index;
            
            int rndval = rand();
            if (rndval % 30 < 10) 
            {
                itm->TypeID = 8;
                if (_mainCharacter->Level / 5 < 2) 
                    itm->InfoID = 6;
                else 
                    itm->InfoID = 6 + (rand() % (_mainCharacter->Level / 5));
            }
            else if (rndval % 30 < 20) 
            {
                itm->TypeID = 7;
                if (_mainCharacter->Level / 5 < 2)
                    itm->InfoID = 3;
                else
                    itm->InfoID = 3 + (rand() % (_mainCharacter->Level / 5));
            }
            else 
            {
                itm->TypeID = 6;
                if (_mainCharacter->Level / 5 < 2)
                    itm->InfoID = 0;
                else 
                    itm->InfoID = rand() % (_mainCharacter->Level / 5);
            }
            
            if ((_mainCharacter->Level / 8) + 1 < 2)  // CHECK
                itm->BonusID = ((rand() % 4) + 1) * 10 + (rand() % 2);
            else
                itm->BonusID = ((rand() % 4) + 1) * 10 + (rand() % ((_mainCharacter->Level / 8) + 1)) * 3 + (rand() % 2);
        }
    }
    
    for (int i = 3; i < 5; ++i)
    {
        if (vlg->TraderItems[i] == 0 && (rand() % 100) > 50)
        {
            ItemInfo *itm = AllocItem();
            if (!itm)
                return;
            
            vlg->TraderItems[i] = itm->Index;
            itm->TypeID = 11;
            itm->InfoID = rand() % 10;
        }
    }
    
    for (int i = 5; i < 8; ++i)
    {
        if (vlg->TraderItems[i] == 0 && (rand() % 100) > 30) 
        {
            ItemInfo *itm = AllocItem();
            if (!itm)
                return;
            
            vlg->TraderItems[i] = itm->Index;
            
            if (_mainCharacter->Level / 5 + 1 < 2) 
                itm->InfoID = (rand() % 2) * 3 + FUN_00417170(iVar19) * 9;
            else
                itm->InfoID = (rand() % 2) * 3 + (rand() % (_mainCharacter->Level / 5 + 1)) + FUN_00417170(iVar19) * 9;
            
            itm->Weight = ArmorWeaponInfo[itm->InfoID].unk1;
            itm->Concentration = itm->Weight;

            if (itm->InfoID % 9 < 3)
                itm->TypeID = 2;
            else
                itm->TypeID = 0;
            
            if (rand() % 101 < 10) 
            {
                if (FUN_004171d4(_mainCharacter->Level) < 2)
                    itm->BonusID = (rand() % 5) * 10;
                else 
                    itm->BonusID = (rand() % FUN_004171d4(_mainCharacter->Level)) + (rand() % 5) * 10;
            }
        }
    }
    
    for(int i = 8; i < 14; ++i)
    {
        if (vlg->TraderItems[i] == 0 && (rand() % 100) > 50) 
        {
            ItemInfo *itm = AllocItem();
            if (!itm)
                return;
            
            vlg->TraderItems[i] = itm->Index;
            
            int tmp = rand() % 3;
            if (tmp == 0) 
            {
                if (_mainCharacter->Level / 5 + 1 < 2)
                    itm->InfoID = 34 + FUN_00417170(iVar19) * 11;
                else
                    itm->InfoID = 34 + (rand() % (_mainCharacter->Level / 5 + 1)) + FUN_00417170(iVar19) * 11;
                itm->TypeID = 3;
            }
            else if (tmp < 2)
            {
                if (_mainCharacter->Level / 4 + 1 < 2)
                    itm->InfoID = 37 + FUN_00417170(iVar19) * 11;
                else
                    itm->InfoID = 37 + (rand() % (_mainCharacter->Level / 4 + 1)) + FUN_00417170(iVar19) * 11;
                itm->TypeID = 5;
            }
            else if (tmp == 2) 
            {
                if (_mainCharacter->Level / 4 + 1 < 2)
                    itm->InfoID = 41 + FUN_00417170(iVar19) * 11;
                else 
                    itm->InfoID = 41 + (rand() % (_mainCharacter->Level / 4 + 1)) + FUN_00417170(iVar19) * 11;                
                itm->TypeID = 4;
            }
            itm->Weight = ArmorWeaponInfo[ itm->InfoID ].unk1;
            itm->Concentration = itm->Weight;
            
            if (rand() % 100 < 7) 
            {
                int tmp2 = FUN_004171d4(_mainCharacter->Level);
                if (tmp2 < 2) 
                    itm->BonusID = (rand() % 5) * 10;
                else 
                    itm->BonusID = (rand() % tmp2) + (rand() % 5) * 10;
            }
        }
    }
    
    for (int i = 14; i < 32; ++i) 
    {
        int32_t id = vlg->TraderItems[i];
        if (id != 0)
            _state.Items[id].TypeID = -1;
    }
    
    
    ItemInfo *itm = AllocItem();
    if (!itm) 
        return;
    
    vlg->TraderItems[14] = itm->Index;
    
    itm->TypeID = 7;
    if (_mainCharacter->Level / 5 < 2)
        itm->InfoID = 3;
    else
        itm->InfoID = (rand() % (_mainCharacter->Level / 5)) + 3;
    
    if (_mainCharacter->Level / 8 + 1 < 2)
        itm->BonusID = (rand() % 2) + ((rand() % 4) + 1) * 10;
    else
        itm->BonusID = (rand() % 2) + (rand() % (_mainCharacter->Level / 8 + 1)) * 3 + ((rand() % 4) + 1) * 10;
   
    
    
    itm = AllocItem();
    if (!itm) 
        return;
    
    vlg->TraderItems[15] = itm->Index;
    
    itm->TypeID = 6;
    if (_mainCharacter->Level / 5 < 2)
        itm->InfoID = 0;
    else
        itm->InfoID = (rand() % (_mainCharacter->Level / 5));
    
    if (_mainCharacter->Level / 8 + 1 < 2)
        itm->BonusID = (rand() % 2) + ((rand() % 4) + 1) * 10;
    else 
        itm->BonusID = (rand() % 2) + (rand() % (_mainCharacter->Level / 8 + 1)) * 3 + ((rand() % 4) + 1) * 10;
    
    
    
    itm = AllocItem();
    if (!itm)
        return;
    
    vlg->TraderItems[16] = itm->Index;
    
    itm->TypeID = 8;
    if (_mainCharacter->Level / 5 < 2)
        itm->InfoID = 6;
    else
        itm->InfoID = 6 + (rand() % (_mainCharacter->Level / 5));
    
    if (_mainCharacter->Level / 8 + 1 < 2)
        itm->BonusID = (rand() % 2) + ((rand() % 4) + 1) * 10;
    else 
        itm->BonusID = (rand() % 2) + (rand() % (_mainCharacter->Level / 8 + 1)) * 3 + ((rand() % 4) + 1) * 10;
        
    
    
    itm = AllocItem();
    if (!itm)
        return;
    
    vlg->TraderItems[17] = itm->Index;
    
    if (_mainCharacter->Level / 5 + 1 < 2)
        itm->InfoID = (rand() % 2) * 3 + FUN_00417220(iVar19) * 9;
    else 
        itm->InfoID = (rand() % 2) * 3 + (rand() % (_mainCharacter->Level / 5 + 1)) + FUN_00417220(iVar19) * 9;
    
    if (itm->InfoID % 9 < 3)
        itm->TypeID = 2;
    else
        itm->TypeID = 0;
    
    itm->Weight = ArmorWeaponInfo[itm->InfoID].unk1;
    itm->Concentration = itm->Weight;
    
    
    
    itm = AllocItem();
    if (!itm)
        return;
    
    vlg->TraderItems[18] = itm->Index;
    if (_mainCharacter->Level / 5 + 1 < 2)
        itm->InfoID = (rand() % 2) * 3 + FUN_00417220(iVar19 + 1) * 9;
    else
        itm->InfoID = (rand() % 2) * 3 + (rand() % (_mainCharacter->Level / 5 + 1)) + FUN_00417220(iVar19 + 1) * 9;

    if (itm->InfoID % 9 < 3)
        itm->TypeID = 2;
    else
        itm->TypeID = 0;

    itm->Weight = ArmorWeaponInfo[itm->InfoID].unk1;
    itm->Concentration = itm->Weight;
    
    
    
    itm = AllocItem();
    if (!itm)
        return;
    
    vlg->TraderItems[19] = itm->Index;
    itm->TypeID = 1;
    if (_mainCharacter->Level / 5 + 1 < 2)
        itm->InfoID = 6 + FUN_00417220(iVar19) * 9;
    else
        itm->InfoID = 6 + (rand() % (_mainCharacter->Level / 5 + 1)) + FUN_00417220(iVar19) * 9;

    itm->Weight = ArmorWeaponInfo[itm->InfoID].unk1;
    itm->Concentration = itm->Weight;
    
    
    
    itm = AllocItem();
    if (!itm)
        return;
        
    vlg->TraderItems[20] = itm->Index;
    itm->TypeID = 1;
    if (_mainCharacter->Level / 5 + 1 < 2)
        itm->InfoID = 6 + FUN_00417220(iVar19 + 1) * 9;
    else
        itm->InfoID = 6 + (rand() % (_mainCharacter->Level / 5 + 1)) + FUN_00417220(iVar19 + 1) * 9;

    itm->Weight = ArmorWeaponInfo[itm->InfoID].unk1;
    itm->Concentration = itm->Weight;
    
    
    
    itm = AllocItem();
    if (!itm)
        return;
    
    vlg->TraderItems[21] = itm->Index;
    itm->TypeID = 3;
    if (_mainCharacter->Level / 5 + 1 < 2)
        itm->InfoID = 34 + FUN_00417220(iVar19) * 11;
    else
        itm->InfoID = 34 + (rand() % (_mainCharacter->Level / 5 + 1)) + FUN_00417220(iVar19) * 11;

    itm->Weight = ArmorWeaponInfo[itm->InfoID].unk1;
    itm->Concentration = itm->Weight;
    
    
    
    itm = AllocItem();
    if (!itm)
        return;
    
    vlg->TraderItems[22] = itm->Index;
    itm->TypeID = 5;
    if (_mainCharacter->Level / 4 + 1 < 2)
        itm->InfoID = 37 + FUN_00417220(iVar19) * 11;
    else
        itm->InfoID = 37 + (rand() % (_mainCharacter->Level / 4 + 1)) + FUN_00417220(iVar19) * 11;

    itm->Weight = ArmorWeaponInfo[itm->InfoID].unk1;
    itm->Concentration = itm->Weight;
    
    
    
    itm = AllocItem();
    if (!itm)
        return;
    
    vlg->TraderItems[23] = itm->Index;
    itm->TypeID = 4;
    if (_mainCharacter->Level / 4 + 1 < 2)
        itm->InfoID = 41 + FUN_00417220(iVar19) * 11;
    else
        itm->InfoID = 41 + (rand() % (_mainCharacter->Level / 4 + 1)) + FUN_00417220(iVar19) * 11;
    
    itm->Weight = ArmorWeaponInfo[itm->InfoID].unk1;
    itm->Concentration = itm->Weight;
    
    
    
    itm = AllocItem();
    if (!itm)
        return;
        
    vlg->TraderItems[24] = itm->Index;
    itm->TypeID = 3;
    if (_mainCharacter->Level / 5 + 1 < 2)
        itm->InfoID = 34 + FUN_00417220(iVar19 + 1) * 11;
    else
        itm->InfoID = 34 + (rand() % (_mainCharacter->Level / 5 + 1)) + FUN_00417220(iVar19 + 1) * 11;
    
    itm->Weight = ArmorWeaponInfo[itm->InfoID].unk1;
    itm->Concentration = itm->Weight;
    
    
    
    itm = AllocItem();
    if (!itm)
        return;
    
    vlg->TraderItems[25] = itm->Index;
    itm->TypeID = 5;
    if (_mainCharacter->Level / 4 + 1 < 2)
        itm->InfoID = 37 + FUN_00417220(iVar19 + 1) * 11;
    else 
        itm->InfoID = 37 + (rand() % (_mainCharacter->Level / 4 + 1)) + FUN_00417220(iVar19 + 1) * 11;
   
    itm->Weight = ArmorWeaponInfo[itm->InfoID].unk1;
    itm->Concentration = itm->Weight;
    
    
    
    itm = AllocItem();
    if (!itm)
        return;
    
    vlg->TraderItems[26] = itm->Index;
    itm->TypeID = 4;
    if (_mainCharacter->Level / 4 + 1 < 2)
        itm->InfoID = 41 + FUN_00417220(iVar19 + 1) * 11;
    else
        itm->InfoID = 41 + (rand() % (_mainCharacter->Level / 4 + 1)) + FUN_00417220(iVar19 + 1) * 11;
    
    itm->Weight = ArmorWeaponInfo[itm->InfoID].unk1;
    itm->Concentration = itm->Weight;
    
    
    
    itm = AllocItem();
    if (!itm)
        return;
    
    vlg->TraderItems[27] = itm->Index;
    if (_mainCharacter->Level / 5 + 1 < 2)
        itm->InfoID = (rand() % 2) * 3 + (rand() % 3) * 9;
    else
        itm->InfoID = (rand() % 2) * 3 + (rand() % (_mainCharacter->Level / 5 + 1)) + (rand() % 3) * 9;

    if (itm->InfoID % 9 < 3)
        itm->TypeID = 2;
    else
        itm->TypeID = 0;

    itm->Weight = ArmorWeaponInfo[itm->InfoID].unk1;
    itm->Concentration = itm->Weight;
    
    if (_mainCharacter->Level / 8 + 1 < 2)
        itm->BonusID = (rand() % 2) + (rand() % 4) * 10;
    else
        itm->BonusID = (rand() % 2) + (rand() % (_mainCharacter->Level / 8 + 1)) * 3 + (rand() % 4) * 10;
        
    
    
    itm = AllocItem();
    if (!itm)
        return;
    
    vlg->TraderItems[28] = itm->Index;
    if (_mainCharacter->Level / 5 + 1 < 2)
        itm->InfoID = 6 + (rand() % 3) * 9;
    else 
        itm->InfoID = 6 + (rand() % 3) * 9 + (rand() % (_mainCharacter->Level / 5 + 1));
    
    itm->TypeID = 1;
    itm->Weight = ArmorWeaponInfo[itm->InfoID].unk1;
    itm->Concentration = itm->Weight;
    
    if (_mainCharacter->Level / 8 + 1 < 2)
        itm->BonusID = (rand() % 2) + (rand() % 4) * 10;
    else
        itm->BonusID = (rand() % 2) + (rand() % (_mainCharacter->Level / 8 + 1)) * 3 + (rand() % 4) * 10;
    
    
    
    itm = AllocItem();
    if (!itm)
        return;
    
    vlg->TraderItems[29] = itm->Index;
    itm->TypeID = 3;
    if (_mainCharacter->Level / 5 + 1 < 2)
        itm->InfoID = 34 + (rand() % 3) * 11;
    else
        itm->InfoID = 34 + (rand() % 3) * 11 + (rand() % (_mainCharacter->Level / 5 + 1));
    
    itm->Weight = ArmorWeaponInfo[itm->InfoID].unk1;
    itm->Concentration = itm->Weight;
    
    if (_mainCharacter->Level / 8 + 1 < 2)
        itm->BonusID = (rand() % 2) + ((rand() % 4) + 1) * 10;
    else
        itm->BonusID = (rand() % 2) + (rand() % (_mainCharacter->Level / 8 + 1)) * 3 + ((rand() % 4) + 1) * 10;
   
    
    
    itm = AllocItem();
    if (!itm)
        return;
    
    vlg->TraderItems[30] = itm->Index;
    itm->TypeID = 5;
    if (_mainCharacter->Level / 4 + 1 < 2)
        itm->InfoID = 37 + (rand() % 3) * 11;
    else
        itm->InfoID = 37 + (rand() % 3) * 11 + (rand() % (_mainCharacter->Level / 4 + 1));

    itm->Weight = ArmorWeaponInfo[itm->InfoID].unk1;
    itm->Concentration = itm->Weight;
    
    if (_mainCharacter->Level / 8 + 1 < 2)
        itm->BonusID = (rand() % 2) + ((rand() % 4) + 1) * 10;
    else
        itm->BonusID = (rand() % 2) + (rand() % (_mainCharacter->Level / 8 + 1)) * 3 + ((rand() % 4) + 1) * 10;
    
    
    
    itm = AllocItem();
    if (!itm)
        return;
    
    vlg->TraderItems[31] = itm->Index;
    itm->TypeID = 4;
    if (_mainCharacter->Level / 4 + 1 < 2) 
        itm->InfoID = 41 + (rand() % 3) * 11;
    else
        itm->InfoID = 41 + (rand() % 3) * 11 + (rand() % (_mainCharacter->Level / 4 + 1));

    itm->Weight = ArmorWeaponInfo[itm->InfoID].unk1;
    itm->Concentration = itm->Weight;

    if (_mainCharacter->Level / 8 + 1 < 2)
        itm->BonusID = ((rand() % 4) + 1) * 10 + (rand() % 2);
    else
        itm->BonusID = ((rand() % 4) + 1) * 10 + (rand() % (_mainCharacter->Level / 8 + 1)) * 3 + (rand() % 2);
}


Engine::Village * Engine::FUN_0043a1f8(int32_t mapId)
{
    for(Village &vlg : _state.VillageState)
    {
        if (vlg.MapID == mapId)
            return &vlg;
    }
    return nullptr;
}



int32_t Engine::GetVillageCharacterJob(Character *pchar)
{
    MapChar &mchar = _state.MapChar_ARRAY.at(pchar->MapCharID);
    Village *vlg = FUN_0043a1f8(mchar.MapID);
    if (!vlg)
        return 0;
    
    for (int32_t i = 0; i < vlg->Jobs.size(); ++i)
    {
        Village::Job &job = vlg->Jobs.at(i);
        if (job.CharID == pchar->Index)
            return i + 1;
    }
    return 0;
}


void Engine::UpdateVillageMedic(Village *vlg)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
}

void Engine::UpdateVillageSmith(Village *vlg)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
}

void Engine::UpdateVillageVoevoda(Village *vlg)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
}


}