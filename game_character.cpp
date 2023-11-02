#include "game.h"
#include "resources.h"
#include "fmt/core.h"
#include "fmt/printf.h"
#include "system.h"

namespace Game
{
    

int32_t Engine::CheckKharUp(Engine::Character &pchar, int32_t param)
{
    /* [class][level][0 - min for lvlup, 1 - max on this level] */
    static const uint8_t KharLovkost[5][13][2] = 
   {{{3, 7}, {5, 10}, {8, 14}, {12, 18}, {16, 22}, {20, 26}, {24, 30}, {28, 32}, {31, 35}, {34, 37}, {36, 39}, {38, 41}, {40, 120} },
    {{3, 7}, {5, 10}, {8, 14}, {12, 18}, {16, 22}, {20, 26}, {24, 30}, {28, 32}, {31, 35}, {34, 37}, {36, 39}, {38, 41}, {40, 120} },
    {{7, 14}, {11, 20}, {17, 28}, {24, 36}, {32, 44}, {40, 52}, {48, 60}, {56, 65}, {63, 70}, {68, 75}, {73, 78}, {77, 81}, {80, 84} },
    {{7, 14}, {11, 20}, {17, 28}, {24, 36}, {32, 44}, {40, 52}, {48, 60}, {56, 65}, {63, 70}, {68, 75}, {73, 78}, {77, 81}, {80, 84} },
    {{10, 19}, {16, 26}, {22, 34}, {30, 43}, {38, 53}, {48, 63}, {58, 74}, {68, 80}, {77, 90}, {85, 95}, {92, 97}, {96, 100}, {98, 150} }};
    
    static const uint8_t KharHarizm[5][13][2] = 
   {{{10, 19}, {16, 26}, {22, 34}, {30, 43}, {38, 53}, {48, 63}, {58, 74}, {68, 80}, {77, 90}, {85, 95}, {92, 97}, {96, 100}, {98, 150} },
    {{3, 7}, {5, 10}, {8, 14}, {12, 18}, {16, 22}, {20, 26}, {24, 30}, {28, 32}, {31, 35}, {34, 37}, {36, 39}, {38, 41}, {40, 120} },
    {{7, 14}, {11, 20}, {17, 28}, {24, 36}, {32, 44}, {40, 52}, {48, 60}, {56, 65}, {63, 70}, {68, 75}, {73, 78}, {77, 81}, {80, 84} },
    {{10, 19}, {16, 26}, {22, 34}, {30, 43}, {38, 53}, {48, 63}, {58, 74}, {68, 80}, {77, 90}, {85, 95}, {92, 97}, {96, 100}, {98, 150} },
    {{3, 7}, {5, 10}, {8, 14}, {12, 18}, {16, 22}, {20, 26}, {24, 30}, {28, 32}, {31, 35}, {34, 37}, {36, 39}, {38, 41}, {40, 120} }};
    
    static const uint8_t KharSila[5][13][2] =
   {{{3, 7}, {5, 10}, {8, 14}, {12, 18}, {16, 22}, {20, 26}, {24, 30}, {28, 32}, {31, 35}, {34, 37}, {36, 39}, {38, 41}, {40, 120} },
    {{7, 14}, {11, 20}, {17, 28}, {24, 36}, {32, 44}, {40, 52}, {48, 60}, {56, 65}, {63, 70}, {68, 75}, {73, 78}, {77, 81}, {80, 84} },
    {{7, 14}, {11, 20}, {17, 28}, {24, 36}, {32, 44}, {40, 52}, {48, 60}, {56, 65}, {63, 70}, {68, 75}, {73, 78}, {77, 81}, {80, 84} },
    {{10, 19}, {16, 26}, {22, 34}, {30, 43}, {38, 53}, {48, 63}, {58, 74}, {68, 80}, {77, 90}, {85, 95}, {92, 97}, {96, 100}, {98, 150} },
    {{3, 7}, {5, 10}, {8, 14}, {12, 18}, {16, 22}, {20, 26}, {24, 30}, {28, 32}, {31, 35}, {34, 37}, {36, 39}, {38, 41}, {40, 120} }};
    
    static const uint8_t KharVinoslivost[5][13][2] =
   {{{3, 7}, {5, 10}, {8, 14}, {12, 18}, {16, 22}, {20, 26}, {24, 30}, {28, 32}, {31, 35}, {34, 37}, {36, 39}, {38, 41}, {40, 120} },
    {{7, 14}, {11, 20}, {17, 28}, {24, 36}, {32, 44}, {40, 52}, {48, 60}, {56, 65}, {63, 70}, {68, 75}, {73, 78}, {77, 81}, {80, 84} },
    {{10, 19}, {16, 26}, {22, 34}, {30, 43}, {38, 53}, {48, 63}, {58, 74}, {68, 80}, {77, 90}, {85, 95}, {92, 97}, {96, 100}, {98, 150} },
    {{3, 7}, {5, 10}, {8, 14}, {12, 18}, {16, 22}, {20, 26}, {24, 30}, {28, 32}, {31, 35}, {34, 37}, {36, 39}, {38, 41}, {40, 120} },
    {{3, 7}, {5, 10}, {8, 14}, {12, 18}, {16, 22}, {20, 26}, {24, 30}, {28, 32}, {31, 35}, {34, 37}, {36, 39}, {38, 41}, {40, 120} }};
    
    if (pchar.field_0xce != 0) 
        return 0;
    
    int32_t classID = 0;
    
    if ((pchar.ClassID & CLASS_BIT40) == 0) 
        classID = (pchar.ClassID & CLASS_MASK) - 1;
    else 
        classID = 1;
    
    static const int8_t MainKharByClass[6] = {-1, KHAR_HARIZMA, KHAR_VINOSLIVOST, KHAR_LOVKOST, KHAR_HARIZMA, KHAR_SILA};
    
    bool isSecondKhar = MainKharByClass[classID] != param; 
    
    if (param == KHAR_LEVEL) 
    {
        if (pchar.Level == 13) // Max level, can't level up
            return 0;

        // Check minimal needed khars for lvlup
        if (pchar.BaseLovkost < KharLovkost[classID][pchar.Level][0])
            return 0;
        
        if (pchar.BaseHarizm < KharHarizm[classID][pchar.Level][0])
            return 0;
        
        if (pchar.BaseSila < KharSila[classID][pchar.Level][0])
            return 0;
        
        if (pchar.BaseVinoslivost < KharVinoslivost[classID][pchar.Level][0])
            return 0;

        return 1;
    }
    
    int32_t khar = 0;
    switch(param) 
    {
        case KHAR_LOVKOST:
            khar = pchar.BaseLovkost;
            if (pchar.BaseLovkost >= KharLovkost[classID][pchar.Level][1]) 
                return 0;
            break;

        case KHAR_HARIZMA:
            khar = pchar.BaseHarizm;
            if (pchar.BaseHarizm >= KharHarizm[classID][pchar.Level][1]) 
                return 0;
            break;

        case KHAR_SILA:
            khar = pchar.BaseSila;
            if (pchar.BaseSila >= KharSila[classID][pchar.Level][1]) 
                return 0;
            break;

        case KHAR_VINOSLIVOST:
            khar = pchar.BaseVinoslivost;
            if (pchar.BaseVinoslivost >= KharVinoslivost[classID][pchar.Level][1]) 
                return 0;
            break;

        default:
            break;
    }
    
    // [level][0,1 - kharlvl for main/secondary for class, 2 - points cost]
    static const int32_t KharValLvlPoints[13][3] = 
    {{13, 9, 2}, {19, 14, 5}, {26, 20, 11}, 
     {34, 28, 14}, {43, 36, 18}, {53, 44, 23}, 
     {63, 52, 29}, {74, 60, 35}, {80, 65, 72}, 
     {90, 70, 100}, {95, 75, 150}, {100, 80, 200}, 
     {999999, 999999, 999} };
    
    for (int32_t i = 0; i < 13; ++i) 
    {
        if (khar < KharValLvlPoints[i][isSecondKhar ? 1 : 0]) 
        {
            if ( pchar.FreePoints >= KharValLvlPoints[i][2] ) 
                return KharValLvlPoints[i][2];
            return 0;
        }
    }
    
    return 0;
}

}