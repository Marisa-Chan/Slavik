#include "game.h"



namespace Game {

int32_t Engine::GetLootImage(const GS1 &ploot) const
{
    if (ploot.unk3[0] !=0 || ploot.unk2 != 0) 
        return 163;
    
    const ItemInfo &pIVar6 = _state.Items.at(ploot.ItemID);
    if (pIVar6.TypeID < 6) 
        return ArmorWeaponInfo.at(pIVar6.InfoID).LootImgID;
    else if (pIVar6.TypeID < 9)
        return AcessoriesInfo.at(pIVar6.InfoID).LootImgID;
    else if (pIVar6.TypeID == 9) 
        return AlchemiesInfo.at(pIVar6.InfoID).LootImgID;
    else if (pIVar6.TypeID == 11) 
        return MiscItemsInfo.at(pIVar6.InfoID).LootImgID;
    else if (pIVar6.TypeID == 12) 
        return 153;
    
    return 163;
}

Engine::GS1* Engine::GetLootByTile(Common::Point tile)
{
    for(int32_t i = 0; i < mapGS1Count; ++i)
    {
        if (mapGS1.at(i).Tile == tile)
            return &mapGS1.at(i);
    }
    
    return nullptr;
}


};