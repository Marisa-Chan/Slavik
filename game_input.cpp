#include "game.h"
#include "game_locale.h"

namespace Game {
    
void Engine::PlayHandleKey(int16_t keyCode)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
}

int32_t Engine::FUN_00439a28(Common::Point pt, const Common::PlaneVector<uint8_t> *mask)
{
    if (pt.x >= 0 && pt.y >= 0 && pt.x < mask->Width() && pt.y < mask->Height())
        return mask->At(pt);
    return 0;
}

int32_t Engine::FUN_004392f4()
{
    int32_t iVar4;
    int32_t id;
    
    if (_playScreenID == PLSCREEN_7)
    {
        int32_t iVar4 = GetMouseOnScreenBox(_uiMousePos, tagRECT_ARRAY_00465ab4);
        if (iVar4 < 0)
        {
            id = GetMouseOnScreenBox(_uiMousePos, tagRECT_ARRAY_00465a54);
            if (id < 0)
            {
                iVar4 = FUN_00439a28(_uiMousePos, Res.ScreenMask);
                if ((iVar4 < 30) || (iVar4 > 39) || (_mainMapChar->GroupSize <= iVar4 - 30))
                {
                    id = -1;
                }
                else
                {
                    id = (iVar4 - 30) * 0x100 + 30;
                }
            }
            else
            {
                id += 200;
            }
        }
        else if (_uiMousePos.x < tagRECT_ARRAY_00465ab4[0].left + ScrlBtnWidth)
        {
            id = iVar4 * 0x100 + 120;
        }
        else if (_uiMousePos.x < 6 * ItemInvSlotWidth + tagRECT_ARRAY_00465ab4[0].left + ScrlBtnWidth)
        {
            id = (((_uiMousePos.x - tagRECT_ARRAY_00465ab4[0].left) - ScrlBtnWidth) / ItemInvSlotWidth) * 0x100 + 50 + iVar4 * 0x10000;
        }
        else {
            id = iVar4 * 0x100 + 121;
        }
        return id;
    }

    if (_playScreenID == PLSCREEN_MAP)
    {
        Common::Point pt = _uiMousePos - MAPRECT.Pos() - SRect_ARRAY_0046280c.Pos();
        if ( pt.x >= 0 && pt.x < SRect_ARRAY_0046280c.w &&
             pt.y >= 0 && pt.y < SRect_ARRAY_0046280c.h )
        {
            iVar4 = FUN_00439a28(pt, Res.MapMask);
            if (MapsOpened[iVar4] != 0)
                return iVar4 * 0x100 + 0x103;
            
            return -1;
        }
    }
    
    if (_playScreenID == PLSCREEN_2)
    {
        id = GetMouseOnScreenBox(_uiMousePos, ScreenBox_ARRAY_0045b0ac);
        if (id > -1)
            return id * 0x100 + 202;
    }

    id = FUN_00439a28(_uiMousePos, Res.ScreenMask);
    if (id == 0)
    {
        if (_uiMousePos.x < GScrOff.x || _uiMousePos.y > MSGRECT.top)
            return -1;
    }
    else if (id < 8)
    {
        return id;
    }
    
    if (DisplayInvOfCharID != 0)
    {
        if (id > 119)
            return id;
        
        if (DAT_00a3e870 < _uiMousePos.x && DAT_00a3e88c < _uiMousePos.y && _uiMousePos.y < MSGRECT.top)
            return ((_uiMousePos.x - DAT_00a3e870) / ItemInvSlotWidth) * 0x100 + 50;
    }
    
    if (id < 30 || id > 39)
    {
        if (_playScreenID == PLSCREEN_1)
        {
            for (int32_t i = 0; i < qOut2.size(); ++i)
            {
                DlgTxt &t = qOut2.at(i);
                if (t.Rect.IsIn(_uiMousePos))
                    return 100 + i * 0x100;
            }
            return -1;
        }
        else if (_playScreenID == PLSCREEN_3)
        {
            if ((id < 8 || id > 19) && (id < 40 || id > 45))
                return -1;
        }
        else
            return 0;
        
        return id;
    }

    if (id - 30 < _mainMapChar->GroupSize)
        return (id - 30) * 0x100 + 30;
    
    return -1;
}

void Engine::PlayProcessMouse()
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    int32_t retval = FUN_004392f4();
    
    if (DAT_00a3e704)
    {
        return;
    }
    
    if (_mousePress & MOUSEB_L)
    {
        uint8_t uVar11 = retval & 0xff;
        
        switch (uVar11)
        {
            case 0:
            {
                if (_playScreenID == PLSCREEN_0)
                {
                    if (DAT_00a3e790 == 7)
                        DAT_00a3e790 = 4;

                    Common::Point tile = FUN_00439bdc(_mouseMapPos + _camera);
                    if (DAT_00a3e790 == 4)
                    {
                        for (int32_t i = 0; i < 10 && SelectedCharacters[i]; ++i)
                        {
                            Character *chr = SelectedCharacters[i];
                            chr->MoveTile = tile;
                            chr->field2_0x2 = 0x48;
                            if (chr != _mainCharacter)
                                chr->field2_0x2 |= 2;

                            chr->field17_0x13[1] = 0xff;
                            chr->field_0x3 &= 0xbf;
                        }
                    }
                    else
                    {
                        DAT_00a3e790 = 4;
                        //FUN_004138c8((uint)DAT_00a3e520->Tile.y,(uint)DAT_00a3e520->Tile.x,DWORD_00a3e528);
                    }
                }
            }
            break;
            
            case 1:
                ChangeWeapon(-1);
                break;
                
            case 2:
                FUN_0042179c();
                break;
                
            case 3:
            {
                if (_playScreenID == PLSCREEN_MAP && retval >= 0x100)
                {
                    int32_t mid = retval >> 8;
                    if (_mainMapChar->MapID != mid)
                    {
                        int32_t invpos = FUN_00434c90(0x15);
                        if (invpos)
                        {
                            FUN_00421d24(_mainCharacter, _mainCharacter->Inventory[invpos - 1]);

                            for (int32_t i = 0; i < _state.GS2ARRAYCount; ++i)
                            {
                                GS2& wrap = _state.GS2ARRAY.at(i);
                                if (wrap.TargetMapID == mid)
                                {
                                    _playScreenID = PLSCREEN_0;
                                    
                                    if (DisplayInvOfCharID)
                                        FUN_0042f50c(CharInfoCharacter, 0);
                                    
                                    _nextMapID = wrap.TargetMapID;
                                    _mainMapChar->spwnTlRect.top = wrap.field5_0x5;
                                    _mainMapChar->spwnTlRect.left = wrap.field6_0x7;
                                    _mainMapChar->spwnTlRect.bottom = wrap.field2_0x2;
                                    return;
                                }
                            }
                        }
                    }
                }
                else if (_playScreenID == PLSCREEN_0 || _playScreenID == PLSCREEN_MAP)
                {
                    FUN_00429194(true);
                    PlaySound(4, 0, 0, 0);
                    
                    FUN_004290ac(2, 14);
                    if (_playScreenID == PLSCREEN_MAP)
                    {
                        _playScreenID = 0;
                        FUN_004292e4();
                    }
                    else
                        PlayChangeScreen(PLSCREEN_MAP);
                }
            }
            break;
            
            case 4:
                FUN_004226c0();
            break;
            
            case 5:
                FUN_00421698();
            break;
            
            case 6:
                FUN_0042459c();
            break;
            
            case 7:
                ShowHideCharInfo();
            break;
            
            case 8:
                FUN_0041e500(ESLT_4);
            break;
            
            case 9:
            {
                if (DAT_00a3e790 == 4)
                {
                    if (CharInfoCharacter->ArmorWeapons[ESLT_0] == 0 ||
                       (CharInfoCharacter->field_0x12 != 0 && CharInfoCharacter->ArmorWeapons[ESLT_2] != 0))
                        FUN_0041e500(ESLT_2);
                    else
                        FUN_0041e500(ESLT_0);
                }
                else if (_state.Items.at(InfItemID).TypeID)
                    FUN_0041e500(ESLT_2);
                else 
                    FUN_0041e500(ESLT_0);
            }
            break;
            
            case 10:
                FUN_0041e500(ESLT_5);
            break;
            
            case 11:
                FUN_0041e500(ESLT_3);
            break;
            
            case 12:
                FUN_0041e500(ESLT_1);
            break;
            
            case 13:
                FUN_0041e500(ESLT_ARROWS);
            break;
            
            case 14: 
            case 15:
            case 16: 
            case 17:
            case 18:
                FUN_0041e778(retval - 14);
            break;
            
            case 19:
            {
                if ((DAT_00a3e7a0 == 0) || (DAT_00a3e790 == 4))
                    FUN_0041e96c();
                else if (DAT_00a3e790 == 8 &&
                        FUN_0041db64(InfPchar, &_state.Items.at(DAT_00a3e7a0), &_state.Items.at(InfItemID)) != 0)
                    DAT_00a3e790 = 4;
                else if (DAT_00a3e790 == 9 &&
                        FUN_00414e64(InfPchar, &_state.Items.at(DAT_00a3e7a0), &_state.Items.at(InfItemID)) != 0)
                    DAT_00a3e790 = 4;
                else
                {
                    FUN_0042f9b8(Locale::GameMessages[Locale::GMSM_UNUSABLE]);
                    FUN_00429194(1);
                }
            }
            break;
            
            case 30:
                
            break;
        }
    }
    else if (_mousePress & MOUSEB_R)
    {
        
    }
    else
    {
        if (_playScreenID == PLSCREEN_0)
        {
            Common::Point tile = FUN_00439bdc(_mouseMapPos + _camera);
            if (_currentMap->FootMap.At(tile).Flags & 0x2)
            {
                GS1* loot = GetLootByTile(tile);
                if (loot && loot->unk2 > -1)
                {
                    FUN_0042f9b8(Locale::GameMessages[Locale::GMSM_SMTH_LOOT]); //"Что-то лежит на земле"
                    return;
                }
            }
            if (MouseOnCharacter)
            {
                //printf("Mouse on 0x%X\n", MouseOnCharacter->ClassID);
                //Character::PrintCharHint(MouseOnCharacter);
                FUN_0042f9b8(MouseOnCharacter->GetCharHint());
                return;
            }
        }
        
        uint8_t uVar11 = retval & 0xff;
        switch(uVar11)
        {
            case 1:
                FUN_0042f9b8(Locale::GameMessages[Locale::GMSM_CHANGE_WPN]);
                return;
            
            case 2:
                FUN_0042f9b8(Locale::GameMessages[Locale::GMSM_DRAW_WEAPON]);
                return;
                
            case 3:
                if (_playScreenID == PLSCREEN_MAP && retval >= 0x100)
                    FUN_0042f9b8(Locale::MapNames[(retval >> 8) - 1]);
                else
                    FUN_0042f9b8(Locale::GameMessages[Locale::GMSM_MAP]);
                return;
                
            case 4:
                FUN_0042f9b8(Locale::GameMessages[Locale::GMSM_ALL_TO_ME]);
                return;
                
            case 5:
                FUN_0042f9b8(Locale::GameMessages[Locale::GMSM_ATTACK]);
                return;
            
            case 6:
                FUN_0042f9b8(Locale::GameMessages[Locale::GMSM_THINK_INFO]);
                return;
            
            case 7:
                FUN_0042f9b8(Locale::GameMessages[Locale::GMSM_CHAR_PANEL]);
                return;
                
            case 8:
                if (CharInfoCharacter->ArmorWeapons[ESLT_4]) 
                    FUN_0042f9b8(GetItemHint( &_state.Items.at(CharInfoCharacter->ArmorWeapons[ESLT_4]) ));
                return;
                
            case 9:
            {
                int32_t id = CharInfoCharacter->ArmorWeapons[ESLT_0];
                if (id == 0)
                    return;
                
                if (CharInfoCharacter->field_0x12 == 2)
                    id = CharInfoCharacter->ArmorWeapons[ESLT_2];
                 
                if (id == 0 || id == DAT_00a3e7a0)
                    return;
                
                FUN_0042f9b8(GetItemHint( &_state.Items.at(id) ));
                return;
            }
            
            case 10:
            {
                int32_t id = CharInfoCharacter->ArmorWeapons[ESLT_5];
                if (id)
                    FUN_0042f9b8(GetItemHint( &_state.Items.at(id) ));
                return;
            }
            
            case 11:
            {
                int32_t id = CharInfoCharacter->ArmorWeapons[ESLT_3];
                if (id)
                    FUN_0042f9b8(GetItemHint( &_state.Items.at(id) ));
                return;
            }
            
            case 12:
            {
                int32_t id = CharInfoCharacter->ArmorWeapons[ESLT_1];
                if (id)
                    FUN_0042f9b8(GetItemHint( &_state.Items.at(id) ));
                return;
            }
            
            case 13:
            {
                int32_t id = CharInfoCharacter->Arrows;
                if (id == 0 || id == DAT_00a3e7a0)
                    return;
                FUN_0042f9b8(GetItemHint( &_state.Items.at(id) ));
                return;
            }
            
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            {
                int32_t id = CharInfoCharacter->Accessories[uVar11 - 14];
                if (id)
                    FUN_0042f9b8(GetItemHint( &_state.Items.at(id) ));
                return;
            }
            
            case 19:
            {
                if (DAT_00a3e7a0)
                    FUN_0042f9b8(GetItemHint( &_state.Items.at(DAT_00a3e7a0) ));
                return;
            }
            
            case 30:
            {
                Character& chr = _state.Characters.at(_mainMapChar->CharacterIndex + (retval >> 8));
                if ((chr.ClassID & CLASS_BIT80) == 0 && chr.State != CHSTATE_9 && chr.State != CHSTATE_3)
                    FUN_0042f9b8(chr.GetCharHint());
                
                return;
            }
            
            case 50:
            {
                if (_playScreenID == PLSCREEN_7)
                {
    //                        iVar12 = (int)((uVar9 + iVar12 * -0x10000) - (uint)(iVar12 << 0xf < 0)) >> 0x10;
    //                        local_1c = ((int)(uVar9 & 0xffff) >> 8) + (*(int *)(&DAT_0083dd4a + iVar12 * 2) >> 0x10) & 0x1f;
    //                        local_20 = *(int *)(&DAT_0083dc4a + local_1c * 2 + iVar12 * 0x40) >> 0x10;
    //                        if (local_20 == 0) {
    //                            return;
    //                        }
    //                        PrintItemHint(_state.Items + local_20);
                    return;
                }
                
                if (_playScreenID == PLSCREEN_1 ||
                    _playScreenID == PLSCREEN_2 ||
                    _playScreenID == PLSCREEN_MAP)
                    return;
                
                int32_t id = _state.Characters.at(DisplayInvOfCharID - 1).Inventory.at( (InvPos + (retval >> 8)) & 0x1f );
                
                if (id && id != DAT_00a3e7a0)                
                    FUN_0042f9b8(GetItemHint( &_state.Items.at(id) ));
                return;
            }
            
            case 100:
                PlayChangeScreen(PLSCREEN_1);
                return;
        }
        
        if (_playScreenID == PLSCREEN_1)
        {
            PlayChangeScreen(PLSCREEN_1);
            return;
        }
    }
}
    
    
}