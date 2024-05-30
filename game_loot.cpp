#include "game.h"



namespace Game {

int32_t Engine::GetLootImage(const GS1 &ploot) const
{
    if (ploot.ItemID[1] !=0 || ploot.unk2 != 0) 
        return 163;
    
    const ItemInfo &pIVar6 = _state.Items.at(ploot.ItemID[0]);
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

void Engine::FUN_004170a0(GS1 *loot)
{    
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    /*DAT_00a3e76c = loot->Index;
    DAT_00a3e7c8 = _mainCharacter->Gold;
    DAT_00a3e830 = 0;
    DAT_00a3e82c = 0;
    DAT_00a3e7cc = 0;
    PInteractChar = 0;
    memset(&DAT_0083dc4c,0,0x100);
    memcpy((undefined4 *)&DAT_0083dc4c,(undefined4 *)Game::CharInfoCharacter->field39_0x72,0x40);
    memcpy((undefined4 *)(int16_t_ARRAY_0083dcca + 1),(undefined4 *)&loot->ItemID,0x2c);
    memset(&DAT_0083dd4c,0,8);
    FUN_00431d70(0);*/
}


void Engine::FUN_0041093c(GS1 *loot)
{
    _currentMap->FootMap.At(loot->Tile).Flags &= ~2;

    if (loot->LootID > -1)
    {
        GS1 &GLoot = _state.GS1ARRAY[ loot->LootID ];
        
        ItemInfo *ditm = nullptr;
        if (GLoot.ItemID[0] != 0 && GLoot.ItemID[1] == 0)
        {
            ItemInfo &itm = _state.Items.at(GLoot.ItemID[0]);
            
            if (itm.TypeID == 9 &&
                (itm.InfoID == 1 || itm.InfoID == 2) )
                ditm = AllocItem();
        }
        
        if (ditm)
        {
            GLoot.Pos.y = -_counter;
            GLoot.Pos.x = -_counter;
            
            *ditm = _state.Items.at( GLoot.ItemID[0] );
            
            GLoot.ItemID[0] = ditm->Index;
        }
        else
        {
            GLoot.MapID = 0;
            
            while (_state.GS1ARRAYCount != 0 && 
                   _state.GS1ARRAY[_state.GS1ARRAYCount - 1].MapID == 0)
                _state.GS1ARRAYCount--;
        }
    }
    mapGS1Count--;
    
    for(int32_t i = loot->Index; i < mapGS1Count; ++i)
        mapGS1[i] = mapGS1[i + 1];
}


bool Engine::FUN_004138c8(Common::Point tile, int32_t itemId)
{
    int32_t itmSlot = 0;
    GS1 *gs = nullptr;
    
    if (_currentMap->FootMap.At(tile).Flags & 2)
        gs = GetLootByTile(tile);
    
    if (gs)
    {
        for (itmSlot = 0; itmSlot < 20; ++itmSlot)
        {
            if ( gs->ItemID[itmSlot] != 0 )
                break;
        }
        
        if (itmSlot == 20)
            return false;
    }
    else
    {
        if (mapGS1Count >= mapGS1.size())
            return false;
        
        itmSlot = 0;
        gs = &mapGS1.at(mapGS1Count);
        
        mapGS1Count++;
        
        *gs = GS1();
        gs->Tile = tile;
        gs->LootID = -1;
        
        _currentMap->FootMap.At(tile).Flags |= 2;
    }
    
    gs->ItemID[itmSlot] = itemId;
    gs->ImgID = GetLootImage(*gs);
    gs->Pos = FUN_00439ba0(gs->Tile) - _menuImages.at(gs->ImgID)->GetSize() / 2;
    return true;
}



};