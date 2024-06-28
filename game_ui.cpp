#include "game.h"
#include "game_locale.h"
#include "fmt/printf.h"

namespace Game {


void Engine::DrawTradeInv(int inv, int p)
{
    Common::Point outPos = tagRECT_ARRAY_00465ab4[inv].Pos();
    int32_t startPos = int16_t_ARRAY_0083dd4c[inv];
    int32_t endPos = startPos + 6;
    
    GFX::Image *img;
    if (p < 0)
        img = _menuImages.at(19);
    else
        img = _menuImages.at(18);
    
    ImgQueue2(img, outPos, MAPRECT);
    
    outPos.x += img->GetSize().x;
    outPos.y += (ScrlBtnHeight - _menuImages.at(17)->GetSize().y) / 2;
    
    Common::Point ppos = outPos;
    for (int32_t i = startPos; i < endPos; ++i)
    {
        int32_t itemId = int16_t_ARRAY_0083dc4c[inv].at(i);
        vec3f clr;
        
        if (itemId)
            clr = FUN_0042c914(CharInfoCharacter, &(_state.Items.at(itemId)));
        
        ImgQueue2(_menuImages.at(17), ppos, MAPRECT, clr);
        
        ppos.x += ItemInvSlotWidth;
    }
    
    if (p > 0)
        img = _menuImages.at(21);
    else
        img = _menuImages.at(20);        
    
    ImgQueue2(img, ppos, MAPRECT);
    
    ppos = outPos;
    for (int32_t i = startPos; i < endPos; ++i)
    {
        int32_t itemId = int16_t_ARRAY_0083dc4c[inv].at(i);
        if (itemId)
        {
            int32_t imgId = FUN_0042c870(&_state.Items.at(itemId));
            img = _menuImages.at(imgId);
            
            Common::Point p = ppos + Common::Point( ((ItemInvSlotWidth + 1) - img->GetSize().x) / 2,
                                                    ((ScrlBtnHeight + 1) - img->GetSize().y) / 2 );
            ImgQueue2(img, p, MAPRECT);
        }
        ppos.x += ItemInvSlotWidth;
    }
}

int32_t Engine::FUN_0041b348(ItemInfo *itm)
{
    if (itm->TypeID < 6)
    {
        int32_t val = ArmorWeaponInfo.at( itm->InfoID ).unk5;
        if (itm->BonusID > -1 && (itm->Flags & ItemInfo::FLAG_IDENTIFIED) == 0)
        {
            val += BonusesInfo.at(itm->BonusID).unk1;
            if (val < 1)
                val = 1;
        }
        return val;
    }
    else if (itm->TypeID < 9)
    {
        int32_t val = AcessoriesInfo.at( itm->InfoID ).BonusID;
        if (itm->BonusID > -1 && (itm->Flags & ItemInfo::FLAG_IDENTIFIED) == 0)
        {
            val += BonusesInfo.at(itm->BonusID).unk1;
            if (val < 1)
                val = 1;
        }
        return val;
    }
    else if (itm->TypeID == 12)
    {
        return ArmorWeaponInfo.at( itm->InfoID ).unk5 * itm->Concentration;
    }
    else if (itm->TypeID == 9)
    {
        int32_t val = AlchemiesInfo.at( itm->InfoID ).BonusID;
        if (val < 0)
            val = -(itm->Concentration * val);
        return val * 8;
    }
    else
        return MiscItemsInfo.at( itm->InfoID ).BonusID;

    return 0;
}
    
void Engine::DrawTrade(int p)
{
    FillBkgRect(MAPRECT);
    
    ImgQueue2(_menuImages[6], MAPRECT.Pos(), MAPRECT);
    ImgQueue2(_menuImages[180], tagRECT_ARRAY_00465a54[0].Pos(), MAPRECT);
    ImgQueue2(_menuImages[182], tagRECT_ARRAY_00465a54[1].Pos(), MAPRECT);
    
    if (PInteractChar)
    {
        std::string name = PInteractChar->FmtCharName();
        int32_t nameW = _Fonts[0]->StringWidth(name);
        const Common::Point outPos( (MAPRECT.left + MAPRECT.right  - nameW) / 2,
                                    SCREENRES.y * 0.002083 * 20.0 );
        
        TextQueue(name, _Fonts[0], outPos, MAPRECT);
    }
    
    for (int32_t i = 0; i < 4; ++i)
    {
        if (Common::ABS(p) == i + 1)
            DrawTradeInv(i, p);
        else
            DrawTradeInv(i, 0);
    }
    
    DAT_00a3e850 = 0;
    for (int32_t i = 0; i < INVSIZE; ++i)
    {
        int32_t itemId = int16_t_ARRAY_0083dc4c[1].at(i);
        if (itemId)
            DAT_00a3e850 += FUN_0041b348(&_state.Items.at(itemId));
    }
    
    DAT_00a3e854 = 0;
    for (int32_t i = 0; i < INVSIZE; ++i)
    {
        int32_t itemId = int16_t_ARRAY_0083dc4c[3].at(i);
        if (itemId)
            DAT_00a3e854 += FUN_0041b348(&_state.Items.at(itemId));
    }
    
    if (PInteractChar)
    {
        int32_t local_20 = DAT_00a3e82c + FUN_0041aea0(DAT_00a3e850);
        int32_t local_1c = DAT_00a3e830 + FUN_0041b65c(DAT_00a3e854);
        if (local_1c < local_20)
        {
            for (; DAT_00a3e82c != 0 && local_1c < local_20; --local_20)
            {
                DAT_00a3e7c8++;
                DAT_00a3e82c--;
            }
            
            for (; DAT_00a3e7cc != 0 && local_1c < local_20; ++local_1c)
            {
                DAT_00a3e7cc--;
                DAT_00a3e830++;
            }
        }
        else
        {
            for (; DAT_00a3e830 != 0 && local_20 < local_1c; --local_1c)
            {
                DAT_00a3e7cc++;
                DAT_00a3e830--;
            }
            
            for (; DAT_00a3e7c8 != 0 && local_20 < local_1c; ++local_20)
            {
                DAT_00a3e7c8--;
                DAT_00a3e82c++;
            }
        }
    }
    
    TextQueue(std::to_string(DAT_00a3e7cc), _Fonts[3], tagRECT_ARRAY_00465ab4[5].Pos(), MAPRECT);
    TextQueue(std::to_string(DAT_00a3e830), _Fonts[3], tagRECT_ARRAY_00465ab4[6].Pos(), MAPRECT);
    TextQueue(std::to_string(FUN_0041b65c(DAT_00a3e854) + DAT_00a3e830), _Fonts[3], tagRECT_ARRAY_00465ab4[7].Pos(), MAPRECT);
    
    TextQueue(std::to_string(DAT_00a3e82c), _Fonts[3], tagRECT_ARRAY_00465ab4[8].Pos(), MAPRECT);
    TextQueue(std::to_string(DAT_00a3e7c8), _Fonts[3], tagRECT_ARRAY_00465ab4[9].Pos(), MAPRECT);
    TextQueue(std::to_string(FUN_0041aea0(DAT_00a3e850) + DAT_00a3e82c), _Fonts[3], tagRECT_ARRAY_00465ab4[10].Pos(), MAPRECT);
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
                DisplayInvOfCharID = CharInfoCharacter->Index + 1;
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
    
    static const std::array<Common::Point, 5> AWPos =
    {{
        //{208, 94},
        {255, 126},
        //{208, 22},
        {255, 54},
        //{280, 70},
        {328, 103},
        //{136, 70},
        {182, 103},
        //{172, 199},
        {221, 233}
    }};

    static const std::array<int32_t, 6> AWInd = {{3, 4, 3, 0, 1, 2}};
    
    bool wpnDrawed = false;
    for (int32_t i = 0; i < 6; ++i)
    {
        int32_t itemId = CharInfoCharacter->ArmorWeapons[i];
        if (itemId && itemId != DAT_00a3e7a0)
        {
            ItemInfo &inf = _state.Items.at(itemId);
            
            bool draw = false;
            if (i == 0)
            {
                if (CharInfoCharacter->field_0x12 != ESLT_2 || CharInfoCharacter->ArmorWeapons[ESLT_2] == 0)
                {
                    wpnDrawed = true;
                    draw = true;                    
                }
            }
            else if (i != 2 || !wpnDrawed)
                draw = true;
            
            if (draw)
                ImgQueue2(_menuImages.at( ArmorWeaponInfo.at(inf.InfoID).unk7 ),
                          AWPos[ AWInd[i] ],
                          MAPRECT);
        }
    }
    
    if (CharInfoCharacter->Arrows != 0 && CharInfoCharacter->Arrows != DAT_00a3e7a0)
    {
        //(245, 199)
        constexpr const Common::Point ArrPos(295, 232);
        ImgQueue2(_menuImages.at( ArmorWeaponInfo.at( _state.Items.at(CharInfoCharacter->Arrows).InfoID ).unk7 ),
                  ArrPos,
                  MAPRECT);
    }
    
    static const std::array<Common::Point, 5> AccPos =
    {{
        //{181, 44},
        {232, 74},
        //{166, 157},
        {214, 190},
        //{317, 157},
        {367, 190},
        //{199, 182},
        {248, 215},
        //{284, 182}
        {334, 215}
    }};
    
    for (int32_t i = 0; i < 5; ++i)
    {
        int32_t itemId = CharInfoCharacter->Accessories[i];
        if (itemId && itemId != DAT_00a3e7a0)
        {
            GFX::Image *img = _menuImages.at(  AcessoriesInfo.at( _state.Items.at(itemId).InfoID ).ImgID  );   
            ImgQueue2(img, AccPos[i] - img->GetSize() / 2, MAPRECT);
        }
    }
    
    if (DAT_00a3e7a0)
    {
        GFX::Image *img = _menuImages.at( FUN_0042c870(&_state.Items.at(DAT_00a3e7a0)) );
        //(167, 322)
        constexpr const Common::Point slotPlace(215, 393);
        ImgQueue2(img, slotPlace - img->GetSize() / 2, MAPRECT);
    }
    
    constexpr const std::array<Common::Point, 25> txtPos =
    {{
    /*{413, 48},*/{519, 63},
    /*{415, 68},*/{519, 86},
    /*{532, 110},*/{665, 138},
    /*{532, 126},*/{665, 158},
    /*{532, 145},*/{665, 181},
    /*{532, 162},*/{665, 203},
    /*{310, 281},*/{388, 352},
    /*{482, 110},*/{602, 138},
    /*{482, 126},*/{602, 158},
    /*{482, 145},*/{602, 181},
    /*{482, 162},*/{602, 203},
    /*{482, 179},*/{602, 224},
    /*{482, 196},*/{602, 245},
    /*{567, 220},*/{715, 275},
    /*{567, 237},*/{715, 296},
    /*{567, 254},*/{715, 318},
    /*{567, 271},*/{715, 339},
    /*{567, 288},*/{715, 360},
    /*{470, 320},*/{588, 400},
    /*{470, 340},*/{588, 425},
    /*{310, 298},*/{388, 373},
    /*{310, 315},*/{388, 394},
    /*{310, 332},*/{388, 415},
    /*{310, 349},*/{388, 436},
    /*{575, 50}*/{700, 63}
    }};

    TextQueue(std::to_string(CharInfoCharacter->Exp), _Fonts[3], txtPos[0], MAPRECT);
    TextQueue(std::to_string(CharInfoCharacter->Level), _Fonts[3], txtPos[1], MAPRECT);
    
    TextQueue(std::to_string(CharInfoCharacter->CurrentHarizm), _Fonts[3], txtPos[2], MAPRECT);
    TextQueue(std::to_string(CharInfoCharacter->CurrentSila), _Fonts[3], txtPos[3], MAPRECT);
    TextQueue(std::to_string(CharInfoCharacter->CurrentLovkost), _Fonts[3], txtPos[4], MAPRECT);
    TextQueue(std::to_string(CharInfoCharacter->CurrentVinoslivost), _Fonts[3], txtPos[5], MAPRECT);
    
    TextQueue(std::to_string((CharInfoCharacter->HP + 15) / 16), _Fonts[3], txtPos[6], MAPRECT);
    
    TextQueue(std::to_string(CharInfoCharacter->BaseHarizm), _Fonts[3], txtPos[7], MAPRECT);
    TextQueue(std::to_string(CharInfoCharacter->BaseSila), _Fonts[3], txtPos[8], MAPRECT);
    TextQueue(std::to_string(CharInfoCharacter->BaseLovkost), _Fonts[3], txtPos[9], MAPRECT);
    TextQueue(std::to_string(CharInfoCharacter->BaseVinoslivost), _Fonts[3], txtPos[10], MAPRECT);
    
    TextQueue(std::to_string(CharInfoCharacter->Metkost / 100), _Fonts[3], txtPos[11], MAPRECT);
    TextQueue(std::to_string(CharInfoCharacter->Fehtovanie / 100), _Fonts[3], txtPos[12], MAPRECT);
    TextQueue(std::to_string(CharInfoCharacter->Identification), _Fonts[3], txtPos[13], MAPRECT);
    TextQueue(std::to_string(CharInfoCharacter->Trading), _Fonts[3], txtPos[14], MAPRECT);   
    TextQueue(std::to_string(CharInfoCharacter->Medicine), _Fonts[3], txtPos[15], MAPRECT);
    TextQueue(std::to_string(CharInfoCharacter->KuznechnoeDelo), _Fonts[3], txtPos[16], MAPRECT);
    TextQueue(std::to_string(CharInfoCharacter->PlotnickoeDelo), _Fonts[3], txtPos[17], MAPRECT);
    TextQueue(fmt::sprintf("%4.1f", (float)GetCurrentWeight(CharInfoCharacter) * 0.001), _Fonts[3], txtPos[18], MAPRECT);
    TextQueue(fmt::sprintf("%4.1f", ((CharInfoCharacter->CurrentVinoslivost * 1000) / 3 + 10000) * 0.001), _Fonts[3], txtPos[19], MAPRECT);
    TextQueue(std::to_string(CharInfoCharacter->Otravlenie), _Fonts[3], txtPos[20], MAPRECT);
    TextQueue(std::to_string(GetArmor(CharInfoCharacter)), _Fonts[3], txtPos[21], MAPRECT);

    if (CharInfoCharacter->Arrows == 0 || CharInfoCharacter->field_0x12 != ESLT_1 || CharInfoCharacter->ArmorWeapons[ESLT_1] == 0)
        TextQueue(std::to_string(FUN_0041af48(CharInfoCharacter, 0)), _Fonts[3], txtPos[22], MAPRECT);
    else
        TextQueue(std::to_string(FUN_0041ade4(CharInfoCharacter)), _Fonts[3], txtPos[22], MAPRECT);

    TextQueue(std::to_string(CharInfoCharacter->Gold), _Fonts[3], txtPos[23], MAPRECT);
    
    constexpr const std::array<Common::Point, 6> upPos =
    {{
    /*{578, 70},*/{722, 89},
    /*{573, 110},*/{716, 138},
    /*{573, 126},*/{716, 158},
    /*{573, 145},*/{716, 181},
    /*{573, 162},*/{716, 203},
    /*{515, 303}*/{644, 379}
    }};
    
    std::string tmp = std::to_string(CharInfoCharacter->FreePoints);
    Common::Point p(upPos[0].x - (_menuImages.at(171)->GetSize().x - _Fonts[3]->StringWidth(tmp)) / 2,
                    txtPos[24].y);
    
    TextQueue(tmp, _Fonts[3], p, MAPRECT);
    
    for (int32_t i = 0; i < 5; ++i)
    {
        int32_t points = 0;
        switch(i)
        {
        case 0:
            points = CheckKharUp(*CharInfoCharacter, KHAR_LEVEL);
            break;
            
        case 1:
            points = CheckKharUp(*CharInfoCharacter, KHAR_HARIZMA);
            break;
            
        case 2:
            points = CheckKharUp(*CharInfoCharacter, KHAR_SILA);
            break;
            
        case 3:
            points = CheckKharUp(*CharInfoCharacter, KHAR_LOVKOST);
            break;
            
        case 4:
            points = CheckKharUp(*CharInfoCharacter, KHAR_VINOSLIVOST);
            break;
        }
        
        if (points)
        {
            if (i == 0)
                ImgQueue2(_menuImages.at(171), upPos[i] + Common::Point(0, 2), MAPRECT);
            else
            {
                TextQueue(std::to_string(points), _Fonts[3], upPos[i], MAPRECT);
                ImgQueue2(_menuImages.at(171), upPos[i] + Common::Point(24, 2), MAPRECT);
            }
        }
    }
    
    if ((CharInfoCharacter->field_0x3 & 2) == 0)
        ImgQueue2(_menuImages.at(169), upPos[5], MAPRECT);
    else
        ImgQueue2(_menuImages.at(170), upPos[5], MAPRECT);
}

void Engine::DrawJournal()
{
    FillBkgRect(MAPRECT);
    //(114, 0)
    constexpr const Common::Point pimg(144, 0);
    ImgQueue2(_menuImages.at(1), pimg, MAPRECT);
    
    //485
    const int32_t wi = 606;
    //(133, 20)
    Common::Point pos = Common::Point(166, 25);
    
    DAT_00a3e7c4 = 0;
    for (int32_t i = 0; i < 300; ++i)
    {
        if ((QuestsState[i].State & 0x80) && QuestsState[i].QuestID != 0xffff)
        {
            DAT_00a3e7c4++;
            if (DAT_00a3e7c4 > DWORD_00a3e7b4)
            {
                
                pos.y = PlaceTextWidth(FUN_004363e8(QuestsState[i].QuestID), _Fonts[3], pos, wi) + 3;
                if (pos.y >= DAT_00a3e88c - 45)
                    break;
            }
        }
    }
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
    
//    if (_playScreenID == PLSCREEN_3)
//        PlayChangeScreen(PLSCREEN_3);
    
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
    //PlayChangeScreen(PLSCREEN_3);
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

bool Engine::FUN_0041eb9c(std::array<int16_t, INVSIZE> *inv, int32_t p1, int32_t p2)
{
    if (p2 + p1 >= INVSIZE)
        return false;
    
    if (inv->at(p2 + p1) == DAT_00a3e7a0 || inv->at(p2 + p1) == 0)
        return false;

    return true;
}

int32_t Engine::FUN_0041ade4(Character *pchar)
{
    const WeapArmorItemInfo &bow = ArmorWeaponInfo.at(  _state.Items.at(pchar->ArmorWeapons[ESLT_1]).InfoID  );
    const WeapArmorItemInfo &arrow = ArmorWeaponInfo.at(  _state.Items.at(pchar->Arrows).InfoID  );
    
    int32_t dmg = bow.Dmg * arrow.Dmg;
    if (pchar->CurrentUdr)
    {
        if ((pchar->Flags & 2) == 0)
            dmg = pchar->CurrentUdr;
        else
            dmg += pchar->CurrentUdr;
    }
    return dmg;
}

}
