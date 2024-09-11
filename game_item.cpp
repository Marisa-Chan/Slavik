#include "game.h"
#include "game_locale.h"
#include "fmt/printf.h"

namespace Game
{
    

int32_t Engine::GetItemWeight(const ItemInfo *itm) const
{
    if (itm->TypeID < 6)
        return ArmorWeaponInfo[itm->InfoID].unk6;
    else if (itm->TypeID < 9)
        return AcessoriesInfo[itm->InfoID].Weight;
    else if (itm->TypeID == 9)
        return AlchemiesInfo[itm->InfoID].Weight;
    else if (itm->TypeID == 11)
        return MiscItemsInfo[itm->InfoID].Weight;
    else if (itm->TypeID == 12)
        return ArmorWeaponInfo[itm->InfoID].unk6 * itm->Concentration;
    return 0;
}

std::string Engine::GetBonusValueString(const Bonus *b) const
{
    if (b->BonusType == 0)
        return Locale::BonusName.at(b->BonusID - 1) + std::string("=") + std::to_string(b->BonusValue);
    else if (b->BonusValue >= 0)
        return Locale::BonusName.at(b->BonusID - 1) + std::string("+") + std::to_string(b->BonusValue);
    else
        return Locale::BonusName.at(b->BonusID - 1) + std::to_string(b->BonusValue);
}

std::string Engine::GetItemHint(const ItemInfo *itm) const
{
    switch(itm->TypeID)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            if ((itm->Flags & ItemInfo::FLAG_UNIDENTIFIED) == 0)
            {
                std::string s = Locale::WeapArmorNames.at(itm->InfoID);
                if (itm->TypeID < 3)
                    s.append( Locale::ItemDescr[Locale::ITMDES_DMG_FORCE] );
                else
                    s.append( Locale::ItemDescr[Locale::ITMDES_ARMR] );
                
                const WeapArmorItemInfo &aw = ArmorWeaponInfo.at(itm->InfoID);
                
                s.append( std::to_string(aw.Dmg) );
                s.append( Locale::ItemDescr[Locale::ITMDES_WEAR] );
                
                int32_t conc = itm->Concentration;
                if (conc == 0)
                    conc = 1;
                
                s.append( std::to_string( conc ) );
                s.append("/");
                s.append( std::to_string( (int32_t)(itm->Weight) ) );
                
                s.append(Locale::ItemDescr[Locale::ITMDES_WEIGHT]);
                
                s.append( fmt::sprintf("%4.2f", (float)aw.unk6 * 0.001) );

                if ((itm->TypeID == 0 || itm->TypeID == 2) && (itm->Poison != 0))
                {
                    s.append( Locale::ItemDescr[Locale::ITMDES_POISON] );
                    s.append( std::to_string((int32_t)(itm->Poison)) );
                }
                
                if (itm->SpecialID > -1)
                    s.append(Locale::SpecialDamagesDescr.at(itm->SpecialID));

                if (itm->BonusID > -1)
                {
                    const BonusSet &bns = BonusesInfo.at(itm->BonusID);
                    if (bns.Bonuses[0].BonusID)
                        s.append(GetBonusValueString(&bns.Bonuses[0]));
                    
                    if (bns.Bonuses[1].BonusID)
                        s.append(GetBonusValueString(&bns.Bonuses[1]));
                    
                    if (bns.Bonuses[2].BonusID)
                        s.append(GetBonusValueString(&bns.Bonuses[2]));

                    if (bns.unk3)
                    {
                        s.append(Locale::ItemDescr[Locale::ITMDES_NEEDS]);
                        s.append(Locale::BonusName.at(bns.unk3 - 1));
                        s.append(":");
                        s.append(std::to_string(bns.unk4));
                    }
                }
                
                if (aw.unk2)
                {
                    s.append(Locale::ItemDescr[Locale::ITMDES_NEEDS]);
                    s.append(Locale::BonusName.at(aw.unk2 - 1));
                    s.append(":");
                    s.append(std::to_string(aw.unk3));
                }
                return s;
            }
            else if ((itm->TypeID == 0) || (itm->TypeID == 2))
                return Locale::ItemDescr[Locale::ITMDES_UNK_WPN];
            else if (itm->TypeID == 1)
                return Locale::ItemDescr[Locale::ITMDES_UNK_BOW];
            else
                return Locale::ItemDescr[Locale::ITMDES_UNK_ITEM];
        
        case 6:
        case 7:
        case 8:
            if ((itm->Flags & ItemInfo::FLAG_UNIDENTIFIED) == 0)
            {
                const AccessorieItemInfo& acc = AcessoriesInfo.at(itm->InfoID);
                
                std::string s = Locale::AcessoriesInfoNames.at(itm->InfoID);
                s.append(Locale::ItemDescr[Locale::ITMDES_WEIGHT]);
                s.append( fmt::sprintf("%4.2f", (float)acc.Weight * 0.001) );

                if (itm->BonusID > -1)
                {
                    const BonusSet &bns = BonusesInfo.at(itm->BonusID);
                    if (bns.Bonuses[0].BonusID)
                        s.append(GetBonusValueString(&bns.Bonuses[0]));
                    
                    if (bns.Bonuses[1].BonusID)
                        s.append(GetBonusValueString(&bns.Bonuses[1]));
                    
                    if (bns.Bonuses[2].BonusID)
                        s.append(GetBonusValueString(&bns.Bonuses[2]));

                    if (bns.unk3)
                    {
                        s.append(Locale::ItemDescr[Locale::ITMDES_NEEDS]);
                        s.append(Locale::BonusName.at(bns.unk3 - 1));
                        s.append(":");
                        s.append(std::to_string(bns.unk4));
                    }
                }
                return s;
            }
            else
                return Locale::ItemDescr[Locale::ITMDES_UNK_ITEM];
            
        case 9:
        {
            const AccessorieItemInfo& alch = AlchemiesInfo.at(itm->InfoID);
            std::string s = Locale::AlchemiesInfoNames.at(itm->InfoID);

            if (alch.BonusID < 0)
            {
                s.append(Locale::ItemDescr[Locale::ITMDES_CONCENTRATION]);
                s.append(fmt::sprintf("%5.2f", itm->Concentration));
            }
            
            if ((itm->InfoID == 7 && itm->Concentration < 10.0) ||
                (itm->InfoID == 12 && itm->Concentration < 6.0) ||
                (itm->InfoID == 13 && itm->Concentration < 10.0))
                s.append(Locale::ItemDescr[Locale::ITMDES_INS_CONC]);
                
            s.append(Locale::ItemDescr[Locale::ITMDES_WEIGHT]);
            s.append( fmt::sprintf("%4.2f", (float)alch.Weight * 0.001) );
            return s;
        }
        
        case 11:
            if ((itm->Flags & ItemInfo::FLAG_UNIDENTIFIED) == 0)
            {
                const AccessorieItemInfo& misc = MiscItemsInfo.at(itm->InfoID);
                std::string s = Locale::MiscItemsInfoNames.at(itm->InfoID);

                s.append(Locale::ItemDescr[Locale::ITMDES_WEIGHT]);
                s.append( fmt::sprintf("%4.2f", (float)misc.Weight * 0.001) );
                return s;
            }
            else
                return Locale::ItemDescr[Locale::ITMDES_UNK_ITEM]; 
        
        case 12:
        {
            std::string s = Locale::ItemDescr[Locale::ITMDES_QUIVER];
            s.append(std::to_string((int32_t)(itm->Concentration)));
            
            if ((itm->Flags & ItemInfo::FLAG_UNIDENTIFIED) == 0)
            {
                const WeapArmorItemInfo &aw = ArmorWeaponInfo.at(itm->InfoID);
                
                s.append( Locale::ItemDescr[Locale::ITMDES_DMG_FORCE] );
                s.append( std::to_string(aw.Dmg) );
                
                s.append(Locale::ItemDescr[Locale::ITMDES_WEIGHT]);
                s.append( fmt::sprintf("%4.2f", itm->Concentration * (float)aw.unk6 * 0.001) );
                
                if (itm->Poison)
                {
                    s.append( Locale::ItemDescr[Locale::ITMDES_POISON] );
                    s.append( std::to_string((int32_t)(itm->Poison)) );
                }
                
                if (itm->SpecialID > -1)
                    s.append( Locale::SpecialDamagesDescr.at(itm->SpecialID) );
            }
            else
                s.append(Locale::ItemDescr[Locale::ITMDES_UNK_ARROWS]);
            return s;
        }
    }
    return Locale::ItemDescr[Locale::ITMDES_UNK_ITEM];
}

void Engine::MixPotions(Character *pchar, ItemInfo *itm1, ItemInfo *itm2)
{
    const MixInfo *recipe = nullptr;
    
    for (const MixInfo &mx : MixPotionsInfo)
    {
        if (mx.Potion1id == itm1->InfoID && mx.Potion2id == itm2->InfoID)
        {
            recipe = &mx;
            break;
        }
    }
    
    if (!recipe)
    {
        itm1->InfoID = 4;
        itm2->TypeID = -1;
        return;
    }
    
    
    switch(recipe->MixType)
    {
        case 0:
            itm1->Concentration = pchar->Medicine * 0.01;
            break;
        case 1:
            itm1->Concentration += pchar->Medicine * 0.01;
            break;
        case 2:
            itm1->Concentration += itm2->Concentration;
            break;
        case 3:
            if (itm2->Concentration < itm1->Concentration)
                itm1->Concentration = itm2->Concentration;
            break;
        case 4:
            itm1->Concentration = itm2->Concentration + pchar->Medicine * 0.01;
    }
    
    if (itm1->Concentration > 15.0)
        itm1->Concentration = 15.0;
    
    itm1->InfoID = recipe->Result1id;
    itm2->InfoID = recipe->Result2id;

    if (itm2->InfoID < 0)
        itm2->TypeID = -1;
    
    pchar->Medicine += recipe->ExpPoints;
    
    if (pchar->Medicine > 500)
        pchar->Medicine = 500;
}


bool Engine::FUN_00414b3c(Character *pchar)
{
    if (_currentMap->FootMap.At(pchar->Tile).ID != 0)
    {
        Common::Point pt;
        if (FUN_00410ffc(&pt, pchar->Tile) == 0)
        {
            MapChar &mchr = _state.MapChar_ARRAY.at(pchar->MapCharID);
            
            Common::Rect rct;
            if ((mchr.unk5 & 0xc0) == 0)
            {
                rct.top = _mainCharacter->Tile.y - ViewTiles.y * 2;
                rct.bottom = _mainCharacter->Tile.y + ViewTiles.y * 2;
                rct.left = _mainCharacter->Tile.x - ViewTiles.x * 2;
                rct.right = _mainCharacter->Tile.x + ViewTiles.x * 2;
            }
            else
                rct = mchr.spwnTlRect;
            
            if (rct.Height() >= 2)
                pt.y = rct.top + (rand() % rct.Height());
            else
                pt.y = rct.top;
            
            if (rct.Width() >= 2)
                pt.x = rct.left + (rand() % rct.Width());
            else
                pt.x = rct.left;
            
            int32_t iter = 0;
            while (iter < 100)
            {
                if (_currentMap->FootMap.IsIn(pt) && _currentMap->FootMap.At(pt).ID == 0)
                    break;
                
                pt.x++;
                if (pt.x > rct.right)
                {
                    pt.x = rct.left;
                    pt.y++;
                    
                    if (pt.y > rct.bottom)
                        pt.y = rct.top;
                }
                iter++;
            }
            
            if (iter == 100)
                return false;
        }
        pchar->Tile = pt;
    }
    
    _currentMap->FootMap.At(pchar->Tile).ID = pchar->Index + 1;
    
    pchar->HP = 1600;
    pchar->Otravlenie = 0;
    
    if (pchar->ClassID == 0x4c)
        pchar->field_0x12 = (rand() % 5) + 1;
    
    pchar->ClassID &= ~CLASS_BIT80;
    
    FUN_00414ab4(pchar);
    
    //if (pchar->MapCharID == _mainCharacter->MapCharID)
    //    FUN_0042d574(pchar);
    
    return true;
}


bool Engine::FUN_0041db64(Character *pchar, ItemInfo *itm1, ItemInfo *itm2)
{
    if (itm1 && itm1->TypeID == 9)
        MixPotions(pchar, itm1, itm2);
    else
    {
        if (itm2->InfoID < 4)
            return false;
        
        switch(itm2->InfoID)
        {
        case 4:
            pchar->HP = pchar->HP / 2;
            if (pchar->HP == 0)
                pchar->HP = 1;
            break;
            
        case 5:
            pchar->HP += itm2->Concentration * 160.0;
            
            if (pchar->HP > 1600)
                pchar->HP = 1600;
            break;
            
        case 6:
            if (itm1)
            {
                if (itm1->TypeID != 0 && itm1->TypeID != 2 && itm1->TypeID != 12)
                    return false;
                
                itm1->Poison += itm2->Concentration;
            }
            else
                pchar->Otravlenie += itm2->Concentration;
            
            break;
            
        case 7:
            if (!itm1 || itm2->Concentration < 10.0)
                return false;
            
            if (itm1->TypeID != 12)
                return false;
            
            itm1->SpecialID = 0;
            break;
            
        case 8:
            if (pchar->Otravlenie)
            {
                pchar->Otravlenie -= itm2->Concentration * 10.0;
                if (pchar->Otravlenie < 0)
                    pchar->Otravlenie = 0;
            }
            break;
            
        case 9:
        {
            int32_t tmp = (pchar->BaseLovkost) / 3;
            
            if (tmp > itm2->Concentration)
                tmp = itm2->Concentration;
            
            if (pchar->field_0xce)
                return false;
            
            pchar->field76_0xbc = pchar->BaseLovkost;
            pchar->field78_0xbe = pchar->BaseHarizm;
            pchar->field80_0xc0 = pchar->BaseSila;
            pchar->field82_0xc2 = pchar->BaseVinoslivost;
            pchar->field_0xce = tmp * 30;
            pchar->field90_0xca = 0;
            pchar->BaseLovkost -= tmp * 3;
            pchar->BaseHarizm += tmp;
            pchar->BaseSila += tmp;
            pchar->BaseVinoslivost += tmp;
            break;
        }
        
        case 10:
        {
            if (pchar->field_0xce)
                return false;
            
            pchar->field76_0xbc = pchar->BaseLovkost;
            pchar->field78_0xbe = pchar->BaseHarizm;
            pchar->field80_0xc0 = pchar->BaseSila;
            pchar->field82_0xc2 = pchar->BaseVinoslivost;
            
            int32_t tmp = itm2->Concentration;
            pchar->field_0xce = tmp * 30;
            pchar->field90_0xca = 0;
            pchar->BaseLovkost += tmp;
            pchar->BaseSila += tmp;
            pchar->BaseVinoslivost += tmp;
            
            pchar->HP += itm2->Concentration * 160.0;
            if (pchar->HP > 1600)
                pchar->HP = 1600;
            break;
        }
        
        case 11:
        {
            DWORD_00a3e758 = true;
            
            int32_t tmp = itm2->Concentration;
            if (pchar->field_0xce == 0)
            {
                pchar->field76_0xbc = pchar->BaseLovkost;
                pchar->field78_0xbe = pchar->BaseHarizm;
                pchar->field80_0xc0 = pchar->BaseSila;
                pchar->field82_0xc2 = pchar->BaseVinoslivost;
            }
            pchar->field_0xce = tmp * 30;
            pchar->field90_0xca = sqrtf(tmp) * 0.1; //CHECK IT
            break;
        }
        
        case 12:
        {
            int32_t tmp = itm2->Concentration;
            
            if (tmp < 6)
                return false;
            
            FUN_0041079c(pchar, sqrtf(tmp - 5) * 100.0);
            
            if (pchar->field_0xce == 0)
            {
                pchar->field76_0xbc = pchar->BaseLovkost;
                pchar->field78_0xbe = pchar->BaseHarizm;
                pchar->field80_0xc0 = pchar->BaseSila;
                pchar->field82_0xc2 = pchar->BaseVinoslivost;
            }
            
            pchar->field_0xce = tmp * 60;
            pchar->field90_0xca = 0;
            
            pchar->BaseSila -= tmp * 5;
            if (pchar->BaseSila < 1)
                pchar->BaseSila = 1;
            
            pchar->HP -= tmp * 160;
            if (pchar->HP < 160)
                pchar->HP = 160;
            break;
        }
        
        case 13:
            if (itm2->Concentration < 10.0)
                return false;
            
            pchar->HP = 1600;
            
            if (pchar->ClassID & CLASS_BIT80)
                FUN_00414b3c(pchar);
            
            break;
        }
        
        itm2->InfoID = 3;
    }
    
    if (itm1 && itm2->TypeID > -1)
        FUN_00424918(pchar, itm2->Index);
    
    PlaySound(9,0,0,0);
    
    if (pchar->MapCharID == _mainMapChar->Index)
    {
        //FUN_0042d574(pchar);
        
        //if (_playScreenID == PLSCREEN_3)
        //    PlayChangeScreen(PLSCREEN_3);
        
        if (DisplayInvOfCharID != 0)
            FUN_0042f50c(CharInfoCharacter, 0);
    }
    return true;
}


void Engine::IdentifyCharacterItems(Character *pchar)
{
    for (int32_t itmid : pchar->ArmorWeapons)
    {
        if (itmid)
        {
            ItemInfo &itm = _state.Items.at(itmid);
            if (itm.Flags & ItemInfo::FLAG_UNIDENTIFIED)
            {
                _mainCharacter->Identification++;
                itm.Flags &= ~ItemInfo::FLAG_UNIDENTIFIED;
            }
        }
    }
    
    if (pchar->Arrows)
    {
        ItemInfo &itm = _state.Items.at(pchar->Arrows);
        if (itm.Flags & ItemInfo::FLAG_UNIDENTIFIED)
        {
            _mainCharacter->Identification++;
            itm.Flags &= ~ItemInfo::FLAG_UNIDENTIFIED;
        }
    }
    
    for (int32_t itmid : pchar->Inventory)
    {
        if (itmid)
        {
            ItemInfo &itm = _state.Items.at(itmid);
            if (itm.Flags & ItemInfo::FLAG_UNIDENTIFIED)
            {
                _mainCharacter->Identification++;
                itm.Flags &= ~ItemInfo::FLAG_UNIDENTIFIED;
            }
        }
    }
    
    for (int32_t itmid : pchar->Accessories)
    {
        if (itmid)
        {
            ItemInfo &itm = _state.Items.at(itmid);
            if (itm.Flags & ItemInfo::FLAG_UNIDENTIFIED)
            {
                _mainCharacter->Identification++;
                itm.Flags &= ~ItemInfo::FLAG_UNIDENTIFIED;
            }
        }
    }
        
    if (_mainCharacter->Identification > 100)
        _mainCharacter->Identification = 100;
}

bool Engine::FUN_00410010(Character *pchar, ItemInfo *itm1, ItemInfo *itm2)
{
    if (itm1->TypeID != 11)
        return false;

    int32_t id = 0;
    
    if (  (itm2->InfoID == 25 && itm1->InfoID == 26)
       || (itm2->InfoID == 26 && itm1->InfoID == 25))
        id = 41;
    
    if (  (itm2->InfoID == 25 && itm1->InfoID == 27)
       || (itm2->InfoID == 27 && itm1->InfoID == 25))
        id = 42;
    
    
    if (  (itm2->InfoID == 25 && itm1->InfoID == 43)
       || (itm2->InfoID == 43 && itm1->InfoID == 25))
        id = 40;
    
    if (  (itm2->InfoID == 26 && itm1->InfoID == 27)
       || (itm2->InfoID == 27 && itm1->InfoID == 26))
        id = 43;
    
    if (  (itm2->InfoID == 26 && itm1->InfoID == 42)
       || (itm2->InfoID == 42 && itm1->InfoID == 26))
        id = 40;
    
    if (  (itm2->InfoID == 27 && itm1->InfoID == 41)
       || (itm2->InfoID == 41 && itm1->InfoID == 27))
        id = 40;
    
    if (!id)
        return false;
    
    itm1->InfoID = id;
    
    FUN_0041079c(pchar, 100);

    return true;
}


bool Engine::FUN_00414e64(Character *pchar, ItemInfo *itm1, ItemInfo *itm2)
{
    if (itm2->TypeID != 11)
        return false;
    
    switch(itm2->InfoID)
    {
    case 0:
    {
        int32_t mapid = rand() % 56;

        itm2->TypeID = -1;

        for (int32_t i = 0; i < 56; ++i)
        {
            if (MapsOpened.at(mapid) == 0 && _dungeonMaps.find(mapid) == _dungeonMaps.end())
                MapsOpened.at(mapid) = 1;
                break;

            mapid++;
            if (mapid == 56)
                mapid = 0;
        }
        break;
    }
    
    case 1:
        pchar->Medicine += 100;
        if (pchar->Medicine > 500)
            pchar->Medicine = 500;
        
        FUN_0042f9b8(Locale::GameMessages[Locale::GMSM_MEDICTEACH]);
        
        itm2->TypeID = -1;
        break;
        
    case 2:
        pchar->PlotnickoeDelo += 5;
        
        FUN_0042f9b8(Locale::GameMessages[Locale::GMSM_PLOTNTEACH]);
        
        itm2->TypeID = -1;
        break;
        
    case 3:
        pchar->KuznechnoeDelo += 4;
        FUN_0042f9b8(Locale::GameMessages[Locale::GMSM_BLKSMTHTEACH]);
        itm2->TypeID = -1;
        break;
        
    case 4:
        pchar->Trading += 10;
        if (pchar->Trading > 100)
            pchar->Trading = 100;
        
        FUN_0042f9b8(Locale::GameMessages[Locale::GMSM_TRADETEACH]);
        itm2->TypeID = -1;
        break;
        
    case 5:
        pchar->Identification += 100;
        if (pchar->Identification > 100)
            pchar->Identification = 100;
        
        FUN_0042f9b8(Locale::GameMessages[Locale::GMSM_IDENTIFYTEACH]);
        itm2->TypeID = -1;
        break;
        
    case 6:
        for(MapChar &mchr : _state.MapChar_ARRAY)
        {
            if (mchr.MapID == _currentMapID)
            {
                for (int32_t i = 0; i < mchr.GroupSize; ++i)
                {
                    Character &chr = _state.Characters.at(mchr.CharacterIndex + i);
                    if (chr.ClassID & CLASS_BIT80)
                        FUN_00414b3c(&chr);
                }
            }
        }
        itm2->TypeID = -1;
        break;
        
    case 7:
        if (!itm1)
            return false;
            
        *itm2 = *itm1;
        break;
        
    case 8:
        pchar->BaseSila += 3;
        itm2->TypeID = -1;
        break;
        
    case 9:
        IdentifyCharacterItems(pchar);
        
        if (pchar->Index + 1 == DisplayInvOfCharID)
        {
            //if (_playScreenID == PLSCREEN_3)
            //    PlayChangeScreen(PLSCREEN_3);
            
            FUN_0042f50c(pchar, 0);
        }
        
        itm2->TypeID = -1;
        break;
        
    case 10:
        FUN_0041079c(pchar, 200);
        itm2->TypeID = -1;
        break;
        
    case 12:
    {
        int32_t tmp = (pchar->BaseLovkost) / 3;
        if (tmp > 5)
            tmp = 5;
        
        if (pchar->field_0xce == 0)
        {
            pchar->field76_0xbc = pchar->BaseLovkost;
            pchar->field78_0xbe = pchar->BaseHarizm;
            pchar->field80_0xc0 = pchar->BaseSila;
            pchar->field82_0xc2 = pchar->BaseVinoslivost;
        }
        pchar->field_0xce = tmp * 30;
        pchar->field90_0xca = 0;
        pchar->BaseLovkost -= tmp * 3;
        pchar->BaseHarizm += tmp;
        pchar->BaseSila += tmp;
        pchar->BaseVinoslivost += tmp;
        itm2->TypeID = -1;
        break;
    }
        
    case 13:
    {
        for (MapChar &mchr : _state.MapChar_ARRAY)
        {
            if (pchar->MapCharID != mchr.Index && mchr.MapID == _currentMapID)
            {
                for (int32_t i = 0; i < mchr.GroupSize; ++i)  
                {
                    Character &chr = _state.Characters.at(mchr.CharacterIndex + i);
                    _mainCharacter->Gold += chr.Gold;
                    chr.Gold = 0;
                    
                    for (int32_t j = chr.Inventory.size() - 1; j > -1; --j)
                    {
                        int32_t itmid = chr.Inventory.at(j);
                        if (itmid)
                        {
                            if (!FUN_00424918(pchar, itmid)) 
                                break;
                            
                            chr.Inventory.at(j) = 0;
                        }
                    }
                }
            }
        }
        
        itm2->TypeID = -1;
        break;
    }
        
    case 14:
        GoToMapPos(28, 184, 18, 1);
        break;
    case 15:
        GoToMapPos(5, 60, 68, 2);
        break;
    case 16:
        GoToMapPos(10,147,66,1);
        break;
    case 17:
        GoToMapPos(20,49,41,2);
        break;
    case 18:
        GoToMapPos(3,141,54,1);
        break;
    case 19:
        GoToMapPos(40,42,31,1);
        break;
        
    case 20:
        DisplayInvOfCharID = 0;
        PlayChangeScreen(PLSCREEN_MAP);
        PlaySound(10, 0, 0, 0);
        break;
        
    case 21:
        GoToMapPos(43,18,24,2);
        break;
        
    case 22:
        if (!itm1)
            return false;
        
        if (itm1->TypeID != 11 || itm1->InfoID != 23)
            return false;
        
        FUN_0041079c(pchar, 30);
        
        itm1->TypeID = 2;
        itm1->Weight = ArmorWeaponInfo[27].unk1;
        itm1->Concentration = itm1->Weight;
        itm1->BonusID = -1;
        itm1->SpecialID = 14;
        itm1->InfoID = 27;
        itm1->Poison = 0;
        itm2->TypeID = -1;
        break;
        
    case 23:
        if (!itm1)
            return false;
        
        if (itm1->TypeID != 11 || itm1->InfoID != 22)
            return false;
        
        FUN_0041079c(pchar, 30);
        itm1->TypeID = 2;
        itm1->Weight = ArmorWeaponInfo[27].unk1;
        itm1->Concentration = itm1->Weight;
        itm1->BonusID = -1;
        itm1->SpecialID = 14;
        itm1->InfoID = 27;
        itm1->Poison = 0;
        itm2->TypeID = -1;
        break;
        
    case 24:
        _playScreenID = PLSCREEN_0;
        DAT_00a3e790 = 7;
        PlaySound(10, 0, 0, 0);
        break;
        
    case 25:
    case 26:
    case 27:
    case 41:
    case 42:
    case 43:
        if (!itm1)
            return false;
        
        if (!FUN_00410010(pchar,itm1,itm2))
            return false;
        
        itm2->TypeID = -1;
        break;
        
    case 28:
        QuestsState[0].State |= 0x80;
        FUN_0042f9b8(Locale::GameMessages[Locale::GMSM_READBIRCHBARK]);
        itm2->TypeID = -1;
        break;
        
    case 30:
        for (MapChar &mchr: _state.MapChar_ARRAY)
        {
            if ( pchar->MapCharID != mchr.Index && 
                 mchr.MapID == _currentMapID &&
                (mchr.unk5 & 0x80) == 0 &&
                 mchr.unk5 != 0x53 )
            {
                for (int32_t i = 0; i < mchr.GroupSize; ++i)
                {
                    Character &chr = _state.Characters.at(mchr.CharacterIndex + i);
                    if ((chr.ClassID & CLASS_BIT80) == 0 && chr.State != CHSTATE_9 && chr.State != CHSTATE_3)
                    {
                        chr.field_0x12 = ESLT_0;
                        chr.field2_0x2 = 0;
                        chr.HP = 0;
                        
                        if ((chr.field_0x3 & 4) == 0)
                            FUN_004143dc(&chr, CHSTATE_9);
                        else
                            FUN_004143dc(&chr, CHSTATE_3);
                        
                        FUN_00418510(&chr, 0, _mainCharacter->Level);
                    }
                }
            }
        }
        itm2->TypeID = -1;
        break;
        
    case 31:
        DWORD_00a3e758 = true;
        break;
        
    case 33:
        for (MapChar &mchr: _state.MapChar_ARRAY)
        {
            if ( pchar->MapCharID != mchr.Index && 
                 mchr.MapID == _currentMapID )
            {
                for (int32_t i = 0; i < mchr.GroupSize; ++i)
                {
                    Character &chr = _state.Characters.at(mchr.CharacterIndex + i);
                    if ((chr.ClassID & CLASS_BIT80) == 0 && chr.State != CHSTATE_9 && chr.State != CHSTATE_3)
                        chr.field98_0xd2 = 0x13;
                }
            }
        }
        itm2->TypeID = -1;
        break;
    
        
    case 35:
    case 37:
    {
        MapChar *mchr = FUN_0041fd74(_mainMapChar);
        
        if (mchr && (mchr->unk5 & 0x80) == 0)
        {
            for (int32_t i = 0; i < mchr->GroupSize; ++i)
            {
                Character &chr = _state.Characters.at(mchr->CharacterIndex + i);
                if ((chr.ClassID & CLASS_BIT80) == 0 && chr.State != CHSTATE_9 && chr.State != CHSTATE_3)
                {
                    if ( chr.ClassID != 0x44 && chr.ClassID != 0x53 &&
                         chr.ClassID != 0x57 && chr.ClassID != 0x54 &&
                        (chr.ClassID & CLASS_BIT40) )
                    {
                        Common::Point pt;
                        if (FUN_004101e4(&chr, &pt))
                            FUN_004142d4(&chr, chr.Tile + pt, 8);
                    }
                }
            }
        }
        break;
    }
    
    case 36:
        pchar->BaseHarizm += 10;
        itm2->TypeID = -1;
        break;
        
    case 38:
        pchar->field99_0xd3 = 70;
        itm2->TypeID = -1;
        break;
        
    case 39:
        pchar->BaseHarizm += 5;
        itm2->TypeID = -1;
        break;
        
    case 44:
        if (_mainMapChar->MapID == 50 && pchar->Tile == Common::Point(11, 34))
            GoToMapPos(46, 68, 13, 2);
        break;
        
    case 48:
        for (int32_t i = 0; i < _state.GS1ARRAYCount; ++i)
        {
            GS1 &gs = _state.GS1ARRAY.at(i);
            if (gs.unk2 < 0 && gs.MapID == _currentMapID)
                gs.unk2 = -gs.unk2;
        }
        for (int32_t i = 0; i < mapGS1Count; ++i)
        {
            GS1 &gs = mapGS1.at(i);
            if (gs.unk2 < 0)
                gs.unk2 = -gs.unk2;
        }
        itm2->TypeID = -1;
        break;
        
    case 52:
        if (_mainMapChar->MapID == 48 && pchar->Tile == Common::Point(9, 23))
            GoToMapPos(49, 106, 17, 2);
        break;
        
    case 53:
        if (_mainMapChar->MapID == 49 && pchar->Tile == Common::Point(17, 15))
            GoToMapPos(50, 134, 12, 2);
        break;
        
    default:
        return false;
    }
    
    if (!itm1)
    {
        if (itm2->TypeID < 0)
            FUN_00421d24(pchar, itm2->Index);
    }    
    else if (itm2->TypeID < 0)
    {
        FUN_00421d24(pchar, itm2->Index);
    }
    else
    {
        FUN_00424918(pchar, itm2->Index);
    }
    
    PlaySound(10, 0, 0, 0);
    
    if (pchar->MapCharID == _mainCharacter->MapCharID)
    {
        //FUN_0042d574(pchar);
        //if (_playScreenID == PLSCREEN_3)
        //    PlayChangeScreen(PLSCREEN_3);
        
        if (DisplayInvOfCharID)
            FUN_0042f50c(CharInfoCharacter, 0);
    }
    return true;
}

bool Engine::FUN_0042054c(Character *pchar)
{
    ItemInfo &itm2 = _state.Items.at(InfItemID);
    
    if (DAT_00a3e790 == 8)
    {
        if (FUN_0041db64(pchar, nullptr, &itm2))
        {
            if (itm2.TypeID != -1)
                FUN_00429194(1);
            
            DAT_00a3e790 = 4;
            return true;
        }
    }
    else if (DAT_00a3e790 == 9 && itm2.TypeID == 11 && FUN_00414e64(pchar, nullptr, &itm2))
    {
        if (itm2.TypeID != -1)
            FUN_00429194(1);
        
        DAT_00a3e790 = 4;
        return true;
    }
    
    FUN_0042f9b8(Locale::GameMessages[Locale::GMSM_UNUSABLE]);
    FUN_00429194(1);
    return false;
}


bool Engine::FUN_004246f8(Character *pchar)
{
    if ((pchar->CurrentVinoslivost * 1000) / 3 + 10000 < GetItemWeight(&_state.Items.at(InfItemID)) + GetCurrentWeight(pchar))
    {
        FUN_0042f9b8(Locale::GameMessages[Locale::GMSG_INV_OVERLOAD]);
        FUN_00429194(1);
        return false;
    }
    
    if (!FUN_00424918(pchar, InfItemID))
    {
        FUN_0042f9b8(Locale::GameMessages[Locale::GMSG_INV_FULL]);
        FUN_00429194(1);
        return false;
    }
        
    DAT_00a3e790 = 4;
//    if (_playScreenID == PLSCREEN_3)
//    {
//        PlayChangeScreen(PLSCREEN_3);
//    }
    if (DisplayInvOfCharID != 0)
    {
        FUN_0042f50c(&_state.Characters.at(DisplayInvOfCharID - 1), 0);
    }

    return true;
}




}