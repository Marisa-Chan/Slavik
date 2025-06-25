#include "fmt/format.h"
#include "fmt/printf.h"
#include "system.h"
#include "game.h"
#include "game_locale.h"

namespace Game
{
    
const std::vector<Common::Rect> Engine::_newGameMenuBoxes =
{
    //{44, 94, 101, 160},
    {44, 108, 120, 190},
    //{137, 94, 195, 160},
    {155, 108, 236, 190},
    //{229, 94, 287, 160},
    {265, 108, 346, 190},
    //{481, 68, 501, 88},
    {582, 81, 602, 101},
    //{448, 68, 468, 88},
    {549, 81, 569, 101},
    //{448, 36, 468, 56},
    {549, 49, 569, 69},
    //{481, 36, 501, 56},
    {582, 49, 602, 69},
    //{465, 160, 485, 175},
    {545, 195, 575, 210},
    //{465, 178, 485, 193},
    {545, 218, 575, 233},
    //{465, 196, 485, 211},
    {545, 239, 575, 254},
    //{465, 214, 485, 229},
    {545, 259, 575, 274},
    //{513, 254, 543, 269},
    {607, 311, 643, 326},
    //{513, 272, 543, 287},
    {607, 333, 643, 348},
    //{575, 334, 605, 349},
    {695, 414, 725, 429},
    //{575, 352, 605, 367},
    {695, 436, 725, 451},
    //{434, 333, 454, 353},
    {542, 413, 562, 433},
    //{434, 350, 454, 370},
    {542, 434, 562, 454},
    //{461, 387, 481, 407},
    {542, 466, 562, 486},
    //{461, 404, 481, 424},
    {542, 487, 562, 507},
    //{461, 421, 481, 441},
    {542, 509, 562, 529},
    //{65, 427, 165, 460},
    {85, 527, 185, 560},
    //{175, 427, 275, 460},
    {195, 527, 295, 560},
    //{470, 127, 470, 127},
    {570, 160, 570, 160},
    //{40, 206, 302, 406},
    {40, 266, 357, 566},
    //{463, 134, 493, 149},
    {543, 164, 577, 179},
    //{435, 160, 465, 175},
    {515, 195, 545, 210},
    //{485, 160, 525, 175},
    {575, 195, 605, 210},
    //{435, 178, 465, 193},
    {515, 218, 545, 233},
    //{485, 178, 525, 193},
    {575, 218, 605, 233},
    //{435, 196, 465, 211},
    {515, 239, 545, 254},
    //{485, 196, 525, 211},
    {575, 239, 605, 254},
    //{435, 214, 465, 229},
    {515, 259, 545, 274},
    //{485, 214, 525, 229},
    {575, 259, 605, 274}
};

  
bool Engine::LoadNEWH() 
{
    _imgQueue1Count = 0;
    _imgQueue2Count = 0;
    _textQueueCount = 0;
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    
    std::string tmp = fmt::format("NEWH{:d}{:s}.RES", SCREENRESX, _langLiter);
    
    FSMgr::File f = FSMgr::Mgr::ReadFile(tmp);
    if (!f)
    {
        System::PostQuitMessage();
        fmt::printf("Menu file %s not found\n", tmp);
        return false;
    }
    
    FreeMenuImages();
    
    uint32_t entrySize;
    size_t pos;
    
    for (int32_t i = 0; i < 13; ++i)
    {
        f->seek(4, 1);
        entrySize = f->readU32L();
        pos = f->tell();
        _menuImages.push_back( Resources::LoadRL16BitImage(&f) );
        f->seek(pos + entrySize, 0);
    }

    return true;
};

void Engine::DrawNewGameMenu(int32_t n)
{    
    for (int32_t y = 0; y < SCREENRESY; y += _menuImages[1]->SW->h) 
    {
        for (int32_t x = 0; x < SCREENRESX; x += _menuImages[1]->SW->w) 
        {
            ImgQueue1(_menuImages[1], Common::Point(x, y), Common::Rect());
        }
    }
    
    ImgQueue1(_menuImages[0], Common::Point(), Common::Rect());
    
    if (_mainCharacter->CharacterBase == 1) 
        ImgQueue2(_menuImages[3], _newGameMenuBoxes[0].Pos(), Common::Rect());
    else
        ImgQueue2(_menuImages[4], _newGameMenuBoxes[0].Pos(), Common::Rect());
    
    if (_mainCharacter->CharacterBase == 2) 
        ImgQueue2(_menuImages[5], _newGameMenuBoxes[1].Pos(), Common::Rect());
    else
        ImgQueue2(_menuImages[6], _newGameMenuBoxes[1].Pos(), Common::Rect());
    
    if (_mainCharacter->CharacterBase == 0) 
        ImgQueue2(_menuImages[7], _newGameMenuBoxes[2].Pos(), Common::Rect());
    else
        ImgQueue2(_menuImages[8], _newGameMenuBoxes[2].Pos(), Common::Rect());

    Common::Point dotSz ( (_newGameMenuBoxes[3].Width()  - _menuImages[2]->SW->w) / 2, 
                          (_newGameMenuBoxes[3].Height() - _menuImages[2]->SW->h) / 2 );
    
    ImgQueue2(_menuImages[2], _newGameMenuBoxes[1 + _mainCharacter->ClassID].Pos() + dotSz, Common::Rect());
    
    TextQueue(std::to_string(_mainCharacter->BaseHarizm), _Fonts[3], _newGameMenuBoxes.at(7).Pos(), Common::Rect());
    TextQueue(std::to_string(_mainCharacter->BaseSila), _Fonts[3], _newGameMenuBoxes.at(8).Pos(), Common::Rect());
    TextQueue(std::to_string(_mainCharacter->BaseLovkost), _Fonts[3], _newGameMenuBoxes.at(9).Pos(), Common::Rect());
    TextQueue(std::to_string(_mainCharacter->BaseVinoslivost), _Fonts[3], _newGameMenuBoxes.at(10).Pos(), Common::Rect());

    float weight = ((_mainCharacter->CurrentVinoslivost * 1000) / 3 + 10000) * 0.001;
    TextQueue(fmt::sprintf("%4.1f", weight), _Fonts[3], _newGameMenuBoxes.at(11).Pos(), Common::Rect());
    
    TextQueue(std::to_string(_mainCharacter->GetMaxPartySize()), _Fonts[3], _newGameMenuBoxes.at(12).Pos(), Common::Rect());
    TextQueue(std::to_string(_mainCharacter->KuznechnoeDelo), _Fonts[3], _newGameMenuBoxes.at(13).Pos(), Common::Rect());
    TextQueue(std::to_string(_mainCharacter->PlotnickoeDelo), _Fonts[3], _newGameMenuBoxes.at(14).Pos(), Common::Rect());

    if (_mainCharacter->Fehtovanie < _mainCharacter->Metkost || _mainCharacter->Fehtovanie == _mainCharacter->Metkost)
        ImgQueue2(_menuImages[2], _newGameMenuBoxes.at(16).Pos() + dotSz, Common::Rect());
    else
        ImgQueue2(_menuImages[2], _newGameMenuBoxes.at(15).Pos() + dotSz, Common::Rect());
    
    if (_mainCharacter->Identification > 10)
        ImgQueue2(_menuImages[2], _newGameMenuBoxes.at(17).Pos() + dotSz, Common::Rect());
    
    if (_mainCharacter->Trading > 10)
        ImgQueue2(_menuImages[2], _newGameMenuBoxes.at(18).Pos() + dotSz, Common::Rect());
    
    if (_mainCharacter->Medicine > 50)
        ImgQueue2(_menuImages[2], _newGameMenuBoxes.at(19).Pos() + dotSz, Common::Rect());

    TextQueue(std::to_string(_mainCharacter->FreePoints), _Fonts[3], _newGameMenuBoxes.at(24).Pos(), Common::Rect());
    
    if (n == 1)
        ImgQueue2(_menuImages[12], _newGameMenuBoxes.at(20).Pos(), Common::Rect());
    else
        ImgQueue2(_menuImages[11], _newGameMenuBoxes.at(20).Pos(), Common::Rect());
    
    if (n == -1)
        ImgQueue2(_menuImages[10], _newGameMenuBoxes.at(21).Pos(), Common::Rect());
    else
        ImgQueue2(_menuImages[9], _newGameMenuBoxes.at(21).Pos(), Common::Rect());
    
    DrawNewGameChar();
}

void Engine::DrawNewGameChar()
{
    Common::Rect drawRect = _newGameMenuBoxes.at(22);
    drawRect.SetSize(160, 160);
    Common::Point pos = drawRect.Pos() + Common::Point(60, 20);

    ImgQueue2(Res.CharacterBases.at(_mainCharacter->CharacterBase).Images[_mainCharacter->pFrame], 
                                    Res.CharBasePal.at(_mainCharacter->CharacterBase),
                                    pos,
                                    drawRect);
    
    if ((_mainCharacter->field_0x3 & 4) == 0) 
    {
        for (int32_t i : EqLookUp1[ _mainCharacter->Direction ]) 
        {
            int32_t itemID = _mainCharacter->ArmorWeapons[i];
            if (itemID)
            {
                const WeapArmorItemInfo &info = ArmorWeaponInfo.at( _state.Items.at(itemID).InfoID );
                if (info.SprImage > -1)
                {
                    Resources::CharacterSprites &spr = Res.CharacterEquip.at(info.SprImage);
                    Resources::CharacterSprites::FrameInfo &frm = spr.Seq[_mainCharacter->State][_mainCharacter->Direction].FrameData.at(_mainCharacter->Frame);
                    
                    ImgQueue2(spr.Images.at(frm.FrameID), 
                              Res.CharEquipPal.at(info.SprImage), 
                              pos - _mainCharacter->imgOffset + _mainCharacter->wpnOffset + frm.ImgOffset, 
                              drawRect);
                }
            }
        }
    }
    else
    {
        int32_t tmp = 0;
        
        if (_mainCharacter->State == CHSTATE_1)
            tmp = 3;
        else if (_mainCharacter->State == CHSTATE_5)
            tmp = 2;
        else if (_mainCharacter->State == CHSTATE_4)
            tmp = 1;
        else
            tmp = 0;
        
        int32_t eqSlot = _mainCharacter->field_0x12;
        int32_t eqSlot2 = ESLT_0;
        
        const int8_t *lkp = nullptr;
        if ((eqSlot == ESLT_1) && (tmp != ESLT_2)) 
        {
            lkp = EqLookUp2[tmp][_mainCharacter->Direction];
            eqSlot2 = EQSLOT_UNK;
        }
        else 
        {
            if (eqSlot == ESLT_1)
                eqSlot = EQSLOT_UNK;
            
            lkp = EqLookUp3[tmp][_mainCharacter->Direction];
            eqSlot2 = ESLT_5;
        }
        
        for (int32_t i = 0; i < 6; ++i) 
        {
            int32_t itemID = _mainCharacter->ArmorWeapons[ lkp[i] ];
            if (itemID)
            {
                const WeapArmorItemInfo &info = ArmorWeaponInfo.at( _state.Items.at(itemID).InfoID );
                if (info.SprImage > -1)
                {
                    int32_t simg = info.SprImage;
                    
                    if ((eqSlot == lkp[i]) || (eqSlot2 == lkp[i]))
                        simg += 1;
                    
                    Resources::CharacterSprites &spr = Res.CharacterEquip.at(simg);
                    Resources::CharacterSprites::FrameInfo &frm = spr.Seq[_mainCharacter->State][_mainCharacter->Direction].FrameData.at(_mainCharacter->Frame);
                                        
                    ImgQueue2(spr.Images.at(frm.FrameID), 
                              Res.CharEquipPal.at(simg), 
                              pos - _mainCharacter->imgOffset + _mainCharacter->wpnOffset + frm.ImgOffset, 
                              drawRect);
                }
            }
        }
    }
}

void Engine::UpdateNewGameMenu()
{
    int32_t id = GetMouseOnScreenBox(_uiMousePos, _newGameMenuBoxes);
    int32_t drawID = 0;
    int32_t drawTextID = -1;
    
    if (((_mousePress & MOUSEB_L) == 0))
    {
        if ( (id >= 0 && (id < 20)) || (id > 24)) 
        {
            drawTextID = id;
            
            if ((id == 25) || (id == 26))
                drawTextID = 7;
            else if ((id == 27) || (id == 28))
                drawTextID = 8;
            else if ((id == 29) || (id == 30))
                drawTextID = 9;
            else if ((id == 31) || (id == 32))
                drawTextID = 10;
        }
    }
    else
    {
        switch(id) 
        {
        case 0:
                // Драгомир
            _mainCharacter->NameID = 124;
            _mainCharacter->CharacterBase = 1;
            _mainCharacter->paletteOffset = Resources::CharBasePal[1];
            
            for( auto &t : _mainCharacter->ArmorWeapons)
                t = 0;
            
            if (_mainCharacter->Fehtovanie < _mainCharacter->Metkost || _mainCharacter->Fehtovanie == _mainCharacter->Metkost) 
                _mainCharacter->field_0x12 = ESLT_1;
            else
                _mainCharacter->field_0x12 = ESLT_0;
            
            _mainCharacter->ArmorWeapons[ESLT_0] = 3;
            _mainCharacter->ArmorWeapons[ESLT_1] = 4;
            _mainCharacter->ArmorWeapons[ESLT_3] = 1;
            _mainCharacter->ArmorWeapons[ESLT_4] = 8;
            _mainCharacter->ArmorWeapons[ESLT_5] = 11;
            
            FUN_004143dc(_mainCharacter, CHSTATE_0);
            drawTextID = 0;
            break;
        case 1:
                // Михаил
            _mainCharacter->NameID = 151;
            _mainCharacter->CharacterBase = 2;
            _mainCharacter->paletteOffset = Resources::CharBasePal[2];
            
            for( auto &t : _mainCharacter->ArmorWeapons)
                t = 0;
            
            if (_mainCharacter->Fehtovanie < _mainCharacter->Metkost || _mainCharacter->Fehtovanie == _mainCharacter->Metkost)
                _mainCharacter->field_0x12 = ESLT_1;
            else 
                _mainCharacter->field_0x12 = ESLT_2;
            
            _mainCharacter->ArmorWeapons[ESLT_1] = 4;
            _mainCharacter->ArmorWeapons[ESLT_2] = 5;
            _mainCharacter->ArmorWeapons[ESLT_4] = 7;
            _mainCharacter->ArmorWeapons[ESLT_5] = 10;
            
            FUN_004143dc(_mainCharacter, CHSTATE_0);
            drawTextID = 1;
            break;
        case 2:
                // Волк
            _mainCharacter->NameID = 10;
            _mainCharacter->CharacterBase = 0;
            _mainCharacter->paletteOffset = Resources::CharBasePal[0];
            
            for( auto &t : _mainCharacter->ArmorWeapons)
                t = 0;
            
            if (_mainCharacter->Fehtovanie < _mainCharacter->Metkost || _mainCharacter->Fehtovanie == _mainCharacter->Metkost)
                _mainCharacter->field_0x12 = ESLT_1;
            
            else
                _mainCharacter->field_0x12 = ESLT_2;
            
            _mainCharacter->ArmorWeapons[ESLT_1] = 4;
            _mainCharacter->ArmorWeapons[ESLT_2] = 5;
            _mainCharacter->ArmorWeapons[ESLT_3] = 2;
            _mainCharacter->ArmorWeapons[ESLT_4] = 6;
            _mainCharacter->ArmorWeapons[ESLT_5] = 9;
            
            FUN_004143dc(_mainCharacter, CHSTATE_0);
            drawTextID = 2;
            break;
        case 3:
                // КУПЕЦ
            _mainCharacter->CurrentLovkost = _mainCharacter->BaseLovkost = 3;
            _mainCharacter->CurrentHarizm = _mainCharacter->BaseHarizm = 3;
            _mainCharacter->CurrentSila = _mainCharacter->BaseSila = 10;
            _mainCharacter->CurrentVinoslivost = _mainCharacter->BaseVinoslivost = 12;
            _mainCharacter->FreePoints = 0;
            _mainCharacter->ClassID = CLASS_KUPETC;
            
            drawTextID = id;
            break;
        case 4:
                // ОХОТНИК
            _mainCharacter->CurrentLovkost = _mainCharacter->BaseLovkost = 15;
            _mainCharacter->CurrentHarizm = _mainCharacter->BaseHarizm = 3;
            _mainCharacter->CurrentSila = _mainCharacter->BaseSila = 3;
            _mainCharacter->CurrentVinoslivost = _mainCharacter->BaseVinoslivost = 10;
            _mainCharacter->FreePoints = 0;
            _mainCharacter->ClassID = CLASS_OHOTNIK;
            
            drawTextID = id;
            break;
        case 5:
                // ВОЖДЬ
            _mainCharacter->CurrentLovkost = _mainCharacter->BaseLovkost = 3;
            _mainCharacter->CurrentHarizm = _mainCharacter->BaseHarizm = 25;
            _mainCharacter->CurrentSila = _mainCharacter->BaseSila = 7;
            _mainCharacter->CurrentVinoslivost = _mainCharacter->BaseVinoslivost = 7;
            _mainCharacter->FreePoints = 0;
            _mainCharacter->ClassID = CLASS_VOJD;
            
            drawTextID = id;
            break;
        case 6:
                // ВОИН
            _mainCharacter->CurrentLovkost = _mainCharacter->BaseLovkost = 3;
            _mainCharacter->CurrentHarizm = _mainCharacter->BaseHarizm = 3;
            _mainCharacter->CurrentSila = _mainCharacter->BaseSila = 15;
            _mainCharacter->CurrentVinoslivost = _mainCharacter->BaseVinoslivost = 10;
            _mainCharacter->FreePoints = 0;
            _mainCharacter->ClassID = CLASS_VOIN;
            
            drawTextID = id;
            break;
            
        case 15:
            _mainCharacter->Fehtovanie = 100;
            _mainCharacter->Metkost = 0;
            _mainCharacter->PlotnickoeDelo = 1;
            _mainCharacter->KuznechnoeDelo = 10;
            
            if (_mainCharacter->CharacterBase == 1)
                _mainCharacter->field_0x12 = ESLT_0;
            else
                _mainCharacter->field_0x12 = ESLT_2;
            
            drawTextID = id;
            break;
            
        case 16:
            _mainCharacter->Fehtovanie = 0;
            _mainCharacter->Metkost = 100;
            _mainCharacter->PlotnickoeDelo = 2;
            _mainCharacter->KuznechnoeDelo = 5;
            _mainCharacter->field_0x12 = ESLT_1;
            drawTextID = id;
            break;
            
        case 17:
            _mainCharacter->Trading = 10;
            _mainCharacter->Medicine = 50;
            _mainCharacter->Identification = 20;
            drawTextID = id;
            break;
            
        case 18:
            _mainCharacter->Trading = 20;
            _mainCharacter->Medicine = 50;
            _mainCharacter->Identification = 10;
            drawTextID = id;
            break;
            
        case 19:
            _mainCharacter->Trading = 10;
            _mainCharacter->Medicine = 100;
            _mainCharacter->Identification = 10;
            drawTextID = id;
            break;
            
        case 20:
            _stateMode = STATEMD_MODE8;
            drawID = 1;
            break;
            
        case 21:
            _stateMode = STATEMD_MODE7;
            _isGameStarted = false;
            _nextStateMode = STATEMD_NEWGAME;
            drawID = -1;
            break;
            
        case 25:
            if (_mainCharacter->BaseHarizm) 
            {
                _mainCharacter->BaseHarizm -= 1;
                _mainCharacter->FreePoints += 1000;
                int32_t points = CheckKharUp(*_mainCharacter, KHAR_HARIZMA);
                _mainCharacter->FreePoints -= 1000;
                
                if (points == 0)
                    _mainCharacter->BaseHarizm += 1;
                else 
                {
                    _mainCharacter->FreePoints += points;
                    _mainCharacter->CurrentHarizm -= 1;

                    drawTextID = 7;
                }
            }
            break;
            
        case 26:
        {
            int32_t points = CheckKharUp(*_mainCharacter, KHAR_HARIZMA);
            if (points != 0) {
                _mainCharacter->CurrentHarizm += 1;
                _mainCharacter->BaseHarizm += 1;
                _mainCharacter->FreePoints -= points;
                
                drawTextID = 7;
            }
        }
            break;
            
        case 27:
            if (_mainCharacter->BaseSila) 
            {
                _mainCharacter->BaseSila -= 1;
                _mainCharacter->FreePoints += 1000;
                int32_t points = CheckKharUp(*_mainCharacter, KHAR_SILA);
                _mainCharacter->FreePoints -= 1000;
                
                if (points == 0)
                    _mainCharacter->BaseSila += 1;
                else 
                {
                    _mainCharacter->FreePoints += points;

                    drawTextID = 8;
                }
            }
            break;
            
        case 28:
        {
            int32_t points = CheckKharUp(*_mainCharacter, KHAR_SILA);
            if (points != 0) {
                _mainCharacter->BaseSila += 1;
                _mainCharacter->FreePoints -= points;
                
                drawTextID = 8;
            }
        }
            break;
            
        case 29:
            if (_mainCharacter->BaseLovkost) 
            {
                _mainCharacter->BaseLovkost -= 1;
                _mainCharacter->FreePoints += 1000;
                int32_t points = CheckKharUp(*_mainCharacter, KHAR_LOVKOST);
                _mainCharacter->FreePoints -= 1000;
                
                if (points == 0)
                    _mainCharacter->BaseLovkost += 1;
                else 
                {
                    _mainCharacter->FreePoints += points;

                    drawTextID = 9;
                }
            }
            break;
            
        case 30:
        {
            int32_t points = CheckKharUp(*_mainCharacter, KHAR_LOVKOST);
            if (points != 0) {
                _mainCharacter->BaseLovkost += 1;
                _mainCharacter->FreePoints -= points;
                
                drawTextID = 9;
            }
        }
            break;
            
        case 31:
            if (_mainCharacter->BaseVinoslivost) 
            {
                _mainCharacter->BaseVinoslivost -= 1;
                _mainCharacter->FreePoints += 1000;
                int32_t points = CheckKharUp(*_mainCharacter, KHAR_VINOSLIVOST);
                _mainCharacter->FreePoints -= 1000;
                
                if (points == 0)
                    _mainCharacter->BaseVinoslivost += 1;
                else 
                {
                    _mainCharacter->FreePoints += points;
                    _mainCharacter->CurrentVinoslivost -= 1;

                    drawTextID = 10;
                }
            }
            break;
            
        case 32:
        {
            int32_t points = CheckKharUp(*_mainCharacter, KHAR_VINOSLIVOST);
            if (points != 0) {
                _mainCharacter->BaseVinoslivost += 1;
                _mainCharacter->CurrentVinoslivost += 1;
                _mainCharacter->FreePoints -= points;
                
                drawTextID = 10;
            }
        }
            break;
            
        default:
            break;
        }
    }
    
    DrawNewGameMenu(drawID);
    if (drawTextID >= 0)
        DrawNewGameText(drawTextID);
}

void Engine::DrawNewGameText(int32_t id)
{
    const Common::Rect &rect = _newGameMenuBoxes.at(23);
    PlaceTextWidth(Locale::NewGameMenu.at(id) , _Fonts[3], rect.Pos(), rect.Width());
}

}