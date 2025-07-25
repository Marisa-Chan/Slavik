#include <regex>
#include <GL/gl.h>
#include <thread>
#include <chrono>

#include "game.h"
#include "system.h"
#include "resources.h"
#include "fmt/core.h"
#include "fmt/printf.h"

#include <set>

#include <signal.h>

namespace Game
{
 
Engine Engine::Instance;
    
bool Engine::Process()
{
    if (_screenSize != System::GetResolution())
    {
        Common::Point screensz = System::GetResolution();
        Common::Point bufsz((screensz.x * MAPVIEWW) / SCREENRESX, (screensz.y * MAPVIEWH) / SCREENRESY);

        GFXDrawer.SetBufferSize(GFX::BUF_0, bufsz);
        GFXDrawer.SetBufferSize(GFX::BUF_1, bufsz);
        GFXDrawer.SetBufferSize(GFX::BUF_2, bufsz);
    }
    
    _screenSize = System::GetResolution();
    _gameViewport = (MAPVIEW * _screenSize) / SCREENRES;
    
    uint32_t start = SDL_GetTicks();
    if (Update())
        return true;
    
    UpdateMusic();
    
    Draw();
    
    UpdateCursor();
    
    
    uint32_t frmTime = SDL_GetTicks() - start;
    
    if (frmTime > 1000)
        frmTime = 1000;
        
    int32_t delay = (FrameTime - frmTime) - (1000 / FrameTime) / 5;
    if (delay > 0)
        SDL_Delay(delay);
    
    return false;
}

bool Engine::Update()
{
    if (_KeyQueue.empty())
        _KeyPressed = -1;
    else
    {
        _KeyPressed = _KeyQueue.front();
        _KeyQueue.pop_front();
    }


    if (_KeyPressed == KEYFN_ESC)
    {
        _KeyPressed = -1;

        switch(_stateMode)
        {
            case STATEMD_PLAY:
                ResetMouseItemHold();

                if (_playScreenID == PLSCREEN_0)
                {
                    _stateMode = STATEMD_MODE7;
                    _isGameStarted = true;
                    _nextStateMode = STATEMD_PLAY;
                }
                else if (_playScreenID != PLSCREEN_1)
                {
                    _playScreenID = STATEMD_PLAY;
                    DWORD_00a3e7b4 = 0;
                    FUN_004292e4();
                }
                break;

            case STATEMD_MAINMENU:
                if (_isGameStarted)
                {
                    _stateMode = STATEMD_PLAY;
                    _imgQueue2Count = 0;
                    LoadINTR();
                    PlayChangeScreen(PLSCREEN_0);
                    FUN_004292e4();
                }
                break;
            
            case STATEMD_NEWGAME:
                _stateMode = STATEMD_MODE7;
                _isGameStarted = false;
                _nextStateMode = STATEMD_NEWGAME;
                break;
            
            case STATEMD_MODE4:
            case STATEMD_MODE5:
                _stateMode = STATEMD_MODE7;
                _nextStateMode = STATEMD_MODE5;
                break;
            
            case STATEMD_SETTINGS:
                _stateMode = STATEMD_MODE7;
                _nextStateMode = STATEMD_SETTINGS;
                _bConfTransparency = _svTransparency;
                _bConfDayNight = _svDayNight;
                _bConfLightEffects = _svLightEffects;
                _bConfMusic = _svMusic;
                _bConfShadows = _svShadows;
                _bConfSound = _svSound;
                break;
            
            default:
                break;
        }
    }

    
    if (_mouseDownPrev == 0)
        _mouseDownPrev = _mousePress;
    else
    {
        if ((_mouseDownPrev & MOUSEB_L) == 0)
            _mouseDownPrev |= _mousePress & MOUSEB_L;
        else 
        {
            if ((_mousePress & MOUSEB_L) != 0)
                _mousePress &= ~MOUSEB_L;
            
            if ((_mouseDown & MOUSEB_L) == 0)
                _mouseDownPrev &= ~MOUSEB_L;
            
        }
        
        if ((_mouseDownPrev & MOUSEB_R) == 0)
            _mouseDownPrev |= _mousePress & MOUSEB_R;
        else 
        {
            if ((_mousePress & MOUSEB_R) != 0)
                _mousePress &= ~MOUSEB_R;
            
            if ((_mouseDown & MOUSEB_R) == 0)
                _mouseDownPrev &= ~MOUSEB_R;
            
        }
        
        if ((_mouseDownPrev & MOUSEB_3) == 0)
            _mouseDownPrev |= _mousePress & MOUSEB_3;
        else 
        {
            if ((_mousePress & MOUSEB_3) != 0)
                _mousePress &= ~MOUSEB_3;
            
            if ((_mouseDown & MOUSEB_3) == 0)
                _mouseDownPrev &= ~MOUSEB_3;
        }
    }
    
    _mousePos = _mouseCurPos;
    _mouseMapPos = Common::Point(-1, -1);
    
    _uiMousePos = _mousePos * SCREENRES / _screenSize;
    
    
    
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    
    switch(_stateMode)
    {
        case STATEMD_PLAY:
            UpdateGame();
            break;
        
        case STATEMD_MAINMENU:
            UpdateMainMenu();
            break;
            
        case STATEMD_NEWGAME:
            UpdateNewGameMenu();
            break;
            
        case STATEMD_QUIT:
            break;
            
        case STATEMD_MODE4:
            break;
            
        case STATEMD_MODE5:
            break;
            
        case STATEMD_SETTINGS:
            break;
            
        case STATEMD_MODE7:
            Update7();
            break;
            
        case STATEMD_MODE8:
            Update8();
            break;
            
        case STATEMD_MODE9:
            Update9();
            break;
            
        default:
            break;
    }

    return _doQuit;
}

void Engine::ShutdownWithMsg(const std::string &str)
{
    fmt::printf("%s\n", str);
    _doQuit = true;
}

void Engine::CreateCursors()
{
    for(int32_t i = 0; i < 11; ++i)
    {
        
        if (_cursors[i])
            SDL_FreeCursor(_cursors[i]);
        
        _cursors[i] = SDL_CreateColorCursor(Res.CursorImages[i]->SW, 0, 0);
    }
}

void Engine::Init(int gfxmode)
{    
    InitKeyMapping();
    InitHotKeys();
    _KeyState.fill(0);
    
    System::EventsAddHandler(EventsWatcher);

    _mixer.Init(32);
    _mixer.VolumeMusic(90); // 90db volume
    
    Res.Load(&_mixer);
    
    GFXDrawer.SetPalettes(Res.Palettes);
    
    CreateCursors();
    
    for(int32_t i = 0; i < 4; ++i)
    {
        if ( (_Fonts.at(i) = GFXDrawer.LoadFont(fmt::sprintf("res/font%d.res", i))) == nullptr )
        {
            fmt::printf("Can't load font res/font%d.res\n", i);
            System::PostQuitMessage();
        }
    }                
    
    _screenSize = System::GetResolution();//Common::Point(1024, 768);
   
    _langLiter = "R";
    
    _stateMode = STATEMD_MODE9;
    
    CursorAssume = 4;

    _musicId = -1;
    
    _mainMapChar = &_state.MapChar_ARRAY[0];
    _mainCharacter = & _state.Characters.at(_state.MapChar_ARRAY[0].CharacterIndex);
    
    Common::Point bufsz((_screenSize.x * MAPVIEWW) / SCREENRESX, (_screenSize.y * MAPVIEWH) / SCREENRESY);
    
    GFXDrawer.SetBufferSize(GFX::BUF_0, bufsz);
    GFXDrawer.SetBufferSize(GFX::BUF_1, bufsz);
    GFXDrawer.SetBufferSize(GFX::BUF_2, bufsz);

    MapObjUseCount.fill(-1);
}

int Engine::EventsWatcher(void *, SDL_Event *event)
{
    switch(event->type)
    {
        default:
        break;
        
        case SDL_KEYDOWN:
        {
            auto it = Instance._KeyMap.find(event->key.keysym.scancode);
            if (it != Instance._KeyMap.end())
            {
                Instance._KeyState[it->second] = 1;
                Instance._KeyQueue.push_back(it->second);
            }            
        }
        break;

        case SDL_KEYUP:
        {
            auto it = Instance._KeyMap.find(event->key.keysym.scancode);
            if (it != Instance._KeyMap.end())
                Instance._KeyState[it->second] = 0;
        }
        break;

        case SDL_TEXTINPUT:
        {
        }
        break;

        case SDL_MOUSEBUTTONDOWN:
        {
            switch(event->button.button)
            {
                default:
                    break;
                
                case SDL_BUTTON_LEFT:
                    Instance._mousePress |= MOUSEB_L;
                    Instance._mouseDown |= MOUSEB_L;
                    break;
                    
                case SDL_BUTTON_RIGHT:
                    Instance._mousePress |= MOUSEB_R;
                    Instance._mouseDown |= MOUSEB_R;
                    break;
                
                case SDL_BUTTON_MIDDLE:
                    Instance._mousePress |= MOUSEB_3;
                    Instance._mouseDown |= MOUSEB_3;
                    break;
            }
            
        }
        break;

        case SDL_MOUSEBUTTONUP:
        {
            switch(event->button.button)
            {
                default:
                    break;
                
                case SDL_BUTTON_LEFT:
                    Instance._mousePress &= ~MOUSEB_L;
                    Instance._mouseDown &= ~MOUSEB_L;
                    break;
                    
                case SDL_BUTTON_RIGHT:
                    Instance._mousePress &= ~MOUSEB_R;
                    Instance._mouseDown &= ~MOUSEB_R;
                    break;
                    
                case SDL_BUTTON_MIDDLE:
                    Instance._mousePress &= ~MOUSEB_3;
                    Instance._mouseDown &= ~MOUSEB_3;
                    break;
            }
        }
        break;

        case SDL_MOUSEMOTION:
        {
            Instance._mouseCurPos.x = event->motion.x;
            Instance._mouseCurPos.y = event->motion.y;
        }
        break;
        
//        case SDL_WINDOWEVENT_RESIZED:
//        case SDL_WINDOWEVENT_SIZE_CHANGED:
//        {                
//            Common::Point res = System::GetResolution();
//            Common::Point bufsz((res.x * MAPVIEWW) / SCREENRESX, (res.y * MAPVIEWH) / SCREENRESY);
//
//            GFXDrawer.SetBufferSize(GFX::BUF_0, bufsz);
//            GFXDrawer.SetBufferSize(GFX::BUF_1, bufsz);
//            GFXDrawer.SetBufferSize(GFX::BUF_2, bufsz);
//        }
//        break;
    }
    
    return 1; // This event can be passed to another event watcher
}

static bool incr = false;

void Engine::Draw()
{
    MouseOnCharacter = nullptr;
    MouseOnObject = nullptr;
    
    GFXDrawer.Begin();
    
    GFXDrawer.SetOutBuffer(GFX::BUF_OFF);
    GFXDrawer.Clear();
    
    if ( _stateMode == STATEMD_PLAY )
    {
        if (_playScreenID == PLSCREEN_0 || _bTransparentMenu)
        {
            GFXDrawer.SetVirtResolution();
            GFXDrawer.SetModelViewMatrix(mat4x4f());

            GFXDrawer.SetOutBuffer(GFX::BUF_2);
            GFXDrawer.Clear();
            GFXDrawer.SetOutBuffer(GFX::BUF_1);
            GFXDrawer.Clear();
            GFXDrawer.SetOutBuffer(GFX::BUF_0);
            GFXDrawer.Clear();

            DrawGame();
        }
        
        if (_playScreenID == PLSCREEN_1)
            DrawQuestScreen();
        else if (_playScreenID == PLSCREEN_2)
            DrawJournal();
        else if (_playScreenID == PLSCREEN_3)
            DrawCharInfo();
        else if (_playScreenID == PLSCREEN_MAP)
            FillBkgRect(MAPRECT);
        else if (_playScreenID == PLSCREEN_7)
            DrawTrade(0);
        
        if (_playScreenID == PLSCREEN_0 || _playScreenID == PLSCREEN_3)
        {
            if (DisplayInvOfCharID)
                DrawInventory(&_state.Characters.at(DisplayInvOfCharID - 1), _invButton, !_bTransparentMenu);
        }
        
        TextQueue(fmt::sprintf("Screen %d", _playScreenID), _Fonts[0], {0,0}, Common::Rect());
    }
    
    GFXDrawer.SetFade(false);
    GFXDrawer.SetOutBuffer(GFX::BUF_OFF);
    
    GFXDrawer.SetVirtResolution(Common::Point(SCREENRESX, SCREENRESY));
    GFXDrawer.SetModelViewMatrix( mat4x4f() );
    
    Common::FRect outpos;
    outpos.left = 140;
    outpos.top = 0;
    outpos.right = SCREENRESX;
    outpos.bottom = 555;
    GFXDrawer.SetLinearFilter(true);
    GFXDrawer.DrawRect(GFXDrawer.GetBufferTex(GFX::BUF_0), outpos );
    GFXDrawer.DrawRect(GFXDrawer.GetBufferTex(GFX::BUF_1), outpos, 0.5);
    GFXDrawer.DrawRect(GFXDrawer.GetBufferTex(GFX::BUF_2), outpos );

    
    
    GFXDrawer.SetOutBuffer(GFX::BUF_OFF);
    GFXDrawer.SetVirtResolution(Common::Point(SCREENRESX, SCREENRESY));
    GFXDrawer.SetModelViewMatrix( mat4x4f() );

    GFXDrawer.SetLinearFilter(true);

    for(int32_t i = 0; i < _imgQueue1Count; ++i)
    {
        ImagePlace &im = _imgQueue1.at(i);
        if (im.Img || im.PalImg)
        {
            if (im.Fade != vec3f())
                GFXDrawer.SetFade(true, im.Fade);
            else
                GFXDrawer.SetFade(false);
            
            if (!im.Limits.IsEmpty())
            {
                GFXDrawer.SetScissor(true, im.Limits);
                if (im.Pal != -1)
                    GFXDrawer.Draw(im.PalImg, im.Pal, im.DrawPlace);
                else
                    GFXDrawer.Draw(im.Img, im.DrawPlace);
                GFXDrawer.SetScissor(false);
            }
            else
            {
                if (im.Pal != -1)
                    GFXDrawer.Draw(im.PalImg, im.Pal, im.DrawPlace);
                else
                    GFXDrawer.Draw(im.Img, im.DrawPlace);
            }
        }
    }

    for(int32_t i = 0; i < _imgQueue2Count; ++i)
    {
        ImagePlace &im = _imgQueue2.at(i);
        if (im.Img || im.PalImg)
        {
            if (im.Fade != vec3f())
                GFXDrawer.SetFade(true, im.Fade);
            else
                GFXDrawer.SetFade(false);
            
            if (!im.Limits.IsEmpty())
            {
                GFXDrawer.SetScissor(true, im.Limits);
                if (im.Pal != -1)
                    GFXDrawer.Draw(im.PalImg, im.Pal, im.DrawPlace);
                else
                    GFXDrawer.Draw(im.Img, im.DrawPlace);
                GFXDrawer.SetScissor(false);
            }
            else
            {
                if (im.Pal != -1)
                    GFXDrawer.Draw(im.PalImg, im.Pal, im.DrawPlace);
                else
                    GFXDrawer.Draw(im.Img, im.DrawPlace);
            }
        }
    }
    
    GFXDrawer.SetFade(false);

    for(int32_t i = 0; i < _textQueueCount; ++i)
    {
        TextPlace &tx = _textQueue.at(i);
        if (!tx.Text.empty() && tx.Font)
        {
            if (!tx.Limits.IsEmpty())
            {
                GFXDrawer.SetScissor(true, tx.Limits);
                GFXDrawer.DrawText(tx.Text, tx.Font, tx.DrawPlace);
                GFXDrawer.SetScissor(false);
            }
            else
                GFXDrawer.DrawText(tx.Text, tx.Font, tx.DrawPlace);
        }
    }

    _imgQueue1Count = 0;
    _imgQueue2Count = 0;
    _textQueueCount = 0;
    
    if ( _stateMode == STATEMD_PLAY )
    {
        if (_playScreenID == PLSCREEN_MAP /*&& (DWORD_00a3e74c & 2)*/)
        {
            DWORD_00a3e74c &= ~2;
            DrawMap(); //Draw map
        }
    }
    

    GFXDrawer.Flip();
}

bool Engine::LoadMap(int32_t mapID, int32_t param)
{
    DWORD_00a3e758 = false;
    MapObjects2Count = 0;
    MapsOpened.set(mapID - 1);
    _objectsToLoadCount = 0;
    
    if (_playScreenID == PLSCREEN_MAP)
        UpdateMapImage();
    
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    if (_currentMap)
        delete _currentMap;
    
    _camMax = Common::Point();
    
    Res.UnloadDynSounds(&_mixer);

    _currentMap = LoadGameMap(mapID);
    if (!_currentMap)
        return false;
    
    _currentMapID = mapID;

    //Load ambience sounds
    for(int i = 0; i < SND_AMBIENCE_CNT; ++i)
        Res.LoadDynSound(&_mixer, GetMapSoundId(mapID, i));
    
    _camMax.y = 0;
    for (int32_t i = 159; i > 0 && _camMax.y == 0; --i)
    {
        for(int32_t j = 0; j < 80; ++j)
        {
            if (_currentMap->TileMap[i][j][0] != 0)
            {
                _camMax.y = i * TileH;
                break;
            }
        }
        
    }
    
    _camMax.x = 0;
    for (int32_t i = 79; i > 0 && _camMax.x == 0; --i)
    {
        for(int32_t j = 0; j < 160; ++j)
        {
            if (_currentMap->TileMap[j][i][0] != 0)
            {
                _camMax.x = (i - 1) * TileW;
                break;
            }
        }
        
    }
    

    for (auto &cnt : MapObjUseCount)
    {
        if (cnt <= -1)
            cnt = -1;
        else
            cnt = 0;
    }
    
    std::bitset<32> monstersInUse;
    monstersInUse.reset();
    
    //int32_t local_1c = 0;
    
//    for (int32_t i = 1; i < 200; ++i) 
//    {
//        MapChar &mchar = _state.MapChar_ARRAY.at(i)
    for (MapChar &mchar : _state.MapChar_ARRAY)
    {
        if (&mchar == _mainMapChar)
            continue;
            
        if (mchar.unk5 != 0 && mchar.MapID == _currentMapID) 
        {
            if (param == 0 && (mchar.unk5 & 0x40)) 
                mchar.unk4 = 0;
            
            for (int32_t j = 0; j < mchar.GroupSize; ++j) 
            {
                Character &rchar = _state.Characters.at(mchar.CharacterIndex + j);
                
                if (!param && !(mchar.unk5 & 0x80) && mchar.unk5 < 0x51 &&
                    !PlaceMob(&rchar)) 
                {
                    mchar.GroupSize = j;
                    break;
                }
                
                bool bVar21 = false;
                
                if (!(rchar.ClassID & CLASS_BIT80) && (rchar.State != CHSTATE_9) && (rchar.State != CHSTATE_3)) 
                    bVar21 = true;
                
                if (bVar21)
                    _currentMap->FootMap(rchar.Tile).ID = rchar.Index + 1;
                
                if (rchar.ClassID & CLASS_BIT40) 
                {
                    monstersInUse.set( rchar.GetMonsterId() );
                    
                    if (param == 0)
                        rchar.field2_0x2 = 0x40;
                    
                    if (MapObjUseCount[rchar.CharacterBase] >= 0) 
                        MapObjUseCount[rchar.CharacterBase]++;
                    else 
                    {
                        if (MapObjUseCount[rchar.CharacterBase] == -1) 
                        {
                            MapObjUseCount[rchar.CharacterBase] = -2;

                            //local_1c += ObjectsHeaders[rchar.CharacterBase].Size;
                            
                            _objectsToLoad[_objectsToLoadCount] = rchar.CharacterBase;
                            _objectsToLoadCount++;
                        }
                        
                        rchar.field_0x3 |= 0x80;
                    }
                }
                
                if (param == 0)
                    FUN_004143dc(&rchar, rchar.State);
                else
                    FUN_00414078(&rchar);
            }
        }
    }
    
    if (param == 0)
    {
        bool bVar21 = true;
//        for (int32_t i = 1; i < _state.MapChar_ARRAY.size(); ++i)
//        {
//            MapChar &mchar = _state.MapChar_ARRAY.at(i);
        for (MapChar &mchar : _state.MapChar_ARRAY)
        {
            if ((mchar.unk5 & 0x40) && (mchar.MapID == _currentMapID) && 
                (mchar.unk5 - 0x41 < 0x10))
            {
                if (bVar21)
                    bVar21 = false;
                else 
                {
                    while (mchar.field1_0x2 < 0  &&  mchar.GroupSize < mchar.unk1) 
                    {
                        Character *pCVar25 = CalcMapChar(&mchar);
                        
                        if (!pCVar25)
                            break;
                        
                        if (!PlaceMob(pCVar25))
                            break;
                                                                
                        mchar.field1_0x2 += 0;//DAT_0045e276[mchar.unk5];
                        
                        _currentMap->FootMap(pCVar25->Tile).ID = pCVar25->Index + 1;
                        
                        if (pCVar25->ClassID & CLASS_BIT40) 
                        {
                            monstersInUse.set( pCVar25->GetMonsterId() );
                            
                            if (MapObjUseCount[pCVar25->CharacterBase] >= 0)
                                MapObjUseCount[pCVar25->CharacterBase]++;
                            else {
                                if (MapObjUseCount[pCVar25->CharacterBase] == -1) 
                                {
                                    MapObjUseCount[pCVar25->CharacterBase] = -2;
                                    
                                    //local_1c += Game::ObjectsHeaders[pCVar25->CharacterBase].Size;
                                    
                                    _objectsToLoad[_objectsToLoadCount] = pCVar25->CharacterBase;
                                    _objectsToLoadCount++;
                                }
                                pCVar25->field_0x3 |= 0x80;
                            }
                        }
                        
                        FUN_0041c750(pCVar25);
                        FUN_004143dc(pCVar25, pCVar25->State);
                    }
                    
                    if (mchar.field1_0x2 < 0)
                        mchar.field1_0x2 = 0;
                }
            }
        }
        
        FUN_004118dc();
        
        _mainMapChar->unk1 = _mainCharacter->GetMaxPartySize();
    }
    
    CurrentVillage = nullptr;
    for (Village &vlg : _state.VillageState) 
    {
        if (vlg.MapID == _currentMapID)
        {
            CurrentVillage = &vlg;
            for (int32_t i = 0; i < (vlg.Bld1Num + vlg.Bld2Num + 7); ++i) 
            {
                Village::BldState &bld = vlg.BuildingState[i];
                
                if (bld.BuildInfoID > -1)
                    _currentMap->MapObjects[ bld.ObjID ].ObjId = bld.State + BuildingInfo[bld.BuildInfoID].GraphObjectOffset;
            }
            
            OnEnterVillageUpdateTrader(&vlg);
            //break; 
        }
    }
    
    MapObjectsCount = _currentMap->MapObjects.size();
    
    for (GameMap::Object &mObj : _currentMap->MapObjects)
    {
        int puVar27 = mObj.ObjId + 30;
        /*pSVar17 = mObj->pGameObj;
        pSimple = Game::SimpleObject + (int)pSVar17;
        mObj->pGameObj = pSimple;
        if (mObj.PaletteIndex == 0) {
            mObj.PaletteIndex = Game::SimpleObject[(int)pSVar17].PaletteIndex;
        }*/
        Resources::SimpleObject &rSimple = Res.SimpleObjects.at(mObj.ObjId);
        
        mObj.Pos.x = ((mObj.Pos.x * 116) / 2) + (mObj.Pos.y & 1) * -29;
        mObj.Pos.y = (mObj.Pos.y * 64 / 4);
        
        if (rSimple.FrameTime < 1) 
        {
            mObj.FrameTimeLeft = 0;
            mObj.CurrentFrame = 0;
        }
        else 
        {
            if (rSimple.NumFrames < 2)
                mObj.CurrentFrame = 0;
            else
                mObj.CurrentFrame = System::rand() % rSimple.NumFrames;

            mObj.FrameTimeLeft = rSimple.FrameTime;
        }
        
        
        
        for (int i = 0; i < rSimple.Flames.size(); ++i) 
        {
            int FlameID = rSimple.Flames[i].FlameID;

            int FrmCnt = FlameAnims.at(FlameID).second - FlameAnims.at(FlameID).first + 1;
            if (FrmCnt < 2) 
                mObj.Flames[i] = FlameAnims.at(FlameID).first;
            else
                mObj.Flames[i] = FlameAnims.at(FlameID).first + (System::rand() % FrmCnt);
        }
        
        if (MapObjUseCount[puVar27] < 0)
        {
            if (MapObjUseCount[puVar27] == -1) 
            {
                MapObjUseCount[puVar27] = -2;
                
                //local_1c += ObjectsHeaders[puVar27].Size;
                
                _objectsToLoad[ _objectsToLoadCount ] = puVar27;
                _objectsToLoadCount++;
                
                rSimple.Flags |= Resources::SimpleObject::FLAG_LOADING;
            }
        }
        else 
            MapObjUseCount[puVar27]++;

        mObj.ObjectFlags = rSimple.Flags;
        //MapObjectsCount ++;
    }
    
    //if (MMGR::FREE < local_1c) {
    // Free unused objects if no memory
    //}
    
    if (param == 0) 
    {
        mapGS1Count = 0;
        for (int32_t i = 0; i < _state.GS1ARRAYCount; ++i)
        {
            GS1 &globalLoot = _state.GS1ARRAY.at(i);
            
            if ( (globalLoot.MapID == _currentMapID) && 
                   ( (globalLoot.Pos.x > -1 || 
                     (globalLoot.Pos.x != globalLoot.Pos.y)) || 
                     (_counter + globalLoot.Pos.x > 5399) )  ) 
            {
                _currentMap->FootMap(globalLoot.Tile).Flags |= 0x2;

                mapGS1[mapGS1Count] = globalLoot;
                mapGS1[mapGS1Count].LootID = i;
                mapGS1[mapGS1Count].Index = mapGS1Count;

                mapGS1[mapGS1Count].Pos = FUN_00439ba0(globalLoot.Tile) - (_menuImages.at(globalLoot.ImgID)->GetSize() / 2);
                mapGS1Count += 1;
            }
        }
    }
    
    // Load monster sounds
    for(int i = 0; i < monstersInUse.size(); ++i)
    {
        if (monstersInUse.test(i))
        {
            for(int j = 0; j < SND_PER_CHAR; j++)
                Res.LoadDynSound(&_mixer, Character::GetMonsterSoundId(i, j));
        }
    }
    
    for (int32_t j = 0; j < _state.GS2ARRAYCount; ++j)
    {
        GS2 &warp = _state.GS2ARRAY.at(j);
        if (warp.MapID == _currentMapID) 
        {
            for (int32_t y = warp.WarpZone.top; y <= warp.WarpZone.bottom; ++y)
            {
                for (int32_t x = warp.WarpZone.left; x <= warp.WarpZone.right; ++x)
                    _currentMap->FootMap.At(x, y).Flags |= 0x1;
            }
        }
    }

    LoadUsedObjects();
    
    return true;
}

void Engine::LoadUsedObjects()
{
    for (int32_t i = 0; i < _objectsToLoadCount; ++i)
    {
        MapObjUseCount[ _objectsToLoad[i] ] = 0;
        Res.LoadObjectData( _objectsToLoad[i] );
    }
    
    _objectsToLoadCount = 0;
}


void Engine::DrawGame()
{    
    GFXDrawer.SetVirtResolution(_gameViewport);
    
    _outOff = -_camera;
    
    ViewTiles.y = _gameViewport.y / TileHh + 3;
    ViewTiles.x = _gameViewport.x / TileW + 3;
    
    _ambientColor = CalculateLight() / 100.0;
    _lightColor = vec3f((System::rand() % 8) + 16, (System::rand() % 4) + 8, 0);
    
    _tracePos = _mouseMapPos;
    
    GFXDrawer.SetLightColor(_lightColor / 100.0);
    
    DrawGroundAndObj();
    DrawObjects();
}

void Engine::DrawGroundAndObj()
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    
    if (!_currentMap) return;
    
    GFXDrawer.SetOutBuffer(GFX::BUF_0);
    
    Common::Point MaxTile = _viewStartPos + ViewTiles;
    
    GFXDrawer.SetLightMask(nullptr);
    GFXDrawer.SetLinearFilter(true);
    GFXDrawer.SetAlphaLogic(true);
    
    if (!_bLightGround || !_bConfLightEffects)
    {
        GFXDrawer.SetFade(false);
        
        for (int32_t y = _viewStartPos.y; y < MaxTile.y; ++y) 
        {
            Common::Point pnt;
            pnt.x = _outOff.x + _viewStartPos.x * TileW;
            if ((y & 1) != 0)
                pnt.x += TileWh;
            
            pnt.y = _outOff.y + y * TileH;
            
            for (int32_t x = _viewStartPos.x; x < MaxTile.x; ++x) 
            {
                if (_currentMap->TileMap[y][x][0] != 0)
                    GFXDrawer.Draw( Res.Tiles[ _currentMap->TileMap[y][x][0] - 1 ], pnt);

                if (_currentMap->TileMap[y][x][1] != 0)              
                    GFXDrawer.Draw( Res.Tiles[ _currentMap->TileMap[y][x][1] - 1 ], pnt);
                
                pnt.x += TileW;
            }
        }
    }
    else
    {
        GFXDrawer.SetFade(true, _ambientColor);
        
        if (!DAT_00a3e6a8)
            GFXDrawer.SetLightColor(_lightColor / 200.0);
        /*else if (_state.MapChar_ARRAY[DAT_00a3e6a8->field2].field27_0x1c == 1)
            GFXDrawer.SetLightColor(_lightColor / 100.0);*/
        else
            GFXDrawer.SetLightColor(_lightColor / 200.0);
        
        for (int32_t y = _viewStartPos.y; y < MaxTile.y; ++y) 
        {
            Common::Point pnt;
            pnt.x = _outOff.x + _viewStartPos.x * TileW;
            if ((y & 1) != 0) 
                pnt.x += TileWh;
            
            pnt.y = _outOff.y + y * TileH;
            
            for (int32_t x = _viewStartPos.x; x < MaxTile.x; ++x) 
            {
                if (_currentMap->LightMap(x, y) != 0)
                    GFXDrawer.SetLightMask(Res.TilesLight[ _currentMap->LightMap(x, y) - 1 ]);
                else
                    GFXDrawer.SetLightMask(nullptr);
                
                if (_currentMap->TileMap[y][x][0] != 0)
                    GFXDrawer.Draw( Res.Tiles[ _currentMap->TileMap[y][x][0] - 1 ], pnt);

                if (_currentMap->TileMap[y][x][1] != 0)              
                    GFXDrawer.Draw( Res.Tiles[ _currentMap->TileMap[y][x][1] - 1 ], pnt);
                
                pnt.x += TileW;
            }
        }
    }
    
    GFXDrawer.SetAlphaLogic(false);
    
    GFXDrawer.SetLightMask(nullptr);
    
    GFXDrawer.SetLinearFilter(true);
    for(const auto &dec : _currentMap->Decorations)   
        GFXDrawer.Draw( Res.Tiles[ dec.TileID ], _outOff + dec.Position);

    MapObjDrawRefCount1 = 0;
    MapObjDrawRefCount2 = 1;
    MapObjDraw.fill( MapObjDrawRef() );
    MapObjDrawHeap.fill( MapObjDrawRef() );
    
    
    
    for(MapChar& obj: _state.MapChar_ARRAY)
    {
        if ((obj.unk5 != 0) && (obj.MapID == _currentMapID)) 
        {
            
            for (int32_t i = 0; i < obj.GroupSize; ++i) 
            {
                Character& charObj = _state.Characters.at( obj.CharacterIndex + i );
                
                if ((charObj.ClassID & CLASS_BIT40) && (charObj.field_0x3 & 0x80)) 
                {
                    if (MapObjUseCount[charObj.CharacterBase] < 0) 
                        continue;

                    MapObjUseCount[charObj.CharacterBase] += 1;
                    charObj.field_0x3 ^= 0x80;
                    
                    FUN_00414078(&charObj);
                }
                    
                if (charObj.ClassID & CLASS_BIT80) 
                {
                    Common::Point pos = charObj.ViewPos - _camera;
                    Common::Point rbpos = pos + charObj.imgSize;

                    if ( (pos.x < _gameViewport.x) && (rbpos.x >= 0) &&
                         (pos.y < _gameViewport.y) && (rbpos.y >= 0) ) 
                    {
                        if (MapObjDrawRefCount1 < rbpos.y)
                            MapObjDrawRefCount1 = rbpos.y;

                        if (MapObjDraw[rbpos.y].CHAR == NULL) 
                        {
                            MapObjDraw[rbpos.y].CHAR = &charObj;
                            MapObjDraw[rbpos.y].Pos = pos;
                        }
                        else 
                        {
                            MapObjDrawHeap[MapObjDrawRefCount2].NextObjInHeap = MapObjDraw[rbpos.y].NextObjInHeap;
                            MapObjDrawHeap[MapObjDrawRefCount2].CHAR = &charObj;
                            MapObjDrawHeap[MapObjDrawRefCount2].Pos = pos;

                            MapObjDraw[rbpos.y].NextObjInHeap = MapObjDrawRefCount2;
                            MapObjDrawRefCount2 += 1;
                        }
                    }
                }
            }
        }
    }
    
    for (int32_t local_18 = 0; local_18 <= MapObjDrawRefCount1; ++local_18) 
    {
        Character *pCVar6 = MapObjDraw[local_18].CHAR;
        if (pCVar6) 
        {
            MapObjDrawRef *local_6c = &MapObjDraw[local_18];
            
            while(true)
            {
                if (pCVar6->ClassID & CLASS_BIT40) 
                {
                    if ( _currentMap->LightMap2(pCVar6->Tile) == 0 ) 
                        GFXDrawer.SetFade(true, _ambientColor);
                    else
                        GFXDrawer.SetFade(false);

                    GFXDrawer.Draw(Res.DynObjects.at(pCVar6->CharacterBase).Images.at(pCVar6->pFrame), 
                                   pCVar6->paletteOffset,
                                   MapObjDraw[local_18].Pos);
                }
                else
                {
                    if ( _currentMap->LightMap2(pCVar6->Tile) == 0 ) 
                        GFXDrawer.SetFade(true, _ambientColor);
                    else
                        GFXDrawer.SetFade(false);

                    GFXDrawer.Draw(Res.CharacterBases.at(pCVar6->CharacterBase).Images.at(pCVar6->pFrame), 
                                   pCVar6->paletteOffset,
                                   MapObjDraw[local_18].Pos);

                    DrawCharacterSprite(*pCVar6);
                }
                
                if (!local_6c->NextObjInHeap)
                    break;
                    
                local_6c = &MapObjDrawHeap.at(local_6c->NextObjInHeap);
            }
        }
    }
    
    for (int32_t y = 0; y < 10; ++y) 
    {
        Character *pch = SelectedCharacters.at(y);
        if ( !pch )
            break;
        
        GFX::Image *img = nullptr;
        
        if (pch->HP < 401)
            img = _menuImages.at(172);
        else if (pch->HP < 801)
            img = _menuImages.at(126);
        else
            img = _menuImages.at(178);
        
        Common::Point pos = (GScrOff - _camera) + pch->POS - Common::Point(29, 16);
        
        if ((pch->State == CHSTATE_7) || (pch->State == CHSTATE_1)) 
        {
            switch(pch->Direction) 
            {
            case DIR_0:
                pos.x -= pch->Frame * 5;
                break;
                
            case DIR_1:
                pos -= Common::Point(pch->Frame / 2 + pch->Frame * 2, pch->Frame / 2 + pch->Frame);
                break;
                
            case DIR_2:
                pos.y -= pch->Frame / 2 + pch->Frame * 2;
                break;
                
            case DIR_3:
                pos += Common::Point(pch->Frame / 2 + pch->Frame * 2, -(pch->Frame / 2 + pch->Frame));
                break;
                
            case DIR_4:
                pos.x += pch->Frame * 5;
                break;
                
            case DIR_5:
                pos += Common::Point(pch->Frame / 2 + pch->Frame * 2, pch->Frame / 2 + pch->Frame);
                break;
                
            case DIR_6:
                pos.y += pch->Frame / 2 + pch->Frame * 2;
                break;
                
            case DIR_7:
                pos += Common::Point(-(pch->Frame / 2 + pch->Frame * 2), pch->Frame / 2 + pch->Frame);
                break;
               
            default:
                break;
            }
        }
        
        
        GFXDrawer.SetFade(false);
        GFXDrawer.Draw(img, pos);
        printf("%d %d   %d %d\n", pos.x, pos.y, pch->POS.x, pch->POS.y);        
    }
    
    if (mapGS1Count != 0) 
    {
        Common::Point iVar18 = _currentMap->CheckFoot(_some + ViewTiles * 2); // xxx 
        
        
        for (int32_t y = _some.y; y < iVar18.y; ++y) 
        {
            for (int32_t x = _some.x; x < iVar18.x; ++x) 
            {
                if ((_currentMap->FootMap(x, y).Flags & 0x2))
                {
                    GS1 *loot = GetLootByTile({x, y});
                                        
                    if (loot->unk2 > -1)
                    {
                        Common::Point outpos = _outOff + loot->Pos;
                        
                        //iVar22 = _outOff + loot->Pos;
                        if (!DWORD_00a3e758) 
                        {
                            GFXDrawer.SetFade(true, _ambientColor);
                            
                            
                            GFXDrawer.Draw(_menuImages.at(loot->ImgID), outpos);
//                                   MapObjDraw[local_18].Pos);
//                            
//                            iVar15._0_2_ = loot->field3_0xa;
//                            iVar15._2_2_ = loot->field4_0xc;
//                            puVar19 = GFX::PreparePalette(ImgSprite_ARRAY_006304b8[iVar15 >> 0x10].paletteOffset);
//                            iVar16._0_2_ = loot->field3_0xa;
//                            iVar16._2_2_ = loot->field4_0xc;
//                            GFX::DrawRLEPalette(out,(ushort *)(ImgSprite_ARRAY_006304b8[iVar16 >> 0x10].pData + _bkgImage),(short *)puVar19,ypos,
//                                                iVar22);
                        }
                        else 
                        {
                            
                            GFXDrawer.SetFade(false, _ambientColor);
                            
                            
                            GFXDrawer.Draw(_menuImages.at(loot->ImgID), outpos);
//                            iVar12._0_2_ = pGVar21->field3_0xa;
//                            iVar12._2_2_ = pGVar21->field4_0xc;
//                            FUN_0043de4d((undefined *)out,&LIGHTSRES2,
//                                         ypos + -0x20 + *(ushort *)(ImgSprite_ARRAY_006304b8[iVar12 >> 0x10].pData + _bkgImage) / 2,
//                                         iVar22 + -0x15 + *(ushort *)((int)(ImgSprite_ARRAY_006304b8[iVar12 >> 0x10].pData + _bkgImage) + 2) / 2,
//                                         0x40,0x2b);
//                            iVar13._0_2_ = pGVar21->field3_0xa;
//                            iVar13._2_2_ = pGVar21->field4_0xc;
//                            iVar14._0_2_ = pGVar21->field3_0xa;
//                            iVar14._2_2_ = pGVar21->field4_0xc;
//                            GFX::DrawRLEPalette(out,(ushort *)(ImgSprite_ARRAY_006304b8[iVar14 >> 0x10].pData + _bkgImage),
//                                                (short *)((int)GFX::pRawPalettes + ImgSprite_ARRAY_006304b8[iVar13 >> 0x10].paletteOffset),ypos,
//                                                iVar22);
                        }
                    }
                }
            }
        }
    }
    
    
    /*if (_currentMap)
    {       
        for(const auto &obj : _currentMap->MapObjects)
        {
            if (Res.SimpleObjects[obj.ObjId].Images[0])
            {
                Common::Point pnt(obj.unk1 * TileWh, obj.unk2 * TileHh);
                if (obj.unk2 & 1)
                    pnt.x -= TileWhh;
                GFXDrawer.SetOutBuffer(GFX::BUF_1);
                GFXDrawer.SetFade(false);
                GFXDrawer.DrawShadow( Res.SimpleObjects[obj.ObjId].Shadows[0], pnt + Res.SimpleObjects[obj.ObjId].unk[0][0]);
                GFXDrawer.SetFade(true);
                GFXDrawer.SetOutBuffer(GFX::BUF_2);
                GFXDrawer.Draw( Res.SimpleObjects[obj.ObjId].Images[0], pnt + Res.SimpleObjects[obj.ObjId].unk[0][1]);
            }
        }
    }*/
}

void Engine::DrawObjects()
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    
    MapObjDrawRefCount2 = 1;
    MapObjDraw.fill(MapObjDrawRef());
    MapObjDrawHeap.fill(MapObjDrawRef());
    
    GFXDrawer.SetLinearFilter(true);
    
    if (_bConfShadows)
    {
        GFXDrawer.SetOutBuffer(GFX::BUF_1);
        // Draw Characters shadows
        for ( MapChar &mchr : _state.MapChar_ARRAY)
        {
            if ((mchr.unk5 != 0) && (mchr.MapID == _currentMapID)) 
            {
                for (int32_t i = 0; i < mchr.GroupSize; ++i) 
                {
                    Character &chr = _state.Characters.at(mchr.CharacterIndex + i);
                    //printf("grpsz %d id %d\n",mchr.GroupSize, i );
                    if ((chr.field_0x3 & 0x80) == 0) 
                    {
                        GFX::PalImage *shdw = nullptr;
                        if ((chr.ClassID & CLASS_BIT40) == 0)
                            shdw = Res.CharacterBases.at(chr.CharacterBase).Shadows.at(chr.pFrame);
                        else
                            shdw = Res.DynObjects.at(chr.CharacterBase).Shadows.at(chr.pFrame);
                        
                        if (shdw) 
                        {
                            //Common::Point p = chr.shdOffset + _outOff + chr.POS;
                            //printf("Shadow %d %d\n",p.x, p.y );
                            GFXDrawer.SetFade(false);
                            GFXDrawer.DrawShadow( shdw, chr.shdOffset + _outOff + chr.POS);
                        }
                        
                        if ((chr.ClassID & CLASS_BIT40) == 0)
                            FUN_004251d0(chr);
                    }
                }
            }
        }
        // Draw object shadows
        //GameMap::Object &local_34 = _currentMap->MapObjects;
        //for (local_28 = 0; local_28 < MapObjectsCount; local_28 += 1) {
        if (_currentMap) 
        {
            for( GameMap::Object &obj : _currentMap->MapObjects )
            {
                Resources::SimpleObject &pSimple = Res.SimpleObjects.at(obj.ObjId);

                GFX::PalImage *shdw = nullptr;

                if ((obj.ObjectFlags & Resources::SimpleObject::FLAG_LOADING) == 0) 
                {
                    shdw = pSimple.Shadows.at(obj.CurrentFrame);
                }
                else if ((pSimple.Flags & Resources::SimpleObject::FLAG_LOADING) == 0) 
                {
                    obj.ObjectFlags &= ~Resources::SimpleObject::FLAG_LOADING;
                    MapObjUseCount[obj.ObjId + 30]++;

                    shdw = pSimple.Shadows.at(obj.CurrentFrame);
                }

                if (shdw && (pSimple.Flames.size() < 2)) 
                {
    //                iVar4._0_2_ = local_34->Pos.y;
    //                iVar4._2_1_ = local_34->unk3;
    //                iVar4._3_1_ = local_34->CurrentFrame;
    //                iVar5._0_2_ = local_34->Pos.x;
    //                iVar5._2_2_ = local_34->Pos.y;
    //                iVar6._0_2_ = local_34->Pos.y;
    //                iVar6._2_1_ = local_34->unk3;
    //                iVar6._3_1_ = local_34->CurrentFrame;
    //                iVar7._0_2_ = local_34->FrameTimeLeft;
    //                iVar7._2_2_ = local_34->Pos.x;
    //                GFX::PlaceShadow((ushort *)(pSimple->Data + iVar31),
    //                                 (iVar7 >> 0x10) + _outOff.x + (*(int *)((int)pSimple->Images[(iVar6 >> 0x18) + 7] + 6) >> 0x10),
    //                                 (iVar5 >> 0x10) + _outOff.y + (*(int *)pSimple->unk[iVar4 >> 0x18] >> 0x10));
                    GFXDrawer.SetFade(false);
                    GFXDrawer.DrawShadow(shdw, pSimple.Offsets.at(obj.CurrentFrame).ShdOffset + _outOff + obj.Pos);
                }

            }
        }
//        GFX::DrawShadows((int)out);
    }
    GFXDrawer.SetOutBuffer(GFX::BUF_2);
    
    MapObjDrawRefCount1 = 0;    
    if (_currentMap) 
    {
        for( GameMap::Object &obj : _currentMap->MapObjects )
        {
            Resources::SimpleObject &pSimple = Res.SimpleObjects.at(obj.ObjId);
            GFX::Image *img = nullptr;
            
            if ((obj.ObjectFlags & Resources::SimpleObject::FLAG_LOADING) == 0) 
            {
                img = pSimple.Images.at(obj.CurrentFrame);
            }
            else if ((pSimple.Flags & Resources::SimpleObject::FLAG_LOADING) == 0) 
            {
                obj.ObjectFlags &= ~Resources::SimpleObject::FLAG_LOADING;
                MapObjUseCount[obj.ObjId + 30]++;
                img = pSimple.Images.at(obj.CurrentFrame);
            }
            
            if (img)
            {
                Common::Point pt = _outOff + pSimple.Offsets.at(obj.CurrentFrame).Offset + obj.Pos;
                int32_t refid = pt.y + img->GetSize().y;
//                if (std::set<int>({43,44}).count(obj.Index))
//                {
//                    printf("\n OBJ %d index %d x %d y %d refid %d  %x\n", obj.ObjId, obj.Index, pt.x, pt.y, refid, obj.unk3);
//                    printf("%d %d %d\n", _gameViewport.x, _gameViewport.y, GScrOff.y);
//                }
//                GFXDrawer.DrawText(fmt::sprintf("%d %d", obj.ObjId, obj.Index),_Fonts[3], pt);
                if (pt.x <= _gameViewport.x && pt.y <= _gameViewport.y && refid >= GScrOff.y)
                {
                    if (obj.ObjectFlags & 8)
                    {
                        refid -= img->GetSize().y / 4;
                        if (refid < 0)
                            refid = 0;
                    }
                    
                    if (MapObjDrawRefCount1 < refid) 
                        MapObjDrawRefCount1 = refid;
                    
                    MapObjDrawRef &ref = MapObjDraw.at(refid);
                    if (ref.Type == MapObjDrawRef::TYPE_UNK)
                    {
                        ref.Type = MapObjDrawRef::TYPE_OBJ1;
                        ref.MOBJ1 = &obj;
                        ref.Pos = pt;
                    }
                    else 
                    {
                        MapObjDrawRef &href = MapObjDrawHeap.at(MapObjDrawRefCount2);
                        href.Type = MapObjDrawRef::TYPE_OBJ1;
                        href.NextObjInHeap = ref.NextObjInHeap;
                        href.MOBJ1 = &obj;
                        href.Pos = pt;
                        
                        ref.NextObjInHeap = MapObjDrawRefCount2;
                        MapObjDrawRefCount2++;
                    }
                }
            }
        }
    }
    
    
    
    for(MapChar& obj: _state.MapChar_ARRAY)
    {
        if ((obj.unk5 != 0) && (obj.MapID == _currentMapID)) 
        {
            for (int32_t i = 0; i < obj.GroupSize; ++i) 
            {
                Character& charObj = _state.Characters.at( obj.CharacterIndex + i );
                
                if ((charObj.ClassID & CLASS_BIT80) || (charObj.field_0x3 & 0x80)) 
                    continue;
                
                Common::Point pt = charObj.ViewPos - _camera; 
                int32_t refid = pt.y + charObj.imgSize.y;
                
                if (pt.x > _gameViewport.x || pt.y > _gameViewport.y || refid < GScrOff.y)
                    continue;
                                    
                if (charObj.ClassID == 0x49 || charObj.ClassID == 0x56) 
                    refid += 60;
                
                if (MapObjDrawRefCount1 < refid) 
                    MapObjDrawRefCount1 = refid;
                
                MapObjDrawRef &ref = MapObjDraw.at(refid);
                if (ref.Type == MapObjDrawRef::TYPE_UNK)
                {
                    ref.Type = MapObjDrawRef::TYPE_CHAR;
                    ref.CHAR = &charObj;
                    ref.Pos = pt;
                }
                else 
                {
                    MapObjDrawRef &href = MapObjDrawHeap.at(MapObjDrawRefCount2);
                    href.Type = MapObjDrawRef::TYPE_CHAR;
                    href.NextObjInHeap = ref.NextObjInHeap;
                    href.CHAR = &charObj;
                    href.Pos = pt;
                    
                    ref.NextObjInHeap = MapObjDrawRefCount2;
                    MapObjDrawRefCount2++;
                }
            }
        }
    }
    
    for(int32_t i = 0; i < MapObjects2Count; ++i)
    {
        MapObject2 &arrow = MapObjects2.at(i);
        Common::Point pt = _outOff + arrow.p3 + arrow.p2;
        
        if (pt.x > _gameViewport.x || pt.y > _gameViewport.y)
            continue;
        
        int32_t refid = pt.y + 94;
        
        if (MapObjDrawRefCount1 < refid) 
            MapObjDrawRefCount1 = refid;
        
        MapObjDrawRef &ref = MapObjDraw.at(refid);
        if (ref.Type == MapObjDrawRef::TYPE_UNK)
        {
            ref.Type = MapObjDrawRef::TYPE_OBJ2;
            ref.MOBJ2 = &arrow;
            ref.Pos = pt;
        }
        else 
        {
            MapObjDrawRef &href = MapObjDrawHeap.at(MapObjDrawRefCount2);
            href.Type = MapObjDrawRef::TYPE_OBJ2;
            href.NextObjInHeap = ref.NextObjInHeap;
            href.MOBJ2 = &arrow;
            href.Pos = pt;

            ref.NextObjInHeap = MapObjDrawRefCount2;
            MapObjDrawRefCount2++;
        }
    }
    
//    local_38 = MapObjects2;
//    for (local_28 = 0; local_28 < MapObjects2Count; local_28 += 1) {
//        fVar35 = __CHP((float10)local_38->p3.x + (float10)_outOff.x + (float10)local_38->p2.x);
//        iVar31 = (int)ROUND(fVar35);
//        fVar35 = __CHP((float10)local_38->p3.y + (float10)_outOff.y + (float10)local_38->p2.y);
//        iVar33 = System::MapObjDrawRefCount2;
//        iVar32 = (int)ROUND(fVar35);
//        if (((iVar31 <= DAT_00a3e880) && (GScrOff.x <= iVar31 + 0x40)) && ((iVar32 <= BottomPanelY && (GScrOff.y <= iVar32 + 0x40)))) {
//            iVar1 = iVar32 + 0x5e;
//            if (System::MapObjDrawRefCount1 < iVar1) {
//                System::MapObjDrawRefCount1 = iVar1;
//            }
//            if (MapObjDraw[iVar1].pObject.MOBJ2 == NULL) {
//                MapObjDraw[iVar1].type = 1;
//                MapObjDraw[iVar1].pObject.MOBJ2 = local_38;
//                MapObjDraw[iVar1].Pos.x = iVar31;
//                MapObjDraw[iVar1].Pos.y = iVar32;
//            }
//            else {
//                pMVar34 = MapObjDrawHeap + System::MapObjDrawRefCount2;
//                MapObjDrawHeap[System::MapObjDrawRefCount2].type = 1;
//                MapObjDrawHeap[iVar33].NextObjInHeap = MapObjDraw[iVar1].NextObjInHeap;
//                (pMVar34->pObject).MOBJ2 = local_38;
//                MapObjDrawHeap[iVar33].Pos.x = iVar31;
//                MapObjDrawHeap[iVar33].Pos.y = iVar32;
//                iVar31 = System::MapObjDrawRefCount2;
//                System::MapObjDrawRefCount2 += 1;
//                MapObjDraw[iVar1].NextObjInHeap = (short)iVar31;
//            }
//        }
//        local_38 = local_38 + 1;
//    }
    
    
    for(int32_t y = GScrOff.y; y <= MapObjDrawRefCount1; ++y)
    {
        MapObjDrawRef *ref = &MapObjDraw.at(y);
        if (ref->Type == MapObjDrawRef::TYPE_UNK)
            continue;
        
        while(true)
        {
            if (ref->Type == MapObjDrawRef::TYPE_OBJ2)
            {
                MapObject2 *obj = ref->MOBJ2;
                if (obj->SpecialID == 0)
                {
                    ///
                }
                
                GFXDrawer.SetFade(false);
                
                GFXDrawer.Draw(_menuImages.at( (_counter & 1) + obj->dmg2), ref->Pos);
            }
            else if (ref->Type == MapObjDrawRef::TYPE_OBJ1)
            {
                GameMap::Object *obj = ref->MOBJ1;
                Resources::SimpleObject &pSimple = Res.SimpleObjects.at( obj->ObjId );
                GFX::Image *img = pSimple.Images.at(obj->CurrentFrame);
 
                if (img)
                {
                    if (pSimple.Flames.empty())
                    {
                        GFXDrawer.SetFade(true, _ambientColor);
                        GFXDrawer.Draw(img, ref->Pos);
                        if ((obj->ObjectFlags & 1) && CheckTraceImage(img, ref->Pos) )
                            MouseOnObject = obj;
                    }
                    else
                    {
                        if (pSimple.Flames.size() < 2)
                        {
                            GFXDrawer.SetFade(true, _ambientColor);
                            GFXDrawer.Draw(img, ref->Pos);
                        }
                        else
                        {
                            GFXDrawer.SetFade(false, _ambientColor);
                            GFXDrawer.Draw(img, ref->Pos);
                        }
                        
                        if ((obj->ObjectFlags & 1) && CheckTraceImage(img, ref->Pos) )
                            MouseOnObject = obj;
                        
                        for(Resources::SimpleObject::FlamePos &flame : pSimple.Flames)
                        {
                            Common::Point pt = _outOff + flame.Position + obj->Pos;
                            int16_t &frame = obj->Flames.at(flame.Index);
                            
                            GFXDrawer.SetFade(false, _ambientColor);
                            GFXDrawer.DrawFlame(Res.Flames.at(frame), pt);
                            
                            frame++;
                            if (frame >= FlameAnims.at(flame.FlameID).second)
                                frame = FlameAnims.at(flame.FlameID).first;
                        }                        
                    }
                }
            }
            else if (ref->Type == MapObjDrawRef::TYPE_CHAR)
            {
                Character *chr = ref->CHAR;
                
                if (chr->ClassID & CLASS_BIT40)
                {
                    Resources::DynamicObject &obj = Res.DynObjects.at(chr->CharacterBase);
                    GFX::PalImage *img = obj.Images.at(chr->pFrame);
                    
                    if ( _currentMap->LightMap2(chr->Tile) == 0 ) 
                        GFXDrawer.SetFade(true, _ambientColor);
                    else
                        GFXDrawer.SetFade(false);
                    
                    GFXDrawer.Draw(img, chr->paletteOffset, ref->Pos);
                    
                    if (CheckTraceImage(img, ref->Pos))
                        MouseOnCharacter = chr;
                }
                else
                {
                    Resources::CharacterSprites &obj = Res.CharacterBases.at(chr->CharacterBase);
                    GFX::PalImage *img = obj.Images.at(chr->pFrame);
                    
                    if ( _currentMap->LightMap2(chr->Tile) == 0 ) 
                        GFXDrawer.SetFade(true, _ambientColor);
                    else
                        GFXDrawer.SetFade(false);
                    
                    GFXDrawer.Draw(img, chr->paletteOffset, ref->Pos);
                    DrawCharacterSprite(*chr);
                    
                    if (CheckTraceImage(img, ref->Pos))
                        MouseOnCharacter = chr;
                }
            }
            
            if (!ref->NextObjInHeap)
                break;
            
            ref = &MapObjDrawHeap.at(ref->NextObjInHeap);
        }
    }
}


Engine::GameMap *Engine::LoadGameMap(int32_t mapID)
{
    FSMgr::File f = FSMgr::Mgr::ReadFile(fmt::format("KONUNG.{:d}", mapID));
    if (!f)
        return nullptr;
    
    GameMap *map = new GameMap();
    for(size_t j = 0; j < 160; ++j)
    {
        for(size_t i = 0; i < 80; ++i)
        {
            map->TileMap[j][i][0] = f->readU8();
            map->TileMap[j][i][1] = f->readU8();
            
            if (map->TileMap[j][i][0])
            {
                if (map->MapLimits.x < i)
                    map->MapLimits.x = i;
                if (map->MapLimits.y < j)
                    map->MapLimits.y = j;
            }
        }
    }
    
    Common::PlaneVector<uint8_t>tmpReadMap(160, 320);
    f->read(tmpReadMap.data(), 160 * 320);
    
    for(size_t j = 0; j < 320; ++j)
    {
        for(size_t i = 0; i < 160; ++i)
        {
            map->FootMap(i, j).Flags = 0;
            map->FootMap(i, j).ID = 0;
            
            if (tmpReadMap(i, j) == 0xFF)
                map->FootMap(i, j).ID = 0xFFF;
            else if (tmpReadMap(i, j) & 0xF == 0xF)
            {
                map->FootMap(i, j).Flags = tmpReadMap(i, j) >> 4;
                map->FootMap(i, j).ID = 0xFFF;
            }
            else
                map->FootMap(i, j).Flags = tmpReadMap(i, j) >> 4;
        }
    }
    
    map->Decorations.reserve(500);
    for(size_t i = 0; i < 1000; ++i)
    {
        int32_t tileID = f->readS32L();
        
        f->seek(8, 1);

        int16_t x = f->readS16L();
        int16_t y = f->readS16L();
        if (tileID >= 0 )
            map->Decorations.push_back(GameMap::Decoration(tileID, {x, y}));
    }
    
    map->MapObjects.reserve(500);
    for(size_t i = 0; i < 1000; ++i)
    {
        int32_t ID = f->readS32L();
        if (ID >= 0)
        {
            f->seek(4, 1);
            
            int32_t index = map->MapObjects.size();
            
            map->MapObjects.emplace_back();
            GameMap::Object &obj = map->MapObjects.back();
            
            obj.Index = index;

            obj.ObjId = ID;
            for(size_t j = 0; j < 20; ++j)
                obj.Flames[j] = f->readU8();
            
            obj.FrameTimeLeft = f->readS16L();
            obj.Pos.x = f->readS16L();
            obj.Pos.y = f->readS16L();
            obj.ObjectFlags = f->readU8();
            obj.CurrentFrame = f->readU8();
        }
        else
            f->seek(0x20, 1);

    }
    
    FSMgr::File l = FSMgr::Mgr::ReadFile(fmt::format("LIGHTS.{:d}", mapID));
    if (l)
    {
        for(size_t j = 0; j < 160; ++j)
        {
            for(size_t i = 0; i < 80; ++i)
                map->LightMap(i, j) = l->readU8();
        }
        
        for(size_t j = 0; j < 320; ++j)
        {
            for(size_t i = 0; i < 160; ++i)
                map->LightMap2(i, j) = l->readU8();
        }
    }
    else
    {
        map->LightMap.fill(0);
        map->LightMap2.fill(0);
    }
    
    return map;
}
    

vec3f Engine::CalculateLight()
{
    _bLightGround = false;
    int32_t dayTime = _counter % DayDuration;
    
    if (dayTime == 0) 
      _bLightEff = (System::rand() % 100) < 90;
    
    auto mapLight = _dungeonMaps.find(_currentMapID);
    
    if (mapLight != _dungeonMaps.end())
        return mapLight->second;
    
    
    if ((!_bConfDayNight) || (dayTime < _lightPhazes[1]))
        return vec3f(0, 0, 0);

    if (dayTime < _lightPhazes[2]) 
    {
        if ((dayTime - _lightPhazes[1]) < 25)
        {
            float tmp = (dayTime - _lightPhazes[1]) / 2.0;
            return vec3f( tmp, (tmp / 2.0), 0.0);
        }
        else 
            return vec3f(12.0, 6.0, 0.0);
    }
    else if (dayTime < _lightPhazes[3])
    {
        return vec3f(12.0, 6.0, 0.0);
    }
    else if (dayTime < _lightPhazes[4])
    {
        if ((dayTime - _lightPhazes[3]) < 25)
        {
            float tmp = (24.0 - (dayTime - _lightPhazes[3])) / 2.0;
            return vec3f( tmp, (tmp / 2.0), 0.0);
        }
        else 
            return vec3f(0, 0, 0);
    }
    else if (dayTime < _lightPhazes[5])
    {
        return vec3f(0, 0, 0);
    }
    else if (dayTime < _lightPhazes[6])
    {
        if ((dayTime - _lightPhazes[5]) < 101)
        {
            float tmp = (dayTime - _lightPhazes[5]) / 2.0;
            return ((tmp * 0.2), (-tmp * 0.4), -tmp);
        }
        else 
            return vec3f(10.0, -20.0, -50.0);
    }
    else 
    {
        _bLightGround = true;
        
        if (dayTime < _lightPhazes[7])
        {
            if ((dayTime - _lightPhazes[6]) < 61)
            {
                float tmp = (dayTime - _lightPhazes[6]) / 2.0;
                return vec3f( ( 10.0 - tmp * 0.33),
                              (-20.0 - tmp),
                              (-50.0 - tmp * 0.66) );
            }
            else 
                return vec3f(0.0, -50.0, -70.0);
        }
        else if (dayTime < _lightPhazes[8]) 
        {
            if ((dayTime - _lightPhazes[7]) < 121)
            {
                float tmp = (dayTime - _lightPhazes[7]) / 2.0;
                return vec3f( -tmp, -50.0 -(tmp / 3.0), -70 );
            }
            else 
                return vec3f(-60, -70, -70);
        }
        else if (dayTime < _lightPhazes[9]) 
        {
            if (_bLightEff == 0)
                return vec3f(-60, -70, -70);
            else if ((dayTime - _lightPhazes[8]) < 41)
                return vec3f(-60, -70.0 + (dayTime - _lightPhazes[8]) / 2.0, -70);
            else 
                return vec3f(-60, -50, -70);
        }
        else if (dayTime < _lightPhazes[10]) 
        {
            if (_bLightEff == 0)
                return vec3f(-60, -70, -70);
            else 
                return vec3f(-60, -50, -70);
        }
        else if (dayTime < _lightPhazes[11]) 
        {
            if (_bLightEff == 0)
                return vec3f(-60, -70, -70);
            else if ((dayTime - _lightPhazes[10]) < 41)
                return vec3f(-60, -50.0 - (dayTime - _lightPhazes[10]) / 2.0, -70);
            else
                return vec3f(-60, -70, -70);
        }
        else if (dayTime < _lightPhazes[12]) 
        {
            if ((dayTime - _lightPhazes[11]) < 101)
            {
                float tmp = (dayTime - _lightPhazes[11]) / 2.0;
                return vec3f((tmp * 0.5988 - 60.0), (tmp / 5.0) - 70.0, tmp - 70.0);
            }
            else 
                return vec3f(-30, -60, -20);
        }
        else 
        {
            _bLightGround = false;
            if ((dayTime - _lightPhazes[12]) < 121)
            {
                float tmp = (dayTime - _lightPhazes[12]) / 2.0;
                return vec3f(tmp / 2.0 - 30.0, tmp - 60.0, tmp / 3.0 - 20.0);
            }
            else 
                return vec3f(0, 0, 0);
        }
    }
    
    return vec3f(0, 0, 0);
}  


bool Engine::StartPlayMovie(const std::string &movie)
{
    _isPlayingVideo = false;
    SetMusicOn(false);
    
    bool doPlay = false;
    
    std::string tmp = fmt::format("VIDEO/%s", movie);
    if (FSMgr::Mgr::IsFile(tmp))
    {
        //strcat(local_11c,",80,120,640,360");
        //doPlay = (*(code *)CreateAviWnd)(MainWindow,tmp,&DAT_00a4e428);
        printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    }
    
    if (!doPlay)
    {
        _stateMode = _nextStateMode;
        SetMusicOn(_bConfMusic);
        
        if (_stateMode == STATEMD_PLAY) 
        {
            //PlayChangeScreen(PLSCREEN_0);
            FUN_004292e4();
        }
        return false;
    }

    _isPlayingVideo = true;
    return true;
}

void Engine::SetMusicOn(bool en)
{
    _mixer.StopMusic(!en);
}

void Engine::PlayMusicId(int32_t id)
{
    _musicId = id;
    Res.PlayMusic(&_mixer, _musicId);
}

void Engine::UpdateMusic()
{
    if (_musicId == -1)
        return;

    if (_mixer.IsMusicEnd())
    {
        int32_t id = System::rand() % Res.MUSIC_NUM;
        if (id == _musicId)
            id = (id + 1) % Res.MUSIC_NUM;
        
        PlayMusicId(id);
    }
}
  
void Engine::PlayChangeScreen(int32_t screen)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    switch(screen)
    {
        case PLSCREEN_MAP:
            UpdateMapImage();
            break;
    }
    _playScreenID = screen;
}

void Engine::FUN_004292e4()
{
    DisplayInvOfCharID = DisplayInvOfCharID2;
    if (DisplayInvOfCharID2 != 0)
    {
        CharInfoCharacter = &_state.Characters.at(DisplayInvOfCharID2 - 1);
        FUN_0042f50c(CharInfoCharacter, 0);
    }
}

void Engine::SetCamPos(Common::Point pos)
{
    _camera = pos - _gameViewport / 2;
    
    if (_camera.x < TileWh)
        _camera.x = TileWh;
    else if (_camera.x + _gameViewport.x > _camMax.x)
        _camera.x = _camMax.x - _gameViewport.x;
    
    if (_camera.y < TileH)
        _camera.y = TileH;
    else if (_camera.y + _gameViewport.y > _camMax.y)
        _camera.y = _camMax.y - _gameViewport.y;
    
    _some = CalcLootLTTile(_camera);
    _viewStartPos = CalcViewLTTile(_camera);
    
    if (_viewStartPos.y != 0)
        _viewStartPos.y -= 1;
    
    if (_viewStartPos.x != 0)
        _viewStartPos.x -= 1;
}


void Engine::ProcessCamera()
{
    Common::Point tmp = _camera;
    
    //if (_mousePos.y < _screenSize.y) 
    {
        if ((_mousePos.x <= 0) && (_camera.x > TileWh))
            tmp.x = _camera.x - SCREENSHIFT;
        else if ((_mousePos.x >= _screenSize.x - 1) && (_camera.x + _gameViewport.x < _camMax.x))
            tmp.x = _camera.x + SCREENSHIFT;
    }
    
    if ((_mousePos.y <= 0) && (_camera.y > TileH))
        tmp.y = _camera.y - SCREENSHIFT;
    else if ((_mousePos.y >= _screenSize.y - 1) && (_camera.y + _gameViewport.y < _camMax.y))
        tmp.y = _camera.y + SCREENSHIFT;
        
    if (_KeyState[KEYFN_LEFT])
        tmp.x -= SCREENSHIFT;
    if (_KeyState[KEYFN_RIGHT])
        tmp.x += SCREENSHIFT;
    if (_KeyState[KEYFN_UP])
        tmp.y -= SCREENSHIFT;
    if (_KeyState[KEYFN_DOWN])
        tmp.y += SCREENSHIFT;
    
    if (_camLockChar) 
    {
        int32_t tmpx = 0;
        
        if ( ((_camLockChar->State == CHSTATE_7) || (_camLockChar->State == CHSTATE_1)) && 
              (_camLockChar->Direction != DIR_6) && (_camLockChar->Direction != DIR_2)) 
        {
            switch(_camLockChar->Direction) 
            {
                case DIR_0:
                    tmpx = _camLockChar->POS.x - _camLockChar->Frame * 4 - _camLockChar->Frame;
                    break;
                case DIR_1:
                case DIR_7:
                    tmpx = _camLockChar->POS.x - _camLockChar->Frame * 2 - _camLockChar->Frame / 2;
                    break;
                case DIR_3:
                case DIR_5:
                    tmpx = _camLockChar->POS.x + _camLockChar->Frame * 2 + _camLockChar->Frame / 2;
                    break;
                case DIR_4:
                    tmpx = _camLockChar->POS.x + _camLockChar->Frame * 4 + _camLockChar->Frame;
                    break;
                default:
                    break;
            }
        }
        else
            tmpx = (((_camLockChar->imgSize.x / 2) + _camLockChar->ViewPos.x + 1) & ~1) - GScrOff.x;
        
        if (tmpx < tmp.x + 60)
            tmp.x = tmpx - 60;
        else if (tmpx >= tmp.x + _gameViewport.x - 60)
            tmp.x = (tmpx - _gameViewport.x) + 60;
        
        int32_t tmpy = (((_camLockChar->imgSize.y / 2) + _camLockChar->ViewPos.y + 1) & ~1) - GScrOff.y;
        if (tmpy < tmp.y + 50)
            tmp.y = tmpy - 50;
        else if (tmpy >= tmp.y + _gameViewport.y - 50)
            tmp.y = (tmpy - _gameViewport.y) + 50;
    }
    
    if (tmp != _camera) 
    {
        _camera = tmp;
        
        if (_camera.x < TileWh)
            _camera.x = TileWh;
        else if (_camMax.x < _camera.x + _gameViewport.x)
            _camera.x = _camMax.x - _gameViewport.x;
        
        if (_camera.y < TileH)
            _camera.y = TileH;
        else if (_camMax.y < _camera.y + _gameViewport.y)
            _camera.y = _camMax.y - _gameViewport.y;
        
        //printf("%d %d %d %d\n", _camMax.y, _gameViewport.y, _camera.y, _camera.y + _gameViewport.y);
        
        _some = CalcLootLTTile(_camera);
        _viewStartPos = CalcViewLTTile(_camera);
        
        if (_viewStartPos.y != 0)
            _viewStartPos.y--;
        
        if (_viewStartPos.x != 0)
            _viewStartPos.x--;
    }
}


Common::Point Engine::CalcLootLTTile(Common::Point pos)
{
    Common::Point out;
    Common::Point cpos;
    
    out.y = pos.y / TileHh;
    if ((out.y & 1) == 0) 
    {
        out.x = (pos.x - TileWhh) / TileWh;
        cpos.x = out.x * TileWh + TileWh;
    }
    else 
    {
        out.x = pos.x / TileWh;
        cpos.x = out.x * TileWh + TileWhh;
    }
    
    cpos.y = (out.y + 1) * TileHh;
    
    if (pos.x == cpos.x) 
    {}
    else if (pos.x < cpos.x) 
    {
        if (((pos.y - (cpos.y - TileHh)) * (pos.x - (cpos.x - TileWhh)) < (cpos.x - pos.x) * (cpos.y - pos.y)))
        {
            out.y -= 1;
            
            if ((out.y & 1) == 0)
                out.x -= 1;
        }
    }
    else if ((cpos.y - pos.y) * (cpos.x - pos.x) < (pos.x - (cpos.x + TileWhh)) * (pos.y - (cpos.y - TileHh))) 
    {
        if ((out.y & 1) == 0)
            out.x += 1;
        
        out.y -= 1;
    }
    
    if (out.y < 0)
        out.y = 0;
    else if (out.y > (TILEMAXH * 2 - 1))
        out.y = (TILEMAXH * 2 - 1);

    if (out.x < 0)
        out.x = 0;
    else if (out.x > (TILEMAXW * 2 - 1))
        out.x = (TILEMAXW * 2 - 1);
    
    return out;
}


Common::Point Engine::CalcViewLTTile(Common::Point pos)
{
    Common::Point out;
    
    out.y = pos.y / TileH;
    out.x = (pos.x - TileWh * (out.y & 1)) / TileW;
    
    Common::Point cpos;
    
    if (out.y & 1)
        cpos.x = out.x * TileW + TileW;
    else
        cpos.x = out.x * TileW + TileWh;
    
    cpos.y = (out.y + 1) * TileH;
    
    if (pos.x == cpos.x) 
    {}
    else 
    {
        if (pos.x < cpos.x) 
        {
            if ((((cpos.y - TileH) - pos.y) * ((cpos.x - TileWh) - pos.x) < (pos.x - cpos.x) * (pos.y - TileH * (out.y + 1))))
            {
                out.y -= 1;
                if (out.y & 1) 
                    out.x -= 1;
            }
        }
        else if ((cpos.y - pos.y) * (cpos.x - pos.x) < (pos.x - (cpos.x + TileWh)) * (pos.y - (cpos.y - TileH)))
        {
            if (out.y & 1)
                out.x += 1;

            out.y -= 1;
        }
    }
    
    if (out.y < 0)
        out.y = 0;
    else if (out.y > (TILEMAXH - 1))
        out.y = (TILEMAXH - 1);

    if (out.x < 0)
        out.x = 0;
    else if (out.x > (TILEMAXW - 1))
        out.x = (TILEMAXW - 1);
    
    return out;
}





void Engine::FUN_004290d8()
{
    DWORD_00a3e74c &= 0xfe;
    
    Character *chr = nullptr;
    
    if (_playScreenID == PLSCREEN_3)
        chr = CharInfoCharacter;
    else
    {
        chr = SelectedCharacters[0];
        if (!SelectedCharacters[0])
            chr = _mainCharacter;
    }
    
    if (chr->field_0x12 == ESLT_0)
        DrawElm[0] = 8;
    else if (chr->field_0x12 == ESLT_1)
        DrawElm[0] = 9;
    else
        DrawElm[0] = 16;
    
    if ((chr->field_0x3 & 4) == 0)
        DrawElm[1] = 15;
    else
        DrawElm[1] = 10;
    
    DrawElm[2] = 14;
    DrawElm[3] = 11;
    DrawElm[4] = 7;
    DrawElm[5] = 12;
    DrawElm[6] = 13;
}

void Engine::UpdateMainMenu()
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    
    ImgQueue1(_bkgImage, Common::Point(), Common::Rect(_screenSize));
            
    int32_t id = GetMouseOnScreenBox(_uiMousePos, _mainMenuBoxes);
    
    if (!_isGameStarted && (id < 2))
        id = -1;

    MainMenuDraw(id);
    
    if (((_mousePress & MOUSEB_L) != 0) && id >= 0)
    {
        PlaySound(4,0,0,0);
        switch(id) 
        {
            default:break;
            
            case MAINBTN_RESUME:
                _stateMode = STATEMD_PLAY;
                _imgQueue2Count = 0;
                LoadINTR();
                //PlayChangeScreen(PLSCREEN_0);
                FUN_004292e4();
                break;
                
            case MAINBTN_SAVE:
                _stateMode = STATEMD_MODE5;
                LoadSAVE();
                /*strcpy(&DAT_0083df5c,MapName[CurrentMapID - 1]);
                strcat(&DAT_0083df5c," ");
                time((int *)&DAT_00a53990);
                pcVar6 = (char *)FUN_004413b6((uint *)&DAT_00a53990);
                strcpy(&System::TempString,pcVar6);
                while (' ' < System::TempString) {
                    memcpy((undefined4 *)&System::TempString,(undefined4 *)&DAT_00a3e121,0x3ff);
                }
                memcpy((undefined4 *)&System::TempString,(undefined4 *)&DAT_00a3e121,0x3ff);
                while (iVar8 = strlen(&System::TempString), (&DAT_00a3e11f)[iVar8] != ':') {
                    iVar8 = strlen(&System::TempString);
                    (&DAT_00a3e11f)[iVar8] = 0;
                }
                iVar8 = strlen(&System::TempString);
                (&DAT_00a3e11f)[iVar8] = 0;
                strcat(&DAT_0083df5c,&System::TempString);*/
                _saveMenuBtnID = GetMouseOnScreenBox(_uiMousePos, _saveMenuBoxes);
                SaveLoadMenuDraw(true);
                break;
                
            case MAINBTN_LOAD:
                _stateMode = STATEMD_MODE4;
                LoadSAVE();
                _saveMenuBtnID = GetMouseOnScreenBox(_uiMousePos, _saveMenuBoxes);
                SaveLoadMenuDraw(false);
                break;
                
            case MAINBTN_NEW:
                _stateMode = STATEMD_NEWGAME;
                LoadNEWH();
                
                _mainCharacter->Fehtovanie = 100;
                _mainCharacter->Trading = 10;
                _mainCharacter->Metkost = 0;
                _mainCharacter->PlotnickoeDelo = 1;
                _mainCharacter->Medicine = 50;
                _mainCharacter->CurrentLovkost = _mainCharacter->BaseLovkost = 3;
                _mainCharacter->FreePoints = 0;
                _mainCharacter->Direction = 5;
                _mainCharacter->CurrentHarizm = _mainCharacter->BaseHarizm = 3;
                _mainCharacter->Identification = 20;
                _mainCharacter->NameID = 124;
                _mainCharacter->Tile = Common::Point();
                _mainCharacter->Level = 10;
                _mainCharacter->field_0xce = 0;
                _mainCharacter->CharacterBase = 1;
                _mainCharacter->paletteOffset = 33;
                _mainCharacter->KuznechnoeDelo = 10;
                _mainCharacter->field_0x3 = 4;
                _mainCharacter->CurrentSila = _mainCharacter->BaseSila = 15;
                _mainCharacter->ClassID = CLASS_VOIN;
                _mainCharacter->CurrentVinoslivost = _mainCharacter->BaseVinoslivost = 10;
                _mainCharacter->field_0x12 = ESLT_0;
                
                _mainCharacter->ArmorWeapons[ESLT_0] = 3;
                _mainCharacter->ArmorWeapons[ESLT_1] = 4;
                _mainCharacter->ArmorWeapons[ESLT_2] = 0;
                _mainCharacter->ArmorWeapons[ESLT_3] = 1;
                _mainCharacter->ArmorWeapons[ESLT_4] = 8;
                _mainCharacter->ArmorWeapons[ESLT_5] = 11;
                
                FUN_004143dc(_mainCharacter, CHSTATE_0);
                
                _state.Items[1].TypeID = 3;
                _state.Items[1].InfoID = 36;
                
                _state.Items[2].TypeID = 3;
                _state.Items[2].InfoID = 34;
                
                _state.Items[3].TypeID = 0;
                _state.Items[3].InfoID = 3;
                
                _state.Items[4].TypeID = 1;
                _state.Items[4].InfoID = 6;
                
                _state.Items[5].TypeID = 2;
                _state.Items[5].InfoID = 0;
                
                _state.Items[6].TypeID = 4;
                _state.Items[6].InfoID = 52;
                
                _state.Items[7].TypeID = 4;
                _state.Items[7].InfoID = 43;
                
                _state.Items[8].TypeID = 4;
                _state.Items[8].InfoID = 55;
                
                _state.Items[9].TypeID = 5;
                _state.Items[9].InfoID = 37;
                
                _state.Items[10].TypeID = 5;
                _state.Items[10].InfoID = 38;
                
                _state.Items[11].TypeID = 5;
                _state.Items[11].InfoID = 61;
                
                DrawNewGameMenu(0);
                break;
                
            case MAINBTN_SETTINGS:
                _stateMode = STATEMD_SETTINGS;
                
                _svTransparency = _bConfTransparency;
                //_svScreenMode = ScreenMode;
                _svDayNight = _bConfDayNight;
                _svLightEffects = _bConfLightEffects;
                _svMusic = _bConfMusic;
                _svShadows = _bConfShadows;
                _svSound = _bConfSound;
                
                DrawSettingsScreen(0);
                break;
                
            case MAINBTN_EXIT:
                //System::PostQuitMessage();
                //System::ShutdownMsg(&DAT_0045066d,0);
                break;
        }
    }
}

void Engine::Update7()
{
    _stateMode = STATEMD_MAINMENU;
    _mouseMove = Common::Point();
    
    ResetMouseItemHold();
    
    int32_t boxid = GetMouseOnScreenBox(_uiMousePos, _mainMenuBoxes);
    
    if (!_isGameStarted && (boxid < 2))
        boxid = -1;
    
    if (_nextStateMode == STATEMD_SETTINGS)
        ImgQueue1(_bkgImage, Common::Point(), Common::Rect(_screenSize));
    else
        LoadMainMenuImages();
    
    MainMenuDraw(boxid);
    
    PlayMusicId(System::rand() % Resources::MUSIC_NUM);
}

void Engine::Update8()
{
    CharInfoCharacter = nullptr;
    _KeyState[KEYFN_SHIFT] = 0;
    _KeyState[KEYFN_CTRL] = 0;
    _playScreenID = 0;
    _ambientColor = 0;
    
    DAT_00a3e704 = 0;
    DisplayInvOfCharID2 = _mainCharacter->Index + 1;
    DisplayInvOfCharID = _mainCharacter->Index + 1;
    HardComputeUnit = nullptr;
    MsgTextTimeout = 0;
    
    _counter = 0;
    
    DAT_00a3e0a8.fill(0);
    SelectedCharacters.fill(nullptr);
    
    SelectedCharacters[0] = _mainCharacter;
    
    CharTmp = *_mainCharacter;
   
    //Res.LoadFlames();
    LoadGameState(CharTmp.CharacterBase);
    LoadINTR();
    
    _mainCharacter->Fehtovanie = CharTmp.Fehtovanie;
    _mainCharacter->Trading = CharTmp.Trading;
    _mainCharacter->Metkost = CharTmp.Metkost;
    _mainCharacter->PlotnickoeDelo = CharTmp.PlotnickoeDelo;
    _mainCharacter->Medicine = CharTmp.Medicine;
    _mainCharacter->CurrentLovkost = _mainCharacter->BaseLovkost = CharTmp.BaseLovkost;
    _mainCharacter->FreePoints = CharTmp.FreePoints;
    _mainCharacter->Direction = 5;
    _mainCharacter->CurrentHarizm = _mainCharacter->BaseHarizm = CharTmp.BaseHarizm;
    _mainCharacter->Identification = CharTmp.Identification;
    _mainCharacter->NameID = CharTmp.NameID;
    _mainCharacter->Level = 1;
    _mainCharacter->CharacterBase = CharTmp.CharacterBase;
    _mainCharacter->paletteOffset = CharTmp.paletteOffset;
    _mainCharacter->KuznechnoeDelo = CharTmp.KuznechnoeDelo;
    _mainCharacter->CurrentSila = _mainCharacter->BaseSila = CharTmp.BaseSila;
    _mainCharacter->ClassID = CharTmp.ClassID;
    _mainCharacter->CurrentVinoslivost = _mainCharacter->BaseVinoslivost = CharTmp.BaseVinoslivost;
    
    _mainMapChar->MapID = _mainCharacter->CharacterBase * 3 + 1;
    bool loaded = LoadMap(_mainMapChar->MapID, 0);//dMap(0);
    if (loaded == 0)
    {
        //ShutdownMsg("Map not found!",0x10);
        printf("Map not found!\n");
        System::PostQuitMessage();
    }
    else 
    {
        LoadUsedObjects();

        SetCamPos(_mainCharacter->ViewPos);
        
        _stateMode = STATEMD_PLAY;
        
        FUN_004290d8();
        //PlayChangeScreen(PLSCREEN_0);
        FUN_004292e4();
    }
}

void Engine::Update9()
{
    _isGameStarted = false;
    _nextStateMode = STATEMD_MODE7;
    StartPlayMovie("POSTER.FLM");
}

void Engine::OnMovieEnd()
{
    if (!_isPlayingVideo)
    {
        //(*(code *)FreeAviWnd)(&DAT_00a4e428);
        printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    }
        
    _stateMode = _nextStateMode;
    _isPlayingVideo = false;
    
    SetMusicOn(_bConfMusic);
    
    if (_stateMode == STATEMD_PLAY) 
    {
        //PlayChangeScreen(PLSCREEN_0);
        FUN_004292e4();
    }
}

void Engine::ResetMouseItemHold()
{
    if (CursorAssume != 4)
    {
        CursorAssume = 4;
        
        if (InfTyp == 6)
            InfPchar->ArmorWeapons = InfSvArmrWpn;
        else if (InfTyp == 5)
            InfPchar->Accessories = InfSvAccess;
        else if (InfTyp == 32)
        {
            for(int32_t i = InfSvInv.size() - 1; i >= 0; --i)
            {
                if (i == 0 || InfSvInv[i])
                {
                    InfSvInv[i] = 0;
                    
                    for(int32_t j = 0; j < InfSvInv.size() - 1; ++j)
                        InfPchar->Inventory[j + 1] = InfSvInv[j];
                    
                    InfPchar->Inventory[0] = InfItemID;
                    break;
                }
            }
        }
        else
            InfPchar->Arrows = InfItemID;
        
//        if ((param_1 != 0) && (Game::_stateMode == 0)) {
//            if (Game::_playScreenID == 3) {
//                Game::PlayChangeScreen(2);
//            }
//            if (Game::DisplayInvOfCharID != 0) {
//                FUN_0042f50c(_state.Characters + Game::DisplayInvOfCharID + -1,0);
//            }
//        }
    }
}

bool Engine::FUN_0041e96c()
{
    if (CursorAssume == 4)
    {
        if (DAT_00a3e7a0 == 0)
            return false;
        
        FUN_00425104(&_state.Items[DAT_00a3e7a0]);
        
        if (CharInfoCharacter->Arrows == DAT_00a3e7a0)
            FUN_0041e210(CharInfoCharacter, 1, DAT_00a3e7a0);
        else
        {
            bool fnext = true;
            for(int32_t i = 0; i < CharInfoCharacter->ArmorWeapons.size(); ++i)
            {
                if (CharInfoCharacter->ArmorWeapons[i] == DAT_00a3e7a0)
                {
                    FUN_0041e210(CharInfoCharacter, 6, DAT_00a3e7a0);
                    fnext = false;
                    break;
                }
            }
            
            if (fnext)
            {
                for(int32_t i = 0; i < CharInfoCharacter->Accessories.size(); ++i)
                {
                    if (CharInfoCharacter->Accessories[i] == DAT_00a3e7a0)
                    {
                        FUN_0041e210(CharInfoCharacter, 5, DAT_00a3e7a0);
                        fnext = false;
                        break;
                    }
                }
            }
            
            if (fnext)
                FUN_0041e210(CharInfoCharacter, 32, DAT_00a3e7a0);
        }
        
        DAT_00a3e7a0 = 0;
    }
    else
    {
        CursorAssume = 4;
        
        if (InfTyp == 6)
            InfPchar->ArmorWeapons = InfSvArmrWpn;
        else if (InfTyp == 5)
            InfPchar->Accessories = InfSvAccess;
        else if (InfTyp == 32)
            InfPchar->Inventory = InfSvInv;
        else
            InfPchar->Arrows = InfItemID;
        
        DAT_00a3e7a0 = InfItemID;
    }
    
    PlayChangeScreen(PLSCREEN_3);
    FUN_0042f50c(CharInfoCharacter, 0);
    PlaySound(4, 0, 0, 0);
    return true;
}

int32_t Engine::GetMouseOnScreenBox(Common::Point point, const std::vector<Common::Rect> &boxes)
{
    for(uint32_t i = 0; i < boxes.size(); ++i)
    {
        if (boxes.at(i).IsIn(point))
            return i;
    }
    return -1;
}

void Engine::MainMenuDraw(int32_t highlight)
{
    int32_t id = (_isGameStarted ? 0 : 2);
    
    for (; id < 6; ++id)
    {
        if (id == highlight)
            ImgQueue2(_menuImages.at(3 + id * 2), _mainMenuBoxes.at(id).Pos(), Common::Rect(_screenSize));
        else
            ImgQueue2(_menuImages.at(2 + id * 2), _mainMenuBoxes.at(id).Pos(), Common::Rect(_screenSize));
    }
}

void Engine::ImgQueue1(GFX::Image *img, Common::Point pos, Common::Rect limits, vec3f fade)
{
    if (_imgQueue1Count < _imgQueue1.size())
    {
        ImagePlace &place = _imgQueue1.at(_imgQueue1Count);
        place.Img = img;
        place.PalImg = nullptr;
        place.DrawPlace = pos;
        place.Limits = limits;
        place.Pal = -1;
        place.Fade = fade / 100.0;
        
        _imgQueue1Count++;
    }
}

void Engine::ImgQueue2(GFX::Image *img, Common::Point pos, Common::Rect limits, vec3f fade)
{
    if (_imgQueue2Count < _imgQueue2.size())
    {
        ImagePlace &place = _imgQueue2.at(_imgQueue2Count);
        place.Img = img;
        place.PalImg = nullptr;
        place.DrawPlace = pos;
        place.Limits = limits;
        place.Pal = -1;
        place.Fade = fade / 100.0;
        
        _imgQueue2Count++;
    }
}

void Engine::ImgQueue1(GFX::PalImage *img, int32_t Pal, Common::Point pos, Common::Rect limits, vec3f fade)
{
    if (_imgQueue1Count < _imgQueue1.size())
    {
        ImagePlace &place = _imgQueue1.at(_imgQueue1Count);
        place.Img = nullptr;
        place.PalImg = img;
        place.DrawPlace = pos;
        place.Limits = limits;
        place.Pal = Pal;
        place.Fade = fade / 100.0;
        
        _imgQueue1Count++;
    }
}

void Engine::ImgQueue2(GFX::PalImage *img, int32_t Pal, Common::Point pos, Common::Rect limits, vec3f fade)
{
    if (_imgQueue2Count < _imgQueue2.size())
    {
        ImagePlace &place = _imgQueue2.at(_imgQueue2Count);
        place.Img = nullptr;
        place.PalImg = img;
        place.DrawPlace = pos;
        place.Limits = limits;
        place.Pal = Pal;
        place.Fade = fade / 100.0;
        
        _imgQueue2Count++;
    }
}

void Engine::TextQueue(const std::string &text, GFX::Font *font, Common::Point pos, Common::Rect limits)
{
    if (_textQueueCount < _textQueue.size())
    {
        TextPlace &place = _textQueue.at(_textQueueCount);
        place.Text = text;
        place.Font = font;
        place.DrawPlace = pos;
        place.Limits = limits;
        
        _textQueueCount++;
    }
}

int32_t Engine::PlaceTextWidth(const std::string &text, GFX::Font *font, Common::Point pos, int32_t width)
{
    std::string txt = text;
    std::string txtline;
    int32_t xpos = 0;
    while (true)
    {
        std::string token = GetStrToken(txt);
        if (token.empty())
            break;
        
        int32_t w = font->StringWidth(token);
        if (xpos + w >= width)
        {
            TextQueue(txtline, font, pos, Common::Rect());
            xpos = 0;
            pos.y += TXTHEIGHT;
            txtline.clear();
        }
        
        txt = txt.substr(token.size());
        txtline += token;
        xpos += w;
    }
    
    if (!txtline.empty())
    {
        TextQueue(txtline, font, pos, Common::Rect());
        pos.y += TXTHEIGHT;
    }
    return pos.y;
}

std::string Engine::GetStrToken(const std::string &text)
{
    int32_t chrlen = 0;
    for(int32_t i = 0; i < text.size(); i += chrlen)
    {
        if (text.at(i) == ' ')
        {
            if (i == 0)
                return std::string(" ");
            
            return text.substr(0, i);
        }
        
        chrlen = GFX::Font::Utf8CharLen(text.at(i));
    }
    
    return text;
}


void Engine::FUN_004143dc(Character *ch, int state)
{
    ch->POS = FUN_00439ba0(ch->Tile);
        
    if (!(ch->ClassID & CLASS_BIT40)) 
    {
        int32_t soundId = 0;
        switch(state) 
        {
            case CHSTATE_0:
            case CHSTATE_6:
                if (System::rand() % 100 >= 96)
                    soundId = ch->GetSoundId(3);
                break;
                
            case CHSTATE_1:
            case CHSTATE_7:
                if (ch == _mainCharacter)
                    soundId = 7;
                break;
                
            case CHSTATE_2:
            case CHSTATE_8:
                soundId = ch->GetSoundId(4);
                break;
                
            case CHSTATE_3:
            case CHSTATE_9:
                soundId = ch->GetSoundId(2);
                break;
                
            case CHSTATE_4:
                soundId = 3;
                break;
                
            case CHSTATE_5:
                if (ch->field_0x12 == ESLT_2)
                    soundId = 8;
                else
                    soundId = 2;
                break;
                
            default:
                break;
        }
        
        if (soundId != 0) 
            PlaySound(soundId, ComputeVolume(ch->Tile), ComputePan(ch->Tile), 0);
        
        Resources::CharacterSprites &chbase = Res.CharacterBases.at(ch->CharacterBase);
        auto &seqData = chbase.Seq[state][ch->Direction];
        
        ch->State = state;
        ch->Frame = 0;
        ch->pFrame = seqData.FrameData[0].FrameID;
        ch->FrameCount = seqData.FrmCount;
        ch->imgOffset = seqData.FrameData[0].ImgOffset;
        ch->shdOffset = seqData.FrameData[0].ShdOffset;
        ch->wpnOffset = seqData.FrameData[0].WpnOffset;
        ch->imgSize = chbase.Images.at(ch->pFrame)->SW.Size();
    }
    else
    {
        int32_t soundId = 0;
        switch(state) 
        {
            case CHSTATE_0:
                if (System::rand() % 100 >= 96)
                    soundId = ch->GetSoundId(3);
                break;
                
            case CHSTATE_1:
                if (System::rand() % 100 >= 76)
                    soundId = ch->GetSoundId(3);
                break;
                
            case CHSTATE_2:
                soundId = ch->GetSoundId(4);
                break;
                
            case CHSTATE_3:
                soundId = ch->GetSoundId(2);
                break;
                
            default:
                break;
                
            case CHSTATE_5:
                soundId = ch->GetSoundId(0);
                break;
        }

        if (soundId != 0)
            PlaySound(soundId, ComputeVolume(ch->Tile), ComputePan(ch->Tile), 0);
            
        ch->State = state;
        ch->Frame = 0;
        
        if (state > CHSTATE_5) 
            state -= 6;
        
        Resources::DynamicObject &obj = Res.DynObjects.at(ch->CharacterBase);
        auto &seqData = obj.Seq[state][ch->Direction];
        
        ch->pFrame = seqData.FrameData[0].FrameID;
        ch->FrameCount = seqData.FrmCount;
        ch->imgOffset = seqData.FrameData[0].ImgOffset;
        ch->shdOffset = seqData.FrameData[0].ShdOffset;
        ch->wpnOffset = Common::Point();
        
        if ((ch->field_0x3 & 0x80) == 0) 
            ch->imgSize = obj.Images.at(ch->pFrame)->SW.Size();
        else
            ch->imgSize = Common::Point(1, 1);
    }
    
    ch->RecalcViewPos(GScrOff);
    
    if (((state == CHSTATE_1) || (state == CHSTATE_7)) && (ch->field17_0x13[0] != 0xff)) 
    {
        Common::Point posCor;
        FUN_0043ecba(&posCor, ch->Tile, ch->field17_0x13[0]);
        
        if (_currentMap && _currentMap->FootMap(posCor).ID == 0)
        {
            _currentMap->FootMap(ch->Tile).ID = 0;
            
            ch->Tile = posCor;
            
            _currentMap->FootMap(ch->Tile).ID = ch->Index + 1;
        }
        else 
        {
            int32_t chrID = FUN_00411758(ch, posCor);
            if (chrID == 0) 
            {
                ch->field2_0x2 |= 0x40;
                ch->field17_0x13[0] = 0xff;
            }
            else 
            {
                ch->EnemyCharID = chrID;
                ch->MoveTile = posCor;
                ch->field17_0x13[0] = 0xff;
                ch->field_0x3 |= 4;
                FUN_004143dc(ch, CHSTATE_5);
            }
        }
    }
}


bool Engine::FUN_0043ecba(Common::Point *out, Common::Point tilepos, int offset)
{    
    static const Common::Point d[8][2] = 
    { 
        {{-1, 0},   {-1,  0}},
        {{-1,-1},   {0,  -1}},
        {{0, -2},   {0,  -2}},
        {{0, -1},   {1,  -1}},
        {{1,  0},   {1,   0}},
        {{0,  1},   {1,   1}},
        {{0,  2},   {0,   2}},
        {{-1, 1},   {0,   1}} 
    };
    
    if ((tilepos.y & 1) == 0)
        tilepos += d[offset][1];
    else
        tilepos += d[offset][0];
    
    if ((tilepos.y <= -1) || (tilepos.y >= 320))
        return false;
    
    out->y = tilepos.y;
    
    if ((tilepos.x <= -1) || (tilepos.x >= 160))
        return false;
    
    out->x = tilepos.x;
    return true;
}

int32_t Engine::FUN_00411758(Character *ch1, Common::Point tilepos)
{
    if ((ch1->field2_0x2 & 7) == 0)
        return 0;
    
    auto ch2Id = _currentMap->FootMap(tilepos).ID;
    if (ch2Id == 0)
        return 0;
    
    //ch2Id -= 1;
    Character &ch2 = _state.Characters.at(ch2Id - 1);
    
    if ((ch2.ClassID & CLASS_BIT80) || (ch2.State == CHSTATE_9) || (ch2.State == CHSTATE_3)) 
        return 0;

    if ((ch1->EnemyCharID != ch2Id) || (ch2.MapCharID == ch1->MapCharID)) 
        return 0;

    return ch2Id;
}

void Engine::DrawSettingsScreen(int32_t)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
}


Engine::ItemInfo *Engine::AllocItem()
{
    for(int32_t i = 1; i < _state.Items.size(); ++i)
    {
        ItemInfo &inf = _state.Items[i];
        if (inf.TypeID < 0 )
        {
            inf.Flags = 0;
            inf.unk1 = 0;
            inf.SpecialID = -1;
            inf.BonusID = -1;
            inf.Poison = 0;
            inf.Index = i;
            return &inf;
        }
    }
    
    return nullptr;
}


bool Engine::CheckTraceImage(GFX::Image *img, Common::Point drawPos)
{
    if (_tracePos == Common::Point(-1, -1))
        return false;
    
    Common::Point imgPos = _tracePos - drawPos;
    
    if (imgPos.x < 0 || imgPos.y < 0 ||
        imgPos.x >= img->SW->w || imgPos.y >= img->SW->h)
        return false;
    
    SDL_LockSurface(img->SW);
    void *ptr = (uint8_t *)img->SW->pixels + imgPos.y * img->SW->pitch + imgPos.x * img->SW->format->BytesPerPixel;
    uint32_t px = 0;
    switch(img->SW->format->BytesPerPixel)
    {
        case 1:
        default:
            px = *(uint8_t *)ptr;
            break;
        
        case 2:
            px = *(uint16_t *)ptr;
            break;
            
        case 4:
            px = *(uint32_t *)ptr;
            break;  
    }
    
    uint8_t r = 0, g = 0, b = 0, a = 0;
    SDL_GetRGBA(px, img->SW->format, &r, &g, &b, &a);
    
    SDL_UnlockSurface(img->SW);
    
    return a != 0;
}

bool Engine::CheckTraceImage(GFX::PalImage *img, Common::Point drawPos)
{
    if (_tracePos == Common::Point(-1, -1))
        return false;
    
    Common::Point imgPos = _tracePos - drawPos;
    
    if ( ! Common::Rect(img->SW.Size()).IsIn(imgPos))
        return false;
    
    return img->SW.At(imgPos) != 0;
}

void Engine::UpdateCursor()
{
    int32_t newCursor = CursorAssume;

    if ( _stateMode == STATEMD_PLAY &&
        !_isPlayingVideo && 
         _playScreenID == PLSCREEN_0 &&
         CursorAssume == 4 &&
         MAPRECT.IsInIncl(_uiMousePos) )
    {
        if (!DisplayInvOfCharID == 0 || _uiMousePos.y < DAT_00a3e88c - 1)
        {
            if (MouseOnObject != nullptr && 
                CurrentVillage != nullptr &&
                (MouseOnObject->ObjectFlags & 1) != 0)
            {
                for (Character *chr : SelectedCharacters)
                {
                    if (!chr)
                        break;
                    
                    if ((chr->ClassID & CLASS_BIT80) == 0 && chr->State != CHSTATE_9 && chr->State != CHSTATE_3)
                    {
                        if ((chr->field_0x3 & 4) != 0 && 
                             chr->field_0x12 == ESLT_1 &&
                             chr->ArmorWeapons[ESLT_1] != 0 && 
                             chr->Arrows != 0 &&
                             _state.Items.at(chr->Arrows).SpecialID == 0)
                        {
                            newCursor = 6;
                            break;
                        }
                    }
                }
            }
            
            if (newCursor == CursorAssume)
            {
                if (!MouseOnCharacter && _currentMap)
                {
                    Common::Point pnt = CalcLootLTTile(_mouseMapPos + _camera);
                    GameMap::Cell &cell = _currentMap->FootMap.At(pnt);
                    if ( (cell.Flags & 2) == 0 || cell.ID ||
                         GetLootByTile(pnt)->unk2 < 0 )
                    {
                        if (cell.ID == 0)
                            newCursor = CursorAssume;
                        else
                            newCursor = 3;
                    }
                    else
                        newCursor = 7;
                }
                else if (MouseOnCharacter->MapCharID == _mainCharacter->MapCharID)
                {
                    if (_KeyState[KEYFN_CTRL] == 0)
                        newCursor = CursorAssume;
                    else
                        newCursor = 5;
                }
                else
                {
                    for (Character *chr : SelectedCharacters)
                    {
                        if (!chr)
                            break;

                        if ((chr->ClassID & CLASS_BIT80) == 0 && chr->State != CHSTATE_9 && chr->State != CHSTATE_3)
                        {
                            if ((chr->field_0x3 & 4) != 0)
                            {
                                newCursor = 1;
                                break;
                            }
                        }
                    }
                    
                    if (newCursor == CursorAssume)
                    {
                        if ( MouseOnCharacter->field96_0xd0 == -1 || 
                             _KeyState[KEYFN_SHIFT] || 
                            (_mainCharacter->field_0x3 & 4) != 0 )
                            newCursor = 3;
                        else
                            newCursor = 5;
                    }
                }
            }
        }
        else
            newCursor = CursorAssume;
    }
    else
        newCursor = CursorAssume;
    
    if (newCursor != _curCursor)
    {
        _curCursor = newCursor;
        SDL_SetCursor(_cursors.at(newCursor));
    }
}


}