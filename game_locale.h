#ifndef LOCALE_H
#define LOCALE_H

#include <string>
#include <array>

#include "game.h"


namespace Game {
    
class Locale
{ 
public:
    enum GMSG
    {
        GMSG_VLG_GIVEUP = 0,
        GMSG_INV_OVERLOAD = 1,
        GMSG_INV_FULL = 2,
        GMSG_MAX
    };
    
public:
    
    static std::array<std::string, Engine::BONUS_MAX> BonusName;
    
    static std::array<std::string, 20> NewGameMenu;
    
    static std::array<std::string, 70> WeapArmorNames;
    
    static std::array<std::string, 9> AcessoriesInfoNames;
    
    static std::array<std::string, 14> AlchemiesInfoNames;
    
    static std::array<std::string, 54> MiscItemsInfoNames;
    
    static std::array<std::string, 18> SpecialDamagesDescr;
    
    //BuildingInfo
    static std::array<std::string, 42> BuildingInfoNames;
    
    //Messages
    static std::array<std::string, GMSG_MAX> GameMessages;
};

}


#endif /* LOCALE_H */

