#include "game.h"
#include <bitset>

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

bool Engine::FUN_00417268(Village *vlg, Character *pchr, int32_t iout, int32_t i1, int32_t i2)
{
    int32_t itm1id = vlg->MedicItems.at(i1 - 3);
    if (!itm1id)
        return false;
    
    int32_t itm2id = vlg->MedicItems.at(i2 - 3);
    if (!itm2id)
        return false;
    
    MixPotions(pchr, &_state.Items.at(itm1id), &_state.Items.at(itm2id));
    
    _state.Items.at(itm2id).TypeID = -1;
    vlg->MedicItems.at(iout - 3) = itm1id;
    vlg->MedicItems.at(i1 - 3) = 0;
    vlg->MedicItems.at(i2 - 3) = 0;
    
    return true;
}

void Engine::UpdateVillageMedic(Village *vlg)
{
    if (vlg->Jobs[1].CharID != 0 && _playScreenID != PLSCREEN_7)
    {
        vlg->UpdateMedicTimer--;
        
        if (vlg->UpdateMedicTimer < 1)
        {
            vlg->UpdateMedicTimer = 1440;
            vlg->DoMedicPotionSlot7 = 1;
            vlg->DoMedicPotionSlot6 = 1;
            vlg->DoMedicPotionSlot5 = 1;
        }
        else if (vlg->UpdateMedicTimer == 720)
        {
            vlg->DoMedicPotionSlot7 = 1;
            vlg->DoMedicPotionSlot5 = 1;
        }
        else if ((vlg->UpdateMedicTimer == 1080) || (vlg->UpdateMedicTimer == 360)) {
            vlg->DoMedicPotionSlot5 = 1;
        }
        
        Character &chr = _state.Characters.at(vlg->Jobs[1].CharID);
        
        for (int32_t local_18 = 5; local_18 < 28; ++local_18)
        {
            if (vlg->MedicItems[local_18 - 3] == 0)
            {
                switch(local_18)
                {
                case 5:
                case 19:
                    if (vlg->DoMedicPotionSlot5 != 0)
                    {
                        ItemInfo *itm = AllocItem();
                        if (!itm)
                            break;
                        
                        itm->TypeID = 9;
                        itm->InfoID = 2;
                        
                        ItemInfo itm2;
                        itm2.TypeID = 9;
                        itm2.InfoID = 3;
                        
                        MixPotions(&chr, itm, &itm2);
                        
                        vlg->DoMedicPotionSlot5--;
                        vlg->MedicItems[local_18 - 3] = itm->Index;
                    }
                    break;
                    
                case 6:
                case 20:
                    if (vlg->DoMedicPotionSlot6 != 0)
                    {
                        ItemInfo *itm = AllocItem();
                        if (!itm)
                            break;
                        
                        itm->TypeID = 9;
                        itm->InfoID = 0;
                        
                        ItemInfo itm2;
                        itm2.TypeID = 9;
                        itm2.InfoID = 3;
                        
                        MixPotions(&chr, itm, &itm2);
                        
                        vlg->DoMedicPotionSlot6--;
                        vlg->MedicItems[local_18 - 3] = itm->Index;
                    }
                    break;
                    
                case 7:
                case 21:
                    if (vlg->DoMedicPotionSlot7 != 0)
                    {
                        ItemInfo *itm = AllocItem();
                        if (!itm)
                            break;
                        
                        itm->TypeID = 9;
                        itm->InfoID = 1;
                        
                        ItemInfo itm2;
                        itm2.TypeID = 9;
                        itm2.InfoID = 3;
                        
                        MixPotions(&chr, itm, &itm2);
                        
                        vlg->DoMedicPotionSlot7--;
                        vlg->MedicItems[local_18 - 3] = itm->Index;
                    }
                    break;
                    
                case 8:
                case 22:
                    FUN_00417268(vlg, &chr, local_18, 19, 20);
                    break;
                    
                case 9:
                case 23:
                    FUN_00417268(vlg, &chr, local_18, 19, 21);
                    break;
                    
                case 10:
                case 24:
                    FUN_00417268(vlg, &chr, local_18, 19, 23);
                    break;
                    
                case 11:
                case 25:
                    FUN_00417268(vlg, &chr, local_18, 20, 21);
                    break;
                    
                case 12:
                case 26:
                    FUN_00417268(vlg, &chr, local_18, 20, 23);
                    break;
                    
                case 13:
                case 27:
                    FUN_00417268(vlg, &chr, local_18, 21, 22);
                    break;
                }
            }
        }
    }
}

void Engine::UpdateVillageSmith(Village *vlg)
{
    if (vlg->Jobs[3].CharID == 0 || _playScreenID == PLSCREEN_7)
        return;
    
    Character &bsmith = _state.Characters.at(vlg->Jobs[3].CharID);
    
    if (vlg->SmithItemsCount < 0)
    {
        int32_t local_1c = FUN_00417220((_state.MapChar_ARRAY.at(vlg->ChiefCharId).unk5 & 3) - 1);
        int32_t local_18 = bsmith.KuznechnoeDelo;

        std::bitset<23> hasItms;
        int32_t lastId = -1;

        for(int32_t i = 0; i < 23; ++i)
        {
            int32_t need = 0;
            if (i < 3)
                need = ArmorWeaponInfo.at(30 + i).unk1;
            else if (i < 12)
                need = ArmorWeaponInfo.at((i - 3) + local_1c * 9).unk1;
            else
                need = ArmorWeaponInfo.at(34 + (i - 12) + local_1c * 11).unk1;
            
            if (need <= bsmith.KuznechnoeDelo * 10)
            {
                if (vlg->SmithItems[i] == 0)
                {
                    vlg->SmithItemsCount = i;
                    vlg->SmithUpdateTimer = need * 60 / sqrt(bsmith.KuznechnoeDelo);
                    return;
                }
                lastId = i;
                hasItms.set(i);
            }
        }
        if (lastId > -1)
        {
            MapChar &grp = _state.MapChar_ARRAY.at(vlg->ChiefCharId);
            for(int32_t i = 0; i < 23; ++i)
            {
                if (hasItms[i])
                {
                    int32_t traderItm = 0;
                    int32_t slot = -1;
                    if (i < 12)
                    {
                        traderItm = local_1c * 9 + (i - 3);
                        if (i < 6)
                            slot = ESLT_2;
                        else if (i < 9)
                            slot = ESLT_0;
                        else
                            slot = ESLT_1;
                    }
                    else
                    {
                        traderItm = local_1c * 11 + 34 + (i - 12);
                        if (i < 15)
                            slot = ESLT_3;
                        else if (i < 19)
                            slot = ESLT_5;
                        else
                            slot = ESLT_4;
                    }

                    int32_t itemId = vlg->SmithItems.at(i);

                    for(int32_t j = 0; j < grp.GroupSize; ++j)
                    {
                        Character &chr = _state.Characters.at(grp.CharacterIndex + j);
                        if ( FUN_00418364(&chr, &_state.Items.at(itemId)) )
                        {
                            if (i < 3)
                            {
                                if (chr.Arrows == 0)
                                {
                                    chr.Arrows = itemId;
                                    vlg->SmithItems[i] = 0;
                                    vlg->SmithItemsCount = i;
                                    vlg->SmithUpdateTimer = ArmorWeaponInfo.at(30 + i).unk1 * 60 / sqrt(bsmith.KuznechnoeDelo);
                                    return;
                                }
                            }
                            else
                            {
                                ItemInfo *equiped = nullptr;
                                if (chr.ArmorWeapons.at(slot))
                                    equiped = &_state.Items.at( chr.ArmorWeapons.at(slot) );

                                if (!equiped || equiped->InfoID < traderItm)
                                {
                                    if (equiped)
                                        equiped->TypeID = -1;
                                        
                                    chr.ArmorWeapons.at(slot) = itemId;
                                    vlg->SmithItems[i] = 0;
                                    vlg->SmithItemsCount = i;
                                    vlg->SmithUpdateTimer = ArmorWeaponInfo.at(traderItm).unk1 * 60 / sqrt(bsmith.KuznechnoeDelo);
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        vlg->SmithUpdateTimer--;
        
        if (vlg->SmithUpdateTimer < 1)
        {
            ItemInfo *itm = AllocItem();
            if (!itm)
                return;
            
            int32_t i = vlg->SmithItemsCount;
            vlg->SmithItemsCount = -1;

            int32_t local_1c = FUN_00417220((_state.MapChar_ARRAY.at(vlg->ChiefCharId).unk5 & 3) - 1);
            int32_t local_18 = bsmith.KuznechnoeDelo;

            int32_t complexity = 0;
            vlg->SmithItems[i] = itm->Index;
            if (i < 3)
            {
                itm->TypeID = 12;
                itm->Concentration = 30;
                itm->InfoID = 30 + i;
                complexity = ArmorWeaponInfo.at(itm->InfoID).unk1;
            }
            else if (i < 12)
            {
                itm->InfoID = i - 3 + local_1c * 9;
                itm->Weight = ArmorWeaponInfo.at(itm->InfoID).unk1;
                itm->Concentration = itm->Weight;
                
                if (i < 6)
                    itm->TypeID = 2;
                else if (i < 9)
                    itm->TypeID = 0;
                else
                    itm->TypeID = 1;
                
                complexity = itm->Weight;
            }
            else
            {
                itm->InfoID = i - 12 + 34 + local_1c * 11;
                itm->Weight = ArmorWeaponInfo.at(itm->InfoID).unk1;
                itm->Concentration = itm->Weight;
                
                if (i < 15)
                    itm->TypeID = 3;
                else if (i < 19)
                    itm->TypeID = 5;
                else
                    itm->TypeID = 4;
                
                complexity = itm->Weight;
            }

            int32_t tmp = (1 + (float)complexity / sqrt(bsmith.KuznechnoeDelo));
            bsmith.KuznechnoeDelo = sqrt( (float)tmp / 24.0 +  (bsmith.KuznechnoeDelo * bsmith.KuznechnoeDelo) );
        }
    }
}

void Engine::UpdateVillageVoevoda(Village *vlg)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
}


}