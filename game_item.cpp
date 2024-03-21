#include "game.h"

namespace Game
{
    

int32_t Engine::GetItemWeight(const ItemInfo *itm) const
{
    if (itm->TypeID < 6)
        return ArmorWeaponInfo[itm->InfoID].unk6;
    else if (itm->TypeID < 9)
        return AcessoriesInfo[itm->InfoID].Weight;
    else if (itm->TypeID == 9)
        return AlchemiesInfo[itm->InfoID].Weight;
    else if (itm->TypeID == 11)
        return MiscItemsInfo[itm->InfoID].Weight;
    else if (itm->TypeID == 12)
        return ArmorWeaponInfo[itm->InfoID].unk6 * itm->Concentration;
    return 0;
}


}