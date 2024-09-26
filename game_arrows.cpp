#include "game.h"

namespace Game {
    
int32_t Engine::FUN_0041ace0(Character *pchr, MapObject2 *parrow)
{
    int32_t_00a3e828 = parrow->Poison;
    
    int32_t dmg = 0;
    if (parrow->unk4 >= 1 && SpecialDamages.at(parrow->unk4).ClassID == pchr->ClassID)
        dmg = SpecialDamages.at(parrow->unk4).Dmg * parrow->Dmg;
    else
        dmg = ArmorWeaponInfo.at(parrow->InfoID).Dmg * parrow->Dmg;

    Character &bowman = _state.Characters.at(parrow->CharID);
    if (bowman.CurrentUdr)
    {
        if ((bowman.Flags & 2) == 0)
            dmg = bowman.CurrentUdr;
        else
            dmg += bowman.CurrentUdr;
    }
    return dmg;
}


int32_t Engine::FUN_0041c29c(Character *pchr, int32_t arrowDmg)
{
    FUN_0041c750(pchr);
    
    if ((System::rand() % 101) <= pchr->CurrentLovkost * 0.5)
        return -2;
    
    int32_t armr = GetArmor(pchr);
    float tmp = (float)arrowDmg * 0.7 - (float)armr;
    
    if (tmp < 0.0)
        tmp = 0.0;
    
    int32_t hitDmg = (1 + (tmp + (float)arrowDmg * 0.3) / (float)pchr->CurrentVinoslivost) * 16.0;
            
    int32_t res = 0;
    
    if (hitDmg < pchr->HP)
    {
        pchr->HP -= hitDmg;
        
        FUN_0041c750(pchr);
        
        if (hitDmg < 48)
            res = -1;
    }
    else
        res = 1;
    
    tmp = 1.0;
    
    if (armr)
    {
        tmp = (float)arrowDmg / (float)armr;
        if (tmp > 1.0)
            tmp = 1.0;
    }
    
    pchr->Otravlenie += int32_t_00a3e828;
    
    for (int32_t slotId = ESLT_3; slotId <= ESLT_5; ++slotId)
    {
        int32_t itemid = pchr->ArmorWeapons.at(slotId);
        if (itemid)
        {
            ItemInfo &itm = _state.Items.at(itemid);
            itm.Concentration -= tmp * ArmorWeaponInfo.at(itm.InfoID).Dmg * 0.001;
            
            if (itm.Concentration <= 0.0)
            {
                itm.TypeID = -1;
                pchr->ArmorWeapons.at(slotId) = 0;
            }
        }
    }
    
    return res;
}


    
    
}