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
        GMSM_SMTH_LOOT = 3,
        GMSM_ALL_TO_ME = 4,
        GMSM_ATTACK = 5,
        GMSM_THINK_INFO = 6,
        GMSM_CHAR_PANEL = 7,
        GMSM_CHANGE_WPN = 8,
        GMSM_DRAW_WEAPON = 9,
        GMSM_MAP = 10,
        GMSM_UNUSABLE = 11,
        GMSM_MEDICTEACH = 12,
        GMSM_PLOTNTEACH = 13,
        GMSM_BLKSMTHTEACH = 14,
        GMSM_TRADETEACH = 15,
        GMSM_IDENTIFYTEACH = 16,
        GMSM_READBIRCHBARK = 17,
        
        GMSG_MAX
    };
    
    enum CHARHINT
    {
        CHAR_HP_LVL = 0,
        CHAR_SQUAD_UNIT = 1,
        CHAR_POISONED = 2,
        CHAR_ENEMY = 3,
        CHAR_KNOW_MEDICINE = 4,
        CHAR_KNOW_BLACKSM = 5,
        CHAR_KNOW_TRADING = 6,
        CHAR_KNOW_CONSTRUCT = 7,
        CHAR_KNOW_IDENTIFY = 8,
        CHARHINT_MAX
    };
    
    enum ITEMDESCR
    {
        ITMDES_DMG_FORCE = 0,
        ITMDES_ARMR = 1,
        ITMDES_WEAR = 2,
        ITMDES_WEIGHT = 3,
        ITMDES_POISON = 4,
        ITMDES_NEEDS = 5,
        ITMDES_UNK_WPN = 6,
        ITMDES_UNK_BOW = 7,
        ITMDES_UNK_ITEM = 8,
        ITMDES_CONCENTRATION = 9,
        ITMDES_INS_CONC = 10,
        ITMDES_QUIVER = 11,
        ITMDES_UNK_ARROWS = 12,
        ITMDES_MAX
    };
    
public:
    
    static std::array<std::string, Engine::BONUS_MAX> BonusName;
    
    static std::array<std::string, 20> NewGameMenu;
    
    static std::array<std::string, 70> WeapArmorNames;
    
    static std::array<std::string, 9> AcessoriesInfoNames;
    
    static std::array<std::string, 14> AlchemiesInfoNames;
    
    static std::array<std::string, 54> MiscItemsInfoNames;
    
    static std::array<std::string, 18> SpecialDamagesDescr;
    
    static std::array<std::string, 30> MonsterNames;
    
    static std::array<std::string, 171> PeopleNames;
    
    static std::array<std::string, 21> PeopleNicks;
    
    static std::array<std::string, 5> CharacterSpeciality;
    
    //BuildingInfo
    static std::array<std::string, 42> BuildingInfoNames;
    
    //Messages
    static std::array<std::string, GMSG_MAX> GameMessages;
    
    //Character hints
    static std::array<std::string, CHARHINT_MAX> CharHints;
    
    static std::array<std::string, 5> Jobs;
    
    static std::array<std::string, 56> MapNames;
    
    static std::array<std::string, ITMDES_MAX> ItemDescr;
};

}


#endif /* LOCALE_H */

