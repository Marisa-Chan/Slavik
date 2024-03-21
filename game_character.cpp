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
    
    if (!(pchar.ClassID & CLASS_BIT40)) 
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


void Engine::FUN_00414078(Character *pchar)
{
    if ( !(pchar->ClassID & CLASS_BIT40) ) 
    {
        Resources::CharacterSprites &chbase = Res.CharacterBases.at(pchar->CharacterBase);
        auto &frmInfo = chbase.Seq[pchar->State][pchar->Direction].FrameData[pchar->Frame];
        
        pchar->pFrame = frmInfo.FrameID;
        pchar->imgOffset = frmInfo.ImgOffset;
        pchar->imgSize = chbase.Images.at(pchar->pFrame)->SW.Size();
    }
    else 
    {
        if (pchar->State > CHSTATE_5)
            pchar->State -= 6;
              
        Resources::DynamicObject &obj = Res.DynObjects.at(pchar->CharacterBase);
        auto &frmInfo = obj.Seq[pchar->State][pchar->Direction].FrameData[pchar->Frame];
        
        pchar->pFrame = frmInfo.FrameID;
        pchar->imgOffset = frmInfo.ImgOffset;
        
        if (!(pchar->field_0x3 & 0x80)) 
            pchar->imgSize = obj.Images.at(pchar->pFrame)->SW.Size();
        else 
            pchar->imgSize = Common::Point(1, 1);
    }
    pchar->RecalcViewPos(GScrOff);
}

void Engine::DrawCharacterSprite(Character &ch)
{    
    if ( !(ch.field_0x3 & 4) ) 
    {
        for (int32_t i : EqLookUp1[ ch.Direction ]) 
        {
            int32_t itemID = ch.ArmorWeapons[i];
            if (itemID)
            {
                const WeapArmorItemInfo &info = ArmorWeaponInfo.at( _state.Items.at(itemID).InfoID );
                if (info.SprImage > -1)
                {
                    Resources::CharacterSprites &spr = Res.CharacterEquip.at(info.SprImage);
                    Resources::CharacterSprites::FrameInfo &frm = spr.Seq[ch.State][ch.Direction].FrameData.at(ch.Frame);
                    
                    GFXDrawer.Draw(spr.Images.at(frm.FrameID), 
                                   Res.CharEquipPal.at(info.SprImage), 
                                   _outOff + ch.POS + ch.wpnOffset + frm.ImgOffset); //???
                }
            }
        }
    }
    else
    {
        int32_t tmp = 0;
        
        if (ch.State == CHSTATE_1)
            tmp = 3;
        else if (ch.State == CHSTATE_5)
            tmp = 2;
        else if (ch.State == CHSTATE_4)
            tmp = 1;
        else
            tmp = 0;
        
        int32_t eqSlot = ch.field_0x12;
        int32_t eqSlot2 = ESLT_0;
        
        const int8_t *lkp = nullptr;
        if ((eqSlot == ESLT_1) && (tmp != ESLT_2)) 
        {
            lkp = EqLookUp2[tmp][ch.Direction];
            eqSlot2 = EQSLOT_UNK;
        }
        else 
        {
            if (eqSlot == ESLT_1)
                eqSlot = EQSLOT_UNK;
            
            lkp = EqLookUp3[tmp][ch.Direction];
            eqSlot2 = ESLT_5;
        }
        
        for (int32_t i = 0; i < 6; ++i) 
        {
            int32_t itemID = ch.ArmorWeapons[ lkp[i] ];
            if (itemID)
            {
                const WeapArmorItemInfo &info = ArmorWeaponInfo.at( _state.Items.at(itemID).InfoID );
                if (info.SprImage > -1)
                {
                    int32_t simg = info.SprImage;
                    
                    if ((eqSlot == lkp[i]) || (eqSlot2 == lkp[i]))
                        simg += 1;
                    
                    Resources::CharacterSprites &spr = Res.CharacterEquip.at(simg);
                    Resources::CharacterSprites::FrameInfo &frm = spr.Seq[ch.State][ch.Direction].FrameData.at(ch.Frame);
                                        
                    GFXDrawer.Draw(spr.Images.at(frm.FrameID), 
                                   Res.CharEquipPal.at(info.SprImage), 
                                   _outOff + ch.POS + ch.wpnOffset + frm.ImgOffset); //???
                }
            }
        }
    }
}


bool Engine::PlaceMob(Character *pchar)
{
    Common::Rect spawnRect = _state.MapChar_ARRAY.at( pchar->MapCharID ).spwnTlRect;
    int32_t Hcenter = spawnRect.Height() / 2;
    int32_t Wcenter = spawnRect.Width() / 2;
    
    for(int32_t i = 0; i < 100; ++i)
    {
        int32_t hdist = 0;
        if (Hcenter >= 2)
            hdist = (System::rand() % Hcenter);
        
        int32_t hdir = 1;
        if (System::rand() % 2 == 0)
            hdir = -1;
        
        pchar->Tile.y = ((spawnRect.bottom + spawnRect.top + 1) / 2) + hdir * hdist;
        
        int32_t wdist = 0;
        if (Wcenter >= 2)
            wdist = (System::rand() % Wcenter);
        
        int32_t wdir = 1;
        if (System::rand() % 2 == 0)
            wdir = -1;
        
        pchar->Tile.x = ((spawnRect.right + spawnRect.left + 1) / 2) + wdir * wdist;
        
        if (_currentMap->FootMap(pchar->Tile).ID == 0)
            return true;
    }
    return false;
}



Engine::Character *Engine::CalcMapChar(MapChar *mchar)
{
    Character &pchar = _state.Characters.at(mchar->CharacterIndex);
    
    int32_t lovkost = pchar.CurrentLovkost;
    int32_t harizm = pchar.CurrentHarizm;    
    int32_t level = pchar.Level;
    int32_t gold = pchar.Gold;
    int32_t armor = pchar.Armor;
    int32_t f52 = pchar.field62_0x52;
    int32_t sila = pchar.CurrentSila;
    int32_t f11 = pchar.field15_0x11;
    int32_t vinoslivost = pchar.CurrentVinoslivost;
        
    if (mchar->GroupSize > 1) 
    {
        for (int32_t i = 1; i < mchar->GroupSize; ++i) 
        {
            Character &gchar = _state.Characters.at(mchar->CharacterIndex + i);
            
            lovkost += gchar.CurrentLovkost;
            harizm += gchar.CurrentHarizm;
            level += gchar.Level;
            gold += gchar.Gold;
            armor += gchar.Armor;
            f52 += gchar.field62_0x52;
            sila += gchar.CurrentSila;
            f11 += gchar.field15_0x11;
            vinoslivost += gchar.CurrentVinoslivost;
        }
        
        lovkost /= mchar->GroupSize;
        harizm /= mchar->GroupSize;
        level /= mchar->GroupSize;
        gold /= mchar->GroupSize;
        armor /= mchar->GroupSize;
        f52 /= mchar->GroupSize;
        sila /= mchar->GroupSize;
        vinoslivost /= mchar->GroupSize;
        f11 /= mchar->GroupSize;
    }
    
    int32_t local_44 = 0;
    
    if (mchar->GroupSize >= 2)
        local_44 = System::rand() % mchar->GroupSize;
    
    local_44 += mchar->CharacterIndex;
    
    
    const int32_t lastId = mchar->CharacterIndex + mchar->GroupSize;
    Character &lchar = _state.Characters.at(lastId);
    
    lchar.ClassID = mchar->unk5;
    lchar.ArmorWeapons.fill(0);
    lchar.Inventory.fill(0);
    lchar.Accessories.fill(0);
    
    if (lchar.ClassID == 0x4c || lchar.ClassID == 0x46 || lchar.ClassID == 0x55)
        lchar.State = CHSTATE_4;
    else
        lchar.State = CHSTATE_0;
    
    lchar.Direction = System::rand() % 8;
    lchar.CurrentLovkost = lchar.BaseLovkost = lovkost;
    lchar.CurrentHarizm = lchar.BaseHarizm = harizm;
    lchar.HP = 1600;
    lchar.NameID = lchar.NickID = 0;
    lchar.field96_0xd0 = -1;
    lchar.Tile = Common::Point();
    lchar.Level = level;
    lchar.CharacterBase = _state.Characters[local_44].CharacterBase;
    lchar.Gold = gold;
    lchar.Armor = armor;
    lchar.MapCharID = mchar->Index;
    lchar.field2_0x2 = 0x40;
    lchar.paletteOffset = _state.Characters[local_44].paletteOffset;
    lchar.field17_0x13.at(0) = 0xff;
    lchar.Frame = 0;
    lchar.Otravlenie = 0;
    lchar.field62_0x52 = f52;
    
    if (lchar.ClassID == 0x4c)
        lchar.field_0x12 = (System::rand() % 5) + 1;
    
    lchar.field_0x3 = 4;
    lchar.CurrentSila = lchar.BaseSila = sila;
    lchar.field15_0x11 = f11;
    lchar.CurrentVinoslivost = lchar.BaseVinoslivost = vinoslivost;
    
    if (!PlaceMob(&lchar))
        return nullptr;
    
    mchar->GroupSize++;
    
    return &lchar;
}



void Engine::FUN_0041c750(Character *pchar)
{    
    std::array<int32_t, 9> TempBonusValues;
    std::array<int, 9> TempBonusValueType;
    
    TempBonusValues.fill(0);
    TempBonusValueType.fill(0);
    
    for (int32_t i = 0; i < 6; ++i) 
    {
        int32_t itemID = pchar->ArmorWeapons.at(i);
        if (itemID)
        {
            ItemInfo &inf = _state.Items.at(itemID);
            if (inf.BonusID > -1 && (inf.Flags & 1))
            {   
                for (int i = 0; i < 3; i++)
                {
                    const Bonus &pBonus = BonusesInfo.at(inf.BonusID).Bonuses[i];
                    int32_t bnsID = pBonus.BonusID - 1;
                    if (bnsID > -1 && TempBonusValueType[bnsID]) 
                    {
                        TempBonusValueType[bnsID] &= pBonus.BonusType;

                        if (TempBonusValueType[bnsID] == 0)
                            TempBonusValueType[bnsID] = pBonus.BonusValue;
                        else
                            TempBonusValueType[bnsID] += pBonus.BonusValue;
                    }
                }
            }
        }
    }
    
    for (int32_t i = 0; i < 5; ++i) 
    {
        int32_t itemID = pchar->Accessories.at(i);
        if (itemID)
        {
            ItemInfo &inf = _state.Items.at(itemID);
            if (inf.BonusID > -1 && (inf.Flags & 1))
            {   
                for (int i = 0; i < 3; i++)
                {
                    const Bonus &pBonus = BonusesInfo.at(inf.BonusID).Bonuses[i];
                    int32_t bnsID = pBonus.BonusID - 1;
                    if (bnsID > -1 && TempBonusValueType[bnsID]) 
                    {
                        TempBonusValueType[bnsID] &= pBonus.BonusType;

                        if (TempBonusValueType[bnsID] == 0)
                            TempBonusValueType[bnsID] = pBonus.BonusValue;
                        else
                            TempBonusValueType[bnsID] += pBonus.BonusValue;
                    }
                }
            }
        }
    }
    
    int32_t iVar13 = pchar->field98_0xd2;
    if (pchar->field98_0xd2 > -1) 
    {
        for (int i = 0; i < 3; i++)
        {
            const Bonus &pBonus = BonusesInfo.at(pchar->field98_0xd2).Bonuses[i];
            int32_t bnsID = pBonus.BonusID - 1;
            if (bnsID > -1 && TempBonusValueType[bnsID]) 
            {
                TempBonusValueType[bnsID] &= pBonus.BonusType;

                if (TempBonusValueType[bnsID] == 0)
                    TempBonusValueType[bnsID] = pBonus.BonusValue;
                else
                    TempBonusValueType[bnsID] += pBonus.BonusValue;
            }
        }
    }
    
    if (pchar->field99_0xd3 > -1) 
    {
        for (int i = 0; i < 3; i++)
        {
            const Bonus &pBonus = BonusesInfo.at(pchar->field99_0xd3).Bonuses[i];
            int32_t bnsID = pBonus.BonusID - 1;
            if (bnsID > -1 && TempBonusValueType[bnsID]) 
            {
                TempBonusValueType[bnsID] &= pBonus.BonusType;

                if (TempBonusValueType[bnsID] == 0)
                    TempBonusValueType[bnsID] = pBonus.BonusValue;
                else
                    TempBonusValueType[bnsID] += pBonus.BonusValue;
            }
        }
    }
    
    TempBonusValues[3] *= 16;
        
    if (TempBonusValues[1] == 0) 
        pchar->CurrentLovkost  = pchar->BaseLovkost;
    else 
    {
        pchar->CurrentLovkost = TempBonusValues[1];
        
        if (TempBonusValueType[1] != 0) 
            pchar->CurrentLovkost += pchar->BaseLovkost;
    }
    
    if (pchar->CurrentLovkost < 0)
        pchar->CurrentLovkost = 0;
    
    
    if (TempBonusValues[2] == 0) 
        pchar->CurrentHarizm = pchar->BaseHarizm;
    else 
    {
        pchar->CurrentHarizm = TempBonusValues[2];
        if (TempBonusValueType[2] != 0)
            pchar->CurrentHarizm += pchar->BaseHarizm;
    }

    if (pchar->CurrentHarizm < 0)
        pchar->CurrentHarizm = 0;
    
    
    int16_t tmpHp = 0;
    if (TempBonusValues[3] == 0)
        tmpHp = pchar->HP;
    else if (TempBonusValueType[3] == 0)
        tmpHp = TempBonusValues[3];
    else 
        tmpHp = TempBonusValues[3] + pchar->HP;
    
    if (tmpHp < 1600)
        pchar->HP = tmpHp;
    else
        pchar->HP = 1600;

    
    if (TempBonusValues[4] == 0)
        pchar->CurrentSila = pchar->BaseSila;
    else 
    {
        pchar->CurrentSila = TempBonusValues[4];
        if (TempBonusValueType[4] != 0) 
            pchar->CurrentSila += pchar->BaseSila;
    }
    
    if (pchar->CurrentSila < 0)
        pchar->CurrentSila = 0;
    
    
    if (TempBonusValues[5] == 0)
        pchar->CurrentVinoslivost = pchar->BaseVinoslivost;
    else 
    {
        pchar->CurrentVinoslivost = TempBonusValues[5];
        if (TempBonusValueType[5] != 0)
            pchar->CurrentVinoslivost += pchar->BaseVinoslivost;
    }
    
    if (pchar->CurrentVinoslivost < 1)
        pchar->CurrentVinoslivost = 1;
    
    pchar->CurrentBrn = TempBonusValues[6];
    pchar->CurrentUdr = TempBonusValues[7];
    pchar->CurrentVer = TempBonusValues[8];
    pchar->Flags = 0;
    
    if (TempBonusValueType[6] != 0)
        pchar->Flags |= 1;
    
    if (TempBonusValueType[7] != 0)
        pchar->Flags |= 2;
    
    if (TempBonusValueType[8] != 0)
        pchar->Flags |= 4;
}



int32_t Engine::GetCurrentWeight(Character *pchar)
{
    int32_t weight = 0;
    for (int32_t i = 0; i < 6; ++i)
    {
        int32_t id = pchar->ArmorWeapons[i];
        if (id != 0)
            weight += ArmorWeaponInfo[_state.Items.at(id).InfoID].unk6;
    }
    
    for (int32_t i = 0; i < 5; ++i)
    {
        int32_t id = pchar->Accessories[i];
        if (id != 0)
            weight += AcessoriesInfo[_state.Items.at(id).InfoID].Weight;
    }
    
    for (int32_t i = 0; i < 32; ++i)
    {
        int32_t id = pchar->Inventory[i];
        if (id != 0)
        {
            ItemInfo &itm = _state.Items.at(id);
            if (itm.TypeID < 6)
                weight += ArmorWeaponInfo[itm.InfoID].unk6;
            else if (itm.TypeID < 9)
                weight += AcessoriesInfo[itm.InfoID].Weight;
            else if (itm.TypeID == 9)
                weight += AlchemiesInfo[itm.InfoID].Weight;
            else if (itm.TypeID == 11)
                weight += MiscItemsInfo[itm.InfoID].Weight;
            else if (itm.TypeID == 12)
                weight += ArmorWeaponInfo[itm.InfoID].unk6 * itm.Concentration;
        }
    }
    
    if (pchar->Arrows != 0)
    {
        ItemInfo &itm = _state.Items.at(pchar->Arrows);
        weight += ArmorWeaponInfo[itm.InfoID].unk6 * itm.Concentration;
    }
    
    return weight;
}



}