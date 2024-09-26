#include "game.h"
#include "game_locale.h"

namespace Game {

    
int32_t Engine::FUN_0041aea0(int32_t val)
{   
    if (!PInteractChar)
        return val;

    float price = val * 0.5 * ((_mainCharacter->Trading - PInteractChar->Trading) * 0.002 + 1.0);
    
    if (DAT_00a3e76c > 0)
        price *= 0.6666667;
    
    if (Common::ABS(price) == 0.0)
        return 0;
    
    if (price < 1.0)
        price = 1.0;

    return price;
}
    
int32_t Engine::FUN_0041b65c(int32_t val)
{
    if (!PInteractChar)
        return val;
    
    float price = val * (1.0 - (_mainCharacter->Trading - PInteractChar->Trading) * 0.002);
    
    if (DAT_00a3e76c > 0)
        price *= 1.5;
    
    if (Common::ABS(price) == 0.0)
        return 0;
    
    if (price < 1.0)
        price = 1.0;

    return price;
}


bool Engine::TradeComplete(bool p)
{
    // weight without inventory
    int32_t weight = GetCurrentWeight(CharInfoCharacter, STORE_INVENTORY);

    int32_t numItem = 0;
    for (int32_t local_20 = 0; local_20 < INVSIZE; ++local_20)
    {
        int32_t itemId = int16_t_ARRAY_0083dc4c[0].at(local_20);
        if (itemId)
        {
            numItem++;
            weight += GetItemWeight(&_state.Items.at(itemId));
        }

        if (p)
        {
            itemId = int16_t_ARRAY_0083dc4c[3].at(local_20);
            if (itemId)
            {
                numItem++;
                weight += GetItemWeight(&_state.Items.at(itemId));
            }
        }
        else
        {
            itemId = int16_t_ARRAY_0083dc4c[1].at(local_20);
            if (itemId)
            {
                numItem++;
                weight += GetItemWeight(&_state.Items.at(itemId));
            }
        }
    }
    
    if (numItem > INVSIZE)
    {
        SetHintText(Locale::GameMessages[Locale::GMSG_INV_FULL]);
        return false;
    }
    
    if ((CharInfoCharacter->CurrentVinoslivost * 1000) / 3 + 10000 < weight)
    {
        SetHintText(Locale::GameMessages[Locale::GMSG_INV_OVERLOAD]);
        return false;
    }
    
    if (p)
    {
        if (PInteractChar)
        {
            if (DAT_00a3e82c + FUN_0041aea0(DAT_00a3e850) < DAT_00a3e830 + FUN_0041b65c(DAT_00a3e854))
            {
                SetHintText(Locale::GameMessages[Locale::GMSM_NOMONEY]);
                return 0;
            }
        }
        
        _mainCharacter->Gold += DAT_00a3e830 - DAT_00a3e82c;
        
        if (_mainCharacter->Gold < 0)
            _mainCharacter->Gold = 0;
    }
    
    CharInfoCharacter->Inventory.fill(0);
    
    numItem = 0;
    
    if (p)
    {
        for (int32_t i = 0; i < INVSIZE; ++i)
        {
            int32_t itemId = int16_t_ARRAY_0083dc4c[3].at(i);
            if (itemId)
            {
                CharInfoCharacter->Inventory[numItem] = itemId;
                numItem++;
            }
        }
    }
    else
    {
        for (int32_t i = 0; i < INVSIZE; ++i)
        {
            int32_t itemId = int16_t_ARRAY_0083dc4c[1].at(i);
            if (itemId)
            {
                CharInfoCharacter->Inventory[numItem] = itemId;
                numItem++;
            }
        }
    }
    
    for (int32_t i = 0; i < INVSIZE; ++i)
    {
        int32_t itemId = int16_t_ARRAY_0083dc4c[0].at(i);
        if (itemId)
        {
            CharInfoCharacter->Inventory[numItem] = itemId;
            numItem++;
        }
    }
    
    if (!PInteractChar)
    {
        if (p)
        {
            GS1 &local_2c = mapGS1.at(DAT_00a3e76c);
            local_2c.ItemID.fill(0);
            
            numItem = 0;
            for (int32_t i = 0; i < INVSIZE; ++i)
            {
                int32_t itemId = int16_t_ARRAY_0083dc4c[1].at(i);
                if (itemId)
                {
                    local_2c.ItemID[numItem] = itemId;
                    numItem++;
                    
                    if (numItem >= local_2c.ItemID.size())
                        break;
                }
                
                itemId = int16_t_ARRAY_0083dc4c[2].at(i);
                if (itemId != 0)
                {
                    local_2c.ItemID[numItem] = itemId;
                    numItem++;
                    
                    if (numItem >= local_2c.ItemID.size())
                        break;
                }
            }
            
            if (local_2c.ItemID[0] == 0)
            {
                FUN_0041093c(&local_2c);
                DAT_00a3e76c = 0;
            }
            else
                local_2c.ImgID = GetLootImage(local_2c);
        }
    }
    else
    {
        if (p)
        {
            _mainCharacter->Trading += (DAT_00a3e82c + FUN_0041aea0(DAT_00a3e850)) / 1024;
            
            if (_mainCharacter->Trading > 100)
                _mainCharacter->Trading = 100;
            
            PInteractChar->Trading += (DAT_00a3e82c + FUN_0041aea0(DAT_00a3e850)) / 1024;
            
            if (PInteractChar->Trading > 100)
                PInteractChar->Trading = 100;
        }
        
        int32_t vjob = GetVillageCharacterJob(PInteractChar);
        if (vjob == VJOB_NO)
        {
            if (p)
                PInteractChar->Gold += DAT_00a3e82c - DAT_00a3e830;
            
            PInteractChar->Inventory.fill(0);
            
            numItem = 0;
            for (int32_t i = 0; i < INVSIZE; ++i)
            {
                if (p)
                {
                    int32_t itemId = int16_t_ARRAY_0083dc4c[1].at(i);
                    if ( itemId )
                    {
                        PInteractChar->Inventory[numItem] = itemId;
                        numItem++;
                        if (numItem >= PInteractChar->Inventory.size())
                            break;
                    }
                }
                else
                {
                    int32_t itemId = int16_t_ARRAY_0083dc4c[3].at(i);
                    if ( itemId )
                    {
                        PInteractChar->Inventory[numItem] = itemId;
                        numItem++;
                        if (numItem >= PInteractChar->Inventory.size())
                            break;
                    }
                }
                
                int32_t itemId = int16_t_ARRAY_0083dc4c[2].at(i);
                if ( itemId )
                {
                    PInteractChar->Inventory[numItem] = itemId;
                    numItem++;
                    if (numItem >= PInteractChar->Inventory.size())
                        break;
                }
            }
        }
        else if (p)
        {
            if (vjob == VJOB_MEDIC)
            {
                for (int32_t i = 0; i < INVSIZE; ++i)
                {
                    int32_t itemId = int16_t_ARRAY_0083dc4c[3].at(i);
                    if (itemId)
                    {
                        for(int16_t &pitm : CurrentVillage->MedicItems)
                        {
                            if (pitm == itemId)
                                pitm = 0;
                        }
                    }
                }
            }
            else if (vjob == VJOB_TRADER)
            {
                for (int32_t i = 0; i < INVSIZE; ++i)
                {
                    int32_t itemId = int16_t_ARRAY_0083dc4c[3].at(i);
                    if (itemId)
                    {
                        for(int16_t &pitm : CurrentVillage->TraderItems)
                        {
                            if (pitm == itemId)
                                pitm = 0;
                        }
                    }
                }
            }
            else
            {
                for (int32_t i = 0; i < INVSIZE; ++i)
                {
                    int32_t itemId = int16_t_ARRAY_0083dc4c[3].at(i);
                    if (itemId)
                    {
                        for(int16_t &pitm : CurrentVillage->SmithItems)
                        {
                            if (pitm == itemId)
                                pitm = 0;
                        }
                    }
                }
            }
            
            for (int32_t local_20 = 0; local_20 < INVSIZE; ++local_20)
            {
                int32_t itemId = int16_t_ARRAY_0083dc4c[1].at(local_20);
                if (itemId != 0)
                    _state.Items[itemId].TypeID = -1;
            }
        }
    }
    return true;
}
    
}