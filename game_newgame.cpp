#include "fmt/format.h"
#include "fmt/printf.h"
#include "system.h"
#include "game.h"
#include "locale.h"

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
        _menuImages.push_back( Resources::LoadRL16BitImage(f.get()) );
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
    //for(int i = 0; )
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
    
    Common::Rect ttt = _newGameMenuBoxes.at(22);
    ttt.SetSize(50, 50);
    ImgQueue2(_menuImages[5], _newGameMenuBoxes.at(22).Pos() - Common::Point(30, 30), ttt);
}

void Engine::UpdateNewGameMenu()
{
    int32_t id = GetMouseOnScreenBox(_mousePos, _newGameMenuBoxes);
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
                _mainCharacter->field_0x12 = 1;
            else
                _mainCharacter->field_0x12 = 0;
            
            _mainCharacter->ArmorWeapons[0] = 3;
            _mainCharacter->ArmorWeapons[1] = 4;
            _mainCharacter->ArmorWeapons[3] = 1;
            _mainCharacter->ArmorWeapons[4] = 8;
            _mainCharacter->ArmorWeapons[5] = 11;
            
            FUN_004143dc(*_mainCharacter, 0);
            drawID = 0;
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
                _mainCharacter->field_0x12 = 1;
            else 
                _mainCharacter->field_0x12 = 2;
            
            _mainCharacter->ArmorWeapons[1] = 4;
            _mainCharacter->ArmorWeapons[2] = 5;
            _mainCharacter->ArmorWeapons[4] = 7;
            _mainCharacter->ArmorWeapons[5] = 10;
            
            FUN_004143dc(*_mainCharacter, 0);
            DrawNewGameMenu(0);
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
                _mainCharacter->field_0x12 = 1;
            
            else
                _mainCharacter->field_0x12 = 2;
            
            _mainCharacter->ArmorWeapons[1] = 4;
            _mainCharacter->ArmorWeapons[2] = 5;
            _mainCharacter->ArmorWeapons[3] = 2;
            _mainCharacter->ArmorWeapons[4] = 6;
            _mainCharacter->ArmorWeapons[5] = 9;
            
            FUN_004143dc(*_mainCharacter, 0);
            drawTextID = 2;
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