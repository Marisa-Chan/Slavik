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
    
    static std::array<std::string, 9> AcessoriesInfoNames;
    
    static std::array<std::string, 14> AlchemiesInfoNames;
    
    static std::array<std::string, 54> MiscItemsInfoNames;
    
    //BuildingInfo
    static std::array<std::string, 42> BuildingInfoNames;
};

}


#endif /* LOCALE_H */

