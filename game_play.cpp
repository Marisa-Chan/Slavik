#include "system.h"
#include "game.h"
#include "fmt/core.h"
#include "fmt/printf.h"

namespace Game
{
    
bool Engine::LoadINTR()
{
    DAT_00a3e74c |= 1;
    
    _textQueueCount = 0;
    _imgQueue2Count = 0;
    _imgQueue1Count = 0;
    
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    
    std::string tmp = fmt::format("INTR{:d}{:s}.RES", SCREENRESX, _langLiter);
    
    FSMgr::File f = FSMgr::Mgr::ReadFile(tmp);
    if (!f)
    {
        System::PostQuitMessage();
        fmt::printf("Game res file %s not found\n", tmp);
        return false;
    }
    
    FreeMenuImages();
    
    f->seek(4, 0);
    uint32_t dataSize = f->readU32L();
    
    std::vector<int32_t> sprHdr(512 * 2);
        
    for (int32_t i = 0; i < 512; ++i)
    {
        sprHdr[i * 2] = f->readS32L();
        sprHdr[i * 2 + 1] = f->readS32L() / 2;// / 512;//2;
    }
    
    size_t pos = f->tell();
    
    for (int32_t i = 0; i < 512; ++i)
    {
        if (sprHdr[i * 2] == -1)
            break;
        
        f->seek(pos + sprHdr[i * 2], 0);
        _menuImages.push_back( Resources::LoadRL8BitImage(f.get(), Res.Palettes.data() + sprHdr[i * 2 + 1]) );
    }
    /*
    DAT_00a3e860 = *(ushort *)(ImgSprite_ARRAY_006304b8[17].pData + (int)_bkgImage) - 1;
    DAT_00a3e864 = (uint)*(ushort *)(ImgSprite_ARRAY_006304b8[18].pData + (int)_bkgImage);
    DAT_00a3e784 = (uint)*(ushort *)((int)(ImgSprite_ARRAY_006304b8[18].pData + (int)_bkgImage) + 2);
    iVar1 = (DAT_00a3e880 - GScrOff.x) + 2 + DAT_00a3e864 * -2;
    DAT_00a3e870 = GScrOff.x + -1 + DAT_00a3e864 + (iVar1 - (iVar1 / DAT_00a3e860) * DAT_00a3e860) / 2;
    DAT_00a3e88c = (BottomPanelY - DAT_00a3e784) + 1;
    */
    return true;
}

    
void Engine::UpdateGame()
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    
    if (_mainCharacter->ClassID & CLASS_BIT80) 
    {
        _nextStateMode = STATEMD_MODE7;
        _isGameStarted = false;
        StartPlayMovie("GAMEOVER.FLM");
        return;
    }
    
    
    if ( (System::rand() % 100) > 98 )
        PlaySound( 0x100 + (System::rand() % 8) + (_currentMapID - 1) * 8,
                   0, 0, 0);
    
    ProcessCamera();
    
    Common::Point ppoint(_mousePos.x * SCREENRESX / _screenSize.x, _mousePos.y * SCREENRESY / _screenSize.y);
    
    if ( MAPRECT.IsIn( ppoint ) )
        _mouseMapPos = _mousePos - (MAPRECT.Pos() * _screenSize / SCREENRES);
        
    
    //if (DAT_00a3e84c)
    {
        //DAT_00a3e84c--;
        //if (DAT_00a3e84c == 0) 
            ImgQueue1(_menuImages[3], Common::Point(), Common::Rect());        
    }
    
    
    if (_nextMapID > 0)
    {
        if (_currentMapID == _nextMapID) 
        {
            _nextMapID = 0;
            FUN_004118dc();
        }
        else 
        {
            FUN_00436ad0(_currentMapID);
            FUN_00436a00();
            
            if (!LoadMap(_nextMapID, 0)) 
            {
                ShutdownWithMsg("Map not load!");
                return;
            }
            
            LoadUsedObjects();
        }
        
        FUN_00428f90( Common::Point(_mainCharacter->field107_0xe4,_mainCharacter->field109_0xec) );
        FUN_004290d8();
        PlayChangeScreen(-1);
    }
    
    /*

        _counter += 1;
        FUN_00421170();
        FUN_00421da4();
        FUN_0042480c();
        FUN_0041d0fc();
        for (local_1c = 0; local_1c < Speed; local_1c += 1) {
            _counter += 1;
            FUN_00421170();
            FUN_00421da4();
            FUN_0042480c();
            FUN_0041d0fc();
            if ((System::_isPlayingVideo != 0) || ((_mainCharacter->ClassID & 0x80) != 0)) break;
        }
        if (0 < _objectsToLoadCount) {
            UsedObjectsLoad();
        }
        if (0 < DAT_00a3e814) {
            FUN_0043d1d0();
        }
        if ((DAT_00a3e74c & 4) == 0) {
            if (((DAT_00a3e74c & 8) != 0) && (DAT_00a3e74c ^= 8, _playScreenID == 7)) {
                FUN_00431d70(0);
            }
        }
        else {
            DAT_00a3e74c ^= 4;
            FUN_0042f50c(CharInfoCharacter,0);
        }
        if ((DAT_00a3e74c & 1) == 0) {
            if (keyCode != 0) {
                PlayHandleKey(keyCode);
            }
            FUN_00422864();
        }
        else {
            DAT_00a3e74c ^= 1;
            if (keyCode != 0) {
                PlayHandleKey(keyCode);
            }
            FUN_00422864();
            if ((DAT_00a3e74c & 1) == 0) {
                FUN_004290d8();
            }
        }
    */
}

void Engine::FUN_004118dc()
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    
    if (_mainMapChar->MapID == _currentMapID) 
    {
        for (int i = 0; i < _mainMapChar->GroupSize; ++i) 
        {
            Character &pchar = _state.Characters.at( _mainMapChar->CharacterIndex + i );
            if (((pchar.ClassID & CLASS_BIT80) == 0) && (pchar.State != CHSTATE_9) && (pchar.State != CHSTATE_3)) 
                _currentMap->FootMap(pchar.Tile).ID = 0;
        }
    }
    else
        _mainMapChar->MapID = _currentMapID;
    
    int charIndex = 0;
    Common::Rect spwnRect = _mainMapChar->spwnTlRect;

    //Place main character
    if ( spwnRect.top > 0 && spwnRect.top < 319 && 
         spwnRect.left > 0 && spwnRect.left < 159 &&
         _currentMap->FootMap(spwnRect.Pos()).ID == 0 )
    {
        _mainCharacter->Direction = spwnRect.bottom;
        
        _mainCharacter->_field_0xc = spwnRect.top;
        _mainCharacter->_field_0xe = spwnRect.left;
        _mainCharacter->Tile = spwnRect.Pos();
        
        _mainCharacter->field2_0x2 &= 0xf0;
        _mainCharacter->field17_0x13[0] = 0xff;
        
        _currentMap->FootMap(_mainCharacter->Tile).ID = _mainCharacter->CharIndex;
        
        if ((_mainCharacter->field_0x3 & 4) == 0)
            FUN_004143dc(_mainCharacter, DIR_6);
        else
            FUN_004143dc(_mainCharacter, DIR_0);
        
        charIndex = 1;
    }
    
    // Find place for characters
    int uVar5 = _mainMapChar->spwnTlRect.top & 1;
    for (; charIndex < _mainMapChar->GroupSize; ++charIndex) 
    {
        Character &pchar = _state.Characters.at( _mainMapChar->CharacterIndex + charIndex );
        if (((pchar.ClassID & CLASS_BIT80) == 0) && (pchar.State != CHSTATE_9) && (pchar.State != CHSTATE_3))
        {
            int ldir = 0;
            if (_mainCharacter->Direction < DIR_4)
                ldir = _mainCharacter->Direction + 4;
            else 
                ldir = _mainCharacter->Direction - 4;
            
            int local_1c = ldir;
            int local_20 = ldir - 1;
            
            if (local_20 < 0)
                local_20 = 7;
            
            if (rand() % 2 != 0)
                local_1c = local_20;
            
            for (int local_20 = 0; local_20 < 8; ++local_20) 
            {
                int local_28 = rand() % 6;
                for (int local_18 = 0; local_18 < 6; ++local_18) 
                {
                    Common::Point ppos = spwnRect.Pos() + PlaceDOff[uVar5][local_1c][local_28];
                    
                    if ((ppos.y > 0) && (ppos.y < 319) &&
                        (ppos.x > 0) && (ppos.x < 159) && 
                        _currentMap->FootMap(ppos).ID == 0 )
                    {
                        pchar.Direction = spwnRect.bottom;
                        pchar._field_0xc = ppos.y;
                        pchar._field_0xe = ppos.x;
                        pchar.Tile = ppos;
                        pchar.field2_0x2 &= 0xf0;
                        pchar.field17_0x13[0] = 0xff;
                        
                        _currentMap->FootMap.At(pchar.Tile).ID = pchar.CharIndex;
                        pchar.State = 0xff;
                        
                        FUN_00414ab4(&pchar);
                        
                        local_20 = 8;
                        local_18 = 12;
                    }
                    
                    local_28 += 1;
                    
                    if (local_28 == 6)
                        local_28 = 0;
                }
                
                local_1c += 1;
                
                if (local_1c == 8)
                    local_1c = 0;
            }
        }
    }
}

void Engine::FUN_00414ab4(Character *pchar)
{
    int nextState = CHSTATE_0;
    
    pchar->field2_0x2 |= 0x40;
    
    if ( (pchar != _mainCharacter) && (pchar->MapCharID == _mainCharacter->MapCharID) && ((pchar->field2_0x2 & 0xf) == 0))
        pchar->field2_0x2 |= 0x20;
    
    if ((pchar->field_0x3 & 4) == 0)
        nextState = CHSTATE_6;
    else
        nextState = CHSTATE_0;
    
    if (pchar->State != nextState)
        FUN_004143dc(pchar, nextState);
}

void Engine::FUN_00436ad0(int32_t tmp)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
}

void Engine::FUN_00436a00()
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
}


}