#include "game.h"
#include "resources.h"
#include "fmt/core.h"
#include "fmt/printf.h"
#include "system.h"

namespace Game
{
    
const Common::Point GScrOff(140, 0);

const std::map<uint8_t, vec3i> Engine::_mapsLights = 
{{ 42, {-70,   0,   0} },
 { 43, { -1,  -1,  -1} },
 { 44, {-20,  -1, -70} },
 { 45, {  0, -50, -50} },
 { 46, { -1,  -1,  -1} },
 { 47, { -1,  -1,  -1} },
 { 48, { -1,  -1,  -1} },
 { 49, { -1,  -1,  -1} },
 { 50, { -1,  -1,  -1} }};

const std::array<int32_t, 13> Engine::_lightPhazes = 
{      0,   1800,   1830,   3570,
    3600,   5400,   8100,  11700,
   11880,  11910,  16155,  16200,
   18900};
 
const std::vector<Common::Rect> Engine::_mainMenuBoxes =
{
    //{233, 130, 425, 161},
    {290, 161, 590, 210},
    //{231, 164, 404, 199},
    {288, 205, 504, 250},
    //{212, 200, 390, 240},
    {265, 250, 488, 303},
    //{224, 241, 352, 281},
    {280, 303, 440, 354},
    //{215, 282, 333, 321},
    {268, 353, 416, 402},
    //{222, 327, 300, 371},
    {276, 409, 366, 464}
};

const std::vector<Common::Rect> Engine::_saveMenuBoxes = 
{
    //{140, 69, 510, 100},
    {175, 86, 638, 125},
    //{140, 100, 510, 130},
    {175, 125, 638, 162},
    //{140, 131, 510, 160},
    {175, 164, 638, 200},
    //{140, 162, 510, 190},
    {175, 203, 638, 238},
    //{140, 193, 510, 220},
    {175, 241, 638, 275},
    //{140, 224, 510, 250},
    {175, 280, 638, 313},
    //{140, 255, 510, 280},
    {175, 319, 638, 350},
    //{140, 286, 510, 310},
    {175, 358, 638, 388},
    //{140, 317, 510, 340},
    {175, 396, 638, 425},
    //{140, 348, 510, 370},
    {175, 435, 638, 463} 
};



void Engine::FreeMenuImages()
{
    for(GFX::Image *img : _menuImages)
        delete img;
    
    _menuImages.clear();
    
    if (_bkgImage)
        delete _bkgImage;
    
    _bkgImage = nullptr;
}

bool Engine::LoadMainMenuImages()
{
    _imgQueue1Count = 0;
    _imgQueue2Count = 0;
    _textQueueCount = 0;
    
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    
    std::string tmp = fmt::format("MENU{:d}{:s}.RES", SCREENRESX, _langLiter);
    
    FSMgr::File f = FSMgr::Mgr::ReadFile(tmp);
    if (!f)
    {
        System::PostQuitMessage();
        fmt::printf("Menu file %s not found\n", tmp);
        return false;
    }
    
    FreeMenuImages();
    
    f->seek(4, 0);
    uint32_t entrySize = f->readU32L();
    size_t pos = f->tell();
    
    _bkgImage = Resources::LoadRL16BitImage(f.get());
    
    ImgQueue1(_bkgImage, Common::Point(), Common::Rect(_screenSize));

    f->seek(pos + entrySize, 0);
    for (int32_t i = 0; i < 14; ++i)
    {
        f->seek(4, 1);
        entrySize = f->readU32L();
        pos = f->tell();
        _menuImages.push_back( Resources::LoadRL16BitImage(f.get()) );
        f->seek(pos + entrySize, 0);
    }

    return true;
}


}