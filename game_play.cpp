#include <set>
#include <complex>

#include "system.h"
#include "game.h"
#include "fmt/core.h"
#include "fmt/printf.h"
#include "game_locale.h"

namespace Game
{
    
bool Engine::LoadINTR()
{
    DWORD_00a3e74c |= 1;
    
    _textQueueCount = 0;
    _imgQueue2Count = 0;
    _imgQueue1Count = 0;
    
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    
    std::string tmp = fmt::format("INTR{:d}{:s}.RES", SCREENRESX, _langLiter);
    
    FSMgr::File f = FSMgr::Mgr::ReadFile(tmp);
    if (!f)
    {
        System::PostQuitMessage();
        fmt::printf("Game res file %s not found\n", tmp);
        return false;
    }
    
    FreeMenuImages();
    
    f->seek(4, 0);
    uint32_t dataSize = f->readU32L();
    
    std::vector<int32_t> sprHdr(512 * 2);
        
    for (int32_t i = 0; i < 512; ++i)
    {
        sprHdr[i * 2] = f->readS32L();
        sprHdr[i * 2 + 1] = f->readS32L() / 2;// / 512;//2;
    }
    
    size_t pos = f->tell();
    
    for (int32_t i = 0; i < 512; ++i)
    {
        if (sprHdr[i * 2] == -1)
            break;
        
        f->seek(pos + sprHdr[i * 2], 0);
        _menuImages.push_back( Resources::LoadRL8BitImage(f.get(), Res.Palettes.data() + sprHdr[i * 2 + 1]) );
    }
    
    ItemInvSlotWidth = _menuImages[17]->SW->w - 1;
    ScrlBtnWidth = _menuImages[18]->SW->w;
    ScrlBtnHeight = _menuImages[18]->SW->h;
    int32_t iVar1 = (SCREENRESX - 1 - GScrOff.x) + 2 - ScrlBtnWidth * 2;
    DAT_00a3e870 = MSGRECT.left - 1 + ScrlBtnWidth + (iVar1 - (iVar1 / ItemInvSlotWidth) * ItemInvSlotWidth) / 2;
    DAT_00a3e88c = (MSGRECT.top - ScrlBtnHeight) + 1;
    
    return true;
}

    
void Engine::UpdateGame()
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    
    if (_mainCharacter->ClassID & CLASS_BIT80) 
    {
        _nextStateMode = STATEMD_MODE7;
        _isGameStarted = false;
        StartPlayMovie("GAMEOVER.FLM");
        return;
    }
    
    
    if ( (System::rand() % 100) > 98 )
        PlaySound( 0x100 + (System::rand() % 8) + (_currentMapID - 1) * 8,
                   0, 0, 0);
    
    ProcessCamera();
    
    Common::Point ppoint(_mousePos.x * SCREENRESX / _screenSize.x, _mousePos.y * SCREENRESY / _screenSize.y);
    
    if ( MAPRECT.IsIn( ppoint ) )
        _mouseMapPos = _mousePos - (MAPRECT.Pos() * _screenSize / SCREENRES);
        
    //Draw gameplay UI
    ImgQueue1(_menuImages[3], Common::Point(), Common::Rect());
    
    if (_nextMapID > 0)
    {
        if (_currentMapID == _nextMapID) 
        {
            _nextMapID = 0;
            FUN_004118dc();
        }
        else 
        {
            FUN_00436ad0(_currentMapID);
            FUN_00436a00();
            
            if (!LoadMap(_nextMapID, 0)) 
            {
                ShutdownWithMsg("Map not load!");
                return;
            }
            
            LoadUsedObjects();
        }
        
        FUN_00428f90( _mainCharacter->ViewPos );
        FUN_004290d8();
        //PlayChangeScreen(PLSCREEN_0);
    }
    
    

        _counter++;
        FUN_00421170();
        FUN_00421da4();
        MapObjectsUpdateFrames();
        FUN_0041d0fc();
        
        for (int32_t cnt = 0; cnt < _speed; cnt++)
        {
            _counter++;
            FUN_00421170();
            FUN_00421da4();
            MapObjectsUpdateFrames();
            FUN_0041d0fc();
            
            if (_isPlayingVideo || (_mainCharacter->ClassID & 0x80)) break;
        }
        
        if (_objectsToLoadCount > 0)
            LoadUsedObjects();
        
        if (DAT_00a3e814 > 0)
            FUN_0043d1d0();
        
        if (DWORD_00a3e74c & 4)
        {
            DWORD_00a3e74c ^= 4;
            FUN_0042f50c(CharInfoCharacter,0);
        }
        else if (DWORD_00a3e74c & 8)
        {
            DWORD_00a3e74c &= ~8;
            //if (_playScreenID == PLSCREEN_7)
                //DrawTrade(0);
        }
        
        
        if (DWORD_00a3e74c & 1)
        {
            DWORD_00a3e74c &= ~1;
            
            if (_KeyPressed != -1)
                PlayHandleKey(_KeyPressed);
            
            PlayProcessMouse();
            
            if ((DWORD_00a3e74c & 1) == 0)
                FUN_004290d8();
        }
        else
        {
            if (_KeyPressed != -1) 
                PlayHandleKey(_KeyPressed);
            
            PlayProcessMouse();
        }
    
    if (MsgTextTimeout)
    {
        MsgTextTimeout--;
        
        Common::Point outPos(MSGRECT.left + 10, MSGRECT.top + 5);
        TextQueue(MsgText, _Fonts[3], outPos, MSGRECT);
    }
        
    DrawElmQueue();
    for(int32_t i = 0; i < _mainMapChar->GroupSize; ++i)
        FUN_0042d574(&_state.Characters.at(_mainMapChar->CharacterIndex + i), i);
}

void Engine::FUN_004118dc()
{
    //printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    
    if (_mainMapChar->MapID == _currentMapID) 
    {
        for (int i = 0; i < _mainMapChar->GroupSize; ++i) 
        {
            Character &pchar = _state.Characters.at( _mainMapChar->CharacterIndex + i );
            if (((pchar.ClassID & CLASS_BIT80) == 0) && (pchar.State != CHSTATE_9) && (pchar.State != CHSTATE_3)) 
                _currentMap->FootMap(pchar.Tile).ID = 0;
        }
    }
    else
        _mainMapChar->MapID = _currentMapID;
    
    int charIndex = 0;
    Common::Rect spwnRect = _mainMapChar->spwnTlRect;

    //Place main character
    if ( spwnRect.top > 0 && spwnRect.top < 319 && 
         spwnRect.left > 0 && spwnRect.left < 159 &&
         _currentMap->FootMap(spwnRect.Pos()).ID == 0 )
    {
        _mainCharacter->Direction = spwnRect.bottom;
        
        _mainCharacter->MoveTile = spwnRect.Pos();
        _mainCharacter->Tile = spwnRect.Pos();
        
        _mainCharacter->field2_0x2 &= 0xf0;
        _mainCharacter->field17_0x13[0] = 0xff;
        
        _currentMap->FootMap(_mainCharacter->Tile).ID = _mainCharacter->Index + 1;
        
        if ((_mainCharacter->field_0x3 & 4) == 0)
            FUN_004143dc(_mainCharacter, CHSTATE_6);
        else
            FUN_004143dc(_mainCharacter, CHSTATE_0);
        
        charIndex = 1;
    }
    
    // Find place for characters
    int uVar5 = _mainMapChar->spwnTlRect.top & 1;
    for (; charIndex < _mainMapChar->GroupSize; ++charIndex) 
    {
        Character &pchar = _state.Characters.at( _mainMapChar->CharacterIndex + charIndex );
        if (((pchar.ClassID & CLASS_BIT80) == 0) && (pchar.State != CHSTATE_9) && (pchar.State != CHSTATE_3))
        {
            int ldir = 0;
            if (_mainCharacter->Direction < DIR_4)
                ldir = _mainCharacter->Direction + 4;
            else 
                ldir = _mainCharacter->Direction - 4;
            
            int local_1c = ldir;
            int local_20 = ldir - 1;
            
            if (local_20 < 0)
                local_20 = 7;
            
            if (rand() % 2 != 0)
                local_1c = local_20;
            
            for (int i = 0; i < 8; ++i) 
            {
                int local_28 = rand() % 6;
                for (int j = 0; j < 6; ++j) 
                {
                    Common::Point ppos = spwnRect.Pos() + PlaceDOff[uVar5][local_1c][local_28];
                    
                    if ((ppos.y > 0) && (ppos.y < 319) &&
                        (ppos.x > 0) && (ppos.x < 159) && 
                        _currentMap->FootMap(ppos).ID == 0 )
                    {
                        pchar.Direction = spwnRect.bottom;
                        pchar.MoveTile = ppos;
                        pchar.Tile = ppos;
                        pchar.field2_0x2 &= 0xf0;
                        pchar.field17_0x13[0] = 0xff;
                        
                        _currentMap->FootMap.At(pchar.Tile).ID = pchar.Index + 1;
                        pchar.State = 0xff;
                        
                        FUN_00414ab4(&pchar);
                        
                        i = 8;
                        j = 12;
                    }
                    
                    local_28 += 1;
                    
                    if (local_28 == 6)
                        local_28 = 0;
                }
                
                local_1c += 1;
                
                if (local_1c == 8)
                    local_1c = 0;
            }
        }
    }
}

void Engine::FUN_00414ab4(Character *pchar)
{
    int nextState = CHSTATE_0;
    
    pchar->field2_0x2 |= 0x40;
    
    if ( (pchar != _mainCharacter) && (pchar->MapCharID == _mainCharacter->MapCharID) && ((pchar->field2_0x2 & 0xf) == 0))
        pchar->field2_0x2 |= 0x20;
    
    if ((pchar->field_0x3 & 4) == 0)
        nextState = CHSTATE_6;
    else
        nextState = CHSTATE_0;
    
    if (pchar->State != nextState)
        FUN_004143dc(pchar, nextState);
}

void Engine::FUN_00436ad0(int32_t mapid)
{
    Village *vlg = FUN_0043a1f8(mapid);
    for(int32_t i = 0; i < _state.MapChar_ARRAY.size(); ++i)
    {
        MapChar &mchr = _state.MapChar_ARRAY.at(i);

        if (mchr.MapID == mapid)
        {
            
            for (int32_t j = 0; j < mchr.GroupSize; ++j)
            {
                Character &chr = _state.Characters.at(mchr.CharacterIndex + j);
                
                if ((chr.ClassID & CLASS_BIT80) != 0)
                {
                    FUN_00422778(&chr);
                    
                    if (vlg && vlg->ChiefCharId == i)
                    {
                        for (int32_t k = 0; k < vlg->Jobs.size(); ++k)
                        {
                            if (vlg->Jobs[k].CharID == chr.Index)
                                vlg->Jobs[k].CharID = 0;
                            else if (chr.Index < vlg->Jobs[k].CharID)
                                vlg->Jobs[k].CharID--;
                        }
                    }
                    
                    mchr.GroupSize--;
                    
                    for(int32_t k = j; k < mchr.GroupSize; ++k)
                        _state.Characters.at(mchr.CharacterIndex + k).CopyDataFrom( _state.Characters.at(mchr.CharacterIndex + k + 1));

                    j--;
                }
            }
            
            if (vlg && vlg->ChiefCharId == i && vlg->Jobs[0].CharID == 0)
            {
                for (int32_t j = 0; j < mchr.GroupSize; ++j)
                {
                    Character &chr = _state.Characters.at(mchr.CharacterIndex + j);
                    if (chr.CharacterBase < 3 && (chr.ClassID & 0x38) == 0 && GetVillageCharacterJob(&chr) == 0)
                    {
                        vlg->Jobs[0].CharID = chr.Index;
                        chr.field96_0xd0 = vlg->Jobs[0].unk;
                        chr.field2_0x2 = 0;
                        FUN_00414ab4(&chr);
                        break;
                    }
                }
            }
        }
    }
}

void Engine::FUN_00436a00()
{
    for (int32_t i = 0; i < mapGS1Count; i += 1)
    {
        GS1 &mapLoot = mapGS1.at(i);
        if (mapLoot.LootID < 0)
        {
            for (int16_t itemId : mapLoot.ItemID)
            {
                if (itemId)
                    _state.Items.at(itemId).TypeID = -1;
            }
        }
        else
        {
            GS1 &gloot = _state.GS1ARRAY.at(mapLoot.LootID);
            gloot.ImgID = mapLoot.ImgID;
            gloot.ItemID = mapLoot.ItemID;
        }
    }
}


void Engine::FUN_00421170()
{
    for(int32_t i = 0; i < MapObjects2Count; ++i)
    {
        MapObject2 &arrow = MapObjects2.at(i);
        arrow.unk4--;
        
        if (arrow.unk4 <= 0)
        {
            MapObjects2Count--;
            
            for(int32_t j = i; j < MapObjects2Count; ++j)
                MapObjects2.at(j) = MapObjects2.at(j + 1);
                
            i--;
        }
        else
        {
            Character &pCVar16 = _state.Characters.at(arrow.CharID);
            
            GFX::Image *img = nullptr;
            Common::Point tracept;
            int32_t bldId = -1;
            
            if (pCVar16.EnemyCharID < 0)
            {
                bldId = -pCVar16.EnemyCharID - 1;
                if (!CurrentVillage || (_currentMap->MapObjects.at(bldId).unk3 & 1) == 0)
                {
                    pCVar16.EnemyCharID = 0;
                }
                else
                {
                    GameMap::Object &obj = _currentMap->MapObjects.at(bldId);
                    Resources::SimpleObject &pSimple = Res.SimpleObjects.at( obj.ObjId );
                    
                    img = pSimple.Images.at(obj.CurrentFrame);
                    tracept = obj.Pos + pSimple.Offsets.at(obj.CurrentFrame).Offset;
                }
            }
            
            for (int32_t j = 0; j < 8; ++j)
            {
                _tracePos = arrow.p2 + Common::Point(0, 30);
                Common::Point pt = FUN_00439bdc(_tracePos); //(int *)&local_38,&local_34,
                GameMap::Cell &cell = _currentMap->FootMap.At(pt);
                if (cell.Flags & 0xC)
                {
                    
                    if (pCVar16.EnemyCharID < 0 && CheckTraceImage(img, tracept) != 0)
                    {
                        for(int32_t k = 0; k < CurrentVillage->Bld2Num + CurrentVillage->Bld1Num + 7; ++k)
                        {
                            Village::BldState &bst = CurrentVillage->BuildingState.at(k);
                            if (bst.ObjID == bldId)
                            {
                                if (rand() % 100 <= 20)
                                {
                                    bst.State = 3;
                                    bst.BldLeft = 1;
                                }
                                CurrentVillage->Flags |= 0x15;
                                FUN_004248a0(&_state.MapChar_ARRAY.at(CurrentVillage->ChiefCharId));
                                break;
                            }
                        }
                    }
                    
                    MapObjects2Count--;
                    for(int32_t k = i; k < MapObjects2Count; ++k)
                        MapObjects2.at(k) = MapObjects2.at(k + 1);
                    i--;
                    
                    break;
                }
                else if (cell.ID != 0 && cell.ID != 0xfff && cell.ID - 1 != arrow.CharID)
                {
                    Character &cchr = _state.Characters.at(cell.ID - 1);
                    
                    if (cchr.MapCharID != _mainCharacter->MapCharID)
                    {
                        if ((_state.MapChar_ARRAY.at(cchr.MapCharID).unk5 & 0x80) != 0)
                            CurrentVillage->Flags |= 0x15;
                        
                        FUN_004248a0(&_state.MapChar_ARRAY.at(cchr.MapCharID));
                    }
                    
                    if (cchr.State != CHSTATE_9 && cchr.State != CHSTATE_3)
                    {
                        int32_t arrowDmg = FUN_0041ace0(&cchr, &arrow);
                        int32_t retres = FUN_0041c29c(&cchr, arrowDmg);
                        if (retres == 1)
                        {
                            cchr.HP = 0;
                            cchr.field2_0x2 = 0;
                            
                            int32_t local_1c = FUN_0041b70c(&cchr);
                            
                            if ((cchr.field_0x3 & 4) == 0)
                                FUN_004143dc(&cchr, CHSTATE_9);
                            else
                                FUN_004143dc(&cchr, CHSTATE_3);
                            
                            FUN_0041079c(&pCVar16, local_1c / 4);
                            
                            pCVar16.Metkost += local_1c;
                            FUN_00418510(&pCVar16, local_1c, pCVar16.Level);
                        }
                        else if (retres == 0)
                        {
                            if ((cchr.field_0x3 & 4) == 0)
                                FUN_004143dc(&pCVar16, CHSTATE_8);
                            else 
                                FUN_004143dc(&pCVar16, CHSTATE_2);
                        }
                        //if (cchr.MapCharID == _mainCharacter->MapCharID)
                        //    FUN_0042d574(&pCVar16);
                        
                        MapObjects2Count--;
                        for(int32_t k = i; k < MapObjects2Count; ++k)
                            MapObjects2.at(k) = MapObjects2.at(k + 1);
                        i--;
                        break;
                    }
                }
                
                arrow.p2 += arrow.p1;
            }
        }
    }
}



bool Engine::FUN_0041f258(Character *pchar)
{
    if (pchar->State == CHSTATE_5)
    {
        if (FUN_00411758(pchar, pchar->MoveTile))
            return false;
        
        if (pchar->EnemyCharID == 0)
        {
            //Seems original bug
            printf("Why EnemyCharID == 0?\n");
            return false;
        }
        
        Character &ch = _state.Characters.at(pchar->EnemyCharID - 1);
        if ((ch.ClassID & CLASS_BIT80) || ch.State == CHSTATE_9 || ch.State != CHSTATE_3)
            pchar->EnemyCharID = 0;
        
        FUN_00414ab4(pchar);
        return true;
    }
    
    if ((pchar->field2_0x2 & 1) == 0 || (pchar->State != CHSTATE_7 && pchar->State != CHSTATE_1) || pchar->EnemyCharID == 0)
        return false;

    Character &ch = _state.Characters.at(pchar->EnemyCharID - 1);
    
    if ((ch.ClassID & CLASS_BIT80) || ch.State == CHSTATE_9 || ch.State == CHSTATE_3)
    {
        pchar->EnemyCharID = 0;
        FUN_00414ab4(pchar);
        return true;
    }

    if (pchar->MoveTile == ch.Tile)
        return false;
    
    int dir = FUN_0041165c(ch.Tile, pchar->MoveTile);
    if (dir > -1)
    {
        int32_t maxwp = 0;
        for (maxwp = 0; maxwp < pchar->field17_0x13.size(); ++maxwp)
        {
            if (pchar->field17_0x13[maxwp] == 0xff)
                break;
        }
        
        if (maxwp != 0)
        {
            maxwp--;
            
            Common::Point pt;
            if (pchar->field17_0x13[maxwp] < 4)
                FUN_0043ecba(&pt, pchar->MoveTile, pchar->field17_0x13[maxwp] + 4);
            else
                FUN_0043ecba(&pt, pchar->MoveTile, pchar->field17_0x13[maxwp] - 4);
            
            int dir2 = FUN_0041165c(ch.Tile, pt);
            if (dir2 > -1)
            {
                pchar->MoveTile = ch.Tile;
                pchar->field17_0x13[maxwp] = dir2;
                return false;
            }
            
            uint8_t pgo = PathGo[dir][pchar->field17_0x13[maxwp]];
            if (pgo != 0xff)
            {
                pchar->MoveTile = ch.Tile;
                
                if ((pgo & 0x80) == 0)
                    pchar->field17_0x13[maxwp] = pgo;
                else
                    pchar->field17_0x13[maxwp] = 0xff;
                
                return false;
            }
        }
    }
    
    FUN_00414ab4(pchar);
    return true;
}


void Engine::FUN_00421bb8(Character *pchar)
{
    if (pchar == _mainCharacter ||  _mainCharacter->MoveTile == pchar->Tile)
    {
        for (int32_t local_14 = 0; local_14 < _state.GS2ARRAYCount; ++local_14)
        {
            GS2 &g = _state.GS2ARRAY.at(local_14);
            
            if (g.MapID == _mainMapChar->MapID && 
                pchar->Tile.y >= g.WarpZone.top && 
                pchar->Tile.y <= g.WarpZone.bottom &&
                pchar->Tile.x >= g.WarpZone.left &&
                pchar->Tile.x <= g.WarpZone.right)
            {
                if (CurrentVillage && CurrentVillage->Index == local_14 &&
                        CurrentVillage->BuildingState[5].BuildInfoID > -1 &&
                        CurrentVillage->BuildingState[5].State != 3)
                    return;
                
                //_objectsToLoadCount = -g.field4_0x4;
                _nextMapID = g.TargetMapID;
                _mainMapChar->spwnTlRect.top = g.field5_0x5;
                _mainMapChar->spwnTlRect.left = g.field6_0x7;
                _mainMapChar->spwnTlRect.bottom = g.field2_0x2;
                return;
            }
        }
    }
}


void Engine::FUN_004110ec(Character *pchar)
{
    if (_playScreenID == PLSCREEN_3 && pchar == CharInfoCharacter)
        return;
    
    GS1 *loot = GetLootByTile(pchar->Tile);
    if (!loot)
        return;
    
    if (loot->LootID == 0)
    {
        _nextStateMode = STATEMD_MODE7;
        StartPlayMovie("WIN.FLM");
        return;
    }

    int32_t charWeight = GetCurrentWeight(pchar);

    _mainCharacter->Gold += Common::ABS(loot->unk2);
    
    int32_t invUsed = 0;
    for (invUsed = 0; invUsed < pchar->Inventory.size(); ++invUsed)
    {
        if (pchar->Inventory[invUsed] == 0)
            break;
    }
    
    loot->unk2 = 0;
    
    while (loot->ItemID[0] != 0)
    {
        charWeight += GetItemWeight( &_state.Items.at(loot->ItemID[0]) );
        
        if (pchar->CurrentVinoslivost * 1000 / 3 + 10000 < charWeight || invUsed == pchar->Inventory.size())
        {
            CharInfoCharacter = pchar;
            FUN_004170a0(loot);
            
            if (invUsed == pchar->Inventory.size())
                FUN_0042f9b8(Locale::MiscItemsInfoNames[Locale::GMSG_INV_FULL]);
            else
                FUN_0042f9b8(Locale::MiscItemsInfoNames[Locale::GMSG_INV_OVERLOAD]);
            
            return;
        }
        
        invUsed++;
        
        //Move items in inventory to empty slot 0
        for (int32_t i = pchar->Inventory.size() - 2; i > -1; --i)
            pchar->Inventory[i + 1] = pchar->Inventory[i];
        
        //Set first item in inventory from loot bag
        pchar->Inventory[0] = loot->ItemID[0];
        
        //"Pop" first item from loot bag
        for(int32_t i = 0; i < loot->ItemID.size() - 1; ++i)
            loot->ItemID[i] = loot->ItemID[i + 1];
        
        //Set last item in loot bag to 0
        loot->ItemID.back() = 0;
    }
    
    FUN_0041093c(loot);
    
    if (pchar->Index +1 == DisplayInvOfCharID)
        FUN_0042f50c(pchar, 0);
}

void Engine::FUN_00439230(Character *pchar)
{
    if (pchar->Index + 1 == DisplayInvOfCharID)
    {
        DisplayInvOfCharID2 = 0;
        DisplayInvOfCharID = 0;
    }
    
    for(int32_t i = 0; i < SelectedCharacters.size(); ++i)
    {
        if (SelectedCharacters[i] == nullptr)
            return;
        
        if (pchar == SelectedCharacters[i])
        {
            for(int32_t j = i; j < SelectedCharacters.size() - 1; ++j)
                SelectedCharacters[j] = SelectedCharacters[j + 1];
            
            SelectedCharacters.back() = NULL;
            return;
        }
    }
}
        
void Engine::FUN_00421da4()
{
    HardComputeCount = 0;
    Character *PrevComputeUnit = HardComputeUnit;
    
    for (MapChar &mchr : _state.MapChar_ARRAY)
    {
        if (!mchr.unk5 || mchr.MapID != _currentMapID)
            continue;
        
        if (mchr.unk5 & 0x40)
        {
            FUN_004123bc(&mchr);
            continue;
        }
        
        if (mchr.unk5 & 0x80)
        {
            FUN_00416058(&mchr);
            continue;
        }
        
        for(int32_t i = 0; i < mchr.GroupSize; ++i)
        {
            Character &chr = _state.Characters.at(mchr.CharacterIndex + i);
            switch(chr.State)
            {
                case CHSTATE_0:
                case CHSTATE_6:
                {
                    if (!FUN_00413de4(&chr))
                        FUN_004143dc(&chr, chr.State);
                    
                    if ((chr.field2_0x2 & 0x40) && SkipBecauseOfHardCompute(&chr)) 
                    {
                        HardComputeUnit = &chr;
                        
                        if ((chr.field2_0x2 & 0xf) == 0) 
                        {
                            if (FUN_0041f11c(&mchr, &chr) && SkipBecauseOfHardCompute(&chr))
                                FUN_0041ed8c(&chr);
                        }
                        else
                            FUN_0041ed8c(&chr);
                    }
                }
                    break;
                
                case CHSTATE_1:
                case CHSTATE_7:
                {
                    if (chr.field17_0x13[0] == 0xff)
                    {
                        if (SkipBecauseOfHardCompute(&chr))
                        {
                            HardComputeUnit = &chr;
                            FUN_0041ed8c(&chr);
                        }
                    }
                    else if (chr.Direction == chr.field17_0x13[0])
                    {
                        if (!FUN_00413de4(&chr))
                        {
                            chr.POS = FUN_00439ba0(chr.Tile);
                            
                            for(int32_t i = 0; i < chr.field17_0x13.size() - 1; ++i)
                                chr.field17_0x13[i] = chr.field17_0x13[i + 1];
                            chr.field17_0x13.back() = 0xff;

                            if (!FUN_0041f258(&chr))
                            {
                                if (chr.field17_0x13[0] == 0xff)
                                {
                                    if (chr.Tile == chr.MoveTile)
                                    {
                                        if ((chr.field_0x3 & 0x40) == 0)
                                        {
                                            if (_currentMap->FootMap.At(chr.Tile).Flags & 1)
                                                FUN_00421bb8(&chr);
                                        }
                                        else
                                        {
                                            chr.field_0x3 &= ~0x40;
                                            if ((_currentMap->FootMap.At(chr.Tile).Flags & 2) &&
                                                (GetLootByTile(chr.Tile)->unk2 > -1 ||
                                                 FUN_00434c90(25)))
                                                FUN_004110ec(&chr);
                                        }
                                        
                                        chr.field2_0x2 &= 0xf7;
                                        FUN_00414ab4(&chr);
                                    }
                                    else
                                    {
                                        chr.field2_0x2 |= 0x40;
                                        chr.Frame = 0xff;
                                        FUN_00413de4(&chr);
                                    }
                                }
                                else
                                    FUN_004143dc(&chr, chr.State);
                            }
                        }
                    }
                    else
                    {
                        FUN_004391bc(&chr, chr.field17_0x13[0]);
                    }
                }
                break;
                    
                case CHSTATE_2:
                case CHSTATE_8:
                {
                    //if (chr.Frame == 0 && chr.MapCharID == _mainCharacter->MapCharID)
                    //    FUN_0042d574(&chr);
                    
                    if (!FUN_00413de4(&chr))
                    {
                        FUN_00414ab4(&chr);
                        //if (chr.MapCharID == _mainCharacter->MapCharID)
                        //    FUN_0042d574(&chr);
                    }
                }
                break;
                
                case CHSTATE_3:
                case CHSTATE_9:
                {
                    if (chr.Frame != chr.FrameCount - 1)
                    {
                        if (chr.Frame == chr.FrameCount - 2)
                        {
                            chr.field2_0x2 = 0;
                            chr.ClassID |= CLASS_BIT80;
                            _currentMap->FootMap.At(chr.Tile).ID = 0;
                            
                            if (chr.MapCharID == _mainCharacter->MapCharID)
                            {
                                FUN_00439230(&chr);
                                //FUN_0042d574(&chr);
                            }
                        }
                        FUN_00413de4(&chr);
                    }
                }
                break;
                
                case CHSTATE_4:
                {
                    int32_t dir = chr.Direction;
                    
                    if (chr.Frame == 0)
                        dir = FUN_00439aa4(chr.MoveTile, chr.Tile);

                    if (chr.Direction != dir)
                    {
                        FUN_004391bc(&chr, dir);
                        continue;
                    }
                    
                    if (chr.Frame == chr.FrameCount - 6 && FUN_0041bff8(&chr))
                    {
                        ItemInfo &arrows = _state.Items.at(chr.Arrows);
                        arrows.Concentration--;

                        if (arrows.Concentration == 0)
                        {
                            chr.Arrows = 0;
                            arrows.TypeID = -1;
                        }
                    }
                    if (!FUN_00413de4(&chr))
                        FUN_00414ab4(&chr);
                }
                break;
                
                case CHSTATE_5:
                {
                    if (FUN_0041f258(&chr))
                        continue;
                    
                    int32_t dir = FUN_0043976c(chr.MoveTile, chr.Tile);
                    if (chr.Direction != dir)
                    {
                        FUN_004391bc(&chr, dir);
                        continue;
                    }
                    
                    if (chr.Frame == 2)
                    {
                        MapChar &mch = _state.MapChar_ARRAY.at( _state.Characters.at(chr.EnemyCharID - 1).MapCharID );
                        
                        if (mch.unk5 & 0x80)
                            CurrentVillage->Flags |= 0x15;
                        
                        FUN_004248a0(&mch);
                    }
                    
                    if (FUN_00413de4(&chr))
                        continue;
                    
                    Character &ch1 = _state.Characters.at(chr.EnemyCharID - 1);
                    if ((ch1.ClassID & CLASS_BIT80) || ch1.State == CHSTATE_9 || ch1.State == CHSTATE_3)
                    {
                        chr.EnemyCharID = 0;
                        FUN_00414ab4(&chr);
                        continue;
                    }
                    
                    int32_t val = FUN_0041b100(&ch1, &chr);
                    if (val)
                    {
                        val = FUN_0041c480(&ch1, &chr, val);
                        if (val == -2)
                        {
                            PlaySound(6, ComputeVolume(ch1.Tile), ComputePan(ch1.Tile), 0);
                        }
                        else if (val == 1)
                        {
                            chr.EnemyCharID = 0;
                            ch1.HP = 0;
                            ch1.field2_0x2 = 0;
                            
                            val = FUN_0041b70c(&ch1);
                            if (ch1.field_0x3 & 4)
                                FUN_004143dc(&ch1, CHSTATE_3);
                            else
                                FUN_004143dc(&ch1, CHSTATE_9);
                            
                            FUN_0041079c(&chr, val / 4);
                            chr.Fehtovanie += val;
                            chr.field2_0x2 &= 0xfd;
                            FUN_00418510(&ch1, val, chr.Level);
                            FUN_00414ab4(&chr);
                            continue;
                        }
                        else if (val == 0)
                        {
                            if (ch1.field_0x3 & 4)
                                FUN_004143dc(&ch1, CHSTATE_2);
                            else
                                FUN_004143dc(&ch1, CHSTATE_8);
                        }
                        
                        //if (ch1.MapCharID == _mainCharacter->MapCharID)
                        //    FUN_0042d574(&ch1);
                    }
                    FUN_004143dc(&chr, chr.State);
                }
                break;
            }
        }
    }
    
    if (HardComputeUnit == PrevComputeUnit)
        HardComputeUnit = nullptr;
}

void Engine::MapObjectsUpdateFrames()
{
    if (!_currentMap)
        return;
    
    for (GameMap::Object &obj : _currentMap->MapObjects)
    {
        if (obj.FrameTimeLeft)
        {
            if (obj.FrameTimeLeft == 1) 
            {
                obj.CurrentFrame++;
                
                Resources::SimpleObject &pSimple = Res.SimpleObjects.at(obj.ObjId);
                
                if (obj.CurrentFrame >= pSimple.Flames.size())
                    obj.CurrentFrame = 0;
                
                obj.FrameTimeLeft = pSimple.FrameTime;
            }
            else 
                obj.FrameTimeLeft--;
        }
    }
}



bool Engine::FUN_00412c30(MapChar *pchar)
{
    Common::Rect r = pchar->spwnTlRect;
    
    for(int32_t i = 0; i < _mainMapChar->GroupSize; ++i)
    {
        Character &rchar = _state.Characters.at(_mainMapChar->CharacterIndex + i);
        if ( (rchar.ClassID & CLASS_BIT80) == 0 && 
              rchar.State != CHSTATE_9 &&
              rchar.State != CHSTATE_3 )
        {
            if (rchar.Tile.y >= r.top && rchar.Tile.y <= r.bottom &&
                rchar.Tile.x >= r.left && rchar.Tile.x <= r.right)
            {
                pchar->unk3 = _mainCharacter->MapCharID;
                pchar->unk4 = 1;
                return true;
            }
        }
    }
    
    for(int32_t i = 0; i < pchar->GroupSize; ++i)
    {
        Character &rchar = _state.Characters.at(pchar->CharacterIndex + i);
        if ( (rchar.ClassID & CLASS_BIT80) == 0 && 
              rchar.State != CHSTATE_9 &&
              rchar.State != CHSTATE_3 )
        {
            if (rchar.Tile.y < r.top || rchar.Tile.y > r.bottom &&
                rchar.Tile.x < r.left && rchar.Tile.x > r.right)
            {
                pchar->unk4 = 0x20;
                return true;
            }
        }
    }
    pchar->unk4 = 0;
    return false;
}

void Engine::FUN_00418510(Character *pchar, int32_t maxBonusGld, int32_t lvl)
{
    int32_t invCount = 0;
    for (int32_t i = 0; i < INVSIZE; ++i)
    {
        if (pchar->Inventory[i])
            invCount++;
    }
    
    if (invCount == 0 && pchar->Gold == 0 && (rand() % 101) > 30 && pchar->field62_0x52 == 0)
        return;

    int32_t startSlot = 0;
    GameMap::Cell &cell = _currentMap->FootMap.At(pchar->Tile);
    GS1 *loot = nullptr;
    if ((cell.Flags & 2) == 0)
    {
        if (mapGS1Count >= mapGS1.size())
            return;
        
        loot = &mapGS1.at(mapGS1Count);
        *loot = GS1();
        loot->Index = mapGS1Count;
        loot->Tile = pchar->Tile;
        loot->LootID = -1;
        
        cell.Flags |= 2;
        
        mapGS1Count++;
    }
    else
    {
        for (int32_t i = 0; i < mapGS1Count; ++i)
        {
            GS1 &l = mapGS1.at(i);
            if (l.Tile == pchar->Tile)
            {
                for (int16_t itemId : l.ItemID)
                {
                    if (!itemId)
                        break;
                    
                    startSlot++;
                }
                
                loot = &l;
                break;
            }
        }
    }
    
    if (!loot)
        return;
    
    if (maxBonusGld >= 2)
        loot->unk2 += pchar->Gold + (rand() % maxBonusGld);
    
    int32_t invPos = 0;
    for (int32_t i = 0; i < invCount; ++i)
    {
        //Skip empty slots
        while(pchar->Inventory[invPos] == 0 && invPos < INVSIZE)
        { ++invPos; }
        
        loot->ItemID[startSlot] = pchar->Inventory[invPos];
        pchar->Inventory[invPos] = 0;
        
        startSlot++;
        
        if (startSlot >= loot->ItemID.size())
            break;
    }
    
    if (startSlot < loot->ItemID.size() && lvl != 0)
    {
        int32_t randVal = rand() % 101;
        if (randVal > 85) 
        {
            ItemInfo *itm = AllocItem();
            if (itm)
            {
                loot->ItemID[startSlot] = itm->Index;
                startSlot++;
                
                if (randVal < 87)
                {
                    int32_t tmp = 0;
                    if (lvl / 5 + 1 >= 2)
                        tmp = rand() % (lvl / 5 + 1);
                    
                    itm->InfoID = tmp + (rand() % 9) * 3;
                    itm->Weight = ArmorWeaponInfo[ itm->InfoID ].unk1;
                    itm->Concentration = itm->Weight;
                    
                    tmp = itm->InfoID % 9;
                    if (tmp < 3)
                        itm->TypeID = 2;
                    else if (tmp < 6)
                        itm->TypeID = 0;
                    else
                        itm->TypeID = 1;
                    
                    if (rand() % 101 < 10)
                    {
                        if (_mainCharacter->Identification < rand() % 100)
                            itm->Flags |= ItemInfo::FLAG_UNIDENTIFIED;
                        else
                        {
                            _mainCharacter->Identification++;
                            if (_mainCharacter->Identification > 100)
                                _mainCharacter->Identification = 100;
                        }
                        
                        tmp = 0;
                        
                        if (FUN_004171d4(lvl) >= 2)
                            tmp = rand() % FUN_004171d4(lvl);
                        
                        itm->BonusID = tmp + (rand() % 5) * 10;
                    }
                }
                else if (randVal < 88)
                {
                    int32_t tmp = rand() % 3;
                    if (tmp == 0)
                    {
                        tmp = 0;
                        if (lvl / 5 + 1 >= 2)
                            tmp = rand() % (lvl / 5 + 1);
                        
                        itm->InfoID = tmp + (rand() % 3) * 11 + 34;
                        itm->TypeID = 3;
                    }
                    else if (tmp == 1)
                    {
                        tmp = 0;
                        if (lvl / 4 + 1 >= 2)
                            tmp = rand() % (lvl / 4 + 1);
                        
                        itm->InfoID = tmp + (rand() % 3) * 11 + 37;
                        itm->TypeID = 5;
                    }
                    else if (tmp == 2)
                    {
                        tmp = 0;
                        if (lvl / 4 + 1 >= 2)
                            tmp = rand() % (lvl / 4 + 1);
                        
                        itm->InfoID = tmp + (rand() % 3) * 11 + 41;
                        itm->TypeID = 4;
                    }
                    
                    itm->Weight = ArmorWeaponInfo[itm->InfoID].unk1;
                    itm->Concentration = itm->Weight;
                    
                    if (rand() % 100 < 7)
                    {
                        if (_mainCharacter->Identification < rand() % 100)
                            itm->Flags |= ItemInfo::FLAG_UNIDENTIFIED;
                        
                        tmp = 0;
                        
                        if (FUN_004171d4(lvl) >= 2)
                            tmp = rand() % FUN_004171d4(lvl);
                        
                        itm->BonusID = tmp + (rand() % 5) * 10;
                    }
                }
                else
                {
                    if (randVal < 95)
                    {
                        if (lvl / 5 + 1 < 2)
                            itm->InfoID = 6;
                        else
                            itm->InfoID = 6 + rand() % (lvl / 5 + 1);
                        itm->TypeID = 8;
                    }
                    else if (randVal < 98) 
                    {
                        if (lvl / 5 + 1 < 2)
                            itm->InfoID = 3;
                        else
                            itm->InfoID = 3 + rand() % (lvl / 5 + 1);
                        itm->TypeID = 7;
                    }
                    else 
                    {
                        if (lvl / 5 + 1 < 2)
                            itm->InfoID = 0;
                        else
                            itm->InfoID = rand() % (lvl / 5 + 1);
                        itm->TypeID = 6;
                    }
                    
                    if (_mainCharacter->Identification < rand() % 100)
                        itm->Flags |= ItemInfo::FLAG_UNIDENTIFIED;
                    
                    int32_t tmp = 0;
                        
                    if (FUN_004171d4(lvl) >= 2)
                        tmp = rand() % FUN_004171d4(lvl);
                        
                    itm->BonusID = tmp + (rand() % 5) * 10;
                }
            }
        }
    }
    
    if (startSlot < loot->ItemID.size() && pchar->field62_0x52)
    {
        ItemInfo *itm = AllocItem();
        if (itm)
        {
            loot->ItemID[startSlot] = itm->Index;
            itm->TypeID = 9;
            itm->InfoID = 0;
            itm->Concentration = 1.0;
            itm->Poison = pchar->field62_0x52;
        }
    }
    
    if (loot->unk2 == 0 && loot->ItemID[0] == 0)
    {
        cell.Flags &= ~2;
        mapGS1Count--;
    }
    else
    {
        loot->ImgID = GetLootImage(*loot);
        
        Common::Point pt = FUN_00439ba0(loot->Tile);
        GFX::Image *img = _menuImages.at(loot->ImgID);
        
        loot->Pos = pt - img->GetSize() / 2;
    }
}

void Engine::FUN_0041bdf0(MapChar *mchar)
{
    static const std::array<int32_t, 16> dl =
    {
        3600, 6000, 12000, 9000,
        6000, 3600, 7200, 3600,
        4800, 6000, 5400, 5400,
        3600, 3600, 4800, 4800
    };
    
    int32_t iVar5 = mchar->unk5 - 0x41;
    if (iVar5 < 0x10) 
    {
        if (mchar->unk1 < mchar->unk2)
        {
            mchar->field2_0x6--;
            if (mchar->field2_0x6 < 0)
            {
                mchar->unk1++;
                mchar->field2_0x6 += dl.at(iVar5);
            }
        }
        
        if (mchar->GroupSize < mchar->unk1)
            mchar->field1_0x2--;
    }
    
    for (int32_t i = 0; i < mchar->GroupSize; ++i)
    {
        Character &ch = _state.Characters.at(mchar->CharacterIndex + i);
        if ((ch.ClassID & CLASS_BIT80) == 0 && ch.State != CHSTATE_9 && ch.State != CHSTATE_3)
        {
            if (ch.Otravlenie != 0) 
            {
                ch.HP -= ch.Otravlenie;
                FUN_0041c750(&ch);
                
                if (ch.HP < 1) 
                {
                    ch.field2_0x2 = 0;
                    ch.HP = 0;
                    
                    if (mchar->MapID == _currentMapID)
                    {
                        if ((ch.field_0x3 & 4) == 0) 
                            FUN_004143dc(&ch, CHSTATE_9);
                        else 
                            FUN_004143dc(&ch, CHSTATE_3);
                        
                        FUN_00418510(&ch, 0, _mainCharacter->Level);
                    }
                }
            }
            if (ch.field_0xce)
            {
                ch.field_0xce--;
                if (ch.field_0xce == 0)
                {
                    ch.field90_0xca = 0;
                    ch.BaseLovkost = ch.field76_0xbc;
                    ch.BaseHarizm = ch.field78_0xbe;
                    ch.BaseSila = ch.field80_0xc0;
                    ch.BaseVinoslivost = ch.field82_0xc2;
                }
            }
        }
    }
}

vec3f Engine::FUN_0042c914(Character *pchar, ItemInfo *itm)
{
    int32_t val = FUN_00418364(pchar, itm);
    if (val == 0)
        return vec3f(24, 0, 0);
    else if (itm->Flags & ItemInfo::FLAG_UNIDENTIFIED)
        return vec3f(15, 15, 0);
    else if (itm->BonusID >= 0)
        return vec3f(0, 12, 0);
    return vec3f();
}


int32_t Engine::FUN_0042c870(ItemInfo *itm)
{
    switch(itm->TypeID)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 12:
            return ArmorWeaponInfo.at(itm->InfoID).unk7;
        
        case 6:
        case 7:
        case 8:
            return AcessoriesInfo.at(itm->InfoID).ImgID;
            
        case 9:
            return AlchemiesInfo.at(itm->InfoID).ImgID;
        
            
        case -1:
            return 0;
            
        default:
            return MiscItemsInfo.at(itm->InfoID).ImgID;
    }
}

void Engine::DrawInventory(Character *pchar, int btn, bool bkg)
{
    GFX::Image *bk = _menuImages.at(17);
    Common::Point bksz = bk->GetSize();
    
    int32_t iVar4 = InvPos + ((SCREENRESX - 1 - MSGRECT.left) + 2 - ScrlBtnWidth * 2) / ItemInvSlotWidth;
    
    /*if (bkg)
        FillBkgRect({MSGRECT.left, DAT_00a3e88c, MSGRECT.right, MSGRECT.top});//GScrOff.x,DAT_00a3e88c,DAT_00a3e880,Game::BottomPanelY);
    */
    int32_t imgid = 18;
    if (btn < 0)
        imgid = 19;
    
    ImgQueue2(_menuImages.at(imgid), {MSGRECT.left, DAT_00a3e88c}, Common::Rect());
    
    imgid = 20;
    if (btn > 0)
        imgid = 21;
    
    ImgQueue2(_menuImages.at(imgid), {SCREENRESX - ScrlBtnWidth, DAT_00a3e88c}, Common::Rect());
    
    int32_t local_24 = DAT_00a3e870;
    int32_t y = DAT_00a3e88c + (ScrlBtnHeight - bksz.x) / 2;
    for (int32_t i = InvPos; i < iVar4; ++i)
    {
        int32_t iVar2 = pchar->Inventory.at(i & 0x1f);
        vec3f clr;
        if (iVar2 && iVar2 != DAT_00a3e7a0)
            clr = FUN_0042c914(pchar, &(_state.Items.at(iVar2)));
        
        ImgQueue2(bk, {local_24, y}, Common::Rect(), clr);
        
        if (iVar2 && iVar2 != DAT_00a3e7a0)
        {
            int32_t itemId = FUN_0042c870( &_state.Items.at(iVar2) );
            if (itemId)
            {
                GFX::Image *img = _menuImages.at(itemId);
                ImgQueue2(img, {local_24 + (ItemInvSlotWidth + 1 - img->GetSize().x) / 2, y + (ScrlBtnHeight + 1 - img->GetSize().y) / 2}, Common::Rect(), clr);
            }
        }
        local_24 += ItemInvSlotWidth;
    }
}

void Engine::FUN_0042f50c(Character *pchar, int32_t p)
{
    if (_playScreenID == PLSCREEN_3)
        DisplayInvOfCharID = pchar->Index + 1;
    else
    {
        DisplayInvOfCharID = pchar->Index + 1;
        DisplayInvOfCharID2 = pchar->Index + 1;
    }
    
    if (p != 0)
        DWORD_00a3e74c |= 4;
    
    _invButton = p;
}



int32_t Engine::FUN_0041b70c(Character *pchar)
{
    if (pchar->ClassID & CLASS_BIT40)
        return (float)GetArmor(pchar) * 0.1 + (float)pchar->CurrentVinoslivost * 0.3 + (float)pchar->CurrentSila * 0.3 + (float)pchar->CurrentLovkost * 0.2;

    FUN_0041c750(pchar);

    int32_t val = (float)GetArmor(pchar) * 0.1 +
                  (float)pchar->Metkost * 0.003 +
                  (float)pchar->Fehtovanie * 0.002 + 
                  (float)pchar->CurrentVinoslivost * 0.3 + 
                  (float)pchar->CurrentSila * 0.3 + 
                  (float)pchar->CurrentLovkost * 0.2;

    if ((pchar->field_0x12 == ESLT_0 || pchar->field_0x12 == ESLT_2) &&
         pchar->ArmorWeapons[pchar->field_0x12] != 0)
    {
        ItemInfo &inf = _state.Items.at( pchar->ArmorWeapons[pchar->field_0x12] );
        val += (float)ArmorWeaponInfo.at(inf.InfoID).Dmg * 0.2;
    }
    
    return val;
}

void Engine::FUN_0042f9b8(const std::string &txt)
{
    if (!txt.empty())
    {
        MsgTextTimeout = 12;
        MsgText = txt;
        
        //Common::Point outPos(MSGRECT.left + 10, MSGRECT.top + 5);
        //TextQueue(txt, _Fonts[3], outPos, MSGRECT);
    }
}


bool Engine::FUN_00416700(Village *vlg)
{
    MapChar &mchief = _state.MapChar_ARRAY.at(vlg->ChiefCharId);
    if ((vlg->Flags & 5) != 0)
    {
        Common::Rect r = mchief.spwnTlRect;

        for (int32_t i = 0; i < _mainMapChar->GroupSize; ++i)
        {
            Character &ch = _state.Characters.at(_mainMapChar->CharacterIndex + i);
            if ((ch.ClassID & CLASS_BIT80) == 0 && ch.State != CHSTATE_9 && ch.State != CHSTATE_3)
            {
                if (ch.Tile.y >= r.top && ch.Tile.y <= r.bottom &&
                    ch.Tile.x >= r.left && ch.Tile.x <= r.right)   
                {
                    if ((vlg->Flags & 1) != 0) 
                    {
                        mchief.unk3 = _mainMapChar->Index;
                        mchief.unk4 = 1;
                        return true;
                    }

                    if (ch.CharacterBase != (mchief.unk5 & 3))
                    {
                        if (ch.ArmorWeapons[ESLT_0] != 0 || ch.ArmorWeapons[ESLT_1] != 0 || ch.ArmorWeapons[ESLT_2] != 0)
                        {
                            mchief.unk3 = _mainMapChar->Index;
                            mchief.unk4 = 1;
                            return true;
                        }

                        for (uint16_t itm : ch.Inventory)
                        {
                            if (itm != 0 && _state.Items.at(itm - 1).TypeID < 3)
                            {
                                mchief.unk3 = _mainMapChar->Index;
                                mchief.unk4 = 1;
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    
    mchief.unk4 = 0;
    return false;
}

void Engine::FUN_0041733c(Village::BldState *state)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
}

void Engine::FUN_0041d0fc()
{
    if ( (_counter & 0xf) == 0 )
    {
        for(MapChar &mchar : _state.MapChar_ARRAY)
        {
            if (!mchar.unk5)
                continue;
            
            if (mchar.unk5 & 0x40)
            {
                if (mchar.MapID == _currentMapID)
                    FUN_00412c30(&mchar);
                
                FUN_0041bdf0(&mchar);
                continue;
            }
            
            for (int32_t i = 0; i < mchar.GroupSize; ++i)
            {
                Character &ch = _state.Characters.at(mchar.CharacterIndex + i);
                if ((ch.ClassID & CLASS_BIT80) || ch.State == CHSTATE_9 || ch.State == CHSTATE_3)
                    continue;
                
                if (ch.Otravlenie != 0) 
                {
                    ch.HP -= ch.Otravlenie;
                    FUN_0041c750(&ch);

                    if (ch.HP < 1)
                    {
                        ch.field2_0x2 = 0;
                        ch.HP = 0;

                        if (mchar.MapID == _currentMapID) 
                        {
                            if ((ch.field_0x3 & 4) == 0) 
                                FUN_004143dc(&ch, CHSTATE_9);
                            else 
                                FUN_004143dc(&ch, CHSTATE_3);

                            FUN_00418510(&ch, 0, _mainCharacter->Level);
                        }
                    }
                    //if (ch.MapCharID == _mainCharacter->MapCharID) 
                    //    FUN_0042d574(&ch);
                }
                
                if (ch.field_0xce)
                {
                    ch.field_0xce--;
                    if (ch.field_0xce == 0)
                    {
                        ch.field90_0xca = 0;
                        ch.BaseLovkost = ch.field76_0xbc;
                        ch.BaseHarizm = ch.field78_0xbe;
                        ch.BaseSila = ch.field80_0xc0;
                        ch.BaseVinoslivost = ch.field82_0xc2;

                        if (ch.MapCharID == _mainMapChar->Index)
                        {
                            //FUN_0042d574(&ch);
                            //if (_playScreenID == PLSCREEN_3)
                            //    PlayChangeScreen(PLSCREEN_3);

                            if (DisplayInvOfCharID != 0)
                                FUN_0042f50c(CharInfoCharacter, 0);
                        }
                    }
                }
            }
        }
    }
    else if ( ((_counter + 7) & 0xf) == 0 )
    {
        for(Village &vlg : _state.VillageState)
        {
            int32_t local_24 = 0;
            int32_t local_20 = 0;
            
            MapChar &mchief = _state.MapChar_ARRAY.at(vlg.ChiefCharId);
            for(int32_t i = 0; i < mchief.GroupSize; ++i)
            {
                Character &ch = _state.Characters.at(mchief.CharacterIndex + i);
                if (ch.CharacterBase < 3)
                {
                    if ((ch.ClassID & CLASS_BIT80) == 0 && ch.State != CHSTATE_9 && ch.State != CHSTATE_3)
                    {
                        local_24++;
                        if (ch.ClassID & 0x38)
                            local_20++;
                    }
                }
            }
            
            int32_t local_1c = 0;
            for(Village::Job &job : vlg.Jobs)
            {
                if (job.CharID)
                    local_1c++;
            }
            
            if (!local_24)
            {
                mchief.unk4 = 0;
                vlg.Flags &= 0xea;
                vlg.GoldTributeTimer = 0;
            }
            
            if (mchief.unk4 == 1 && (vlg.Flags & 0x80) == 0)
            {
                int32_t local_18 = 0;
                for(int32_t i = 0; i < _mainMapChar->GroupSize; ++i)
                {
                    local_18 += FUN_0041b70c(&_state.Characters.at(_mainMapChar->CharacterIndex + i));
                }
                
                int32_t local_14 = 0;
                for(int32_t i = 0; i < mchief.GroupSize; ++i)
                {
                    Character &ch = _state.Characters.at(mchief.CharacterIndex + i);
                    if (ch.CharacterBase < 3)
                    {
                        if ((ch.ClassID & CLASS_BIT80) == 0 && ch.State != CHSTATE_9 && ch.State != CHSTATE_3)
                            local_14 += FUN_0041b70c(&ch);
                    }
                }
                
                if ( (float)local_18 / (float)local_14 >= 2.0 )
                {
                    mchief.unk4 = 0;
                    for(int32_t i = 0; i < mchief.GroupSize; ++i)
                    {
                        Character &ch = _state.Characters.at(mchief.CharacterIndex + i);
                        if (ch.State == CHSTATE_9 || ch.State == CHSTATE_3)
                        {
                            ch.Frame = ch.FrameCount - 1;
                            ch.field2_0x2 = 0;
                            ch.ClassID |= CLASS_BIT80;
                            
                            _currentMap->FootMap.At(ch.Tile).ID = 0;
                            FUN_00414078(&ch);
                        }
                        else
                        {
                            if ( (ch.ClassID & CLASS_BIT80) == 0 &&
                                  ch.State != CHSTATE_9 &&
                                  ch.State != CHSTATE_3 )
                            {
                                ch.EnemyCharID = 0;
                                ch.field2_0x2 = 0;
                                FUN_00414ab4(&ch);
                            }
                        }
                    }
                    
                    vlg.Flags |= 0x80;
                    vlg.Flags &= 0xea;
                    vlg.GoldTributeTimer = 0;
                    vlg.GoldTribute = 0;
                    
                    FUN_0042f9b8(Locale::GameMessages[Locale::GMSG_VLG_GIVEUP]);
                    
                    _nextStateMode = STATEMD_PLAY;
                    
                    StartPlayMovie(fmt::sprintf("DEFEAT%d.FLM", _currentMapID));
                    
                    for(int32_t i = 0; i < _mainMapChar->GroupSize; ++i)
                    {
                        Character &ch = _state.Characters.at(_mainMapChar->CharacterIndex + i);
                        if ( (ch.ClassID & CLASS_BIT80) == 0 && 
                              ch.State != CHSTATE_9 &&
                              ch.State != CHSTATE_3 )
                        {
                            ch.EnemyCharID = 0;
                            ch.field2_0x2 = 0;
                            FUN_00414ab4(&ch);
                        }
                    }
                }
            }
            
            if (mchief.MapID == _currentMapID)
                FUN_00416700(&vlg);
            
            if ( (vlg.Flags & 0x10) == 0 )
            {
                vlg.GoldTributeTimer++;
                if (vlg.GoldTributeTimer > 10079)
                {
                    vlg.GoldTribute += 1.0 + (float)mchief.GroupSize * 5.0 +
                                       std::sqrt((float)_mainCharacter->Trading) * (float)vlg.Bld1Num * 20.0;
                    vlg.GoldTributeTimer = 0;
                }
            }
            
            bool local_30 = false;
            
            if (mchief.unk4 == 1)
                local_24 = 0;
            else
                local_24 -= local_20 + local_1c;
            
            for(int32_t i = 0; i < vlg.Bld2Num + vlg.Bld1Num + 7; ++i)
            {
                Village::BldState &bst = vlg.BuildingState.at(i);
                if (bst.BuildInfoID >= 0 && bst.BldLeft <= 0 && bst.State == 6 && local_24 >= 1)
                {
                    local_30 = true;
                    
                    bst.State = 0;
                    
                    bst.BldLeft = (BuildingInfo.at(bst.BuildInfoID).BuildPeriod * 60) / local_24;
                    if (vlg.MapID == _currentMapID)
                        FUN_0041733c(&bst);
                    
                    continue;
                }
                
                if (bst.BuildInfoID <= -1 || bst.BldLeft <= 0)
                    continue;
                
                if (bst.State < 3 && local_24 > 0)
                {
                    bst.BldLeft--;

                    if (bst.BldLeft <= 0)
                    {
                        bst.State++;
                        if (bst.State != 3)
                        {
                            local_30 = true;

                            bst.BldLeft = (BuildingInfo.at(bst.BuildInfoID).BuildPeriod * 60) / local_24;
                        }

                        if (vlg.MapID == _currentMapID)
                            FUN_0041733c(&bst);
                    }
                }
                else
                {
                    bst.BldLeft--;
                    if (bst.BldLeft <= 0)
                    {
                        bst.State++;
                        if (bst.State != 6)
                            bst.BldLeft = BuildingInfo[bst.BuildInfoID].BurnPeriod2;

                        if (vlg.MapID == _currentMapID) {
                            FUN_0041733c(&bst);
                        }
                    }
                }
            }
            
            UpdateVillageMedic(&vlg);
            UpdateVillageSmith(&vlg);
            if (local_30 == 0)
                UpdateVillageVoevoda(&vlg);
        }
    }
}



bool Engine::FUN_00413a84(Character *pchar)
{   
    const Common::Point MoveBounds = ViewTiles * 2;
    
    Common::Rect spwnRect = _state.MapChar_ARRAY.at(pchar->MapCharID).spwnTlRect;
    Common::Point st = pchar->Tile;
    // if out of bounds, then center it
    if (pchar->Tile.y < spwnRect.top ||
        pchar->Tile.y > spwnRect.bottom ||
        pchar->Tile.x < spwnRect.left ||
        pchar->Tile.x > spwnRect.right)
        st = Common::Point( (spwnRect.right + spwnRect.left) / 2,
                            (spwnRect.bottom + spwnRect.top) / 2);
        
    for(int32_t i = 0; i < 10; ++i)
    {
        Common::Point tgt = st;
        
        int32_t dx = 0;
        int32_t dy = 0;
        if (MoveBounds.x >= 2)
            dx = rand() % MoveBounds.x;
        
        if (MoveBounds.y >= 2)
            dy = rand() % MoveBounds.y;
        
        switch(rand() % 4)
        {
            default:
            case 0:
                tgt.x -= dx;
                tgt.y -= dy;
                break;
            case 1:
                tgt.x += dx;
                tgt.y -= dy;
                break;
            case 2:
                tgt.x -= dx;
                tgt.y += dy;
                break;
            case 3:
                tgt.x += dx;
                tgt.y += dy;
                break;
        }
        
        if (tgt.x > spwnRect.left &&
            tgt.x < spwnRect.right &&
            tgt.y > spwnRect.top &&
            tgt.y < spwnRect.bottom &&
            _currentMap->FootMap.At(tgt).ID == 0)
        {
            pchar->MoveTile = tgt;
            pchar->field2_0x2 = 0x48;
            pchar->field17_0x13[1] = 0xff;
            return true;
        }
    }
    return false;
}

bool Engine::FUN_004142d4(Character *pchar, Common::Point pt, uint8_t p)
{
    pchar->MoveTile = pt;
    pchar->field2_0x2 = p;
    pchar->field17_0x13[1] = 0xff;
    _currentMap->FootMap.At(pchar->Tile).ID = 0;
    int32_t distance = FUN_0043edb7(pchar);
    _currentMap->FootMap.At(pchar->Tile).ID = pchar->Index + 1;
    if (!distance) 
        return false;
    
    pchar->field2_0x2 &= 0xbf;
    
    if (pchar->field_0x3 & 4)
        FUN_004143dc(pchar, CHSTATE_1);
    else
        FUN_004143dc(pchar, CHSTATE_7);
    
    return true;
}


int Engine::FUN_0041165c(Common::Point p1, Common::Point p2)
{
    for(int i = 0; i < 8; ++i)
    {
        Common::Point po;        
        if (FUN_0043ecba(&po, p2, i) && po == p1)
            return i;
    }
    return -1;
}

int Engine::FUN_00410ffc(Common::Point *pout, Common::Point p)
{
    for(int i = 0; i < 8; ++i)
    {      
        if (FUN_0043ecba(pout, p, i) && _currentMap->FootMap.At(*pout).ID == 0)
            return i;
    }
    return -1;
}

void Engine::FUN_004391bc(Character *pchar, int dir)
{
    if (dir < pchar->Direction)
        dir += 8;
    
    if (dir - pchar->Direction < 5)
    {
        pchar->Direction += 1;
        if (pchar->Direction > 7)
            pchar->Direction = 0;
    }
    else 
    {
        pchar->Direction -= 1;
        if (pchar->Direction < 0)
            pchar->Direction = 7;
    }
    
    FUN_00414078(pchar);
}



int32_t Engine::GetArmor(Character *pchar)
{
    FUN_0041c750(pchar);
    
    int32_t armor = pchar->Armor;
    for (int32_t i = 3; i < 6; ++i)
    {
        int32_t itmId = pchar->ArmorWeapons[i];
        
        if (itmId)
        {
            ItemInfo &inf = _state.Items.at(itmId);
            armor += ArmorWeaponInfo[ inf.InfoID ].Dmg;
        }
    }
    
    if (pchar->CurrentBrn == 0)
        return armor;
    else if (pchar->Flags & 1)
        return armor + pchar->CurrentBrn;
    else
        return pchar->CurrentBrn;        
}



int32_t Engine::FUN_0041af48(Character *pchar, int32_t classID)
{
    if (pchar->ClassID & CLASS_BIT40)
    {
        int32_t_00a3e828 = pchar->field62_0x52;
        return (float)(pchar->HP * pchar->CurrentSila) * 0.04 * 0.0625;
    }
    
    
    FUN_0041c750(pchar);
    
    int32_t dmg = 0;
    
    if ((pchar->field_0x12 == 0 || pchar->field_0x12 == 2) &&
        (pchar->ArmorWeapons.at( pchar->field_0x12 ) != 0) )
    {
        int32_t itmId = pchar->ArmorWeapons.at( pchar->field_0x12 );
        ItemInfo &inf = _state.Items.at(itmId);
        
        int32_t_00a3e828 = inf.Poison;
        
        
        if (inf.SpecialID < 0 || SpecialDamages.at(inf.SpecialID).ClassID != classID)
            dmg = ArmorWeaponInfo[inf.InfoID].Dmg;
        else
            dmg = SpecialDamages.at(inf.SpecialID).Dmg;

        dmg += (float)(pchar->HP * pchar->CurrentSila) * 0.04 * 0.0625;
    }
    else 
    {
        int32_t_00a3e828 = 0;
        dmg = (float)(pchar->HP * pchar->CurrentSila) * 0.02 * 0.0625;
    }
    
    if (pchar->CurrentUdr == 0)
        return dmg;
    else if (pchar->Flags & 2)
        return pchar->CurrentUdr + dmg;
    else
        return pchar->CurrentUdr;        
}



int32_t Engine::FUN_0041b4cc(Character *pchar, int32_t fkt)
{    
    if (pchar->ClassID & CLASS_BIT40)
        return (100.0 - (100.0 - (float)pchar->CurrentLovkost * 0.5) * 0.5);
    
    FUN_0041c750(pchar);
    
    int32_t val = 0;
    if (pchar->field_0x12 == ESLT_1 && pchar->ArmorWeapons[ESLT_1] != 0 && fkt != 0)
    {
        ItemInfo &inf = _state.Items.at(pchar->ArmorWeapons[1]);
        const WeapArmorItemInfo &w = ArmorWeaponInfo.at(inf.InfoID);
        val = 100.0 - ((100.0 - (float)pchar->CurrentLovkost * 0.5) *
                      (1.0 + (float)fkt / (float)w.unk4) * 0.7) /
                      ((float)pchar->Metkost * 0.00015 + 1.5);
    }
    else
        val = 100.0 - (100.0 - (float)pchar->CurrentLovkost * 0.5) /
                      ((float)pchar->Fehtovanie * 0.00015 + 1.5);

    if (pchar->field_0xce != 0)
        val += pchar->field90_0xca;
    
    if (pchar->CurrentVer == 0)
        return val;
    else if (pchar->Flags & 4)
        return pchar->CurrentVer + val;
    else
        return pchar->CurrentVer;
}




int32_t Engine::FUN_0041b244(Character *pchar, int charId)
{
    switch(charId)
    {
    case KHAR_N1:
    case KHAR_HARIZMA:
        return pchar->CurrentHarizm;
        
    case KHAR_LOVKOST:
        return pchar->CurrentLovkost;
        
    case KHAR_N4:
        return pchar->HP / 16;
        
    case KHAR_SILA:
        return pchar->CurrentSila;
        
    case KHAR_VINOSLIVOST:
        return pchar->CurrentVinoslivost;
        
    case KHAR_ARMOR:
        return GetArmor(pchar);
        
    case KHAR_N8:
        return FUN_0041af48(pchar, -1);
        
    case KHAR_N9:
        return FUN_0041b4cc(pchar, 1);
        
    default:
        break;
    }
    return 0;
}


bool Engine::FUN_00418364(Character *pchar, ItemInfo *pitm)
{
    if (_bGodMode)
        return true;
        
    FUN_0041c750(pchar);
    
    if ((pitm->TypeID < 6) || (pitm->TypeID == 12))
    {
        const BonusSet *bset = nullptr;
        if (pitm->BonusID >= 0)
            bset = &BonusesInfo.at(pitm->BonusID);
        
        if (!bset || bset->NeedsVal == 0 || FUN_0041b244(pchar, bset->NeedsKhar) >= bset->NeedsVal)
        {
            const WeapArmorItemInfo &inf = ArmorWeaponInfo.at(pitm->InfoID);
            
            if (!inf.unk3 == 0 || FUN_0041b244(pchar, inf.unk2) >= inf.unk3)
                return true;
        }
    }
    else if (pitm->TypeID < 9)
    {
        const BonusSet &bset = BonusesInfo.at(pitm->BonusID);
        if (bset.NeedsVal == 0 || FUN_0041b244(pchar, bset.NeedsKhar) >= bset.NeedsVal)
            return true;
    }
    else
        return true;
    
    return false;
}

void Engine::FUN_00421d24(Character *pchar, int32_t itmID)
{
    for(int32_t i = 0; i < pchar->Inventory.size(); ++i)
    {
        if (pchar->Inventory[i] == itmID)
        {
            for(int32_t j = i; j < pchar->Inventory.size() - 1; ++j)
                pchar->Inventory[j] = pchar->Inventory[j + 1];
            pchar->Inventory.back() = 0;
            return;
        }
    }
}

bool Engine::FUN_004104d4(Character *pchar, int32_t awid)
{
    if (pchar->ArmorWeapons[awid])
        return true;
    
    for (int32_t i = 0; i < pchar->Inventory.size(); ++i)
    {
        ItemInfo *inf = nullptr;
        int32_t itemID = pchar->Inventory[i];
        if (itemID)
            inf = &_state.Items.at(itemID);

        if (inf && inf->TypeID == awid && FUN_00418364(pchar, inf))
        {
            pchar->Arrows = itemID;
            FUN_00421d24(pchar, itemID);
            return true;
        }
    }
    
    return false;
}

void Engine::FUN_004105a0(Character *pchar)
{
    HardComputeCount += 1;
    
    if (pchar->Arrows == 0)
    {
        for (int32_t i = 0; i < pchar->Inventory.size(); ++i)
        {
            ItemInfo *inf = nullptr;
            int32_t itemID = pchar->Inventory[i];
            if (itemID)
                inf = &_state.Items.at(itemID);
            
            if (inf && inf->TypeID == 12 && FUN_00418364(pchar, inf))
            {
                pchar->Arrows = itemID;
                FUN_00421d24(pchar, itemID);
                break;
            }
        }
    }
    
    FUN_004104d4(pchar, 0);
    FUN_004104d4(pchar, 1);
    FUN_004104d4(pchar, 3);
    FUN_004104d4(pchar, 4);
    FUN_004104d4(pchar, 5);
    FUN_004104d4(pchar, 2);
    
    if ((pchar->Arrows == 0) || (pchar->ArmorWeapons[1] == 0))
    {
        if (pchar->ArmorWeapons[0] == 0)
            pchar->field_0x12 = ESLT_2;
        else
            pchar->field_0x12 = ESLT_0;
    }
    else 
        pchar->field_0x12 = ESLT_1;
}

Engine::Character * Engine::FUN_004202a8(MapChar *mchar, Character *pchar)
{
    Character *chr = NULL;
    int32_t minval = std::numeric_limits<int32_t>::max();
    for (int32_t i = 0; i < mchar->GroupSize; ++i)
    {
        Character &pchr = _state.Characters.at(mchar->CharacterIndex + i);
        if ((pchr.ClassID & CLASS_BIT80) == 0 && pchr.State != 9 && pchr.State != 3)
        {
            int32_t val = FUN_00439944(pchr.Tile, pchar->Tile);
            if ( val < minval)
            {
                chr = &pchr;
                minval = val;
            }
        }
    }
    return chr;
}

bool Engine::FUN_00416934(Character *pchar)
{
    if ((pchar->ClassID & CLASS_BIT80) || pchar->State == CHSTATE_9 || pchar->State == CHSTATE_3)
        return false;
    
    MapChar &mchr = _state.MapChar_ARRAY.at(pchar->MapCharID);
    if (mchr.unk4 == 0) 
    {
        if ((pchar->ClassID == 0x51) || (pchar->ClassID == 0x52)) 
        {
            if (rand() % 100 < 75)
                return false;
            
            if (FUN_00413a84(pchar)) 
            {
                if (FUN_004142d4(pchar, pchar->MoveTile, 8))
                    return true;
                
                FUN_00414ab4(pchar);
            }
            return false;
        }
        
        if (pchar->field_0x3 & 4) 
        {
            pchar->field_0x3 &= ~4;
            FUN_004143dc(pchar, CHSTATE_6);
            return true;
        }
        
        int32_t job = (pchar->ClassID >> 3) & 7;
        if (job != 0)
        {
            if (CurrentVillage->Jobs[job].CharID != 0)
            {
                if (FUN_0041165c(pchar->Tile, CurrentVillage->Jobs[job].Tile) < 0)
                {
                    Common::Point p;
                    if (FUN_00410ffc(&p, CurrentVillage->Jobs[job].Tile) && 
                        FUN_004142d4(pchar, p, 8))
                        return true;
                }
                else if (pchar->Direction != CurrentVillage->Jobs[job].Direction)
                {
                    FUN_004391bc(pchar, (int)CurrentVillage->Jobs[job].Direction);
                    return true;
                }
                FUN_00414ab4(pchar);
                return false;
            }
            pchar->ClassID &= ~(7 << 3);
        }
        
        int32_t vjob = GetVillageCharacterJob(pchar);
        if (vjob == 0)
        {
            if (rand() % 100 < 50)
                return false;
            
            if (FUN_00413a84(pchar)) 
            {
                if (FUN_004142d4(pchar, pchar->MoveTile, 8))
                    return true;
                
                FUN_00414ab4(pchar);
            }
            return false;
        }
        
        job = vjob - 1;
        if (pchar->Tile == CurrentVillage->Jobs[job].Tile)
        {
            if (pchar->Direction != CurrentVillage->Jobs[job].Direction)
            {
                FUN_004391bc(pchar, CurrentVillage->Jobs[job].Direction);
                return true;
            }
            
            FUN_00414ab4(pchar);
            return false;
        }

        if (FUN_004142d4(pchar, CurrentVillage->Jobs[job].Tile, 8))
            return true;
        
        FUN_00414ab4(pchar);
        return false;
    }
    
    if (mchr.unk4 != 1)
        return false;

    if (pchar->ClassID == 0x51 || pchar->ClassID == 0x52) 
    {
        HardComputeCount += 1;
        
        for(int32_t i = 0; i < 8; ++i)
        {
            Common::Point p;
            
            if (FUN_0043ecba(&p, pchar->Tile, i))
            {
                int32_t id = _currentMap->FootMap.At(p).ID;
                
                if (id != 0 && id != 0xFFF &&
                    _state.Characters.at(id - 1).MapCharID == mchr.unk3)
                {
                    if (FUN_00413dc4(pchar))
                        return true;
                    
                    break;
                }
            }
        }
        
        if (FUN_00413a84(pchar))
        {
            if (FUN_004142d4(pchar, pchar->MoveTile, 8))
                return true;
            
            FUN_00414ab4(pchar);
        }
        return false;
    }
    
    if ((pchar->field_0x3 & 4) == 0)
    {
        FUN_004105a0(pchar);
        pchar->field_0x3 |= 4;
        FUN_00414ab4(pchar);
        return true;
    }
    
    if ((pchar->ArmorWeapons[ESLT_1] != 0) && (pchar->Arrows != 0))
    {
        pchar->field_0x12 = ESLT_1;
        if (FUN_00410e84(pchar))
        {
            pchar->MoveTile = _state.Characters.at(pchar->EnemyCharID - 1).Tile;
            pchar->field2_0x2 |= 4;
            
            FUN_004143dc(pchar, CHSTATE_4);
            
            pchar->field2_0x2 &= 0xbf;
            return true;
        }
    }
    
    if (pchar->ArmorWeapons[ESLT_0] == 0)
        pchar->field_0x12 = ESLT_2;
    else
        pchar->field_0x12 = ESLT_0;
    
    Character *tgtChar = FUN_004202a8(&_state.MapChar_ARRAY.at(mchr.unk3), pchar);
    if (!tgtChar)
        return false;
    
    pchar->EnemyCharID = tgtChar->Index + 1;
    
    if (FUN_004142d4(pchar, tgtChar->Tile, 1))
        return true;
    
    FUN_00414ab4(pchar);
    
    return false;
}


void Engine::FUN_00410728(Character *pchar)
{
    pchar->field2_0x2 |= 0x40;
    
    if ( pchar->State == CHSTATE_8 ||
         pchar->State == CHSTATE_5 || 
         pchar->State == CHSTATE_2 || 
         pchar->field2_0x2 == 0x40)
    {
        if ((pchar->field_0x3 & 4) == 0)
            FUN_004143dc(pchar, CHSTATE_6);
        else
            FUN_004143dc(pchar, CHSTATE_0);
    }
}


bool Engine::FUN_00415cd0(Character *pchar)
{
    if (pchar->State == CHSTATE_5)
    {
        if (FUN_00411758(pchar, pchar->MoveTile))
            return false;

        Character &ch = _state.Characters.at(pchar->EnemyCharID - 1);
        if ((ch.ClassID & CLASS_BIT80) || 
             ch.State == CHSTATE_9 || ch.State == CHSTATE_3)
            pchar->EnemyCharID = 0;

        FUN_00410728(pchar);
        return true;
    }
    
    if ( (pchar->field2_0x2 & 1) == 0 || 
         (pchar->State != CHSTATE_7 && pchar->State != CHSTATE_1) )
        return false;

    Character &ch = _state.Characters.at(pchar->EnemyCharID - 1);
    if ((ch.ClassID & CLASS_BIT80) != 0 || ch.State == CHSTATE_9 || ch.State == CHSTATE_3)
    {
        pchar->EnemyCharID = 0;
        FUN_00410728(pchar);
        return true;
    }
    
    if (ch.Tile == pchar->MoveTile)
        return false;

    int32_t iVar4 = FUN_0041165c(ch.Tile, pchar->MoveTile);
    if (iVar4 > -1)
    {
        for (int32_t local_1c = 0; local_1c < pchar->field17_0x13.size(); ++local_1c)
        {
            if (pchar->field17_0x13[local_1c] == 0xff || local_1c == pchar->field17_0x13.size() - 1)
            {
                if (local_1c == 0)
                    break;
                
                local_1c--;
                
                int32_t local_3c = 0;
                if (pchar->field17_0x13[local_1c] < 4)
                    local_3c = pchar->field17_0x13[local_1c] + 4;
                else
                    local_3c = pchar->field17_0x13[local_1c] - 4;
                
                Common::Point p;
                FUN_0043ecba(&p, pchar->MoveTile, local_3c);
                int32_t local_18 = FUN_0041165c(ch.Tile, p);
                if (local_18 > -1)
                {
                    pchar->MoveTile = ch.Tile;
                    pchar->field17_0x13[local_1c] = local_18;
                    return false;
                }
                
                uint8_t dir = PathGo[iVar4][ pchar->field17_0x13[local_1c] ];
                if (dir != 0xff)
                {
                    pchar->MoveTile = ch.Tile;
                    if ((dir & 0x80) == 0)
                        pchar->field17_0x13[local_1c] = dir;
                    else
                        pchar->field17_0x13[local_1c] = 0xff;
                    return false;
                }
                
                break;
            }
        }
    }
    
    if (pchar != _mainCharacter)
        pchar->EnemyCharID = 0;
    
    FUN_00410728(pchar);
    return true;
}

void Engine::FUN_004248a0(MapChar *mchr)
{
    mchr->unk4 = 1;
    for (int32_t i = 0; i < mchr->GroupSize; ++i)
    {
        Character &ch = _state.Characters.at(mchr->CharacterIndex + i);
        if ((ch.field2_0x2 & 1) == 0 && ch.ClassID != 0x49)
            ch.field17_0x13[1] = 0xff;
    }
}



bool Engine::FUN_0041bff8(Character *pchar)
{
    if (MapObjects2Count >= 100)
        return false;
    
    Common::FPoint p = FUN_00439ba0(pchar->MoveTile) + Common::FPoint(0, -30);
    
    MapObject2 &obj = MapObjects2.at(MapObjects2Count);
    MapObjects2Count++;
    
    obj.CharID = pchar->Index;
    
    ItemInfo &arr = _state.Items.at(pchar->Arrows);
    obj.Poison = arr.Poison;
    obj.SpecialID = arr.SpecialID;
    obj.InfoID = arr.InfoID;
    
    ItemInfo &bow = _state.Items.at(pchar->ArmorWeapons[ESLT_1]);
    const WeapArmorItemInfo &wep = ArmorWeaponInfo.at(bow.InfoID);
    obj.Dmg = wep.Dmg;
    obj.p2 = pchar->POS + O45ad30.at(pchar->Direction);
    
    Common::FPoint d = obj.p2.AbsDistance(p) * Common::FPoint(0.25, 0.25) + Common::FPoint(0.001, 0.001);
    if (d.x < d.y)
        obj.p1 = (p - obj.p2) / d.y;
    else
        obj.p1 = (p - obj.p2) / d.x;
    
    obj.dmg2 = pchar->Direction * 2 + 184; //?
    obj.unk4 = wep.unk4;
    static const Common::Point dp[8] = 
    {
        {  0,  -3},
        {  0,   0},
        { -3,   0},
        {-29,   0},
        {-32,  -3},
        {-29, -17},
        { -3, -23},
        {  0, -17}
    };
    obj.p3 = dp[pchar->Direction];
    
    if (pchar->ArmorWeapons[ESLT_1] != 0)
    {
        bow.Concentration -= 0.1;
        if (bow.Concentration <= 0.0)
        {
            bow.TypeID = -1;
            pchar->ArmorWeapons[ESLT_1] = 0;
        }
    }
    return true;
}



int32_t Engine::FUN_0043976c(Common::Point m, Common::Point t)
{
    if (t.y == m.y)
    {
        if (m.x < t.x)
            return 0;
        else
            return 4;
    }
    
    Common::Point p;
    FUN_0043ecba(&p, t, 6);
    if (p == m)
        return 6;
    
    FUN_0043ecba(&p, t, 7);
    if (p == m)
        return 7;
    
    FUN_0043ecba(&p, t, 5);
    if (p == m)
        return 5;
    
    FUN_0043ecba(&p, t, 2);
    if (p == m)
        return 2;
    
    FUN_0043ecba(&p, t, 3);
    if (p == m)
        return 3;
    
    return 1;
}

int32_t Engine::FUN_0041b100(Character *ch1, Character *ch2)
{
    if (rand() % 101 > FUN_0041b4cc(ch2, 0))
        return 0;
    
    return FUN_0041af48(ch2, ch1->ClassID);
}


int32_t Engine::FUN_0041c480(Character *chr1, Character *chr2, int32_t param_3)
{
    if (_bGodMode && chr1 == _mainCharacter) 
        return -2;
    
    FUN_0041c750(chr1);
    
    static const uint8_t Can[8][8] = 
    {
        {0, 0, 0, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 0, 0, 0}
    };
    
    if (Can[chr1->Direction][chr2->Direction])
    {
        if (rand() % 101 <= (float)chr1->CurrentLovkost * 0.5)
            return -2;
    }
    
    int32_t armor = GetArmor(chr1);
    
    float local_24 = (float)param_3 * 0.7 - (float)armor;
    if (local_24 < 0.0)
        local_24 = 0.0;
    
    int32_t res = 0;
    int32_t hit = (1.0 + (local_24 + (float)param_3 * 0.3) / (float)chr1->CurrentVinoslivost) * 16.0;
    if (hit < chr1->HP) 
    {
        chr1->HP -= hit;
        FUN_0041c750(chr1);
        if (hit < 48)
            res = -1;
        else 
            res = 0;
    }
    else
        res = 1;
    
    if (armor == 0)
        local_24 = 1.0;
    else 
    {
        local_24 = (float)param_3 / (float)armor;
        if (1.0 < local_24) {
            local_24 = 1.0;
        }
    }
    
    chr1->Otravlenie += int32_t_00a3e828;
    
    for (int32_t i = 3; i < 6; ++i)
    {
        int32_t itemID = chr1->ArmorWeapons[i];
        if (itemID != 0) 
        {
            ItemInfo &itm = _state.Items.at(itemID);
            itm.Concentration -= local_24 * (float)ArmorWeaponInfo[itm.InfoID].Dmg * 0.001;
            if (itm.Concentration <= 0.0)
            {
                itm.TypeID = -1;
                chr1->ArmorWeapons[i] = 0;
            }
        }
    }
    if ((chr2->field_0x12 == 0 || chr2->field_0x12 == 2) &&
         chr2->ArmorWeapons[ chr2->field_0x12 ] != 0)
    {
        int32_t itemID = chr2->ArmorWeapons[ chr2->field_0x12 ];
        ItemInfo &itm = _state.Items.at(itemID);
        if (itm.Poison != 0)
            itm.Poison--;
        
        itm.Concentration -= (float)param_3 * 0.00025;
        if (itm.Concentration <= 0.0)
        {
            itm.TypeID = -1;
            chr2->ArmorWeapons[ chr2->field_0x12 ] = 0;
        }
    }
    return res;
}



void Engine::FUN_0041079c(Character *pchar, int32_t exp)
{
    pchar->FreePoints += exp;
    if ((pchar->field_0x3 & 2) == 0)
        return;
    
    bool bvar2;
    while (true)
    {
        int32_t iVar3 = CheckKharUp(*pchar, KHAR_LEVEL);
        if (iVar3)
            pchar->Level++;
        
        int32_t iVar4 = CheckKharUp(*pchar, KHAR_VINOSLIVOST);
        if (iVar4)
        {
            pchar->FreePoints -= iVar4;
            pchar->Exp += iVar4;
            pchar->BaseVinoslivost++;
        }
        
        int32_t iVar5 = CheckKharUp(*pchar, KHAR_SILA);
        if (iVar5 != 0)
        {
            pchar->FreePoints -= iVar5;
            pchar->Exp += iVar5;
            pchar->BaseSila++;
        }
        
        int32_t iVar6 = CheckKharUp(*pchar, KHAR_LOVKOST);
        if (iVar6 != 0)
        {
            pchar->FreePoints -=iVar6;
            pchar->Exp += iVar6;
            pchar->BaseLovkost++;
        }
        
        int32_t iVar7 = CheckKharUp(*pchar, KHAR_HARIZMA);
        if (iVar7 != 0) 
        {
            pchar->FreePoints -= iVar7;
            pchar->Exp += iVar7;
            pchar->BaseHarizm++;
        }
        
        bool bVar1 = iVar7 != 0 || iVar6 != 0 || iVar5 != 0 || iVar4 != 0 || iVar3 != 0;
        bvar2 = (bvar2 || bVar1);
        
        if (!bVar1)
            break;
    }
    if (bvar2)
    {
        if (pchar->Index + 1 == DisplayInvOfCharID)
        {
            //if (_playScreenID == PLSCREEN_3)
            //    PlayChangeScreen(PLSCREEN_3);
            FUN_0042f50c(pchar, 0);
        }
    }
}




void Engine::FUN_00416058(MapChar *mchr)
{
    for(int32_t i = 0; i < mchr->GroupSize; ++i)
    {
        Character &chr = _state.Characters.at(mchr->CharacterIndex + i);
        
        static std::set<int> __tstates({CHSTATE_2, CHSTATE_3, CHSTATE_8, CHSTATE_9});
        
        if ( (chr.field2_0x2 & 0x40) && (__tstates.count(chr.State) == 0))
        {
            if (!SkipBecauseOfHardCompute(&chr))
            {
                if (chr.State != CHSTATE_0 && chr.State != CHSTATE_6)
                    continue;
            }
            else
            {
                HardComputeUnit = &chr;
                if (!FUN_00416934(&chr))
                    continue;
            }
        }
        
        switch(chr.State)
        {
            case CHSTATE_0:
            case CHSTATE_6:
            {
                if (!FUN_00413de4(&chr))
                    FUN_004143dc(&chr, chr.State);
            }
            break;
                
            case CHSTATE_1:
            case CHSTATE_7:
            {
                if (chr.field17_0x13[0] == 0xff)
                {
                    chr.field2_0x2 |= 0x40;
                }
                else if (chr.Direction == chr.field17_0x13[0])
                {
                    if (FUN_00413de4(&chr) == 0 && FUN_00415cd0(&chr) == 0)
                    {
                        chr.POS = FUN_00439ba0(chr.Tile);
                        
                        for(int32_t i = 0; i < chr.field17_0x13.size() - 1; ++i)
                            chr.field17_0x13[i] = chr.field17_0x13[i + 1];
                        chr.field17_0x13.back() = 0xff;
                        
                        if (chr.field17_0x13[0] == 0xff)
                        {
                            if (chr.Tile == chr.MoveTile)
                            {
                                chr.field2_0x2 &= 0xf7;
                                FUN_00410728(&chr);
                            }
                            else
                            {
                                chr.field2_0x2 |= 0x40;
                                chr.Frame = 0xff;
                                FUN_00413de4(&chr);
                            }
                        }
                        else
                            FUN_004143dc(&chr, chr.State);
                    }
                }
                else
                    FUN_004391bc(&chr, chr.field17_0x13[0]);
            }
            break;
                
            case CHSTATE_2:
            case CHSTATE_8:
            {
                if (FUN_00413de4(&chr) == 0)
                {
                    CurrentVillage->Flags |= 0x15;
                    FUN_004248a0(mchr);
                    FUN_00410728(&chr);
                }
            }
            break;
                
            case CHSTATE_3:
            case CHSTATE_9:
            {
                if (chr.Frame != chr.FrameCount - 1)
                {
                    if (chr.Frame == chr.FrameCount - 2)
                    {
                        CurrentVillage->Flags |= 0x15;
                        FUN_004248a0(mchr);
                        chr.field2_0x2 = 0;
                        chr.ClassID |= CLASS_BIT80;
                        _currentMap->FootMap.At(chr.Tile).ID = 0;      
                    }
                    FUN_00413de4(&chr);
                }
            }
            break;
                
            case CHSTATE_4:
            {
                if (chr.Frame == 0)
                {
                    int32_t dir = FUN_00439aa4(chr.MoveTile, chr.Tile);
                    
                    if (dir != chr.Direction)
                    {
                        FUN_004391bc(&chr, dir);
                        break;
                    }
                }
                
                if ((chr.Frame == chr.FrameCount - 6) && FUN_0041bff8(&chr))
                {
                    ItemInfo &arrows = _state.Items.at(chr.Arrows);
                    arrows.Concentration -= 1;
                    if (arrows.Concentration <= 0)
                    {
                        chr.Arrows = 0;
                        arrows.TypeID = -1;   
                    }
                }
                
                if (!FUN_00413de4(&chr))
                {
                    chr.field2_0x2 &= 0xfb;
                    FUN_00414ab4(&chr);
                }
            }
            break;
            
            case CHSTATE_5:
            {
                if (!FUN_00415cd0(&chr)) 
                {
                    int32_t dir = FUN_0043976c(chr.MoveTile, chr.Tile);
                    if (chr.Direction != dir)
                    {
                        FUN_004391bc(&chr, dir);
                    }
                    else
                    {
                        if (FUN_00413de4(&chr) == 0) 
                        {
                            Character &c = _state.Characters.at(chr.EnemyCharID - 1);
                            
                            if ((c.ClassID & CLASS_BIT80) == 0 && c.State != CHSTATE_9 && c.State != CHSTATE_3)
                            {
                                int32_t iVar6 = FUN_0041b100(&c, &chr);
                                if (iVar6) 
                                {
                                    int32_t iVar66 = FUN_0041c480(&c, &chr, iVar6);
                                    
                                    if (iVar66 == 1)
                                    {
                                        chr.EnemyCharID = 0;
                                        c.field2_0x2 = 0;
                                        c.HP = 0;
                                        int32_t iVar61 = FUN_0041b70c(&c);
                                        if ((c.field_0x3 & 4) == 0)
                                            FUN_004143dc(&c, CHSTATE_9);
                                        else
                                            FUN_004143dc(&c, CHSTATE_3);
                                        
                                        FUN_0041079c(&chr, iVar61 / 4);
                                        chr.Fehtovanie += iVar61;
                                        chr.field2_0x2 &= 0xfd;
                                        FUN_00418510(&c, iVar61, chr.Level);
                                        FUN_00410728(&chr);
                                        break;
                                    }
                                    
                                    if (iVar66 == 0)
                                    {
                                        if ((c.field_0x3 & 4) == 0)
                                            FUN_004143dc(&c, CHSTATE_8);
                                        else
                                            FUN_004143dc(&c, CHSTATE_2);
                                    }
                                    //if (c.MapCharID == _mainCharacter->MapCharID)
                                    //    FUN_0042d574(&c);
                                }
                                
                                FUN_004143dc(&chr, chr.State);
                            }
                            else 
                            {
                                chr.EnemyCharID = 0;
                                FUN_00410728(&chr);
                            }
                        }
                    }
                    
                }
            }
            break;
                    
        }
    }
}


Engine::Character * Engine::FUN_00420178(MapChar *mchar)
{
    for(int i = 0; i < mchar->GroupSize; ++i)
    {
        Character &chr = _state.Characters.at(mchar->CharacterIndex + i);
        if ((chr.ClassID & CLASS_BIT80) == 0 && chr.State != CHSTATE_9 && chr.State != CHSTATE_3)
        {
            if ((chr.field2_0x2 & 1) && chr.EnemyCharID)
            {
                Character &chr2 = _state.Characters.at(chr.EnemyCharID - 1);

                if ((chr2.ClassID & CLASS_BIT80) == 0 && chr2.State != CHSTATE_9 && chr2.State != CHSTATE_3)
                    return &chr2;
            }
        }
    }
    return nullptr;
}

void Engine::FUN_00410450(Character *pchar)
{
    pchar->field2_0x2 |= 0x40;
    if (pchar->State == CHSTATE_4 || pchar->State == CHSTATE_5 || 
        pchar->State == CHSTATE_2 || pchar->State == CHSTATE_8 || 
        pchar->field2_0x2 == 0x40)
    {
        if (pchar->field_0x3 & 4)
            FUN_004143dc(pchar, CHSTATE_0);
        else
            FUN_004143dc(pchar, CHSTATE_6);
    }
}

bool Engine::FUN_00411fd0(Character *pchar)
{
    if (pchar->State == CHSTATE_5)
    {
        if (FUN_00411758(pchar, pchar->MoveTile))
            return false;
        
        Character &enm = _state.Characters.at(pchar->EnemyCharID - 1);
        if ((enm.ClassID & CLASS_BIT80) || enm.State == CHSTATE_9 || enm.State == CHSTATE_3)
            pchar->EnemyCharID = 0;
                    
        FUN_00410450(pchar);
        return true;
    }
    
    if ((pchar->field2_0x2 & 1) == 0 || pchar->State != CHSTATE_1)
        return false;

    Character &enm = _state.Characters.at(pchar->EnemyCharID - 1);
    if ((enm.ClassID & CLASS_BIT80) || enm.State == CHSTATE_9 || enm.State == CHSTATE_3)
    {
        pchar->EnemyCharID = 0;
        FUN_00410450(pchar);
        return true;
    }

    if (pchar->MoveTile == enm.Tile)
        return false;
    
    Common::Rect rect = _state.MapChar_ARRAY.at(pchar->MapCharID).someRect;
    
    if (enm.Tile.y < rect.top  || enm.Tile.y > rect.bottom  ||
        enm.Tile.x < rect.left || enm.Tile.x > rect.right )
    {
        pchar->EnemyCharID = 0;
        FUN_00410450(pchar);
        return true;
    }
    
    int32_t iVar5 = FUN_0041165c(enm.Tile, pchar->MoveTile);
    if (iVar5 > -1)
    {
        int32_t local_1c = 0;
        for (local_1c = 0; local_1c < pchar->field17_0x13.size(); ++local_1c)
        {
            if (pchar->field17_0x13[local_1c] == 0xff)
                break;
        }
        
        if (local_1c != 0)
        {
            local_1c--;
            
            int32_t dir = pchar->field17_0x13[local_1c];
            int32_t dir2 = dir;
            
            if (dir < 4)
                dir2 = dir + 4;
            else
                dir2 = dir - 4;
                        
            Common::Point local_20;
            FUN_0043ecba(&local_20, pchar->MoveTile, dir2);
            
            dir2 = FUN_0041165c(enm.Tile, local_20);
            if (dir2 > -1)
            {
                pchar->MoveTile = enm.Tile;
                pchar->field17_0x13[local_1c] = dir2;
                return false;
            }
            
            uint8_t local_14 = PathGo[iVar5][dir];
            if (local_14 != 0xff)
            {
                pchar->MoveTile = enm.Tile;
                
                if ((local_14 & 0x80) == 0)
                    pchar->field17_0x13[local_1c] = local_14;
                else 
                    pchar->field17_0x13[local_1c] = 0xff;
                return false;
            }
        }
    }
    
    pchar->field2_0x2 |= 0x40;
    FUN_00410450(pchar);
    return true;
}



bool Engine::FUN_00412e00(Character *pchar)
{    
    if ((pchar->ClassID & CLASS_BIT80) || pchar->State == CHSTATE_9 || pchar->State == CHSTATE_3)
        return false;
    
    if (pchar->ClassID == 0x51 || pchar->ClassID == 0x52 || pchar->ClassID == 0x53)
        return true;
    
    MapChar &mchar = _state.MapChar_ARRAY.at( pchar->MapCharID );
    
    if (mchar.unk4 == 0)
    {
        if (pchar->State != CHSTATE_0 && pchar->State != CHSTATE_4 && pchar->State != CHSTATE_6)
        {
            if (pchar->field_0x3 & 4)
                FUN_004143dc(pchar, CHSTATE_0);
            else
                FUN_004143dc(pchar, CHSTATE_6);
                
        }
        return false;
    }
    
    if (mchar.unk4 < 2)
    {        
        MapChar &mchar2 = _state.MapChar_ARRAY.at(mchar.unk3);
                
        Character *tgtChar = nullptr;
        
        switch(pchar->ClassID)
        {
            case 0x41:
            case 0x4d:
            case 0x4f:
                tgtChar = FUN_00420178(&mchar);
                break;
                
            case 0x43:
            case 0x45:
                tgtChar = FUN_004203a0(&mchar2, &mchar, pchar);
                break;
                
            case 0x44:
            case 0x57:
            {
                for(int i = 0; i < 8; ++i)
                {
                    Common::Point p;
                    if (FUN_0043ecba(&p, pchar->Tile, i))
                    {
                        int32_t chid = _currentMap->FootMap.At(p).ID;
                        if (chid != 0 && chid != 0xFFF)
                        {
                            Character &ch = _state.Characters.at(chid - 1);
                            if (ch.MapCharID == mchar.unk3)
                            {
                                pchar->Direction = i;
                                pchar->EnemyCharID = chid;
                                pchar->MoveTile = ch.Tile;
                                pchar->field2_0x2 = 2;
                                
                                FUN_004143dc(pchar, CHSTATE_5);
                                return true;
                            }
                        }
                    }
                }
                return true;
            }
                
            case 0x46:
            case 0x4c:
            case 0x55:
            {
                if (pchar->State == 4 && pchar->Frame == 0)
                {
                    pchar->field2_0x2 &= 0xbf;
                    FUN_00413de4(pchar);
                    return false;
                }
                
                tgtChar = FUN_004203a0(&mchar2, &mchar, pchar);
            }
                break;
                
            default:
            {
                tgtChar = nullptr;
                if ((pchar->ClassID & CLASS_BIT40) == 0)
                {
                    if ((pchar->field_0x3 & 4) == 0) 
                    {
                        FUN_004105a0(pchar);
                        pchar->field_0x3 |= 4;
                        FUN_00414ab4(pchar);
                        return true;
                    }
                    
                    if (pchar->ArmorWeapons[ESLT_1] && pchar->Arrows) 
                    {
                        pchar->field_0x12 = ESLT_1;
                        
                        if (FUN_00410e84(pchar)) 
                        {
                            pchar->MoveTile = _state.Characters.at(pchar->EnemyCharID - 1).Tile;
                            pchar->field2_0x2 |= 4;
                            FUN_004143dc(pchar, CHSTATE_4);
                            pchar->field2_0x2 &= 0xbf;
                            return true;
                        }
                    }
                }
            }
                break;                
        }
                        
        if (!tgtChar)
            tgtChar = FUN_004202a8(&mchar2, pchar);
        
        if (!tgtChar)
            return false;
        
        pchar->EnemyCharID = tgtChar->Index + 1;
        if (FUN_004142d4(pchar, tgtChar->Tile, 1))
            return true;

        FUN_00414ab4(pchar);
        return false;
    }
    
    if (mchar.unk4 != 32)
        return false;
    
    if (pchar->field2_0x2 & 1)
    {
        Character *tgtChar = nullptr;
        MapChar *mchar2 = FUN_0041fd74(&mchar);
        if (mchar2)
        {
            switch(pchar->ClassID)
            {
                case 0x41:
                case 0x4d:
                case 0x4f:
                    tgtChar = FUN_00420178(&mchar);
                    break;
                    
                case 0x43:
                case 0x45:
                    tgtChar = FUN_004203a0(mchar2, &mchar, pchar);
                    break;
                    
                case 0x44:
                case 0x57:
                    return true;
                    
                case 0x46:
                case 0x4c:
                case 0x55:
                {
                    if (pchar->State == CHSTATE_4)
                        return false;
                    tgtChar = FUN_004203a0(mchar2, &mchar, pchar);
                }
                break;
                
                default:
                    break;
            }
        }
        
        if (tgtChar) 
        {
            pchar->EnemyCharID = tgtChar->Index + 1;
            if (FUN_004142d4(pchar, tgtChar->Tile, 1))
                return true;
            
            FUN_00414ab4(pchar);
        }
        pchar->field2_0x2 &= 0xfe;
    }
    
    Common::Rect r = mchar.spwnTlRect;
    
    if ( pchar->Tile.y >= r.top && pchar->Tile.y <= r.bottom &&
         pchar->Tile.x >= r.top && pchar->Tile.x <= r.right)  // SEEMS ERROR r.top -> r.left!
    {
        if (pchar->State != CHSTATE_0 && pchar->State != CHSTATE_6)
            FUN_00414ab4(pchar);
        return false;
    }
    
    Common::Point spos ((r.right + r.left) / 2,
                        (r.bottom + r.top) / 2);

    const int32_t rh = (r.bottom - r.top) - 1;
    const int32_t rw = (r.right - r.left) - 1;
    
    if (rand() % 2 == 0)
    {
        if (rh / 4 >= 2)
            spos.y -= rand() % (rh / 4);
        
        if (rw / 4 >= 2)
            spos.x -= rand() % (rw / 4);
    }
    else
    {
        if (rh / 4 >= 2)
            spos.y += rand() % (rh / 4);
        
        if (rw / 4 >= 2)
            spos.x += rand() % (rw / 4);
    }
    
    for(int32_t i = 0; i < rh * rw; ++i)
    {
        if (_currentMap->FootMap.At(spos).ID == 0)
        {
            if (FUN_004142d4(pchar, spos, 8))
                return true;
            
            FUN_00414ab4(pchar);
            return false;
        }
        
        spos.x++;
        if (spos.x >= (r.right - 1)) 
        {
            spos.x = r.left + 1;
            spos.y++;
            if (spos.y >= (r.bottom - 1)) 
                spos.y = r.top + 1;
        }
    }
    return false;
}


bool Engine::FUN_004101e4(Character *pchar, Common::Point *ot)
{
    switch(rand() % 4)
    {
    case 0:
        (*ot).y = -(ViewTiles.y * 2);
        (*ot).x = -ViewTiles.x;
        
        while ((*ot).x < ViewTiles.x)
        {
            Common::Point tmp = *ot + pchar->Tile;
            if (_currentMap->FootMap.IsIn(tmp) &&
                _currentMap->FootMap.At(tmp).ID == 0)
                break;
            
            (*ot).x++;
        }
        break;
        
    case 1:
        (*ot).y = ViewTiles.y * 2;
        (*ot).x = -ViewTiles.x;
        
        while ((*ot).x < ViewTiles.x)
        {
            Common::Point tmp = *ot + pchar->Tile;
            if (_currentMap->FootMap.IsIn(tmp) &&
                _currentMap->FootMap.At(tmp).ID == 0)
                break;
            
            (*ot).x++;
        }
        break;
        
    case 2:
        (*ot).x = -(ViewTiles.x * 2);
        (*ot).y = -ViewTiles.y;
        
        while ((*ot).y < ViewTiles.y)
        {
            Common::Point tmp = *ot + pchar->Tile;
            if (_currentMap->FootMap.IsIn(tmp) &&
                _currentMap->FootMap.At(tmp).ID == 0)
                break;
            
            (*ot).y++;
        }
        break;
        
    case 3:
        (*ot).x = ViewTiles.x * 2;
        (*ot).y = -ViewTiles.y;
        
        while ((*ot).y < ViewTiles.y)
        {
            Common::Point tmp = *ot + pchar->Tile;
            if (_currentMap->FootMap.IsIn(tmp) &&
                _currentMap->FootMap.At(tmp).ID == 0)
                break;
            
            (*ot).y++;
        }
        break;
    }
    
    if (ot->y == ViewTiles.y || ot->x == ViewTiles.x) // CHECK IT
        return false;
    
    return true;
}



void Engine::FUN_004123bc(MapChar *mchr)
{    
    for(int32_t i = 0; i < mchr->GroupSize; ++i)
    {
        Character &chr = _state.Characters.at(mchr->CharacterIndex + i);
        
        static std::set<int> __tstates({CHSTATE_2, CHSTATE_3, CHSTATE_9});
        
        if ( (chr.field2_0x2 & 0x40) && (__tstates.count(chr.State) == 0))
        {
            if (!SkipBecauseOfHardCompute(&chr))
            {
                if (chr.State != CHSTATE_0 && chr.State != CHSTATE_6)
                    continue;
            }
            else
            {
                HardComputeUnit = &chr;
                if (!FUN_00412e00(&chr))
                    continue;
            }
        }
        switch(chr.State)
        {
            case CHSTATE_0:
            case CHSTATE_6:
            {
                if (!FUN_00413de4(&chr))
                    FUN_004143dc(&chr, chr.State);
            }
            break;
            
            case CHSTATE_1:
            case CHSTATE_7:
            {
                if (chr.field15_0x11 == 0 || (_counter & chr.field15_0x11) == chr.field15_0x11)
                {
                    if (chr.field17_0x13[0] == 0xff)
                    {
                        chr.field2_0x2 |= 0x40;
                        continue;
                    }
                    
                    if (chr.Direction != chr.field17_0x13[0])
                    {
                        FUN_004391bc(&chr, chr.field17_0x13[0]);
                        continue;
                    }
                    
                    if (!FUN_00413de4(&chr))
                    {
                        if (chr.ClassID == 0x4d || chr.ClassID == 0x4f)
                        {
                            if (chr.field_0x12 == ESLT_0)
                            {
                                if (chr.FrameCount >= 2)
                                    chr.field_0x12 = (rand() % 2) * (rand() % chr.FrameCount) + ESLT_1;
                                else
                                    chr.field_0x12 = ESLT_1;
                                
                                continue;
                            }
                            
                            chr.field_0x12--;

                            if (chr.field_0x12 != 0)
                                continue;
                        }
                        
                        if (!FUN_00411fd0(&chr))
                        {
                            chr.POS = FUN_00439ba0(chr.Tile);
                            
                            for(int32_t i = 0; i < chr.field17_0x13.size() - 1; ++i)
                                chr.field17_0x13[i] = chr.field17_0x13[i + 1];
                            
                            chr.field17_0x13.back() = 0xff;
                            
                            if (chr.field17_0x13[0] == 0xff)
                            {
                                if (chr.Tile == chr.MoveTile)
                                {
                                    chr.field2_0x2 &= 0xf7;
                                    FUN_00410450(&chr);
                                }
                                else
                                {
                                    chr.field2_0x2 |= 0x40;
                                    chr.Frame = 0xff;
                                    FUN_00413de4(&chr);
                                }
                            }
                            else
                            {
                                FUN_004143dc(&chr, chr.State);
                            }
                        }
                    }
                }
            }
            break;
            
            case CHSTATE_2:
            case CHSTATE_8:
            {
                if (!FUN_00413de4(&chr))
                {
                    mchr->unk4 = 1;
                    FUN_00410450(&chr);
                }
            }
            break;
            
            case CHSTATE_3:
            case CHSTATE_9:
            {
                if (chr.Frame != chr.FrameCount - 1)
                {
                    if (chr.Frame == chr.FrameCount - 2)
                    {
                        mchr->unk4 = 1;
                        if (chr.ClassID == 0x4c && chr.field_0x12 != ESLT_0)
                        {
                            chr.HP = 1600;
                            chr.field_0x12--;
                            FUN_004143dc(&chr,4);
                        }
                        else
                        {
                            chr.field2_0x2 = 0;
                            chr.ClassID |= CLASS_BIT80;
                            _currentMap->FootMap.At(chr.Tile).ID = 0;
                        }
                    }
                    FUN_00413de4(&chr);
                }
            }
            break;
            
            case CHSTATE_4:
            {
                if (chr.ClassID & CLASS_BIT40)
                {
                    if (chr.Frame != 0)
                    {
                        if (chr.Frame == 1)
                            PlaySound(chr.ClassID * 8 - 463, ComputeVolume(chr.Tile), ComputePan(chr.Tile), 0);
                        
                        if (!FUN_00413de4(&chr))
                            FUN_00410450(&chr);
                    }
                }
                else
                {
                    if (chr.Frame == 0)
                    {
                        int32_t dir = FUN_00439aa4(chr.MoveTile, chr.Tile);
                        if (dir != chr.Direction)
                        {
                            FUN_004391bc(&chr, dir);
                            continue;
                        }
                    }
                    
                    if ((chr.Frame == chr.FrameCount - 6) & FUN_0041bff8(&chr))
                    {
                        ItemInfo &arrow = _state.Items.at(chr.Arrows);
                        arrow.Concentration--;
                        if (arrow.Concentration == 0)
                        {
                            chr.Arrows = 0;
                            arrow.TypeID = -1;
                        }
                    }
                    
                    if (!FUN_00413de4(&chr))
                    {
                        chr.field2_0x2 &= 0xfb;
                        FUN_00414ab4(&chr);
                    }
                }
            }
            break;
            
            case CHSTATE_5:
            {
                if (FUN_00411fd0(&chr))
                    break;
                
                int32_t dir = FUN_0043976c(chr.MoveTile, chr.Tile);
                if (dir != chr.Direction)
                {
                    FUN_004391bc(&chr, dir);
                    continue;
                }
                
                if (FUN_00413de4(&chr))
                    continue;
                
                Character &enm = _state.Characters.at(chr.EnemyCharID - 1);
                if ((enm.ClassID & CLASS_BIT80) || enm.State == CHSTATE_9 || enm.State == CHSTATE_3)
                {
                    chr.EnemyCharID = 0;
                    FUN_00410450(&chr);
                    continue;
                }
                
                int32_t local_1c = FUN_0041b100(&enm, &chr);
                if (local_1c != 0)
                {
                    local_1c = FUN_0041c480(&enm, &chr, local_1c);
                    if (local_1c == 1)
                    {
                        chr.EnemyCharID = 0;
                        enm.HP = 0;
                        enm.field2_0x2 = 0;
                        
                        int32_t local_14 = FUN_0041b70c(&enm);
                        
                        if (enm.field_0x3 & 4)
                            FUN_004143dc(&enm, CHSTATE_3);
                        else
                            FUN_004143dc(&enm, CHSTATE_9);
                        
                        FUN_0041079c(&chr, local_14 / 4);
                        
                        chr.Fehtovanie += local_14;
                        
                        FUN_00418510(&enm,0,0);
                        FUN_00410450(&chr);
                        break;
                    }
                    
                    if (local_1c == 0)
                    {
                        if (enm.field_0x3 & 4)
                            FUN_004143dc(&enm, CHSTATE_2);
                        else
                            FUN_004143dc(&enm, CHSTATE_8);
                    }
                    
                    //if (enm.MapCharID == _mainCharacter->MapCharID)
                    //    FUN_0042d574(&enm);
                }
                
                if (chr.ClassID == 0x49)
                {
                    Common::Point pt;
                    if (!FUN_004101e4(&chr, &pt) || !FUN_004142d4(&chr, chr.Tile + pt, 8))
                        FUN_004143dc(&chr, chr.State);
                }
                else
                    FUN_004143dc(&chr, chr.State);
            }
            break;
        }
    }
}

bool Engine::FUN_00413de4(Character *pchar)
{
    pchar->Frame++;
    
    if (pchar->Frame == pchar->FrameCount)
    {
        pchar->Frame--;
        return false;
    }

    if (pchar->ClassID & CLASS_BIT40)
    {
        if (pchar->State > CHSTATE_5)
            pchar->State -= 6;
        
        Resources::DynamicObject &obj = Res.DynObjects.at(pchar->CharacterBase);
        
        if (pchar->Frame >= pchar->FrameCount)
        {
            printf("FRM ERR!!!\n");
            pchar->FrameCount = obj.Seq[pchar->State][pchar->Direction].FrmCount;
            pchar->Frame = 0;
        }
        
        auto &frmData = obj.Seq[pchar->State][pchar->Direction].FrameData.at(pchar->Frame);
        
        pchar->pFrame = frmData.FrameID;
        pchar->imgOffset = frmData.ImgOffset;
        pchar->shdOffset = frmData.ShdOffset;
        pchar->wpnOffset = Common::Point();
        
        if (pchar->field_0x3 & 0x80)
            pchar->imgSize = Common::Point(1, 1);
        else
            pchar->imgSize = obj.Images.at(pchar->pFrame)->SW.Size();
    }
    else
    {
        Resources::CharacterSprites &chbase = Res.CharacterBases.at(pchar->CharacterBase);
        
        if (pchar->Frame >= pchar->FrameCount)
        {
            printf("FRM ERR!!!\n");
            pchar->FrameCount = chbase.Seq[pchar->State][pchar->Direction].FrmCount;
            pchar->Frame = 0;
        }
        
        auto &frmData = chbase.Seq[pchar->State][pchar->Direction].FrameData.at(pchar->Frame);
        
        pchar->pFrame = frmData.FrameID;
        pchar->imgOffset = frmData.ImgOffset;
        pchar->shdOffset = frmData.ShdOffset;
        pchar->wpnOffset = frmData.WpnOffset;
        pchar->imgSize = chbase.Images.at(pchar->pFrame)->SW.Size();
    }
    
    pchar->RecalcViewPos(GScrOff);
    return true;
}

bool Engine::SkipBecauseOfHardCompute(Character *chr)
{
    if (HardComputeCount < 3 && (!HardComputeUnit || chr->Index > HardComputeUnit->Index) )
        return true;
    return false;
}

bool Engine::FUN_0041f11c(MapChar *mchar, Character *pchar)
{
    if (FUN_00410e84(pchar))
        return true;
    
    if (FUN_0041ec0c(pchar))
        return true;

    if (pchar->field2_0x2 & 0x20)
        return FUN_0041f5d8(mchar, pchar);
    
    return false;
}

bool Engine::FUN_00410e84(Character *pchar)
{
    if ((pchar->field_0x12 == ESLT_1) && pchar->ArmorWeapons[ESLT_1] && pchar->Arrows)
    {
        MapChar *pMVar2 = FUN_0041fd74(&_state.MapChar_ARRAY.at(pchar->MapCharID));
        if (pMVar2 == nullptr)
            pchar->field2_0x2 &= 0xfb;
        else 
        {
            HardComputeCount++;
            for (int32_t i = 0; i < pMVar2->GroupSize; ++i)
            {
                Character &tchar = _state.Characters.at(pMVar2->CharacterIndex + i);
                if ((tchar.ClassID & CLASS_BIT80) == 0 && tchar.State != CHSTATE_9 && tchar.State != CHSTATE_3)
                {                 
                    int32_t iVar3 = FUN_0041130c(pchar, tchar.Tile, tchar.Index + 1);
                    if (0 < iVar3) 
                    {
                        pchar->EnemyCharID = tchar.Index + 1;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


Engine::Character * Engine::FUN_004203a0(MapChar *mchr1, MapChar *mchr2, Character *pchar)
{
    Character *selChar = nullptr;
    int32_t minval = std::numeric_limits<int32_t>::max();
    
    for(int32_t i = 0; i < mchr1->GroupSize; ++i)
    {
        Character &chr1 = _state.Characters.at(mchr1->CharacterIndex + i);
        if ((chr1.ClassID & CLASS_BIT80) == 0 && chr1.State != CHSTATE_9 && chr1.State != CHSTATE_3)
        {
            bool notFnd = true;
            
            for(int32_t j = 0; j < mchr2->GroupSize; ++j)
            {
                Character &chr2 = _state.Characters.at(mchr2->CharacterIndex + j);
                if ((chr2.ClassID & CLASS_BIT80) == 0 && chr2.State != CHSTATE_9 && chr2.State != CHSTATE_3)
                {
                    if ((chr2.field2_0x2 & 1) && chr2.EnemyCharID == chr1.Index + 1)
                    {
                        notFnd = false;
                        break;
                    }
                }
            }
            
            if (notFnd)
            {
                int32_t val = FUN_00439944(chr1.Tile, pchar->Tile);
                if (val < minval)
                {
                    selChar = &chr1;
                    minval = val;
                }
            }
        }
    }
    
    return selChar;
}



bool Engine::FUN_0041e384(Character *pchar)
{
    if (pchar->EnemyCharID > 0)
    {
        Character &ch = _state.Characters.at(pchar->EnemyCharID - 1);
        
        if ((ch.ClassID & CLASS_BIT80) == 0 && ch.State != CHSTATE_9 && ch.State != CHSTATE_3)
        {
            pchar->MoveTile = ch.Tile;
            return true;
        }
        
        pchar->EnemyCharID = 0;
    }
    
    MapChar *mchar = &_state.MapChar_ARRAY.at( pchar->MapCharID );
    MapChar *bmchar = FUN_0041fd74(mchar);
    if (!bmchar)
    {
        pchar->field2_0x2 &= 0xfe;
        return false;
    }
    
    Character *tgtChar = FUN_004203a0(bmchar, mchar, pchar);
    
    if (!tgtChar)
        tgtChar = FUN_004202a8(bmchar, pchar);
    
    if (!tgtChar)
        return false;
    
    pchar->EnemyCharID = tgtChar->Index + 1;
    pchar->MoveTile = tgtChar->Tile;
    pchar->field2_0x2 &= 0xfb;
    return true;
}

void Engine::FUN_0041ed8c(Character *pchar)
{
    if ((pchar->field2_0x2 & 4) == 0)
    {
        if ((pchar->field2_0x2 & 1) && !FUN_0041e384(pchar))
            FUN_00414ab4(pchar);
        else
        {
            if ((pchar->field2_0x2 & 9) == 0)
            {
                if (!FUN_0041ec0c(pchar))
                    FUN_00414ab4(pchar);
            }
            else 
            {
                _currentMap->FootMap.At(pchar->Tile).ID = 0;
                int32_t dist = FUN_0043edb7(pchar);
                _currentMap->FootMap.At(pchar->Tile).ID = pchar->Index + 1;
                
                if (dist == 0)
                {
                    if ((pchar->field2_0x2 & 8) != 0)
                        pchar->field2_0x2 &= ~8;
                    
                    FUN_00414ab4(pchar);
                }
                else 
                {
                    if (pchar->field_0x3 & 4)
                        FUN_004143dc(pchar, CHSTATE_1);
                    else 
                        FUN_004143dc(pchar, CHSTATE_7);
                    
                    pchar->field2_0x2 &= 0xbf;
                }
            }
        }
    }
    else if (pchar->field_0x12 == ESLT_1 && pchar->ArmorWeapons[ESLT_1] && pchar->Arrows)
    {
        if (!pchar->EnemyCharID && !FUN_00410e84(pchar))
            FUN_00414ab4(pchar);
        else
        {
            if (pchar->EnemyCharID > 0) 
            {
                Character &chr = _state.Characters.at(pchar->EnemyCharID - 1);
                if ((chr.ClassID & CLASS_BIT80) == 0 && chr.State != CHSTATE_9 && chr.State != CHSTATE_3)
                {
                    pchar->MoveTile = chr.Tile;
                    
                    int32_t tp = FUN_0041130c(pchar, pchar->MoveTile, pchar->EnemyCharID);
                    if (tp > 0)
                    {
                        FUN_004143dc(pchar, CHSTATE_4);
                        pchar->field2_0x2 &= 0xbf;
                        return;
                    }
                    
                    if (tp < 0)
                        pchar->field2_0x2 &= ~4;
                }
            }
            else if (pchar->EnemyCharID < 0)
            {
                if (!CurrentVillage)
                {
                    pchar->EnemyCharID = 0;
                    pchar->field2_0x2 &= 0xfb;
                    FUN_00414ab4(pchar);
                    return;
                }

                GameMap::Object &obj = _currentMap->MapObjects.at(-pchar->EnemyCharID - 1);
                if ((obj.unk3 & 1) != 0)
                {
                    int32_t tp = FUN_0041130c(pchar, pchar->MoveTile, pchar->EnemyCharID);
                    if (0 < tp)
                    {
                        FUN_004143dc(pchar, CHSTATE_4);
                        pchar->field2_0x2 &= 0xbf;
                        return;
                    }
                    
                    if (tp < 0)
                        pchar->field2_0x2 &= ~4;
                }
            }
            
            pchar->EnemyCharID = 0;
            
            if (pchar == _mainCharacter)
                pchar->field2_0x2 &= 0xfb;
            
            FUN_00414ab4(pchar);
        }
    }
    else
    {
        pchar->field2_0x2 &= ~4;
        FUN_00414ab4(pchar);
    }
}

Engine::MapChar *Engine::FUN_0041fd74(MapChar *mchar)
{
    if ((mchar->unk5 & 0x40) == 0) 
    {
        Character &pchar = _state.Characters.at(mchar->CharacterIndex);
        
        Common::Rect tmpRect = mchar->spwnTlRect;
        
        if (mchar->unk5 & 0x20) 
            tmpRect = Common::Rect(pchar.Tile - ViewTiles - ViewTiles / 2,
                                   pchar.Tile + ViewTiles + ViewTiles / 2);

        int32_t local_1c = std::numeric_limits<int32_t>::max();
        MapChar *selChar = nullptr;
        
        for (MapChar &tmchar : _state.MapChar_ARRAY)
        {
            if ( (tmchar.MapID == _currentMapID) && (&tmchar != mchar) &&
                 ((tmchar.unk5 & 0x80) == 0 || (tmchar.unk4 == 1)) &&
                 (tmchar.unk5 != 0x53) ) 
            {
                for (int32_t i = 0; i < tmchar.GroupSize; ++i) 
                {
                    Character &tchar = _state.Characters.at(tmchar.CharacterIndex + i);
                    if ((tchar.ClassID & CLASS_BIT80) == 0 && (tchar.State != CHSTATE_9) && (tchar.State != CHSTATE_3))
                    {
                        if (tchar.Tile.y >= tmpRect.top && tchar.Tile.y <= tmpRect.bottom &&
                            tchar.Tile.x >= tmpRect.left && tchar.Tile.x <= tmpRect.right)
                        {
                            int32_t d = FUN_00439944(tchar.Tile, pchar.Tile);
                            if (d < local_1c) 
                            {
                                selChar = &tmchar;
                                local_1c = d;
                            }
                        }
                    }
                }
            }
        }
        
        return selChar;
    }
    
    if (mchar->MapID != _currentMapID) 
        return nullptr;
    
    for (int32_t i = 0; i < _mainMapChar->GroupSize; ++i) 
    {
        Character &tchar = _state.Characters.at(_mainMapChar->CharacterIndex + i);
        
        if ((tchar.ClassID & CLASS_BIT80) == 0 && tchar.State != CHSTATE_9 && tchar.State != CHSTATE_3)
        {
            if (tchar.Tile.y >= mchar->someRect.top && tchar.Tile.y <= mchar->someRect.bottom &&
                tchar.Tile.x >= mchar->someRect.left && tchar.Tile.x <= mchar->someRect.right) 
            {
                return _mainMapChar;
            }
        }
    }
    return nullptr;
}

int32_t Engine::FUN_00439944(Common::Point p1, Common::Point p2)
{
    Common::Point d = p1.AbsDistance(p2);
    
    if (d.y < d.x) 
    {
        if (d.y > 1)
            d.x++;
        
        return d.x;
    }
    
    if (d.x > 1)
        d.y++;
    return d.y;
}


int32_t Engine::FUN_00439aa4(Common::Point p1, Common::Point p2)
{
    int32_t dy = Common::ABS(p2.y - p1.y);
    int32_t dqy = dy / 4;
    int32_t dx = Common::ABS(p2.x - p1.x);
    
    if (dx / 2 < dy) 
    {
        if (p2.y < p1.y) 
        {
            if (dqy >= dx) 
                return 6;
            
            if (p2.x < p1.x) 
                return 5;
            else 
                return 7;
        }
        else 
        {
            if (dqy >= dx) 
                return 2;
            
            if (p2.x < p1.x)
                return 3;
            else
                return 1;
        }
    }
    else if (p1.x >= p2.x)
        return 4;
    
    return 0;
}




int32_t Engine::FUN_0041130c(Character *pchar, Common::Point t, int32_t chIndex)
{
    GFX::Image *img = nullptr;
    Common::Point pt;
    if (chIndex < 0) 
    {
        GameMap::Object &obj = _currentMap->MapObjects.at(-chIndex - 1);
        Resources::SimpleObject &pSimple = Res.SimpleObjects.at(obj.ObjId);
        
        img = pSimple.Images.at(obj.CurrentFrame);
        pt = obj.Pos + pSimple.Offsets.at(obj.CurrentFrame).Offset;
    }
    
    if (FUN_00439944(t, pchar->Tile) < 3)
        return -1;

    HardComputeCount++;
    
    ItemInfo &pIVar1 = _state.Items.at(pchar->ArmorWeapons[ESLT_1]);

    int32_t local_24 = ArmorWeaponInfo.at(pIVar1.InfoID).unk4;
    
    Common::Point tpt = FUN_00439ba0(pchar->Tile) + O45ad30.at( FUN_00439aa4(t, pchar->Tile) );
    Common::Point tpt2 = FUN_00439ba0(t) - Common::Point(0, 30);
    
    Common::FPoint opt = Common::FPoint(tpt2.AbsDistance(tpt)) * 0.25 + Common::FPoint(0.001, 0.001);
    
    if (opt.y < opt.x)
        opt = Common::FPoint(tpt2 - tpt) / opt.x;
    else
        opt = Common::FPoint(tpt2 - tpt) / opt.y;
    
    Common::FPoint stpt = tpt;
    
    for(int32_t i = 0; i < local_24; ++i)
    {
        for(int32_t j = 0; j < 8; ++j)
        {
            _tracePos = stpt + Common::Point(0, 30);
            Common::Point cpt = FUN_00439bdc(_tracePos);
            
            if (cpt == t)
                return 1;
            
            
            GameMap::Cell &foot = _currentMap->FootMap.At(cpt);
            
            if ( foot.Flags & 0xC )
            {
                if (chIndex > 0)
                    return 0;
                
                if ( CheckTraceImage(img, pt) )
                {
                    pchar->MoveTile = cpt;
                    return 1;
                }
                
                return 0;
            }
            
            if (foot.ID && foot.ID != 0xFFF)
                return 0;
            
            stpt += opt;            
        }
    }
    
    return 0;
}

int32_t Engine::FUN_0041ec0c(Character *pchar)
{
    if ((pchar->field2_0x2 & 9) == 0)
    {
        for (int32_t i = 0; i < 8; ++i)
        {
            Common::Point pt;
            if (FUN_0043ecba(&pt, pchar->Tile, i))
            {
                int32_t chID = _currentMap->FootMap.At(pt).ID;
                if ((chID != 0xfff) && (chID != 0))
                {
                    Character &ch = _state.Characters.at(chID - 1);
                    
                    if (ch.MapCharID != pchar->MapCharID && 
                        (ch.field2_0x2 & 1) &&
                        ch.EnemyCharID == pchar->Index + 1)
                    {
                        pchar->EnemyCharID = chID;
                        pchar->MoveTile = pt;
                        pchar->field2_0x2 &= 0xfb;
                        pchar->field2_0x2 |= 2;
                        pchar->field17_0x13[0] = 0xff;
                        pchar->field_0x3 |= 4;
                        
                        FUN_004143dc(pchar, CHSTATE_5);
                        
                        if (pchar == _mainCharacter)
                            FUN_004290ac(1,10);
                        
                        return chID;
                    }
                }
            }
        }
    }
    return 0;
}



void Engine::DrawElmQueue()
{
    static const Common::Point ElmBox[7] = 
    { //{3, 270},  
      {3, 340},
      //{1, 299},  
      {1, 374},
      //{59, 299}, 
      {72, 372},
      //{1, 364},  
      {1, 455},
      //{64, 364}, 
      {78, 457},
      //{1, 411},  
      {1, 514},
      //{56, 411}, 
      {70, 514} };
    
    //ImgQueue1(_menuImages.at(3), Common::Point(), Common::Rect(0, ElmBox[0].y - 2, MAPRECT.left - 1, SCREENRESY));
    
    for (int32_t i = 0; i < 7; ++i)
    {
        if ((DrawElm[i] & 0x80) == 0)
            ImgQueue2(_menuImages.at(DrawElm[i] & 0x7F), ElmBox[i], Common::Rect());
        else 
            ImgQueue2(_menuImages.at(DrawElm[i] & 0x7F), ElmBox[i].Add(0, -2), Common::Rect());
    }
}


void Engine::FUN_0042d574(Character *pchar, int32_t index)
{
    constexpr const std::array<Common::Point, 10> facePos = 
    {{
    /*{3, 1},*/{6, 4},
    /*{58, 1},*/{74, 4},
    /*{3, 55},*/{6, 71},
    /*{58, 55},*/{74, 71},
    /*{3, 109},*/{6, 139},
    /*{58, 109},*/{74, 139},
    /*{3, 163},*/{6, 206},
    /*{58, 163},*/{74, 206},
    /*{3, 217},*/{6, 273},
    /*{58, 217}*/{74, 273}    
    }};
            
    Common::Point pos = facePos.at(index);
    int hplvl = 0;
    if ((pchar->ClassID & CLASS_BIT40) == 0)
    {
        if ((pchar->ClassID & CLASS_BIT80) == 0)
        {
            if (pchar->HP <= 400)
                hplvl = 3;
            else if (pchar->HP <= 800)
                hplvl = 2;
            else if (pchar->HP <= 1200)
                hplvl = 1;
            else
                hplvl = 0;
        }
        else
            hplvl = 4;
        
        if (pchar == _mainCharacter)
        {
            hplvl += pchar->CharacterBase * 25 + 215;
            
            if (pchar->State == CHSTATE_8 || pchar->State == CHSTATE_2)
                ImgQueue2(_menuImages.at(hplvl), pos, Common::Rect(), vec3f(24, 0, 0));
            else
                ImgQueue2(_menuImages.at(hplvl), pos, Common::Rect());
        }
        else
        {
            hplvl += pchar->CharacterBase * 25 + (pchar->ClassID & CLASS_MASK) * 5 + 195;
            if (pchar->State == CHSTATE_8 || pchar->State == CHSTATE_2)
                ImgQueue2(_menuImages.at(hplvl), pos, Common::Rect(), vec3f(24, 0, 0));
            else
                ImgQueue2(_menuImages.at(hplvl), pos, Common::Rect());
        }
    }
    
    if (IsSelectedCharacter(pchar))
        ImgQueue2(_menuImages.at(173), pos, Common::Rect());
    
    if (pchar->HP)
    {
        constexpr const int32_t hhp = 40; // 32
        int32_t pixH = hhp * pchar->HP / 1600;
        if (pixH == 0)
            pixH = 1;

        int32_t imgId = 174;
        if (pchar->Otravlenie)
            imgId = 175;
        else if (pchar->HP <= 400)
            imgId = 176;
        else if (pchar->HP <= 800)
            imgId = 177;
        else
            imgId = 174;            
        
        Common::Point outpos = pos + Common::Point(6, 10); // (4, 8)
        ImgQueue2(_menuImages.at(imgId), outpos, Common::Rect(outpos.x, outpos.y + hhp - pixH, MAPRECT.right, MAPRECT.bottom));
    }
}



void Engine::FUN_004290ac(int elmid, int imgid)
{
    DrawElm[elmid] = imgid | 0x80;
    DWORD_00a3e74c |= 1;
}



bool Engine::FUN_0041f5d8(MapChar *mchar, Character *pchar)
{
    if ((mchar->unk5 & 0x40) == 0) 
    {
        Character &mpchar = _state.Characters.at(mchar->CharacterIndex);
        
        int32_t iVar8 = FUN_00439944(mpchar.Tile, pchar->Tile);
        if (iVar8 < 10) 
            return false;

        int uVar7 = mpchar.Tile.y & 1;
        int32_t local_18 = FUN_00439aa4(pchar->Tile, mpchar.Tile);
        
        int32_t local_14 = local_18 + -1;
        if (local_14 < 0)
            local_14 = 7;
        
        for (int32_t k = 0; k < 4; ++k)
        {
            int local_28 = rand() % 12;
            for (int32_t i = 0; i < 12; ++i)
            {
                Common::Point tp = mpchar.Tile + PlaceDOff[uVar7][local_18][local_28];
                if (tp.y > 0 && tp.y < 319 &&
                    tp.x > 0 && tp.x < 159 &&
                    _currentMap->FootMap.At(tp).ID == 0)
                {
                    pchar->MoveTile = tp;
                    pchar->field2_0x2 |= 0x48;
                    pchar->field17_0x13[1] = 0xff;
                    return true;
                }

                tp = mpchar.Tile + PlaceDOff[uVar7][local_14][local_28];
                if (tp.y > 0 && tp.y < 319 &&
                    tp.x > 0 && tp.x < 159 &&
                    _currentMap->FootMap.At(tp).ID == 0)
                {
                    pchar->MoveTile = tp;
                    pchar->field2_0x2 |= 0x48;
                    pchar->field17_0x13[1] = 0xff;
                    return true;
                }

                local_28 = (local_28 + 1) % 12;
            }
            
            local_18 = (local_18 + 1) % 8;
            
            local_14--;
            if (local_14 < 0)
                local_14 = 7;
        }
        return false;
    }
        
    if (pchar->Tile.y >= mchar->spwnTlRect.top && 
        pchar->Tile.y <= mchar->spwnTlRect.bottom && 
        pchar->Tile.x >= mchar->spwnTlRect.left && 
        pchar->Tile.x <= mchar->spwnTlRect.right) 
    {
        pchar->field2_0x2 = 0x40;
         
        bool notfound = true;

        for(int32_t i = 0; i < mchar->GroupSize; ++i)
        {
            Character &ch = _state.Characters.at(mchar->CharacterIndex + i);
            if ( !(
                 (ch.ClassID & CLASS_BIT80) ||
                 (ch.Tile.y >= mchar->spwnTlRect.top &&
                  pchar->Tile.y <= mchar->spwnTlRect.bottom &&  // Seems ERROR!!! must be ch->Tile
                  pchar->Tile.x >= mchar->spwnTlRect.left &&    // Seems ERROR!!! must be ch->Tile
                  pchar->Tile.x <= mchar->spwnTlRect.right) ) )   // Seems ERROR!!! must be ch->Tile
            {
                notfound = false;
                break;
            }
        }
        
        if (notfound)
        {
            mchar->unk4 = 0x80;
            return false;
        }
    }

    int32_t dy = 0;
    int32_t dx = 0;
    
    int32_t local_24 = (mchar->spwnTlRect.bottom - mchar->spwnTlRect.top) - 1;
    if (local_24 / 4 >= 2)
        dy = rand() % (local_24 / 4);

    int32_t local_20 = (mchar->spwnTlRect.right - mchar->spwnTlRect.left) - 1;
    if (local_20 / 4 >= 2)
        dx = rand() % (local_20 / 4);
    
    if ( rand() % 2 == 0 )
    {
        dy = -dy;
        dx = -dx;
    }
    
    Common::Point pt =   Common::Point((mchar->spwnTlRect.right + mchar->spwnTlRect.left) / 2,
                                       (mchar->spwnTlRect.bottom + mchar->spwnTlRect.top) / 2)
                       + Common::Point(dx, dy);
    
    for(int32_t i = 0; i < local_24 * local_20; ++i)
    {
        if (_currentMap->FootMap.At(pt).ID == 0)
        {
            pchar->MoveTile = pt;
            pchar->field2_0x2 |= 0x48;
            pchar->field17_0x13[1] = 0xff;
            return true;
        }
        
        pt.x++;
        if (pt.x >= (mchar->spwnTlRect.right - 1) ) 
        {
            pt.x = mchar->spwnTlRect.left + 1;
            pt.y++;
            
            if (pt.y >= (mchar->spwnTlRect.bottom - 1) )
                pt.y = mchar->spwnTlRect.top + 1;
        }
    }
    return false;
}


void Engine::ChangeWeapon(int32_t wpnId)
{
    PlaySound(4, 0, 0, 0);
    FUN_00429194(true);
    
    int32_t num = 1;
    if (_KeyState[KEYFN_SHIFT])
        num = 10;
    
    for(int32_t i = 0; i < num; ++i) 
    {
        Character *pchar = SelectedCharacters[i];
        if (!pchar)
            break;
        
        if ((pchar->ClassID & CLASS_BIT80) || pchar->State == CHSTATE_9 || pchar->State == CHSTATE_3)
            continue;

        switch(wpnId)
        {
            case 0:
                if (pchar->ArmorWeapons[ESLT_0])
                    pchar->field_0x12 = ESLT_0;
                break;
                
            case 1:
                if (pchar->ArmorWeapons[ESLT_1])
                    pchar->field_0x12 = ESLT_1;
                break;
            
            case 2:
                if (pchar->ArmorWeapons[ESLT_2])
                    pchar->field_0x12 = ESLT_2;
                break;
            
            default:
                if (pchar->ArmorWeapons[ESLT_0] || pchar->ArmorWeapons[ESLT_1] || pchar->ArmorWeapons[ESLT_2])
                {
                    switch(pchar->field_0x12)
                    {
                        case ESLT_0:
                            if (pchar->ArmorWeapons[ESLT_1])
                                pchar->field_0x12 = ESLT_1;
                            else if (pchar->ArmorWeapons[ESLT_2])
                                pchar->field_0x12 = ESLT_2;
                            break;
                        
                        case ESLT_1:
                            if (pchar->ArmorWeapons[ESLT_2])
                                pchar->field_0x12 = ESLT_2;
                            else if (pchar->ArmorWeapons[ESLT_0])
                                pchar->field_0x12 = ESLT_0;
                            break;
                            
                        case ESLT_2:
                            if (pchar->ArmorWeapons[ESLT_0])
                                pchar->field_0x12 = ESLT_0;
                            else if (pchar->ArmorWeapons[ESLT_1])
                                pchar->field_0x12 = ESLT_1;
                            break;
                    }
                }
            break;
        }
        
        if (pchar->field_0x12 != ESLT_1 && (pchar->field2_0x2 & 4))
        {
            pchar->field2_0x2 &= ~4;
            FUN_00414ab4(pchar);
        }
    }

    if (_playScreenID == PLSCREEN_3)
    {
        //PlayChangeScreen(PLSCREEN_3);
        FUN_0042f50c(CharInfoCharacter, 0);
    }
    else if (DisplayInvOfCharID != 0) {
        FUN_0042f50c(&_state.Characters.at(DisplayInvOfCharID - 1), 0);
    }
    
    FUN_004290ac(0, DrawElm[0]);
}



void Engine::FUN_0042179c()
{
    PlaySound(4, 0, 0, 0);
    FUN_00429194(true);
    
    if (!SelectedCharacters[0])
        return;
        
    bool wpnState = (SelectedCharacters[0]->field_0x3 & 4) == 0;
    
    for (int32_t i = 0; i < 10; ++i)
    {
        Character *pchar = SelectedCharacters[i];
        if (!pchar)
            break;
        
        if ((pchar->ClassID & CLASS_BIT80) || pchar->State == CHSTATE_9 || pchar->State == CHSTATE_3)
            continue;

        if (wpnState)
        {
            if ((pchar->field_0x3 & 4) == 0)
            {
                pchar->State -= 6;
                pchar->field_0x3 |= 4;

                if (pchar->State == CHSTATE_7 || pchar->State == CHSTATE_1)
                    FUN_00414078(pchar);
                else
                    FUN_004143dc(pchar, pchar->State);
            }
        }
        else if (pchar->field_0x3 & 4)
        {
            if (pchar->State < CHSTATE_4)
                pchar->State += 6;
            else
                pchar->State = CHSTATE_6;
            
            pchar->field2_0x2 &= ~5;
            
            if (pchar->field2_0x2 == 0)
                pchar->field2_0x2 = 0x40;
            
            pchar->field_0x3 &= ~4;
            
            if (pchar->State == CHSTATE_7 || pchar->State == CHSTATE_1)
                FUN_00414078(pchar);
            else
                FUN_004143dc(pchar, pchar->State);
        }
    }
    
    if (SelectedCharacters[0]->field_0x3 & 4)
        FUN_004290ac(1, 10);
    else
        FUN_004290ac(1, 15);    
}



void Engine::FUN_004226c0()
{
    PlaySound(4, 0, 0, 0);
    FUN_00429194(true);
    FUN_004290ac(3, 11);
    
    for (int32_t i = 1; i < _mainMapChar->GroupSize; ++i)
    {
        Character &chr = _state.Characters.at(_mainCharacter->Index + i);
        if ((chr.ClassID & CLASS_BIT80) == 0 && chr.State != CHSTATE_9 && chr.State != CHSTATE_3)
        {
            chr.field2_0x2 = 0x60;
            chr.field17_0x13[1] = 0xff;
        }
    }
}



void Engine::FUN_00421698()
{
    PlaySound(4, 0, 0, 0);
    DrawElm[1] = 10;
    FUN_00429194(true);
    FUN_004290ac(4, 7);
    
    for (int32_t i = 0; i < _mainMapChar->GroupSize; ++i)
    {
        Character &chr = _state.Characters.at(_mainCharacter->Index + i);
        if ((chr.ClassID & CLASS_BIT80) == 0 && chr.State != CHSTATE_9 && chr.State != CHSTATE_3)
        {
            chr.EnemyCharID = 0;
            if (chr.field_0x12 == ESLT_1)
                chr.field2_0x2 = 0x44;
            else
                chr.field2_0x2 = 0x41;
            
            chr.field17_0x13[1] = 0xff;
            if ((chr.field_0x3 & 4) == 0)
            {
                chr.State -= 6;
                chr.field_0x3 |= 4;
                FUN_00414078(&chr);
            }
        }
    }
}


void Engine::FUN_0042459c()
{
    if (_playScreenID == PLSCREEN_0 || _playScreenID == PLSCREEN_2)
    {
        PlaySound(4, 0, 0, 0);
        FUN_004290ac(5, 12);
        
        if (_playScreenID == PLSCREEN_2)
        {
            _playScreenID = PLSCREEN_0;
            FUN_004292e4();
        }
        else
        {
            DWORD_00a3e7b4 = 0;
            PlayChangeScreen(PLSCREEN_2);
        }
    }
}



}