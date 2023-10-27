#ifndef FONT_H
#define FONT_H

#include <string>
#include <SDL2/SDL_surface.h>
#include "common/common.h"
#include "fsmgr.h"

namespace GFX {
    
class Font
{
public:
    SDL_Surface *SW = nullptr;
    uint32_t HW = 0;
    int32_t Width = 0; // used for empty chars
    
    std::array<uint32_t, 256 / sizeof(uint32_t)> Present;
    std::array<Common::PointRect, 256> SWTiles;
    std::array<Common::FRect, 256> HWTiles;

    bool IsPresent(uint32_t chr) { return (Present.at(chr >> 5) & (1 << (chr & 0x1F))) != 0;};
    int32_t StringWidth(const std::string &text);
    
public:
    static uint32_t Utf8CharLen(uint8_t chr);
    static uint32_t Utf8GetUnicodeChar(const char *chr);
    
    static SDL_Surface *LoadRL16BitImage(FSMgr::iFile *pfile);
    
};
    
    
}

#endif /* FONT_H */

