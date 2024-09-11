#include "system.h"
#include "fsmgr.h"
#include "game.h"

namespace Game {


void Engine::TSq1::Load(FSMgr::File *pfile)
{
    TxtId = (*pfile)->readS32L();
    QwNext = (*pfile)->readU16L();
    q4id = (*pfile)->readS16L();
    SoundId = (*pfile)->readS16L();
    unk1 = (*pfile)->readU16L();
    unk2 = (*pfile)->readS16L();
    unk3 = (*pfile)->readS16L();   
    
    if (QwNext == 0xFFFF)
        QwNext = -1;
    else if (QwNext == 0xFFFE)
        QwNext = -2;
    
    if (unk1 == 0xFFFF)
        unk1 = -1;
    else if (unk1 == 0xFFFE)
        unk1 = -2;
    
}

void Engine::TQuestState::Load(FSMgr::File *pfile)
{
    State = (*pfile)->readU16L();
    QuestID = (*pfile)->readU16L(); 
}

int32_t Engine::FUN_0043259c(int32_t funcid)
{
    static const std::array<TQuestFunc, 93> QuestFuncs = {{
        &Game::Engine::FUN_00432f70,
        &Game::Engine::FUN_00432fd0,
        &Game::Engine::FUN_004358b4,
        &Game::Engine::FUN_0043512c,
        &Game::Engine::FUN_0043499c,
        &Game::Engine::FUN_004340d4,
        &Game::Engine::FUN_004347d0,
        &Game::Engine::FUN_00434298,
        &Game::Engine::FUN_00435d6c,
        &Game::Engine::FUN_00434c44,
        &Game::Engine::FUN_00434a28,
        &Game::Engine::FUN_00434a6c,
        &Game::Engine::FUN_00434a94,
        &Game::Engine::FUN_00434abc,
        &Game::Engine::FUN_00434ae4,
        &Game::Engine::FUN_00434b0c,
        &Game::Engine::FUN_00434b34,
        &Game::Engine::FUN_00432820,
        &Game::Engine::FUN_00435740,
        &Game::Engine::FUN_004358e0,
        &Game::Engine::FUN_00435908,
        &Game::Engine::FUN_00435930,
        &Game::Engine::FUN_00435958,
        &Game::Engine::FUN_00435980,
        &Game::Engine::FUN_004359a8,
        &Game::Engine::FUN_004357e4,
        &Game::Engine::FUN_00435814,
        &Game::Engine::FUN_00435844,
        &Game::Engine::FUN_00435460,
        &Game::Engine::FUN_00433f18,
        &Game::Engine::FUN_00434954,
        &Game::Engine::FUN_004342cc,
        &Game::Engine::FUN_00434300,
        &Game::Engine::FUN_00434334,
        &Game::Engine::FUN_00434368,
        &Game::Engine::FUN_0043439c,
        &Game::Engine::FUN_004343d0,
        &Game::Engine::FUN_00434888,
        &Game::Engine::FUN_004348cc,
        &Game::Engine::FUN_00434628,
        &Game::Engine::FUN_0043408c,
        &Game::Engine::FUN_00434108,
        &Game::Engine::FUN_004348fc,
        &Game::Engine::FUN_004349d0,
        &Game::Engine::FUN_004346ac,
        &Game::Engine::FUN_00434788,
        &Game::Engine::FUN_00433e84,
        &Game::Engine::FUN_00434250,
        &Game::Engine::FUN_00434730,
        &Game::Engine::FUN_00433f54,
        &Game::Engine::FUN_00434438,
        &Game::Engine::FUN_00433edc,
        &Game::Engine::FUN_00433fac,
        &Game::Engine::FUN_0043400c,
        &Game::Engine::FUN_00434570,
        &Game::Engine::FUN_00434804,
        &Game::Engine::FUN_004353f4,
        &Game::Engine::FUN_00435100,
        &Game::Engine::FUN_00433af8,
        &Game::Engine::FUN_004352ec,
        &Game::Engine::FUN_00434d1c,
        &Game::Engine::FUN_00434da4,
        &Game::Engine::FUN_004339a8,
        &Game::Engine::FUN_004336e8,
        &Game::Engine::FUN_00433848,
        &Game::Engine::FUN_004338f8,
        &Game::Engine::FUN_004334b8,
        &Game::Engine::FUN_00433798,
        &Game::Engine::FUN_00433568,
        &Game::Engine::FUN_00433618,
        &Game::Engine::FUN_00435874,
        &Game::Engine::FUN_004351d8,
        &Game::Engine::FUN_0043516c,
        &Game::Engine::FUN_004351b4,
        &Game::Engine::FUN_00435190,
        &Game::Engine::FUN_0043536c,
        &Game::Engine::FUN_0043542c,
        &Game::Engine::FUN_00435334,
        &Game::Engine::FUN_004354a8,
        &Game::Engine::FUN_004354e0,
        &Game::Engine::FUN_00435568,
        &Game::Engine::FUN_00433368,
        &Game::Engine::FUN_00433410,
        &Game::Engine::FUN_00433464,
        &Game::Engine::FUN_0043321c,
        &Game::Engine::FUN_004333bc,
        &Game::Engine::FUN_00433270,
        &Game::Engine::FUN_004332c4,
        &Game::Engine::FUN_00435a84,
        &Game::Engine::FUN_00434e98,
        &Game::Engine::FUN_00433acc,
        &Game::Engine::FUN_00435514,
        &Game::Engine::FUN_004353bc }};
    
    if (funcid < 56)
        return FUN_00434160(funcid + 1);
    else if (funcid < 112)
        return FUN_004351fc(funcid - 55);
    else if (funcid < 166)
        return FUN_00434b60(funcid - 111);
    else if (funcid < 220)
        return FUN_004359d4(funcid - 165);
    else if (funcid < 274)
        return FUN_00434c90(funcid - 219);
    else if (funcid < 367)
        return (this->*(QuestFuncs[funcid - 274]))();
    
    return 0;
}

void Engine::FUN_00435dbc(int32_t id)
{
    TSq5 *q5 = &quest_5.at(id);
    
    for (; q5->id != -1; ++q5)
    {
        if (q5->Flags & 2)
            FUN_0043259c(q5->id);
        else
        {
            TQuestState &qst = QuestsState.at(q5->id);
            
            if ((q5->Flags & 8) == 0)
            {
                if ((qst.State & 0x80) != 0)
                    qst.State &= ~0x80;
            }
            else if ((qst.State & 0x80) == 0)
                qst.State |= 0x80;
        }
    }
}

void Engine::FUN_0041f18c(TSq1 *qw, int32_t id) 
{
    if (qw->unk1 != -1)
        FUN_00435dbc(qw->unk1);
        
    if (qw->QwNext == -1)
        qwsel1 = qOut2[id].qsel;
    else if (qw->QwNext == -2)
    {
        if (_playScreenID == PLSCREEN_1)
        {
            _playScreenID = PLSCREEN_0;
            FUN_004292e4();
        }
        return;
    }
    else
        qwsel1 = (int)qw->QwNext;
    
    if (!FUN_00435e78())
    {
        if (_playScreenID == PLSCREEN_1)
        {
            _playScreenID = PLSCREEN_0;
            FUN_004292e4();
        }
    }
    else
        PlayChangeScreen(PLSCREEN_1);
}    

int Engine::FUN_004364f0(Character *pchar)
{
    CharInfoCharacter = _mainCharacter;
    
    qwsel1 = quest_2.at(pchar->field96_0xd0);
    PInteractChar = pchar;
    qwsel2 = qwsel1;
    
    int32_t sndId = 12;
    
    if ((pchar->ClassID & CLASS_BIT40) == 0)
    {
        sndId = rand() % 4 + 12;
        
        static int32_t DWORD_00a3e7c0 = 0;
        
        if (sndId == DWORD_00a3e7c0)
        {
            sndId++;
            
            if (sndId == 16)
                sndId = 12;
        }
        
        DWORD_00a3e7c0 = sndId;
    }
    else if (pchar->ClassID == 0x51)
        sndId = 17;
    else if (pchar->ClassID == 0x52)
        sndId = 18;
    else if (pchar->ClassID == 0x53)
        sndId = 19;
    else
        sndId = 16;
    
    int res = FUN_00435e78();
    
    if (res == 1)
        PlaySound(sndId, 0, 0, 0);
    
    return res;
}

int32_t Engine::FUN_00434160(int32_t id)
{
    for(MapChar &mchr : _state.MapChar_ARRAY)
    {
        if ( mchr.MapID == id && (mchr.unk5 & 0x40) )
        {
            if (mchr.GroupSize < mchr.unk1)
                return 1;
            
            for (int32_t i = 0; i < mchr.GroupSize; ++i)
            {
                Character &chr = _state.Characters.at(mchr.CharacterIndex + i);
                if (chr.ClassID & CLASS_BIT80)
                    return 1;
            }
            
            return 0;
        }
    }
    return 0;
}


int32_t Engine::FUN_004351fc(int32_t id)
{
    for(MapChar &mchr : _state.MapChar_ARRAY)
    {
        if ( mchr.MapID == id && (mchr.unk5 & 0x40) )
        {
            for (int32_t i = 0; i < mchr.GroupSize; ++i)
            {
                Character &chr = _state.Characters.at(mchr.CharacterIndex + i);
                if ((chr.ClassID & CLASS_BIT80) == 0 && chr.State != CHSTATE_9 && chr.State != CHSTATE_3)
                    return 0;
            }
            return 1;
        }
    }
    return 1;
}


int32_t Engine::FUN_00434b60(int32_t id)
{
    for (int32_t i = 0; i < 32; ++i)
    {
        int32_t itmId = _mainCharacter->Inventory[i];
        if (itmId)
        {
            ItemInfo &inf = _state.Items.at(itmId);
            if (inf.TypeID == 11 && inf.InfoID == id - 1)
            {
                inf.TypeID = -1;
                
                for (; i < 31; ++i)
                    _mainCharacter->Inventory[i] = _mainCharacter->Inventory[i + 1];
                
                _mainCharacter->Inventory.back() = 0;
                return 1;
            }
        }
    }
    return 0;
}

int32_t Engine::FUN_004359d4(int32_t id)
{
    ItemInfo *pinf = AllocItem();
    if (!pinf)
        return 0;

    pinf->TypeID = 11;
    pinf->InfoID = id - 1;

    if (FUN_00424918(_mainCharacter, pinf->Index) == 0)
        FUN_004138c8(_mainCharacter->Tile, pinf->Index);
    
    return 1;
}

int32_t Engine::FUN_00434c90(int32_t id)
{
    for(int32_t i = 0; i < _mainCharacter->Inventory.size(); ++i)
    {
        if (_mainCharacter->Inventory[i])
        {
            ItemInfo &inf = _state.Items.at(_mainCharacter->Inventory[i]);
            if (inf.TypeID == 11 && inf.InfoID == id - 1)
                return i + 1;
        }
    }
    return 0;
}



int Engine::FUN_00435e78()
{
    DAT_00a3e704 = 0;
    qOut1.clear();
    qOut2.clear();
    
    while( true )
    {
        TSq1 &mq = quest_1.at(qwsel1);
        
        if (mq.unk1 != -2)
        {
            int res = 1;
            for (int32_t i = 0; i < mq.unk3; ++i)
            {
                TSq1 &qu = quest_1.at(qwsel1 + 1 + i);
                if ((qu.q4id == -1) || FUN_00436248(qu.q4id))
                {
                    if (qu.unk1 != -1)
                        FUN_00435dbc(qu.unk1);
                    
                    if (qu.TxtId != -1)
                    {
                        qOut1.emplace_back();
                        
                        DlgTxt &tx = qOut1.back();
                        tx.Q1ID = qu.Index;
                        tx.q1txtid = qu.TxtId;
                        if (qu.SoundId != -1)
                        {
                            res = -1;
                            SoundPlaySpeech(qu.SoundId);
                        }
                    }
                }
            }
            
            for (int32_t i = 0; i < mq.unk2; ++i)
            {
                TSq1 &qu = quest_1.at(qwsel1 + 1 + mq.unk3 + i);
                
                if (qu.q4id == -1)
                {
                    if (qu.TxtId != -1)
                    {
                        qOut2.emplace_back();
                        
                        DlgTxt &tx = qOut2.back();
                        tx.Q1ID = qu.Index;
                        tx.q1txtid = qu.TxtId;
                        tx.SoundID = qu.SoundId;
                        
                        if (qu.QwNext == -1)
                            tx.qsel = qwsel2;
                    }
                }
                else
                {
                    if (FUN_00436248(qu.q4id) != 0)
                    {
                        qOut2.emplace_back();
                        
                        DlgTxt &tx = qOut2.back();
                        tx.Q1ID = qu.Index;
                        tx.q1txtid = qu.TxtId;
                        
                        if (qu.QwNext == -1)
                            tx.qsel = qwsel2;
                    }
                }
            }
            
            if (qOut2.empty())
                return 0;

            qwsel2 = qwsel1;
            return res;
        }
        
        bool found = false;
        for(int32_t i = 0; i < mq.unk3; ++i)
        {
            TSq1 &qu = quest_1.at(mq.Index + i);
            if (qu.q4id == -1 || FUN_00436248(qu.q4id) != 0)
            {
                if (qu.QwNext == -1 || qu.QwNext == -2)
                    return 0;
                
                found = true;
                qwsel1 = qu.QwNext;
                break;
            }
        }
        
        if (!found)
            return 0;
    }
    return 0;
}

int32_t Engine::FUN_00432f70()
{
    _state.MapChar_ARRAY[PInteractChar->MapCharID].unk4 = 1;
    
    if (CurrentVillage && CurrentVillage->ChiefCharId == PInteractChar->MapCharID)
        CurrentVillage->Flags = CurrentVillage->Flags | 0x15;
    
    return 1;
}


int32_t Engine::FUN_00432fd0()
{
    DAT_00a3e7c8 = _mainCharacter->Gold;
    for (int32_t i = 0; i < 32; ++i)
    {
        int32_t itmId = _mainCharacter->Inventory[i];
        if (itmId)
        {
            ItemInfo &inf = _state.Items.at(itmId);
            if (inf.TypeID == 11 && inf.InfoID == 32)
                DAT_00a3e7c8 += 10;
        }
    }
    
    DAT_00a3e830 = 0;
    DAT_00a3e82c = 0;
    
    for (int32_t i = 0; i < 4; ++i)
        int16_t_ARRAY_0083dc4c[i].fill(0);
    
    int16_t_ARRAY_0083dc4c[0] = CharInfoCharacter->Inventory;
    int16_t_ARRAY_0083dd4c.fill(0);
    
    int32_t job = GetVillageCharacterJob(PInteractChar);
    if (job == VJOB_NO || job == VJOB_CHIEF || job == VJOB_VOEVODA)
    {
        DAT_00a3e76c = 6;
        DAT_00a3e7cc = PInteractChar->Gold;
        int16_t_ARRAY_0083dc4c[2] = PInteractChar->Inventory;
    }
    else
    {
        DAT_00a3e76c = job;
        
        if (FUN_00435460() == 0)
            DAT_00a3e76c = -job;
        
        DAT_00a3e7cc = 99999;
        
        if (job == VJOB_MEDIC)
        {
            int32_t outp = 0;
            for(int32_t i = 0; i < 24; ++i)
            {
                int32_t itmId = CurrentVillage->MedicItems[i];
                if (itmId)
                {
                    ItemInfo &inf = _state.Items.at(itmId);
                    if ((CurrentVillage->Flags & 4) == 0 || inf.TypeID > 2)
                    {
                        int16_t_ARRAY_0083dc4c[2].at(outp) = itmId;
                        outp++;
                        if (outp >= 32)
                            break;
                    }
                }
            }
        }
        else if (job == VJOB_TRADER)
        {
            int32_t outp = 0;
            for(int32_t i = 0; i < 32; ++i)
            {
                int32_t itmId = CurrentVillage->TraderItems[i];
                if (itmId)
                {
                    ItemInfo &inf = _state.Items.at(itmId);
                    if ((CurrentVillage->Flags & 4) == 0 || inf.TypeID > 2)
                    {
                        int16_t_ARRAY_0083dc4c[2].at(outp) = itmId;
                        outp++;
                        if (outp >= 32)
                            break;
                    }
                }
            }
        }
        else if (job == VJOB_SMITH)
        {
            int32_t outp = 0;
            for(int32_t i = 0; i < 23; ++i)
            {
                int32_t itmId = CurrentVillage->SmithItems[i];
                if (itmId)
                {
                    ItemInfo &inf = _state.Items.at(itmId);
                    if ((CurrentVillage->Flags & 4) == 0 || inf.TypeID > 2)
                    {
                        int16_t_ARRAY_0083dc4c[2].at(outp) = itmId;
                        outp++;
                        if (outp >= 32)
                            break;
                    }
                }
            }
        }
    }
    
    //DrawTrade(0);
    PlayChangeScreen(PLSCREEN_7);
    return 1;
}


int32_t Engine::FUN_004358b4()
{
    _mainCharacter->field99_0xd3 = 70;
    return 1;
}


int32_t Engine::FUN_0043512c()
{
    return (rand() % 100) < 50;
}


int32_t Engine::FUN_004323b0(int32_t classId)
{    
    for(int32_t i = 1; i < _mainMapChar->GroupSize; ++i)
    {
        Character &chr = _state.Characters.at(_mainMapChar->CharacterIndex + i);
        
        if ((chr.ClassID & CLASS_BIT80) == 0)
        {
            if ((chr.ClassID & 0x38) == classId << 3)
                return i;
            
            switch(classId)
            {
            case 1:
                if (chr.Medicine > 100)
                    return i;
                break;
                
            case 2:
                if (chr.Trading > 30)
                    return i;
                break;
                
            case 3:
                if (chr.KuznechnoeDelo > 4)
                    return i;
                break;
                
            case 4:
                if ((chr.CurrentSila > 10) && (chr.Fehtovanie > 10))
                    return i;
                break;
            }
        }
    }
    
    return 0;
}

int32_t Engine::FUN_0043499c()
{
    return FUN_004323b0(4) != 0;
}


int32_t Engine::FUN_004340d4()
{
    return FUN_004323b0(1) != 0;
}


int32_t Engine::FUN_004347d0()
{
    return FUN_004323b0(3) != 0;
}


int32_t Engine::FUN_00434298()
{
    return FUN_004323b0(2) != 0;
}


int32_t Engine::FUN_00435d6c()
{
    if ((CurrentVillage == nullptr) || (_mainCharacter->Gold < 120))
        return 0;
    
    CurrentVillage->Flags &= 0xdf;
    _mainCharacter->Gold -= 120;
    return 1;
}


int32_t Engine::FUN_00434c44()
{
    if (!CurrentVillage)
        return 0;

    _mainCharacter->Gold += CurrentVillage->GoldTribute;
    CurrentVillage->GoldTribute = 0;
    return 1;
}


int32_t Engine::FUN_00434a28()
{
    if (CurrentVillage)
    {
        CurrentVillage->Flags |= 0x20;
        _mainCharacter->Gold += 100;
    }
    return 0;
}


int32_t Engine::FUN_00434a6c()
{
    _mainCharacter->Gold -= 5;
    return 1;
}


int32_t Engine::FUN_00434a94()
{
    _mainCharacter->Gold -= 10;
    return 1;
}

int32_t Engine::FUN_00434abc()
{
    _mainCharacter->Gold -= 20;
    return 1;
}


int32_t Engine::FUN_00434ae4()
{
    _mainCharacter->Gold -= 50;
    return 1;
}


int32_t Engine::FUN_00434b0c()
{
    _mainCharacter->Gold -= 100;
    return 1;
}

int32_t Engine::FUN_00434b34()
{
    _mainCharacter->Gold -= 250;
    return 1;
}


int32_t Engine::FUN_00432820()
{
    int32_t id = (PInteractChar->ClassID & 0x38) >> 3;
    if (id != 0)
    {
        int32_t timerCount = (_counter - CurrentVillage->TimeSt[id - 1]) / 1440;
        if (id == 1)
        {
            int32_t charId = CurrentVillage->Jobs[1].CharID;
            if (charId == 0)
                PInteractChar->Medicine += timerCount * 0.03;
            else
                PInteractChar->Medicine += timerCount * (_state.Characters.at(charId).Medicine - PInteractChar->Medicine) * 0.03;
            
            if (PInteractChar->Medicine > 500)
                PInteractChar->Medicine = 500;
        }
        else if (id == 2) 
        {
            PInteractChar->Trading += timerCount;
            
            int32_t charId = CurrentVillage->Jobs[2].CharID;
            if (charId && (_state.Characters[charId].Trading < PInteractChar->Trading))
                PInteractChar->Trading = _state.Characters[charId].Trading;
            
            if (PInteractChar->Trading > 100)
                PInteractChar->Trading = 100;
        }
        else if (id == 3) 
        {
            int32_t charId = CurrentVillage->Jobs[3].CharID;
            if (charId == 0)
                PInteractChar->KuznechnoeDelo = timerCount * 0.03;
            else
                PInteractChar->KuznechnoeDelo += timerCount * (_state.Characters.at(charId).KuznechnoeDelo - PInteractChar->KuznechnoeDelo) * 0.03;
        }
    }

    Character &lastChr = _state.Characters.at(_mainMapChar->CharacterIndex + _mainMapChar->GroupSize);
    lastChr.CopyDataFrom(*PInteractChar);
    
    _mainMapChar->GroupSize++;

    lastChr.MapCharID = _mainMapChar->Index;
    lastChr.field2_0x2 = 0x60;
    lastChr.field17_0x13[1] = 0xff;
    
    _currentMap->FootMap.At(lastChr.Tile).ID = lastChr.Index + 1;
    
    if (PInteractChar->field113_0xf8 == 0)
    {
        std::array<bool, 9> local_34;
        local_34.fill(false);
        
        int32_t clid = lastChr.ClassID & CLASS_MASK;
        if (clid == CLASS_VOIN)
            clid = CLASS_VOJD;
        
        int32_t iVar10 = lastChr.CharacterBase * 4 + clid - 1;
        
        for (int32_t i = 1; i < _mainMapChar->GroupSize - 1; ++i)
        {
            Character &chr = _state.Characters.at(_mainMapChar->CharacterIndex + i);
            if ((chr.ClassID & CLASS_MASK) == (PInteractChar->ClassID & CLASS_MASK) && 
                chr.CharacterBase == PInteractChar->CharacterBase)
                local_34.at( (chr.field113_0xf8 - 1 - iVar10 * 45) / 5 ) = true;
            
        }
        
        int32_t local_18 = rand() % 9;
        for(int32_t i = 0; i < 9; ++i)
        {
            if (local_34[local_18] == false)
                break;
            
            local_18++;
            if (local_18 > 8)
                local_18 = 0;
        }

        lastChr.field113_0xf8 = ((clid - 1) * 9 + lastChr.CharacterBase * 36 + local_18) * 5 + 1;
    }
    
    //PlayChangeScreen(PLSCREEN_0);
    
    MapChar &local_24 = _state.MapChar_ARRAY.at(PInteractChar->MapCharID);
    
    local_24.GroupSize--;
    for(int32_t i = PInteractChar->Index - local_24.CharacterIndex; i < local_24.GroupSize; ++i)
        _state.Characters.at(local_24.CharacterIndex + i).CopyDataFrom( _state.Characters.at(local_24.CharacterIndex + i + 1) );

    for (int32_t i = 0; i < local_24.GroupSize; ++i)
    {
        Character &chr = _state.Characters.at(local_24.CharacterIndex + i);
        
        if ((chr.ClassID & CLASS_BIT80) == 0 && chr.State != CHSTATE_9 && chr.State != CHSTATE_3)
            _currentMap->FootMap.At(chr.Tile).ID = chr.Index + 1;
    }
    
    if (CurrentVillage)
    {
        for (int32_t i = 0; i < 5; ++i)
        {
            if (CurrentVillage->Jobs[i].CharID == PInteractChar->Index)
                CurrentVillage->Jobs[i].CharID = 0;
            else if (PInteractChar->Index < CurrentVillage->Jobs[i].CharID)
                CurrentVillage->Jobs[i].CharID--;
        }
    }
    
    return 1;
}


int32_t Engine::FUN_00435740()
{
    ItemInfo *itm = AllocItem();
    if (!itm)
        return 0;
    
    for (int32_t i = 0; i < INVSIZE; ++i)
    {
        if (_mainCharacter->Inventory[i] == 0)
        {
            itm->TypeID = 8;
            itm->InfoID = 18;
            
            _mainCharacter->Inventory[i] = itm->Index;
            return 1;
        }
    }
    
    return 0;
}


int32_t Engine::FUN_004358e0()
{
    _mainCharacter->Gold += 5;
    return 1;
}


int32_t Engine::FUN_00435908()
{
    _mainCharacter->Gold += 10;
    return 1;
}


int32_t Engine::FUN_00435930()
{
    _mainCharacter->Gold += 20;
    return 1;
}


int32_t Engine::FUN_00435958()
{
    _mainCharacter->Gold += 50;
    return 1;
}


int32_t Engine::FUN_00435980()
{
    _mainCharacter->Gold += 100;
    return 1;
}


int32_t Engine::FUN_004359a8()
{
    _mainCharacter->Gold += 250;
    return 1;
}


int32_t Engine::FUN_004357e4()
{
    FUN_0041079c(_mainCharacter, 10);
    return 1;
}


int32_t Engine::FUN_00435814()
{
    FUN_0041079c(_mainCharacter, 20);
    return 1;
}


int32_t Engine::FUN_00435844()
{
    FUN_0041079c(_mainCharacter, 50);
    return 1;
}


int32_t Engine::FUN_00435460()
{
    if (!CurrentVillage)
        return 0;
    
    if ((CurrentVillage->Flags & 0x10) == 0)
        return 0;
    
    return 1;
}


int32_t Engine::FUN_00433f18()
{
    return _mainCharacter->field99_0xd3 > -1;
}


int32_t Engine::FUN_00434954()
{
    if (!CurrentVillage)
        return 0;
    
    if (CurrentVillage->Jobs[4].CharID == 0)
        return 0;
    
    return 1;
}


int32_t Engine::FUN_004342cc()
{
    return _mainCharacter->Gold > 29;
}


int32_t Engine::FUN_00434300()
{
    return _mainCharacter->Gold > 39;
}


int32_t Engine::FUN_00434334()
{
    return _mainCharacter->Gold > 49;
}


int32_t Engine::FUN_00434368()
{
    return _mainCharacter->Gold > 99;
}


int32_t Engine::FUN_0043439c()
{
    return _mainCharacter->Gold > 499;
}


int32_t Engine::FUN_004343d0()
{
    int gold = 500;
    
    if (FUN_004354a8() == 0)
        gold = 300;
    
    if (FUN_00435460() == 0)
        gold /= 2;
    
    return _mainCharacter->Gold >= gold;
}


int32_t Engine::FUN_00434888()
{
    if (!CurrentVillage)
        return 0;
    
    if (CurrentVillage->GoldTribute == 0)
        return 0;
    
    return 1;
}


int32_t Engine::FUN_004348cc()
{
    return CurrentVillage != nullptr;
}


int32_t Engine::FUN_00434628()
{
    for(int32_t i = 0; i < INVSIZE; ++i)
    {
        int32_t itmId = _mainCharacter->Inventory.at(i);
        if (itmId)
        {
            ItemInfo &inf = _state.Items.at(itmId);
            if (inf.TypeID == 9 && inf.InfoID == 1)
                return 1;
        }
    }
    return 0;
}


int32_t Engine::FUN_0043408c()
{
    if (!CurrentVillage)
        return 0;
    
    if (CurrentVillage->Jobs[1].CharID == 0)
        return 0;
    
    return 1;
}


int32_t Engine::FUN_00434108()
{
    if (!CurrentVillage)
        return 0;
    
    if (CurrentVillage->BuildingState[1].State < 1 &&
        CurrentVillage->BuildingState[1].BldLeft < 1)
        return 0;
    
    return 1;
}


int32_t Engine::FUN_004348fc()
{
    if (!CurrentVillage)
        return 0;
    
    if (CurrentVillage->BuildingState[4].State < 1 &&
        CurrentVillage->BuildingState[4].BldLeft < 1)
        return 0;
    
    return 1;
}


int32_t Engine::FUN_004349d0()
{
    if (!CurrentVillage)
        return 0;
    
    if (CurrentVillage->BuildingState[6].State < 1 &&
        CurrentVillage->BuildingState[6].BldLeft < 1)
        return 0;
    
    return 1;
}


int32_t Engine::FUN_004346ac()
{
    for(int32_t i = 0; i < INVSIZE; ++i)
    {
        int32_t itmId = _mainCharacter->Inventory.at(i);
        if (itmId)
        {
            ItemInfo &inf = _state.Items.at(itmId);
            if (inf.TypeID == 9 && inf.InfoID == 2)
                return 1;
        }
    }
    return 0;
}


int32_t Engine::FUN_00434788()
{
    if (!CurrentVillage)
        return 0;
    
    if (CurrentVillage->Jobs[3].CharID == 0)
        return 0;
    
    return 1;
}


int32_t Engine::FUN_00433e84()
{
    if (!CurrentVillage)
        return 0;
    
    if (CurrentVillage->BuildingState[3].State < 1 &&
        CurrentVillage->BuildingState[3].BldLeft < 1)
        return 0;
    
    return 1;
}


int32_t Engine::FUN_00434250()
{
    if (!CurrentVillage)
        return 0;
    
    if (CurrentVillage->Jobs[2].CharID == 0)
        return 0;
    
    return 1;
}


int32_t Engine::FUN_00434730()
{
    if (!CurrentVillage)
        return 0;
    
    if (CurrentVillage->BuildingState[2].State < 1 &&
        CurrentVillage->BuildingState[2].BldLeft < 1)
        return 0;
    
    return 1;
}


int32_t Engine::FUN_00433f54()
{
    if (!CurrentVillage)
        return 0;
    
    if (CurrentVillage->BuildingState[5].State < 1 &&
        CurrentVillage->BuildingState[5].BldLeft < 1)
        return 0;
    
    return 1;
}


int32_t Engine::FUN_00434438()
{
    for (int32_t itmId : _mainCharacter->ArmorWeapons) 
    {
        if (itmId && (_state.Items.at(itmId).Flags & ItemInfo::FLAG_IDENTIFIED))
            return 1;            
    }

    if (_mainCharacter->Arrows && (_state.Items.at(_mainCharacter->Arrows).Flags & ItemInfo::FLAG_IDENTIFIED))
        return 1; 
    
    for (int32_t itmId : _mainCharacter->Inventory) 
    {
        if (itmId && (_state.Items.at(itmId).Flags & ItemInfo::FLAG_IDENTIFIED))
            return 1;            
    }
    
    for (int32_t itmId : _mainCharacter->Accessories) 
    {
        if (itmId && (_state.Items.at(itmId).Flags & ItemInfo::FLAG_IDENTIFIED))
            return 1;            
    }
    
    return 0;
}


int32_t Engine::FUN_00433edc()
{
    return _mainCharacter->field98_0xd2 > -1; 
}


int32_t Engine::FUN_00433fac()
{
    if (!CurrentVillage)
        return 0;
    
    if (GetGroupMaxPlotnicLevel(&_state.MapChar_ARRAY.at(CurrentVillage->ChiefCharId)) < 5)
        return 0;
    
    return 1;
}


int32_t Engine::FUN_0043400c()
{
    if (!CurrentVillage)
        return 0;
        
    if (CurrentVillage->Bld2Num == 0)
        return 0;
    
    if ((CurrentVillage->BuildingState[CurrentVillage->Bld1Num + 7].State == 0) &&
        (CurrentVillage->BuildingState[CurrentVillage->Bld1Num + 7].BldLeft == 0))
        return 0;
    
    return 1;
}


int32_t Engine::FUN_00434570()
{
    if (!CurrentVillage)
        return 0;
    
    if (FUN_004334b8())
        return 1;
    
    if (FUN_00433568())
        return 1;
    
    if (FUN_00433618())
        return 1;
    
    if (FUN_004336e8())
        return 1;
    
    if (FUN_00433798())
        return 1;
    
    if (FUN_00433848())
        return 1;
    
    if (FUN_004338f8())
        return 1;
    
    return 0;
}


int32_t Engine::FUN_00434804()
{
    for(int32_t i = 0; i < INVSIZE; ++i)
    {
        int32_t itmId = _mainCharacter->Inventory.at(i);
        if (itmId)
        {
            ItemInfo &inf = _state.Items.at(itmId);
            if (inf.TypeID == 9 && inf.InfoID == 0)
                return 1;
        }
    }
    return 0;
}


int32_t Engine::FUN_004353f4()
{
    return _mainCharacter->HP < 1600;
}


int32_t Engine::FUN_00435100()
{
    IdentifyCharacterItems(_mainCharacter);
    return 1;
}


int32_t Engine::FUN_00433af8()
{
    if (!CurrentVillage)
        return 0;
    
    MapChar &mchr = _state.MapChar_ARRAY.at(CurrentVillage->ChiefCharId);
    if (mchr.GroupSize == mchr.unk2)
        return 0;

    Character &pCVar11 = _state.Characters.at(mchr.CharacterIndex + mchr.GroupSize);
    
    mchr.GroupSize++;
    
    pCVar11.CopyDataFrom(*PInteractChar);
            
    pCVar11.MapCharID = CurrentVillage->ChiefCharId;
    pCVar11.field2_0x2 = 0x60;
    pCVar11.field17_0x13[1] = 0xff;
    
    _currentMap->FootMap.At(pCVar11.Tile).ID = pCVar11.Index + 1;
    
    _mainMapChar->GroupSize--;
    if (CurrentVillage->Jobs[0].CharID == 0)
    {
        CurrentVillage->Jobs[0].CharID = pCVar11.Index;
        pCVar11.field96_0xd0 = CurrentVillage->Jobs[0].unk;
        pCVar11.field2_0x2 = 0;
        pCVar11.ClassID &= 0xc7;
        FUN_00414ab4(&pCVar11);
    }
    
    for(int32_t i = PInteractChar->Index - _mainMapChar->CharacterIndex; i < _mainMapChar->GroupSize; ++i)
        _state.Characters.at(_mainMapChar->CharacterIndex + i).CopyDataFrom( _state.Characters.at(_mainMapChar->CharacterIndex + i + 1) );
    
    for (int32_t i = 0; i < 10; ++i)
    {
        if (!SelectedCharacters[i])
            break;
        
        if (PInteractChar == SelectedCharacters[i])
        {
            for(int32_t j = i; j < 9; ++j)
                SelectedCharacters[j] = SelectedCharacters[j + 1];
            SelectedCharacters.back() = nullptr;
            break;
        }
    }
    
    for (int32_t i = 0; i < _mainMapChar->GroupSize; ++i)
    {
        Character &chr = _state.Characters.at(_mainCharacter->Index + i);
        if ((chr.ClassID & CLASS_BIT80) == 0 && chr.State != CHSTATE_9 && chr.State != CHSTATE_3)
            _currentMap->FootMap.At(chr.Tile).ID = chr.Index + 1;
    }
    
    //PlayChangeScreen(PLSCREEN_0);
    return 1;
}


int32_t Engine::FUN_004352ec()
{
    if (!CurrentVillage)
        return 0;
    
    if ((CurrentVillage->Flags & 0x20) == 0)
        return 0;
    return 1;
}


int32_t Engine::FUN_00434d1c()
{
    PInteractChar->Medicine += (1600 - _mainCharacter->HP) * 0.015;
    _mainCharacter->HP = 1600;
    //FUN_0042d574(_mainCharacter);
    return 1;
}


int32_t Engine::FUN_00434da4()
{
    for (int32_t i = 0; i < _mainMapChar->GroupSize; ++i)
    {
        Character &chr = _state.Characters.at(_mainCharacter->Index + i);
        if ((chr.ClassID & CLASS_BIT80) == 0 && chr.State != CHSTATE_9 && chr.State != CHSTATE_3)
        {
            PInteractChar->Medicine += (1600 - chr.HP) * 0.015;
            chr.HP = 1600;
            //FUN_0042d574(&chr);
        }
    }
    return 1;
}


int32_t Engine::FUN_004339a8()
{
    if (!CurrentVillage)
        return 0;
    
    if (FUN_00435460() == 0 && _mainCharacter->Gold <= 99)
        return 0;
    
    MapChar &mchr = _state.MapChar_ARRAY.at(CurrentVillage->ChiefCharId);
    for (int32_t i = 0; i < mchr.GroupSize; ++i)
    {
        Character &chr = _state.Characters.at(mchr.CharacterIndex + i);
        if (chr.CharacterBase < 3 && (chr.ClassID & 0x38) == 0)
        {
            if ((chr.ClassID & CLASS_BIT80) == 0 && chr.State != CHSTATE_9 && chr.State != CHSTATE_3)
            {
                if (GetVillageCharacterJob(&chr) == VJOB_NO)
                    return 1;
            }
        }
    }
    
    return 0;
}


int32_t Engine::FUN_004336e8()
{
    if (!CurrentVillage)
        return 0;
    
    if (CurrentVillage->BuildingState[1].BuildInfoID < 0)
        return 0;
    
    if (CurrentVillage->BuildingState[1].State || CurrentVillage->BuildingState[1].BldLeft)
        return 0;
    
    if (GetGroupMaxPlotnicLevel(&_state.MapChar_ARRAY.at(CurrentVillage->ChiefCharId)) >= 
        BuildingInfo.at(CurrentVillage->BuildingState[1].BuildInfoID).NeededPlotnicLvl)
        return 1;
    return 0;
}


int32_t Engine::FUN_00433848()
{
    if (!CurrentVillage)
        return 0;
    
    if (CurrentVillage->BuildingState[4].BuildInfoID < 0)
        return 0;
    
    if (CurrentVillage->BuildingState[4].State || CurrentVillage->BuildingState[4].BldLeft)
        return 0;
    
    if (GetGroupMaxPlotnicLevel(&_state.MapChar_ARRAY.at(CurrentVillage->ChiefCharId)) >= 
        BuildingInfo.at(CurrentVillage->BuildingState[4].BuildInfoID).NeededPlotnicLvl)
        return 1;
    return 0;
}


int32_t Engine::FUN_004338f8()
{
    if (!CurrentVillage)
        return 0;
    
    if (CurrentVillage->BuildingState[6].BuildInfoID < 0)
        return 0;
    
    if (CurrentVillage->BuildingState[6].State || CurrentVillage->BuildingState[6].BldLeft)
        return 0;
    
    if (GetGroupMaxPlotnicLevel(&_state.MapChar_ARRAY.at(CurrentVillage->ChiefCharId)) >= 
        BuildingInfo.at(CurrentVillage->BuildingState[6].BuildInfoID).NeededPlotnicLvl)
        return 1;
    return 0;
}


int32_t Engine::FUN_004334b8()
{
    if (!CurrentVillage)
        return 0;
    
    if (CurrentVillage->BuildingState[3].BuildInfoID < 0)
        return 0;
    
    if (CurrentVillage->BuildingState[3].State || CurrentVillage->BuildingState[3].BldLeft)
        return 0;
    
    if (GetGroupMaxPlotnicLevel(&_state.MapChar_ARRAY.at(CurrentVillage->ChiefCharId)) >= 
        BuildingInfo.at(CurrentVillage->BuildingState[3].BuildInfoID).NeededPlotnicLvl)
        return 1;
    return 0;
}


int32_t Engine::FUN_00433798()
{
    if (!CurrentVillage)
        return 0;
    
    if (CurrentVillage->BuildingState[2].BuildInfoID < 0)
        return 0;
    
    if (CurrentVillage->BuildingState[2].State || CurrentVillage->BuildingState[2].BldLeft)
        return 0;
    
    if (GetGroupMaxPlotnicLevel(&_state.MapChar_ARRAY.at(CurrentVillage->ChiefCharId)) >= 
        BuildingInfo.at(CurrentVillage->BuildingState[2].BuildInfoID).NeededPlotnicLvl)
        return 1;
    return 0;
}


int32_t Engine::FUN_00433568()
{
    if (!CurrentVillage)
        return 0;
    
    if (CurrentVillage->BuildingState[5].BuildInfoID < 0)
        return 0;
    
    if (CurrentVillage->BuildingState[5].State || CurrentVillage->BuildingState[5].BldLeft)
        return 0;
    
    if (GetGroupMaxPlotnicLevel(&_state.MapChar_ARRAY.at(CurrentVillage->ChiefCharId)) >= 
        BuildingInfo.at(CurrentVillage->BuildingState[5].BuildInfoID).NeededPlotnicLvl)
        return 1;
    return 0;
}


int32_t Engine::FUN_00433618()
{
    if (!CurrentVillage)
        return 0;
    
    if (!CurrentVillage->Bld2Num)
        return 0;

    int32_t bld1num = CurrentVillage->Bld1Num;
    if (CurrentVillage->BuildingState[bld1num + 7].State || CurrentVillage->BuildingState[bld1num + 7].BldLeft)
        return 0;
    
    if (GetGroupMaxPlotnicLevel(&_state.MapChar_ARRAY.at(CurrentVillage->ChiefCharId)) >= 
        BuildingInfo[CurrentVillage->BuildingState[bld1num + 7].BuildInfoID].NeededPlotnicLvl)
        return 1;
    
    return 0;
}


int32_t Engine::FUN_00435874()
{
    if (FUN_00434c90(35))
        return 0;
    
    _mainCharacter->field98_0xd2 = 19;
    return 1;
}


int32_t Engine::FUN_004351d8()
{
    return FUN_00432654(4); //voevoda
}


int32_t Engine::FUN_0043516c()
{
    return FUN_00432654(1);
}


int32_t Engine::FUN_004351b4()
{
    return FUN_00432654(3);
}



int32_t Engine::FUN_00435190()
{
    return FUN_00432654(2);
}


int32_t Engine::FUN_0043536c()
{
    _mainMapChar->unk1 = _mainCharacter->GetMaxPartySize();
    return _mainMapChar->unk1 <= _mainMapChar->GroupSize;
}


int32_t Engine::FUN_0043542c()
{
    return PInteractChar->MapCharID == _mainMapChar->Index;
}


int32_t Engine::FUN_00435334()
{
    return PInteractChar->HP <= 800;
}


int32_t Engine::FUN_004354a8()
{
    return PInteractChar->Level > 3;
}


int32_t Engine::FUN_004354e0()
{
    return (PInteractChar->ClassID & 0x38) != 0;
}


int32_t Engine::FUN_00435568()
{
    if (!CurrentVillage)
        return 0;
    
    MapChar &mchr = _state.MapChar_ARRAY.at(CurrentVillage->ChiefCharId);
    
    for (int32_t i = 0; i < mchr.GroupSize; ++i)
    {
        Character &chr = _state.Characters.at(mchr.CharacterIndex + i);
        if (chr.CharacterBase < 3 && (chr.ClassID & 0x38) == 0)
        {
            if ((chr.ClassID & CLASS_BIT80) == 0 && chr.State != CHSTATE_9 && chr.State != CHSTATE_3)
            {
                if (GetVillageCharacterJob(&chr) == 0)
                {
                    if (CurrentVillage->Jobs[1].CharID == PInteractChar->Index)
                    {
                        chr.ClassID |= 8;
                        CurrentVillage->TimeSt[0] = _counter;
                    }
                    else if (CurrentVillage->Jobs[2].CharID == PInteractChar->Index)
                    {
                        chr.ClassID |= 0x10;
                        CurrentVillage->TimeSt[1] = _counter;
                    }
                    else if (CurrentVillage->Jobs[3].CharID == PInteractChar->Index)
                    {
                        chr.ClassID |= 0x18;
                        CurrentVillage->TimeSt[2] = _counter;
                    }
                    else
                        return 0;
                    
                    chr.field2_0x2 = 0;
                    FUN_00414ab4(&chr);
                    return 1;
                }                
            }
        }
    }
    return 0;
}


int32_t Engine::FUN_00433368()
{
    if (!CurrentVillage)
        return 0;
    
    CurrentVillage->BuildingState[1].BldLeft = BuildingInfo.at(CurrentVillage->BuildingState[1].BuildInfoID).BuildPeriod;
    return 1;
}


int32_t Engine::FUN_00433410()
{
    if (!CurrentVillage)
        return 0;
    
    CurrentVillage->BuildingState[4].BldLeft = BuildingInfo.at(CurrentVillage->BuildingState[4].BuildInfoID).BuildPeriod;
    return 1;
}


int32_t Engine::FUN_00433464()
{
    if (!CurrentVillage)
        return 0;
    
    CurrentVillage->BuildingState[6].BldLeft = BuildingInfo.at(CurrentVillage->BuildingState[6].BuildInfoID).BuildPeriod;
    return 1;
}


int32_t Engine::FUN_0043321c()
{
    if (!CurrentVillage)
        return 0;
    
    CurrentVillage->BuildingState[3].BldLeft = BuildingInfo.at(CurrentVillage->BuildingState[3].BuildInfoID).BuildPeriod;
    return 1;
}


int32_t Engine::FUN_004333bc()
{
    if (!CurrentVillage)
        return 0;
    
    CurrentVillage->BuildingState[2].BldLeft = BuildingInfo.at(CurrentVillage->BuildingState[2].BuildInfoID).BuildPeriod;
    return 1;
}


int32_t Engine::FUN_00433270()
{
    if (!CurrentVillage)
        return 0;
    
    CurrentVillage->BuildingState[5].BldLeft = BuildingInfo.at(CurrentVillage->BuildingState[5].BuildInfoID).BuildPeriod;
    return 1;
}


int32_t Engine::FUN_004332c4()
{
    if (!CurrentVillage)
        return 0;
    
    for (int32_t i = 0; i < CurrentVillage->Bld2Num; ++i)
    {
        int32_t bld1num = CurrentVillage->Bld1Num + 7 + i;
        CurrentVillage->BuildingState[bld1num].BldLeft = BuildingInfo.at(CurrentVillage->BuildingState[bld1num].BuildInfoID).BuildPeriod;
    }
    
    return 1;
}


int32_t Engine::FUN_00435a84()
{
    for (int32_t i = 0; i < _mainMapChar->GroupSize; ++i)
    {
        Character &chr = _state.Characters.at(_mainCharacter->Index + i);
        if ((chr.ClassID & CLASS_BIT80) == 0 && chr.State != CHSTATE_9 && chr.State != CHSTATE_3)
        {
            for (int16_t itemId : chr.ArmorWeapons)
            {
                if (itemId != 0)
                {
                    ItemInfo &inf = _state.Items.at(itemId);
                    
                    float fVar1 = PInteractChar->KuznechnoeDelo * 10.0;
                    if (fVar1 < inf.Weight)
                        inf.Weight = fVar1 + (inf.Weight - fVar1) * 0.7;
                    
                    inf.Concentration = inf.Weight;
                    
                    if (PInteractChar->KuznechnoeDelo)
                    {
                        float val = (inf.Weight - inf.Concentration) / PInteractChar->KuznechnoeDelo;
                        if (val > 0.0)
                            PInteractChar->KuznechnoeDelo = sqrtf((PInteractChar->KuznechnoeDelo * PInteractChar->KuznechnoeDelo) + val);
                    }
                }
            }
            for (int16_t itemId : chr.Inventory)
            {
                if (itemId != 0)
                {
                    ItemInfo &inf = _state.Items.at(itemId);
                    if (inf.TypeID < 6)
                    {
                        float fVar1 = PInteractChar->KuznechnoeDelo * 10.0;
                        if (fVar1 < inf.Weight)
                            inf.Weight = fVar1 + (inf.Weight - fVar1) * 0.7;

                        inf.Concentration = inf.Weight;

                        if (PInteractChar->KuznechnoeDelo)
                        {
                            float val = (inf.Weight - inf.Concentration) / PInteractChar->KuznechnoeDelo;
                            if (val > 0.0)
                                PInteractChar->KuznechnoeDelo = sqrtf((PInteractChar->KuznechnoeDelo * PInteractChar->KuznechnoeDelo) + val);
                        }
                    }
                }
            }
        }
    }
    return 1;
}


int32_t Engine::FUN_00434e98()
{
    MapChar &mchr = _state.MapChar_ARRAY.at(PInteractChar->MapCharID);
    if (CurrentVillage)
    {
        int32_t vjob = GetVillageCharacterJob(PInteractChar);
        
        if (vjob)
        {
            int32_t chrid = CurrentVillage->Jobs[vjob - 1].CharID;
            CurrentVillage->Jobs[vjob - 1].CharID = 0;
            for (int32_t i = 0; i < 5; ++i)
            {
                if (chrid < CurrentVillage->Jobs[i].CharID)
                    CurrentVillage->Jobs[i].CharID--;
            }
        }
    }
    
    _currentMap->FootMap.At(PInteractChar->Tile).ID = 0;
    
    FUN_00422778(PInteractChar);
    
    mchr.GroupSize--;
    
    for(int32_t i = PInteractChar->Index - mchr.CharacterIndex; i < mchr.GroupSize; i++)
        _state.Characters.at(mchr.CharacterIndex + i).CopyDataFrom( _state.Characters.at(mchr.CharacterIndex + i + 1) );
    
    for (int32_t i = 0; i < mchr.GroupSize; ++i)
    {
        Character &chr = _state.Characters.at(mchr.CharacterIndex + i);
        if ((chr.ClassID & CLASS_BIT80) == 0 && chr.State != CHSTATE_9 && chr.State != CHSTATE_3)
            _currentMap->FootMap.At(chr.Tile).ID = chr.Index + 1;
    }
    return 1;
}


int32_t Engine::FUN_00433acc()
{
    _mainCharacter->field98_0xd2 = 0xff;
    return 1; 
}


int32_t Engine::FUN_00435514()
{
    if (!CurrentVillage)
        return 0;

    CurrentVillage->Flags &= 0xea;
    CurrentVillage->GoldTributeTimer = 0;
    CurrentVillage->GoldTribute = 0;
    return 1;
}


int32_t Engine::FUN_004353bc()
{
    return _mainCharacter->CurrentHarizm > 60;
}



int32_t Engine::FUN_00432654(int32_t job)
{
    if (!CurrentVillage)
        return 0;
    
    Village::Job &rjob = CurrentVillage->Jobs[job];
    
    if (rjob.CharID)
        return 0;
    
    int32_t chrInGrp = FUN_004323b0(job);
    
    if (chrInGrp == 0)
        return 0;


    Character *selChar = PInteractChar;

    PInteractChar = &_state.Characters.at(_mainMapChar->CharacterIndex + chrInGrp);

    if (!FUN_00433af8())
        return 0;

    PInteractChar = selChar;


    MapChar &pMVar1 = _state.MapChar_ARRAY.at(CurrentVillage->ChiefCharId);
    
    Character &chr = _state.Characters.at(pMVar1.CharacterIndex + pMVar1.GroupSize - 1);
    chr.ClassID &= 0xC7;
    chr.field96_0xd0 = rjob.unk;
    chr.MoveTile = rjob.Tile;
    chr.field_0x3 &= 0xfb;
    
    rjob.CharID = chr.Index;
    
    if (FUN_0043edb7(&chr) == 0)
        FUN_00414ab4(&chr);
    else 
    {
        chr.field2_0x2 = 8;
        FUN_004143dc(&chr,7);
    }
    return 1;
}


void Engine::FUN_00422778(Character *pchar)
{
    if (pchar->Arrows != 0)
        _state.Items.at(pchar->Arrows).TypeID = -1;
    
    for (int16_t itemId : pchar->ArmorWeapons)
    {
        if (itemId)
            _state.Items.at(itemId).TypeID = -1;
    }
    for (int16_t itemId : pchar->Inventory)
    {
        if (itemId)
            _state.Items.at(itemId).TypeID = -1;
    }
    for (int16_t itemId : pchar->Accessories)
    {
        if (itemId)
            _state.Items.at(itemId).TypeID = -1;
    }
}


bool Engine::FUN_00436248(int32_t id)
{
    bool res = true;
    
    TSq5 *qu = &quest_4.at(id);
    for (; qu->id != -1; ++qu)
    {
        if ((qu->Flags & 4) == 0)
        {
            if (res == 0)
            {
                bool val = true;
                if ((qu->Flags & 2) == 0)
                {
                    if ((QuestsState.at(qu->id).State & 0x80) == 0)
                        val = false;
                    else
                        val = true;
                }
                else
                    val = FUN_0043259c(qu->id) != 0;
                
                if ((qu->Flags & 8) != 0)
                    val = !val;
                
                res = val;
            }
        }
        else if (res != 0)
        {
            bool val = true;
            if ((qu->Flags & 2) == 0)
            {
                if ((QuestsState.at(qu->id).State & 0x80) == 0)
                    val = false;
                else
                    val = true;
            }
            else
                val = FUN_0043259c(qu->id) != 0;
            
            if ((qu->Flags & 8) != 0)
                val = !val;
            
            res &= val;
        }
    }
    return res;
}


std::string Engine::FUN_004363e8(int32_t id)
{
    auto it = QuestTexts.find(id);
    if (it == QuestTexts.end())
        return std::string();
    
    return it->second;
}


void Engine::DrawQuestScreen()
{
    //_playScreenID = 1;
    int32_t selId = -1;
    int32_t sel = FUN_004392f4();
    if ((sel & 0xff) == 100)
        selId = sel >> 8;
    
    FillBkgRect(MAPRECT);
    //                           Common::Point(114, 0)
    ImgQueue2(_menuImages.at(1), Common::Point(144, 0), MAPRECT);
    //                           Common::Point(129, 150)
    ImgQueue2(_menuImages.at(0), Common::Point(161, 187), MAPRECT);
    
    //const int32_t width = 485;
    const int32_t width = 606;
    //Common::Point(133, 20);
    Common::Point outPos = Common::Point(166, 25);

    for (DlgTxt &txt : qOut1)
    {
        txt.Rect.left = outPos.x;
        txt.Rect.right = outPos.x + width;
        txt.Rect.top = outPos.y;
        
        outPos.y = PlaceTextWidth(FUN_004363e8(txt.q1txtid), _Fonts[3], outPos, width);
        
        txt.Rect.bottom = outPos.y + 6;
    }
    
    //Common::Point(133, 186);
    outPos = Common::Point(166, 232);
    
    int32_t i = 0;
    for (DlgTxt &txt : qOut2)
    {
        txt.Rect.left = outPos.x;
        txt.Rect.right = outPos.x + width;
        txt.Rect.top = outPos.y;
        
        if (i == selId)
            outPos.y = PlaceTextWidth(FUN_004363e8(txt.q1txtid), _Fonts[2], outPos, width);
        else
            outPos.y = PlaceTextWidth(FUN_004363e8(txt.q1txtid), _Fonts[3], outPos, width);
        
        txt.Rect.bottom = outPos.y + 6;
        i++;
    }
}

    
}