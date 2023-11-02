#ifndef LOCALE_H
#define LOCALE_H

#include <string>
#include <array>

#include "game.h"


namespace Game {
    
class Locale
{ 
public:
    
    static std::array<std::string, Engine::BONUS_MAX> BonusName;
    
    static std::array<std::string, 20> NewGameMenu;
    
    static std::array<std::string, 70> WeapArmorNames;
};

}


#endif /* LOCALE_H */

