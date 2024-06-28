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
        
        if (_uiMousePos.x > DAT_00a3e870 && _uiMousePos.y > DAT_00a3e88c && _uiMousePos.y < MSGRECT.top)
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
    int32_t retval = FUN_004392f4();
    
    if (DAT_00a3e704)
    {
        int32_t id = retval >> 8;
        if ( ((_mousePress & MOUSEB_L) && (retval & 0xff) == 100 && DAT_00a3e704 - 1 == qOut2[id].Q1ID) ||
             SpeechBufferIndex == -1 ||
             FUN_00429c28(SpeechBufferIndex) == 0 )
        {
                DAT_00a3e704 = 0;
                qwsel1 = qOut2[id].Q1ID;
                FUN_0041f18c(&quest_1.at(qwsel1), id);
        }
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
                        FUN_004138c8(InfPchar->Tile, InfItemID);
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
                        _playScreenID = PLSCREEN_0;
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
            {
                Character &pCVar13 = _state.Characters.at((retval >> 8) + _mainMapChar->CharacterIndex);
                if (DAT_00a3e790 == 4)
                {
                    if ((pCVar13.ClassID & CLASS_BIT80) == 0 && pCVar13.State != CHSTATE_9 && pCVar13.State != CHSTATE_3)
                    {
                        if (_playScreenID != PLSCREEN_MAP && _playScreenID != PLSCREEN_2 && _playScreenID != PLSCREEN_1)
                        {
                            if (_playScreenID == PLSCREEN_7)
                            {
                                if (CharInfoCharacter != &pCVar13)
                                {
                                    for (int32_t itmId : int16_t_ARRAY_0083dc4c[1])
                                    {
                                        if (itmId)
                                            FUN_00421d24(CharInfoCharacter, itmId);
                                    }
                                    
                                    CharInfoCharacter = &pCVar13;
                                    int16_t_ARRAY_0083dc4c[1] = pCVar13.Inventory;
                                    int16_t_ARRAY_0083dd4c[0] = 0;
                                    DrawTrade(0);
                                }
                            }
                            else if (_playScreenID == PLSCREEN_3)
                            {
                                InvPos = 0;
                                DAT_00a3e7a0 = 0;
                                CharInfoCharacter = &pCVar13;
                                FUN_0042f50c(CharInfoCharacter, 0);
                                if (!IsSelectedCharacter(&pCVar13))
                                    SelectCharacter(&pCVar13);
                                FUN_004290d8();
                            }
                            else
                            {
                                if (!IsSelectedCharacter(&pCVar13))
                                {
                                    SelectCharacter(&pCVar13);
                                    if (DisplayInvOfCharID)
                                    {
                                        InvPos = 0;
                                        DAT_00a3e7a0 = 0;
                                        CharInfoCharacter = &pCVar13;
                                        FUN_0042f50c(&pCVar13, 0);
                                    }
                                    //PlayChangeScreen(PLSCREEN_0);
                                    FUN_004290d8();
                                }
                                else
                                {
                                    if (pCVar13.Index + 1 == DisplayInvOfCharID)
                                    {
                                        DisplayInvOfCharID2 = 0;
                                        DisplayInvOfCharID = 0;
                                    }
                                    else
                                    {
                                        InvPos = 0;
                                        DAT_00a3e7a0 = 0;
                                        CharInfoCharacter = &pCVar13;
                                        FUN_0042f50c(&pCVar13, 0);
                                    }
                                }
                            }
                        }
                    }
                }
                else if ((pCVar13.ClassID & CLASS_BIT80) == 0)
                {
                    if (InfPchar == &pCVar13)
                        FUN_0042054c(&pCVar13);
                    else
                        FUN_004246f8(&pCVar13);
                }
                else
                    FUN_0042054c(&pCVar13);
            }
            break;
            
            case 40:
            {
                if (CheckKharUp(*CharInfoCharacter, KHAR_LEVEL))
                {
                    CharInfoCharacter->Level++;
                    
                    //PlayChangeScreen(PLSCREEN_3);
                    FUN_0042f50c(CharInfoCharacter, 0);
                }
                
            }
            break;
            
            case 41:
            {
                int32_t points = CheckKharUp(*CharInfoCharacter, KHAR_HARIZMA);
                if (points)
                {
                    CharInfoCharacter->FreePoints -= points;
                    CharInfoCharacter->Exp += points;
                    CharInfoCharacter->BaseHarizm++;
                    
                    //PlayChangeScreen(PLSCREEN_3);
                    FUN_0042f50c(CharInfoCharacter, 0);
                }
            }
            break;
            
            case 42:
            {
                int32_t points = CheckKharUp(*CharInfoCharacter, KHAR_SILA);
                if (points)
                {
                    CharInfoCharacter->FreePoints -= points;
                    CharInfoCharacter->Exp += points;
                    CharInfoCharacter->BaseSila++;
                    
                    //PlayChangeScreen(PLSCREEN_3);
                    FUN_0042f50c(CharInfoCharacter, 0);
                }
            }
            break;
            
            case 43:
            {
                int32_t points = CheckKharUp(*CharInfoCharacter, KHAR_LOVKOST);
                if (points)
                {
                    CharInfoCharacter->FreePoints -= points;
                    CharInfoCharacter->Exp += points;
                    CharInfoCharacter->BaseLovkost++;
                    
                    //PlayChangeScreen(PLSCREEN_3);
                    FUN_0042f50c(CharInfoCharacter, 0);
                }
            }
            break;
            
            case 44:
            {
                int32_t points = CheckKharUp(*CharInfoCharacter, KHAR_VINOSLIVOST);
                if (points)
                {
                    CharInfoCharacter->FreePoints -= points;
                    CharInfoCharacter->Exp += points;
                    CharInfoCharacter->BaseVinoslivost++;
                    
                    //PlayChangeScreen(PLSCREEN_3);
                    FUN_0042f50c(CharInfoCharacter, 0);
                }
            }
            break;
            
            case 45:
            {
                if ((CharInfoCharacter->field_0x3 & 2) == 0)
                {
                    CharInfoCharacter->field_0x3 |= 2;
                    FUN_0041079c(CharInfoCharacter, 0);
                }
                else
                {
                    CharInfoCharacter->field_0x3 &= ~2;
                }
                
                //PlayChangeScreen(PLSCREEN_3);
                FUN_0042f50c(CharInfoCharacter, 0);
            }
            break;
            
            case 50:
            {
                if (_playScreenID == PLSCREEN_7)
                {
                    int32_t fromPanel = retval >> 0x10;
                    int32_t itmFromPos = (((retval >> 8) & 0xff) + int16_t_ARRAY_0083dd4c[fromPanel]) & 0x1f;
                    if (int16_t_ARRAY_0083dc4c[fromPanel].at(itmFromPos) != 0)
                    {
                        int32_t to_panel = 0;
                        
                        if (fromPanel < 2)
                            to_panel = 1 - fromPanel;
                        else
                            to_panel = 5 - fromPanel;
                        
                        for (int32_t itmToPos = 0; itmToPos < INVSIZE; itmToPos++)
                        {
                            if ( int16_t_ARRAY_0083dc4c[to_panel].at(itmToPos) == 0 )
                            {
                                int16_t_ARRAY_0083dc4c[to_panel].at(itmToPos) = int16_t_ARRAY_0083dc4c[fromPanel].at(itmFromPos);
                                int16_t_ARRAY_0083dc4c[fromPanel].at(itmFromPos) = 0;

                                for (int32_t i = itmFromPos; i < INVSIZE - 1; ++i)
                                    int16_t_ARRAY_0083dc4c[fromPanel].at(i) = int16_t_ARRAY_0083dc4c[fromPanel].at(i + 1);

                                int16_t_ARRAY_0083dc4c[fromPanel].back() = 0;
                                DrawTrade(0);
                                break;
                            }
                        }
                    }
                }
                else
                {
                    if (DAT_00a3e790 == 7)
                    {
                        DAT_00a3e790 = 4;
                    }
                    else if (DAT_00a3e790 == 4)
                    {
                        int32_t idx = (_uiMousePos.x - DAT_00a3e870) / ItemInvSlotWidth + InvPos & 0x1f;
                        int32_t itemId = CharInfoCharacter->Inventory.at(idx);
                        if (itemId != 0 && itemId != DAT_00a3e7a0)
                        {
                            FUN_00425104(&_state.Items[itemId]);

                            FUN_0041e210(CharInfoCharacter, 32, itemId);

                            //if (_playScreenID == PLSCREEN_3)
                            //    PlayChangeScreen(PLSCREEN_3);
                            FUN_0042f50c(CharInfoCharacter, 0);
                        }
                    }
                    else
                        FUN_004246f8(&_state.Characters.at(DisplayInvOfCharID - 1));
                }
            }
            break;
            
            case 100:
            {
                int32_t questTxtId = retval >> 8;
                qwsel1 = qOut2[questTxtId].Q1ID;
                
                if (quest_1[qwsel1].SoundId == -1)
                {
                    DAT_00a3e704 = 0;
                    FUN_0041f18c(&quest_1.at(qwsel1), questTxtId);
                }
                else
                {
                    DAT_00a3e704 = qwsel1 + 1;
                    SoundPlaySpeech(quest_1[qwsel1].SoundId);
                }
            }
            break;
            
            case 120:
            {
                if (_playScreenID == PLSCREEN_7)
                {
                    int32_t invid = retval >> 8;
                    int16_t_ARRAY_0083dd4c[invid]--;
                    if (int16_t_ARRAY_0083dd4c[invid] < 0)
                        int16_t_ARRAY_0083dd4c[invid] = 0;
                    
                    DrawTrade(-(invid + 1));
                }
                else
                {
                    InvPos--;
                    if (InvPos < 0)
                        InvPos = 0;
                    
                    FUN_0042f50c(CharInfoCharacter, -1);
                }
            }
            break;
            
            case 121:
            {
                if (_playScreenID == PLSCREEN_7)
                {
                    int32_t local_20 = retval >> 8;
                    
                    if (FUN_0041eb9c(&int16_t_ARRAY_0083dc4c[local_20], 6, int16_t_ARRAY_0083dd4c[local_20]))
                    {
                        int16_t_ARRAY_0083dd4c[local_20]++;
                        DrawTrade(local_20 + 1);
                    }
                }
                else
                {
                    if (FUN_0041eb9c(&CharInfoCharacter->Inventory, (MAPVIEWW + 2 + ScrlBtnWidth * -2) / ItemInvSlotWidth, InvPos))
                        InvPos++;
                    
                    FUN_0042f50c(CharInfoCharacter, 1);
                }
            }
            break;
            
            case 200:
            {
                if (TradeComplete(false))
                {
                    _playScreenID = PLSCREEN_0;
                    FUN_004292e4();
                }
            }
            break;
            
            case 201:
            {
                if (TradeComplete(true))
                {
                    if (!PInteractChar)
                    {
                        if (DAT_00a3e76c == 0)
                        {
                            _playScreenID = PLSCREEN_0;
                            FUN_004292e4();
                        }
                        else
                            FUN_004170a0(&mapGS1.at(DAT_00a3e76c));
                    }
                    else
                        FUN_00432fd0();
                }
            }
            break;
            
            case 202:
            {
                if (retval > 202)
                {
                    if (DAT_00a3e7c4 - 1 <= DWORD_00a3e7b4)
                        return;
                    
                    DWORD_00a3e7b4++;
                }
                else
                {
                    if (DWORD_00a3e7b4 == 0)
                        return;
                    
                    DWORD_00a3e7b4--;
                }
                
                PlayChangeScreen(PLSCREEN_2);
            }
            break;
            
        }
    }
    else if (_mousePress & MOUSEB_R)
    {
        if (DAT_00a3e790 == 7)
            DAT_00a3e790 = 4;
        else if (DAT_00a3e790 != 4)
        {
            FUN_00429194(1);
            return;
        }
        
        switch(retval & 0xff)
        {
            case 0:
            {
                if (_playScreenID == PLSCREEN_0)
                {
                    Common::Point pnt = FUN_00439bdc(_mouseMapPos + _camera);
                    
                    if (!MouseOnCharacter && !MouseOnObject)
                    {
                        for (Character *chr : SelectedCharacters)
                        {
                            if (!chr)
                                break;
                            
                            chr->MoveTile = pnt;
                            chr->field2_0x2 = 0x48;
                            chr->field17_0x13[1] = 0xff;
                            chr->field_0x3 |= 0x40;
                        }
                    }
                    else if (!MouseOnCharacter || MouseOnCharacter->MapCharID != _mainCharacter->MapCharID)
                    {
                        if (!MouseOnCharacter || 
                             MouseOnCharacter->field96_0xd0 == -1 || 
                             _KeyState[KEYFN_SHIFT] ||
                            (_mainCharacter->field_0x3 & 4) != 0 )
                        {
                            for (Character *chr : SelectedCharacters)
                            {
                                if (!chr)
                                    break;

                                if (chr->field_0x12 == ESLT_1 &&
                                    chr->ArmorWeapons[ESLT_1] &&
                                    chr->Arrows != 0 && 
                                   (chr->field_0x3 & 4) != 0 )
                                {
                                    if (!MouseOnCharacter)
                                    {
                                        if (_state.Items.at(chr->Arrows).SpecialID == 0)
                                        {
                                            chr->EnemyCharID = -(MouseOnObject->Index + 1);
                                            chr->MoveTile = pnt;
                                            chr->field2_0x2 = 0x44;
                                            chr->field17_0x13[1] = 0xff;
                                        }
                                    }
                                    else
                                    {
                                        chr->EnemyCharID = MouseOnCharacter->Index + 1;
                                        chr->field2_0x2 = 0x44;
                                        chr->field17_0x13[1] = 0xff;
                                    }
                                }
                                else if (!MouseOnCharacter)
                                {
                                    chr->MoveTile.y = pnt;
                                    chr->field2_0x2 = 0x48;
                                    chr->field17_0x13[1] = 0xff;
                                    chr->field_0x3 |= 0x40;
                                }
                                else if ((chr->field_0x3 & 4) != 0)
                                {
                                    chr->EnemyCharID = MouseOnCharacter->Index + 1;
                                    chr->field2_0x2 = 0x41;
                                    chr->field17_0x13[1] = 0xff;
                                }
                            }
                        }
                        else
                        {
                            DisplayInvOfCharID = 0;
                            
                            if (FUN_004364f0(MouseOnCharacter))
                                PlayChangeScreen(PLSCREEN_1);
                        }
                    }
                    else if (_KeyState[KEYFN_CTRL] == 0 || MouseOnCharacter->field96_0xd0 == -1)
                    {
                        if (_KeyState[KEYFN_SHIFT] == 0)
                            SelectCharacter(MouseOnCharacter);
                        else
                        {
                            if (IsSelectedCharacter(MouseOnCharacter))
                                FUN_00439230(MouseOnCharacter);
                            else
                                SelectCharacter(MouseOnCharacter);
                        }
                        
                        //PlayChangeScreen(PLSCREEN_0);
                        
                        if (DisplayInvOfCharID2)
                        {
                            if (IsSelectedCharacter(MouseOnCharacter))
                            {
                                CharInfoCharacter = MouseOnCharacter;
                                FUN_0042f50c(MouseOnCharacter, 0);
                            }
                            else
                            {
                                CharInfoCharacter = SelectedCharacters[0];
                                if (SelectedCharacters[0])
                                    FUN_0042f50c(SelectedCharacters[0], 0);
                                else
                                {
                                    DisplayInvOfCharID2 = 0;
                                    DisplayInvOfCharID = 0;
                                }
                            }
                        }
                    }
                    else
                    {
                        DisplayInvOfCharID = 0;
                        
                        if (FUN_004364f0(MouseOnCharacter))
                            PlayChangeScreen(PLSCREEN_1);
                    }
                }
            }
            break;
            
            case 30:
            {
                Character &chr = _state.Characters.at(_mainMapChar->CharacterIndex + (retval >> 8));
                if ((chr.ClassID & CLASS_BIT80) == 0 && chr.State != CHSTATE_9 && chr.State != CHSTATE_3)
                {
                    if (_KeyState[KEYFN_CTRL] == 0 || chr.field96_0xd0 == -1)
                    {
                        FUN_00439230(&chr);
                        
                        if (chr.Index + 1 == DisplayInvOfCharID)
                        {
                            CharInfoCharacter = SelectedCharacters[0];
                            if (CharInfoCharacter)
                            {
                                InvPos = 0;
                                DAT_00a3e7a0 = 0;
                                FUN_0042f50c(CharInfoCharacter, 0);
                                FUN_004290d8();
                            }
                        }
                        
                        //PlayChangeScreen(PLSCREEN_0);
                    }
                    else
                    {
                        DisplayInvOfCharID = 0;
                        if (FUN_004364f0(&chr))
                            PlayChangeScreen(PLSCREEN_1);
                    }
                }
            }
            break;
            
            case 50:
            {
                if (_playScreenID != PLSCREEN_7)
                {
                    int32_t itmIndex = ((_uiMousePos.x - DAT_00a3e870) / ItemInvSlotWidth + InvPos) & 0x1f;
                    int32_t itmId = CharInfoCharacter->Inventory.at(itmIndex);
                    
                    if (itmId != 0)
                    {
                        ItemInfo &itm = _state.Items.at(itmId);
                        switch(itm.TypeID)
                        {
                            case 0:
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                            case 12:
                                FUN_00425104(&itm);
                                FUN_0041e210(CharInfoCharacter, 32, itmId);
                                FUN_0041e500(itm.TypeID);
                                break;
                                
                            case 6:
                                FUN_00425104(&itm);
                                FUN_0041e210(CharInfoCharacter, 32, itmId);
                                FUN_0041e778(0);
                                break;
                                
                            case 9:
                                FUN_0041db64(CharInfoCharacter, nullptr, &itm);
                                break;
                                
                            case 11:
                                FUN_00414e64(CharInfoCharacter, nullptr, &itm);
                                break;
                        }
                    }
                }
            }
            break;
        }
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
                
                if (CharInfoCharacter->field_0x12 == ESLT_2)
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
                    int32_t fromPanel = retval >> 0x10;
                    int32_t itmFromPos = (((retval >> 8) & 0xff) + int16_t_ARRAY_0083dd4c[fromPanel]) & 0x1f;
                    int32_t itmId = int16_t_ARRAY_0083dc4c[fromPanel].at(itmFromPos);
                    if (itmId)
                        FUN_0042f9b8(GetItemHint( &_state.Items.at(itmId) ));
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
                
            default:
                break;
        }
        
//        if (_playScreenID == PLSCREEN_1)
//        {
//            PlayChangeScreen(PLSCREEN_1);
//            return;
//        }
    }
}    

    
}