#include "game.h"
#include "game_locale.h"

namespace Game {


    
    
void Engine::FUN_00431d70(int p)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
}

void Engine::FillBkgRect(Common::Rect rect)
{
    GFX::Image* tile = _menuImages.at(5);
    Common::Point tsz = tile->GetSize();
    
    Common::Rect clip = rect;
    
    for(int32_t y = rect.top; y < rect.bottom; y += tsz.y)
    {
        for(int32_t x = rect.left; x < rect.right; x += tsz.x)
        {
            clip.left = x;
            clip.top = y;
            ImgQueue1(tile, clip.Pos(), clip);
        }
    }
}

void Engine::DrawMap()
{
    GFXDrawer.SetFade(false);
    Common::Point outp = MAPRECT.Pos() + SRect_ARRAY_0046280c.Pos();
    GFXDrawer.Draw(_menuImages.at(4), MAPRECT.Pos());
    
    GFXDrawer.SetFade(true, vec3f(24, 0.0, 0.0) / 100.0);
    if (_mapImage)
        GFXDrawer.Draw(_mapImage, outp);
    
    GFXDrawer.SetFade(false);
}


void Engine::UpdateMapImage()
{
    GFX::Image *src = _menuImages.at(179);
    
    Common::Point sz = src->GetSize();
    
    if (_mapImage && _mapImage->GetSize() != sz)
    {
        delete _mapImage;
        _mapImage = nullptr;
    }
    
    if (!_mapImage)
        _mapImage = GFXDrawer.CreateImage(sz);
    
    Common::PlaneVector<uint8_t>* mask = Res.MapMask; 
    
    GFXDrawer.LockImage(_mapImage);
    
    for(int32_t y = 0; y < sz.y; ++y)
    {
        uint32_t *psrc = (uint32_t *)((char *)src->SW->pixels + src->SW->pitch * y);
        uint32_t *pdst = (uint32_t *)((char *)_mapImage->SW->pixels + _mapImage->SW->pitch * y);
        for(int32_t x = 0; x < sz.x; ++x)
        {
            uint8_t m = mask->At(x, y);
            if (m >= MapsOpened.size() || !MapsOpened[m])
                *pdst = 0;
            else
                *pdst = *psrc;
            
            pdst++;
            psrc++;
        }
    }
    
    GFXDrawer.UnlockImage(_mapImage);
}



void Engine::ShowHideCharInfo()
{
    if (_playScreenID == PLSCREEN_0 || _playScreenID == PLSCREEN_3)
    {
        FUN_00429194(true);
        PlaySound(4, 0, 0, 0);
        FUN_004290ac(6, 13);
        
        if (_playScreenID == PLSCREEN_3)
        {
            _playScreenID = PLSCREEN_0;
            DisplayInvOfCharID = DisplayInvOfCharID2;
            
            if (DisplayInvOfCharID2)
            {
                CharInfoCharacter = &_state.Characters.at(DisplayInvOfCharID2 - 1);
                FUN_0042f50c(CharInfoCharacter, 0);
            }
            else
                _playScreenID = PLSCREEN_0;            
        }
        else
        {
            CharInfoCharacter = SelectedCharacters[0];
            
            if (SelectedCharacters[0] == nullptr)
            {
                CharInfoCharacter = SelectedCharacters[0];
                DisplayInvOfCharID = 0;
                DisplayInvOfCharID2 = 0;
            }
            else
            {
                InvPos = 0;
                DAT_00a3e7a0 = 0;
                PlayChangeScreen(PLSCREEN_3);
            }
        }
    }
}

void Engine::DrawCharInfo()
{
    static const std::array<Common::Point, 5> TxtPlace_ARRAY_00465754 =
    {{
    //{578, 70},
    {722, 89},
    //{573, 110},
    {716, 138},
    //{573, 126},
    {716, 158},
    //{573, 145},
    {716, 181},
    //{573, 162},
    {716, 203} }};
    
    
    if (CharInfoCharacter == NULL)
    {
        _playScreenID = PLSCREEN_0;
        return;
    }
            
    _playScreenID = PLSCREEN_3;
    
    FUN_0041c750(CharInfoCharacter);
    
    FillBkgRect(MAPRECT);
    ImgQueue2(_menuImages.at(2), MAPRECT.Pos(), MAPRECT);
    
    std::string txt = CharInfoCharacter->FmtCharName();
    int32_t txtwidth = _Fonts[0]->StringWidth(txt);
    
    Common::Point outp;
    outp.x = ((MAPRECT.left + MAPRECT.right) - txtwidth) / 2;
    outp.y = SCREENRESY * 10 / 640;
    
    TextQueue(txt, _Fonts[0], outp, MAPRECT);
    
    if ((CharInfoCharacter->ClassID & CLASS_BIT40) == 0)
    {
        txt = Locale::CharacterSpeciality.at( (CharInfoCharacter->ClassID & 7) - 1 );
        txtwidth = _Fonts[0]->StringWidth(txt);
        outp.x = ((MAPRECT.left + MAPRECT.right) - txtwidth) / 2;
        outp.y = (TxtPlace_ARRAY_00465754[2].y - TxtPlace_ARRAY_00465754[1].y + 14) * SCREENRESY  / 640;
        TextQueue(txt, _Fonts[0], outp, MAPRECT);
    }
//    bVar15 = false;
//    for (local_38 = 0; local_38 < 6; local_38 += 1) {
//        iVar18 = *(int *)(CharInfoCharacter->ArmorWeapons + local_38 + -1) >> 0x10;
//        if ((iVar18 != 0) && (iVar18 != DAT_00a3e7a0)) {
//            pIVar1 = _state.Items + iVar18;
//            iVar19._0_1_ = pIVar1->TypeID;
//            iVar19._1_1_ = pIVar1->BonusID;
//            iVar19._2_1_ = pIVar1->SpecialID;
//            iVar19._3_1_ = pIVar1->InfoID;
//            local_2c._0_2_ = ArmorWeaponInfo[iVar19 >> 0x18].unk6;
//            local_2c._2_2_ = ArmorWeaponInfo[iVar19 >> 0x18].unk7;
//            local_2c >>= 0x10;
//            if (local_38 == 0) {
//                if ((*(int *)((int)&CharInfoCharacter->MoveTile.x + 1) >> 0x18 != 2) || (CharInfoCharacter->ArmorWeapons[2] == 0)) {
//                    bVar15 = true;
//LAB_0042e30c:
//                    local_20 = *(int *)(&DAT_004657b4 + *(int *)(&DAT_0046573c + local_38 * 4) * 0x10 + ScreenMode * 8) +
//                               (int)((ItemInvSlotWidth + 1) - (uint)*(ushort *)(ImgSprite_ARRAY_006304b8[local_2c].pData + _bkgImage)) / 2
//                    ;
//                    ImgQueue2((int)(ImgSprite_ARRAY_006304b8[local_2c].pData + _bkgImage),
//                              (int)GFX::pRawPalettes + ImgSprite_ARRAY_006304b8[local_2c].paletteOffset,local_20,
//                              *(int *)(&DAT_004657b8 + ScreenMode * 8 + *(int *)(&DAT_0046573c + local_38 * 4) * 0x10),GScrOff.x,GScrOff.y
//                              ,DAT_00a3e880,BottomPanelY);
//                }
//            }
//            else if ((local_38 != 2) || (!bVar15)) goto LAB_0042e30c;
//        }
//    }
//    iVar21._0_2_ = CharInfoCharacter->field48_0xca;
//    iVar21._2_2_ = CharInfoCharacter->Arrows;
//    local_2c = iVar21 >> 0x10;
//    if ((local_2c != 0) && (local_2c != DAT_00a3e7a0)) {
//        pIVar1 = _state.Items + local_2c;
//        iVar22._0_1_ = pIVar1->TypeID;
//        iVar22._1_1_ = pIVar1->BonusID;
//        iVar22._2_1_ = pIVar1->SpecialID;
//        iVar22._3_1_ = pIVar1->InfoID;
//        iVar24._0_2_ = ArmorWeaponInfo[iVar22 >> 0x18].unk6;
//        iVar24._2_2_ = ArmorWeaponInfo[iVar22 >> 0x18].unk7;
//        local_2c = iVar24 >> 0x10;
//        ImgQueue2((int)(ImgSprite_ARRAY_006304b8[local_2c].pData + _bkgImage),
//                  (int)GFX::pRawPalettes + ImgSprite_ARRAY_006304b8[local_2c].paletteOffset,*(int *)(&DAT_00465804 + ScreenMode * 8),
//                  *(int *)(&DAT_00465808 + ScreenMode * 8),GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    }
//    for (local_38 = 0; local_38 < 5; local_38 += 1) {
//        local_2c = *(int *)(CharInfoCharacter->Inventory + local_38 + 0x1f) >> 0x10;
//        if ((local_2c != 0) && (local_2c != DAT_00a3e7a0)) {
//            pIVar1 = _state.Items + local_2c;
//            iVar25._0_1_ = pIVar1->TypeID;
//            iVar25._1_1_ = pIVar1->BonusID;
//            iVar25._2_1_ = pIVar1->SpecialID;
//            iVar25._3_1_ = pIVar1->InfoID;
//            iVar3._0_2_ = AcessoriesInfo[iVar25 >> 0x18].Weight;
//            iVar3._2_2_ = AcessoriesInfo[iVar25 >> 0x18].ImgID;
//            local_2c = iVar3 >> 0x10;
//            local_44 = (TxtPlace *)(ImgSprite_ARRAY_006304b8[local_2c].pData + _bkgImage);
//            memcpy(&local_34,local_44,4);
//            local_34 >>= 0x10;
//            memcpy(local_14,local_44,2);
//            ImgQueue2((int)(ImgSprite_ARRAY_006304b8[local_2c].pData + _bkgImage),
//                      (int)GFX::pRawPalettes + ImgSprite_ARRAY_006304b8[local_2c].paletteOffset,
//                      *(int *)(&DAT_00465814 + local_38 * 0x10 + ScreenMode * 8) - local_14[0] / 2,
//                      *(int *)(&DAT_00465818 + local_38 * 0x10 + ScreenMode * 8) - local_34 / 2,GScrOff.x,GScrOff.y,DAT_00a3e880,
//                      BottomPanelY);
//        }
//    }
//    if (DAT_00a3e7a0 != 0) {
//        local_2c = FUN_0042c870(_state.Items + DAT_00a3e7a0);
//        local_44 = (TxtPlace *)(ImgSprite_ARRAY_006304b8[local_2c].pData + _bkgImage);
//        memcpy(&local_34,local_44,4);
//        local_34 >>= 0x10;
//        memcpy(local_14,local_44,2);
//        ImgQueue2((int)(ImgSprite_ARRAY_006304b8[local_2c].pData + _bkgImage),
//                  (int)GFX::pRawPalettes + ImgSprite_ARRAY_006304b8[local_2c].paletteOffset,
//                  *(int *)(&DAT_00465864 + ScreenMode * 8) - local_14[0] / 2,*(int *)(&DAT_00465868 + ScreenMode * 8) - local_34 / 2,
//                  GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    }
//    local_44 = (TxtPlace *)(&TxtPlace_ARRAY_00465874[0].Lo + ScreenMode);
//        // 0
//    itoa(CharInfoCharacter->Exp,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//        // 1
//    itoa(*(int *)(CharInfoCharacter->field17_0x13 + 0x1d) >> 0x18,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    iVar4._0_2_ = CharInfoCharacter->BaseLovkost;
//    iVar4._2_2_ = CharInfoCharacter->CurrentHarizm;
//        // 2
//    itoa(iVar4 >> 0x10,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//        // 3
//    iVar5._0_2_ = CharInfoCharacter->HP;
//    iVar5._2_2_ = CharInfoCharacter->CurrentSila;
//    itoa(iVar5 >> 0x10,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//        // 4
//    iVar6._0_2_ = CharInfoCharacter->field28_0x52;
//    iVar6._2_2_ = CharInfoCharacter->CurrentLovkost;
//    itoa(iVar6 >> 0x10,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    iVar7._0_2_ = CharInfoCharacter->BaseSila;
//    iVar7._2_2_ = CharInfoCharacter->CurrentVinoslivost;
//        // 5
//    itoa(iVar7 >> 0x10,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    iVar8._0_2_ = CharInfoCharacter->BaseHarizm;
//    iVar8._2_2_ = CharInfoCharacter->HP;
//    iVar18 = (iVar8 >> 0x10) + 15;
//    iVar19 = iVar18 >> 31;
//        // 6
//    itoa((int)((iVar18 + iVar19 * -0x10) - (uint)(iVar19 << 3 < 0)) >> 4,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    iVar9._0_2_ = CharInfoCharacter->CurrentHarizm;
//    iVar9._2_2_ = CharInfoCharacter->BaseHarizm;
//        // 7
//    itoa(iVar9 >> 0x10,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//        // 8
//    iVar10._0_2_ = CharInfoCharacter->CurrentSila;
//    iVar10._2_2_ = CharInfoCharacter->BaseSila;
//    itoa(iVar10 >> 0x10,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    iVar11._0_2_ = CharInfoCharacter->CurrentLovkost;
//    iVar11._2_2_ = CharInfoCharacter->BaseLovkost;
//        // 9
//    itoa(iVar11 >> 0x10,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    iVar12._0_2_ = CharInfoCharacter->CurrentVinoslivost;
//    iVar12._2_2_ = CharInfoCharacter->BaseVinoslivost;
//        // 10
//    itoa(iVar12 >> 0x10,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    itoa((ushort)CharInfoCharacter->Metkost / 100,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    itoa((ushort)CharInfoCharacter->Fehtovanie / 100,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    itoa((uint)(ushort)CharInfoCharacter->Identification,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    itoa((uint)(ushort)CharInfoCharacter->Trading,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//        // 15
//    itoa((uint)(ushort)CharInfoCharacter->Medicine,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    itoa((uint)(ushort)CharInfoCharacter->KuznechnoeDelo,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    itoa((uint)(ushort)CharInfoCharacter->PlotnickoeDelo,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    iVar18 = Character::GetCurrentWeight(CharInfoCharacter);
//    sprintf((int)txt,(byte *)"%4.1f",SUB81((double)iVar18 * 0.001,0));
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    iVar13._0_2_ = CharInfoCharacter->BaseSila;
//    iVar13._2_2_ = CharInfoCharacter->CurrentVinoslivost;
//    sprintf((int)txt,(byte *)"%4.1f",SUB81((double)(((iVar13 >> 0x10) * 1000) / 3 + 10000) * 0.001,0));
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//        // 20
//    iVar14._0_2_ = CharInfoCharacter->Armor;
//    iVar14._2_2_ = CharInfoCharacter->Otravlenie;
//    itoa(iVar14 >> 0x10,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    pbVar23 = txt;
//    uVar16 = 10;
//    iVar18 = Character::GetArmor(CharInfoCharacter);
//        // 21
//    itoa(iVar18,(char *)pbVar23,uVar16);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    if (((CharInfoCharacter->Arrows == 0) || (*(int *)((int)&CharInfoCharacter->MoveTile.x + 1) >> 0x18 != 1)) ||
//       (CharInfoCharacter->ArmorWeapons[1] == 0)) {
//        pbVar23 = txt;
//        uVar16 = 10;
//        iVar18 = FUN_0041af48(CharInfoCharacter,0);
//        // 22
//        itoa(iVar18,(char *)pbVar23,uVar16);
//    }
//    else {
//        pbVar23 = txt;
//        uVar16 = 10;
//        iVar18 = FUN_0041ade4(CharInfoCharacter);
//        // 22
//        itoa(iVar18,(char *)pbVar23,uVar16);
//    }
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//        // 23
//    itoa(CharInfoCharacter->Gold,(char *)txt,10);
//    local_20 = (local_44->Lo).x;
//    local_1c = (local_44->Lo).y;
//    local_44 = local_44 + 1;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_1c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//        // 24
//    itoa(CharInfoCharacter->FreePoints,(char *)txt,10);
//    local_2c = (local_44->Lo).y;
//    local_44 = (TxtPlace *)(ImgSprite_ARRAY_006304b8[171].pData + _bkgImage);
//    memcpy(local_14,local_44,2);
//    local_44 = (TxtPlace *)(&TxtPlace_ARRAY_00465754[0].Lo + ScreenMode);
//    iVar18 = FUN_0043f821(&Font3,txt);
//    local_20 = (local_44->Lo).x + (int)((uint)local_14[0] - iVar18) / 2;
//    GFX::TextQueue((char *)txt,&Font3,local_20,local_2c,GScrOff.x,GScrOff.y,DAT_00a3e880,BottomPanelY);
//    for (local_38 = 0; local_38 < 5; local_38 += 1) {
//        local_20 = (local_44->Lo).x;
//        local_1c = (local_44->Lo).y;
//        local_44 = local_44 + 1;
//        switch(local_38) {
//        case 0:
//            local_30 = 0;
//            break;
//        case 1:
//            local_30 = 3;
//            break;
//        case 2:
//            local_30 = 5;
//            break;
//        case 3:
//            local_30 = 2;
//            break;
//        case 4:
//            local_30 = 6;
//        }
//        local_30 = CheckKharUp(CharInfoCharacter,local_30);
//        if (local_30 != 0) {
//            if (local_38 == 0) {
//                local_30 = 0;
//            }
//            else {
//                pFVar20 = &Font3;
//                iVar18 = local_20;
//                iVar19 = local_1c;
//                iVar21 = GScrOff.x;
//                iVar22 = GScrOff.y;
//                iVar24 = DAT_00a3e880;
//                iVar25 = BottomPanelY;
//                pcVar17 = itoa(local_30,(char *)txt,10);
//                GFX::TextQueue(pcVar17,pFVar20,iVar18,iVar19,iVar21,iVar22,iVar24,iVar25);
//                local_30 = 0x18;
//            }
//            ImgQueue2((int)(ImgSprite_ARRAY_006304b8[171].pData + _bkgImage),
//                      (int)GFX::pRawPalettes + ImgSprite_ARRAY_006304b8[171].paletteOffset,local_20 + local_30,local_1c + 2,GScrOff.x,
//                      GScrOff.y,DAT_00a3e880,BottomPanelY);
//        }
//    }
//    if ((CharInfoCharacter->field_0x3 & 2) == 0) {
//        local_38 = 0xa9;
//    }
//    else {
//        local_38 = 0xaa;
//    }
//    piVar2 = &(local_44->Lo).y;
//    local_20 = (local_44->Lo).x;
//    iVar18 = local_38 * 8;
//    local_44 = (TxtPlace *)piVar2;
//    ImgQueue2((int)(ImgSprite_ARRAY_006304b8[local_38].pData + _bkgImage),
//              (int)GFX::pRawPalettes + ImgSprite_ARRAY_006304b8[local_38].paletteOffset,local_20,*piVar2,GScrOff.x,GScrOff.y,DAT_00a3e880,
//              BottomPanelY);
}

void Engine::DrawJournal()
{
    
}

bool Engine::FUN_0041e500(int32_t slot)
{
    int32_t sndId = 11;
    
    if (DAT_00a3e790 == 4)
    {
        if (slot == ESLT_ARROWS)
        {
            const int32_t itemId = CharInfoCharacter->Arrows;
            
            if (itemId == 0)
                return false;
            
            FUN_00425104(&_state.Items[itemId]);
            FUN_0041e210(CharInfoCharacter, 1, itemId);
        }
        else
        {
            const int32_t itemId = CharInfoCharacter->ArmorWeapons[slot];
            if (itemId == 0)
                return false;
            
            ItemInfo &inf = _state.Items.at(itemId);
            if (inf.TypeID > 2)
                sndId = 1;
            
            FUN_00425104(&inf);
            FUN_0041e210(CharInfoCharacter, 6, itemId);
        }
    }
    else
    {
        if ((CharInfoCharacter->ClassID & CLASS_BIT40) || _state.Items.at(InfItemID).TypeID != slot)
        {
            FUN_00429194(1);
            return false;
        }
        
        if ( !FUN_00418364(CharInfoCharacter, &_state.Items.at(InfItemID)) )
        {
            FUN_0042f9b8(Locale::GameMessages[Locale::GMSM_UNUSABLE]);
            FUN_00429194(1);
            return false;
        }
        
        DAT_00a3e790 = 4;
        if (slot == ESLT_ARROWS)
        {
            if (CharInfoCharacter->Arrows != 0)
                FUN_00424918(CharInfoCharacter, CharInfoCharacter->Arrows);
            
            CharInfoCharacter->Arrows = InfItemID;
        }
        else
        {
            int32_t itmid = CharInfoCharacter->ArmorWeapons.at(slot);
            if (itmid != 0)
                FUN_00424918(CharInfoCharacter, itmid);
            
            CharInfoCharacter->ArmorWeapons.at(slot) = InfItemID;
            if (_state.Items.at(itmid).TypeID > 2)
                sndId = 1;
        }
        FUN_0041c750(CharInfoCharacter);
    }
    
    PlaySound(sndId,0,0,0);
    
    if (_playScreenID == PLSCREEN_3)
        PlayChangeScreen(PLSCREEN_3);
    
    FUN_0042f50c(CharInfoCharacter, 0);
    return true;
}


bool Engine::FUN_0041e778(int aidx)
{    
    if (DAT_00a3e790 == 4)
    {
        int32_t iid = CharInfoCharacter->Accessories.at(aidx);
        
        if (iid == 0)
            return false;
        
        FUN_00425104(&_state.Items[iid]);
        FUN_0041e210(CharInfoCharacter, 5, iid);
    }
    else
    {
        bool notFit = true;
        int32_t tp = _state.Items[InfItemID].TypeID;
        if ( (tp == 6 && aidx == 0) ||
             (tp == 7 && (aidx == 1 || aidx == 2)) ||
             (tp == 8 && (aidx == 3 || aidx == 4)) )
            notFit = false;
        
        if (notFit)
        {
            FUN_00429194(1);
            return false;
        }
        
        if ( !FUN_00418364(CharInfoCharacter, &_state.Items.at(InfItemID)) )
        {
            FUN_00429194(1);
            return false;
        }
        
        DAT_00a3e790 = 4;
        
        if (CharInfoCharacter->Accessories.at(aidx) != 0)
            FUN_00424918(CharInfoCharacter, CharInfoCharacter->Accessories.at(aidx));
        
        CharInfoCharacter->Accessories.at(aidx) = InfItemID;
        FUN_0041c750(CharInfoCharacter);
    }
    
    PlaySound(0,0,0,0);
    PlayChangeScreen(PLSCREEN_3);
    FUN_0042f50c(CharInfoCharacter, 0);
    return true;
}

void Engine::FUN_00425104(const ItemInfo *inf)
{
    switch(inf->TypeID)
    {
    case 0:
    case 2:
        DAT_00a3e790 = 10;
        break;
        
    case 1:
    case 12:
        DAT_00a3e790 = 2;
        break;
        
    case 3:
    case 4:
    case 5:
        DAT_00a3e790 = 0;
        break;
        
    case 9:
        DAT_00a3e790 = 8;
        break;
       
    default:
    case 11:
        DAT_00a3e790 = 9;
        break;
    }
}

void Engine::FUN_0041e210(Character *pchar, int32_t typ, int32_t itemid)
{
    InfPchar = pchar;
    InfTyp = typ;
    InfItemID = itemid;
    
    if (typ == 1)
    {
        pchar->Arrows = 0;
    }
    else if (typ == 5)
    {
        InfSvAccess = pchar->Accessories;
        for(int16_t &id : pchar->Accessories)
        {
            if (id == itemid)
            {
                id = 0;
                break;
            }
        }
    }
    else if (typ == 6)
    {
        InfSvArmrWpn = pchar->ArmorWeapons;
        for(int16_t &id : pchar->ArmorWeapons)
        {
            if (id == itemid)
            {
                id = 0;
                break;
            }
        }
    }
    else if (typ == INVSIZE)
    {
        int32_t pos = 0;
        for(; pos < pchar->Inventory.size(); ++pos)
        {
            if (pchar->Inventory[pos] == itemid)
            {
                for(int32_t j = pos; j < pchar->Inventory.size() - 1; ++j)
                    pchar->Inventory[j] = pchar->Inventory[j + 1];
                pchar->Inventory.back() = 0;
                
                InfSvInv = pchar->Inventory;
                
                for(int16_t &id : InfSvInv)
                {
                    if (id == 0)
                    {
                        id = itemid;
                        break;
                    }
                }
                break;
            }
        }
    }
}

bool Engine::FUN_00424918(Character *pchar, int32_t itemid)
{
    for(int16_t id : pchar->Inventory)
    {
        if (id == 0)
        {
            for (int32_t i = pchar->Inventory.size() - 1; i > 0; --i)
                pchar->Inventory[i] = pchar->Inventory[i - 1];

            pchar->Inventory.front() = itemid;
            return true;
        }
    }
    return false;
}

}
