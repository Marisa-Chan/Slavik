#include <regex>
#include <GL/gl.h>
#include <thread>
#include <chrono>

#include "game.h"
#include "system.h"
#include "resources.h"
#include "fmt/core.h"
#include "fmt/printf.h"

namespace Game
{
 
Engine Engine::Instance;
    
bool Engine::Process()
{
    uint32_t start = SDL_GetTicks();
    if (Update())
        return true;
    
    Draw();
    
    uint32_t frmTime = start - SDL_GetTicks();
    SDL_Delay((FrameTime - frmTime) - (1000 / FrameTime) / 5);
    
    return false;
}

bool Engine::Update()
{
    
    if (_KeyState[KEYFN_MAPLEFT])
        _camera.x -= 10;
    if (_KeyState[KEYFN_MAPRIGHT])
        _camera.x += 10;
    if (_KeyState[KEYFN_MAPUP])
        _camera.y -= 10;
    if (_KeyState[KEYFN_MAPDOWN])
        _camera.y += 10;
    
    
    
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
    }
    
    _mousePos = _mouseCurPos;
    
    
    
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    
    switch(_stateMode)
    {
        case STATEMD_PLAY:
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
            break;
            
        case STATEMD_MODE9:
            Update9();
            break;
            
        default:
            break;
    }

    return false;
}

void Engine::Init(int gfxmode)
{
    _KeyMap.clear();
    _KeyMap[SDL_SCANCODE_LEFT] = KEYFN_MAPLEFT;
    _KeyMap[SDL_SCANCODE_RIGHT] = KEYFN_MAPRIGHT;
    _KeyMap[SDL_SCANCODE_UP] = KEYFN_MAPUP;
    _KeyMap[SDL_SCANCODE_DOWN] = KEYFN_MAPDOWN;
    
    _KeyState.fill(0);
    
    System::EventsAddHandler(EventsWatcher);
    
    Res.Load();
    
    GFXDrawer.SetPalettes(Res.Palettes);
    
    for(int32_t i = 0; i < 4; ++i)
    {
        if ( (_Fonts.at(i) = GFXDrawer.LoadFont(fmt::sprintf("res/font%d.res", i))) == nullptr )
        {
            fmt::printf("Can't load font res/font%d.res\n", i);
            System::PostQuitMessage();
        }
    }                
    
    _screenSize = Common::Point(1024, 768);
   
    _langLiter = "R";
    
    _stateMode = STATEMD_MODE9;
    
    _mainCharacter = & _state.Characters.at(_state.MapChar_ARRAY[0].CharacterIndex);
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
                Instance._KeyState[it->second] = 1;
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
            }
        }
        break;

        case SDL_MOUSEMOTION:
        {
            Instance._mouseCurPos.x = event->motion.x;
            Instance._mouseCurPos.y = event->motion.y;
        }
        break;
    }
    
    return 1; // This event can be passed to another event watcher
}

static bool incr = false;

void Engine::Draw()
{
    GFXDrawer.Begin();
    
    GFXDrawer.SetOutBuffer(GFX::BUF_OFF);
    GFXDrawer.Clear();
    
    if (_stateMode == STATEMD_PLAY)
    {
        GFXDrawer.SetVirtResolution();
    mat4x4f Translate;
    Translate.setTranslate(vec3f(-_camera.x, -_camera.y, 0));
    GFXDrawer.SetModelViewMatrix(Translate);

    GFXDrawer.SetOutBuffer(GFX::BUF_2);
    GFXDrawer.Clear();
    GFXDrawer.SetOutBuffer(GFX::BUF_1);
    GFXDrawer.Clear();
    GFXDrawer.SetOutBuffer(GFX::BUF_0);
    GFXDrawer.Clear();
    
    _fadeLevelRGB = CalculateLight() / 100.0;
    
    _counter++;
    GFXDrawer.SetFade(true, _fadeLevelRGB);
    //printf("%d %d \n", winSize.x, winSize.y);

    DrawMap();
    
    GFXDrawer.SetFade(false);
    
    static int k = 0, j = 0, i = 0;
                
    Common::Point pnt(700, 700);
    int32_t si = Res.CharacterBases[0].Seq[k][j].FrameData[i].FrameID;
    /*GFXDrawer.SetOutBuffer(1);
    GFXDrawer.Clear();
    GFXDrawer.DrawShadow( Res.CharacterBases[0].Shadows[si], pnt);
    GFXDrawer.SetOutBuffer(0);
    GFXDrawer.DrawOutBuffer(1);
    mat4x4f Translate;
    Translate.setTranslate(vec3f(-_camera.x, -_camera.y, 0));
    GFXDrawer.SetModelViewMatrix(Translate);*/
    GFXDrawer.SetOutBuffer(GFX::BUF_1);
    GFXDrawer.DrawShadow( Res.CharacterBases[0].Shadows[si], pnt + Res.CharacterBases[0].Seq[k][j].FrameData[i].ShdOffset);
    
    GFXDrawer.SetOutBuffer(GFX::BUF_2);

    GFXDrawer.Draw( Res.CharacterBases[0].Images[si], Res.CharBasePal[0], pnt + Res.CharacterBases[0].Seq[k][j].FrameData[i].ImgOffset);

    pnt += Res.CharacterBases[0].Seq[k][j].FrameData[i].WpnOffset;
    si = Res.CharacterEquip[4].Seq[k][j].FrameData[i].FrameID;
    
    GFXDrawer.SetOutBuffer(GFX::BUF_1);
    GFXDrawer.DrawShadow( Res.CharacterEquip[4].Shadows[si], pnt + Res.CharacterEquip[4].Seq[k][j].FrameData[i].ShdOffset);
    
    GFXDrawer.SetOutBuffer(GFX::BUF_2);
    GFXDrawer.Draw( Res.CharacterEquip[4].Images[si], Res.CharEquipPal[4], pnt + Res.CharacterEquip[4].Seq[k][j].FrameData[i].ImgOffset);
    
    i++;
    if (i == Res.CharacterBases[0].Seq[k][j].FrmCount)
    {
        i = 0;
        j++;
        if (j == 8)
        {
            j = 0;
            k++;
            if (k == 10)
            {
                k = 0;
                
            }
        }
    }
    
    pnt.x = 200;
    pnt.y = 200;
    
    static int k2 = 0, j2 = 0, i2 = 0;
    static int aa = 0;
    
    i2++;
    if (i2 == Res.DynObjects[14].Seq[k2][j2].FrmCount)
    {
        i2 = 0;
        j2++;
        if (j2 == 8)
        {
            j2 = 0;
            k2++;
            if (k2 == 6)
            {
                k2 = 0;
                
            }
        }
    }
    
    aa = (aa + 1) & 0xFF;
    
    si = Res.DynObjects[14].Seq[k2][j2].FrameData[i2].FrameID;
    if (si > 0)
    GFXDrawer.Draw( Res.DynObjects[14].Images[si], aa, pnt);
    
    for(auto &obj: _state.MapChar_ARRAY)
    {
        if ( obj.MapID == _currentMapID )
        {
            Character &ch = _state.Characters.at(obj.CharacterIndex);
            if (ch.ClassID & 0x40)
            {
                auto &dynobj = Res.DynObjects[ch.CharacterBase];
                si = dynobj.Seq[ch.State][ch.Direction].FrameData[0].FrameID;
                pnt.x = obj.field_0x14 * TileWh + (2 - (obj.field_0xc & 1)) * TileWhh;
                pnt.y = obj.field_0xc * TileHh + TileHh;
                GFXDrawer.Draw(dynobj.Images[si], ch.paletteOffset, pnt);
            }
        }
    }
    
    GFXDrawer.SetOutBuffer(GFX::BUF_OFF);
    
    GFXDrawer.SetProjectionMatrix( mat4x4f() );
    GFXDrawer.SetModelViewMatrix( mat4x4f() );
    
    GFXDrawer.DrawRect(GFXDrawer.GetBufferTex(GFX::BUF_0), Common::FRect(1.0, 1.0) );
    GFXDrawer.DrawRect(GFXDrawer.GetBufferTex(GFX::BUF_1), Common::FRect(1.0, 1.0), 0.5);
    GFXDrawer.DrawRect(GFXDrawer.GetBufferTex(GFX::BUF_2), Common::FRect(1.0, 1.0) );
    }
    //else
    {
        GFXDrawer.SetOutBuffer(GFX::BUF_OFF);
        GFXDrawer.SetVirtResolution(Common::Point(SCREENRESX, SCREENRESY));
        GFXDrawer.SetModelViewMatrix( mat4x4f() );
        
        for(int32_t i = 0; i < _imgQueue1Count; ++i)
        {
            ImagePlace &im = _imgQueue1.at(i);
            if (im.Img)
            {
                if (!im.Limits.IsEmpty())
                {
                    GFXDrawer.SetScissor(true, im.Limits);
                    GFXDrawer.Draw(im.Img, im.DrawPlace);
                    GFXDrawer.SetScissor(false);
                }
                else
                    GFXDrawer.Draw(im.Img, im.DrawPlace);
            }
        }
        
        for(int32_t i = 0; i < _imgQueue2Count; ++i)
        {
            ImagePlace &im = _imgQueue2.at(i);
            if (im.Img)
            {
                if (!im.Limits.IsEmpty())
                {
                    GFXDrawer.SetScissor(true, im.Limits);
                    GFXDrawer.Draw(im.Img, im.DrawPlace);
                    GFXDrawer.SetScissor(false);
                }
                else
                    GFXDrawer.Draw(im.Img, im.DrawPlace);
            }
        }
        
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
    }
    

    GFXDrawer.Flip();
}

void Engine::LoadMap(int32_t mapID)
{
    if (_currentMap)
        delete _currentMap;
    
    _currentMap = LoadGameMap(mapID);
    _currentMapID = mapID;
}

void Engine::DrawMap()
{
    if (_currentMap)
    {
        for(size_t y = 0; y < 160; ++y)
        {
            for(size_t x = 0; x < 80; ++x)
            {
                Common::Point pnt(x * TileW, y * TileH);
                if ( (y & 1)  )
                    pnt.x += TileWh;

                if (_currentMap->LightMap(x, y) != 0)
                    GFXDrawer.SetLightMask(Res.TilesLight[ _currentMap->LightMap(x, y) - 1 ]);
                else
                    GFXDrawer.SetLightMask(nullptr);

                if (_currentMap->TileMap[y][x][0] != 0)
                    GFXDrawer.Draw( Res.Tiles[ _currentMap->TileMap[y][x][0] - 1 ], pnt);

                if (_currentMap->TileMap[y][x][1] != 0)              
                    GFXDrawer.Draw( Res.Tiles[ _currentMap->TileMap[y][x][1] - 1 ], pnt);
            }
        }

        GFXDrawer.SetLightMask(nullptr);
        
        for(const auto &dec : _currentMap->Decorations)   
            GFXDrawer.Draw( Res.Tiles[ dec.TileID ], dec.Position);
        
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
            if (tmpReadMap(i, j) == 0xFF)
                map->FootMap(i, j) = 0xFFF;
            else if (tmpReadMap(i, j) & 0xF == 0xF)
                map->FootMap(i, j) = (tmpReadMap(i, j) << 8) | 0xFFF;
            else
                map->FootMap(i, j) = (tmpReadMap(i, j) << 8);
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
            
            map->MapObjects.emplace_back();
            GameMap::Object &obj = map->MapObjects.back();
            
            obj.ObjId = ID;
            for(size_t j = 0; j < 20; ++j)
                obj.Flames[j] = f->readU8();
            
            obj.FrameTimeLeft = f->readS16L();
            obj.unk1 = f->readS16L();
            obj.unk2 = f->readS16L();
            obj.unk3 = f->readU8();
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
    }
    else
    {
        map->LightMap.fill(0);
    }
    
    return map;
}
    

vec3f Engine::CalculateLight()
{
    _bLightGround = false;
    int32_t dayTime = _counter % DayDuration;
    
    if (dayTime == 0) 
      _bLightEff = (rand() % 100) < 90;
    
    auto mapLight = _mapsLights.find(_currentMapID);
    
    if (mapLight != _mapsLights.end())
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
            PlayChangeScreen(PLSCREEN_0);
            FUN_004292e4();
        }
        return false;
    }

    _isPlayingVideo = true;
    return true;
}

void Engine::SetMusicOn(bool en)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
}
  
void Engine::PlayChangeScreen(int32_t screen)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
}

void Engine::FUN_004292e4()
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
}

void Engine::UpdateMainMenu()
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    
    ImgQueue1(_bkgImage, Common::Point(), Common::Rect(_screenSize));
            
    int32_t id = GetMouseOnScreenBox(_mousePos, _mainMenuBoxes);
    
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
                PlayChangeScreen(-1);
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
                _saveMenuBtnID = GetMouseOnScreenBox(_mousePos, _saveMenuBoxes);
                SaveLoadMenuDraw(true);
                break;
                
            case MAINBTN_LOAD:
                _stateMode = STATEMD_MODE4;
                LoadSAVE();
                _saveMenuBtnID = GetMouseOnScreenBox(_mousePos, _saveMenuBoxes);
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
                _mainCharacter->BaseLovkost = 3;
                _mainCharacter->CurrentLovkost = _mainCharacter->BaseLovkost;
                _mainCharacter->FreePoints = 0;
                _mainCharacter->Direction = 5;
                _mainCharacter->BaseHarizm = 3;
                _mainCharacter->CurrentHarizm = _mainCharacter->BaseHarizm;
                _mainCharacter->Identification = 20;
                _mainCharacter->NameID = 124;
                _mainCharacter->tileX = 0;
                _mainCharacter->tileY = _mainCharacter->tileX;
                _mainCharacter->Level = 10;
                _mainCharacter->field_0xce = 0;
                _mainCharacter->CharacterBase = 1;
                _mainCharacter->paletteOffset = 33;
                _mainCharacter->KuznechnoeDelo = 10;
                _mainCharacter->field_0x3 = 4;
                _mainCharacter->BaseSila = 15;
                _mainCharacter->CurrentSila = _mainCharacter->BaseSila;
                _mainCharacter->ClassID = 5;
                _mainCharacter->BaseVinoslivost = 10;
                _mainCharacter->CurrentVinoslivost = _mainCharacter->BaseVinoslivost;
                _mainCharacter->field_0x12 = 0;
                
                _mainCharacter->ArmorWeapons[0] = 3;
                _mainCharacter->ArmorWeapons[1] = 4;
                _mainCharacter->ArmorWeapons[2] = 0;
                _mainCharacter->ArmorWeapons[3] = 1;
                _mainCharacter->ArmorWeapons[4] = 8;
                _mainCharacter->ArmorWeapons[5] = 11;
                
                FUN_004143dc(*_mainCharacter, 0);
                
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
    
    FUN_00429194(1);
    
    int32_t boxid = GetMouseOnScreenBox(_mouseMove, _mainMenuBoxes);
    
    if (!_isGameStarted && (boxid < 2))
        boxid = -1;
    
    if (_nextStateMode == STATEMD_SETTINGS)
        ImgQueue1(_bkgImage, Common::Point(), Common::Rect(_screenSize));
    else
        LoadMainMenuImages();
    
    MainMenuDraw(boxid);
    
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    
    /*if ((MusicResData != NULL) && (MusicResDat1_ARRAY_006324b8[0].Size == 0)) 
    {
        strcpy(&System::TempString,WrkDir);
        strcat(&System::TempString,"MUSICS.RES");
        uVar7 = open(&System::TempString,O_BINARY,in_stack_fffffea8);
        if (uVar7 != 0xffffffff) {
            read(uVar7,(undefined *)&System::RESHDR,8);
            read(uVar7,(undefined *)MusicResDat1_ARRAY_006324b8,0x48);
            read(uVar7,MusicResData,System::RESHDR.EntriesSize);
            close(uVar7);
            uVar7 = rand();
            PlayMusicId((int)uVar7 % 9);
        }
    }*/
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
        PlayChangeScreen(PLSCREEN_0);
        FUN_004292e4();
    }
}

void Engine::FUN_00429194(int32_t)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
}

int32_t Engine::GetMouseOnScreenBox(Common::Point point, const std::vector<Common::Rect> &boxes)
{
    Common::Point ppoint(point.x * SCREENRESX / _screenSize.x, point.y * SCREENRESY / _screenSize.y);
    for(uint32_t i = 0; i < boxes.size(); ++i)
    {
        if (boxes.at(i).IsIn(ppoint))
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

void Engine::ImgQueue1(GFX::Image *img, Common::Point pos, Common::Rect limits)
{
    if (_imgQueue1Count < _imgQueue1.size())
    {
        ImagePlace &place = _imgQueue1.at(_imgQueue1Count);
        place.Img = img;
        place.DrawPlace = pos;
        place.Limits = limits;
        
        _imgQueue1Count++;
    }
}

void Engine::ImgQueue2(GFX::Image *img, Common::Point pos, Common::Rect limits)
{
    if (_imgQueue2Count < _imgQueue2.size())
    {
        ImagePlace &place = _imgQueue2.at(_imgQueue2Count);
        place.Img = img;
        place.DrawPlace = pos;
        place.Limits = limits;
        
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


void Engine::FUN_004143dc(Character &ch, int state)
{
    ch.POS = FUN_00439ba0({ch.tileX, ch.tileY});
        
    if ((ch.ClassID & 0x40) == 0) 
    {
        int32_t soundId = 0;
        switch(state) 
        {
            case 0:
            case 6:
                if (rand() % 100 >= 96)
                    soundId = ch.CharacterBase * 8 + 35;
                break;
                
            case 1:
            case 7:
                if (&ch == _mainCharacter)
                    soundId = 7;
                break;
                
            case 2:
            case 8:
                soundId = ch.CharacterBase * 8 + 36;
                break;
                
            case 3:
            case 9:
                soundId = ch.CharacterBase * 8 + 34;
                break;
                
            case 4:
                soundId = 3;
                break;
                
            case 5:
                if (ch.field_0x12 == 2)
                    soundId = 8;
                else
                    soundId = 2;
                break;
                
            default:
                break;
        }
        
        if (soundId != 0) 
            PlaySound(soundId, ComputeVolume({ch.tileX,ch.tileY}), ComputePan({ch.tileX,ch.tileY}), 0);
        
        Resources::CharacterSprites &chbase = Res.CharacterBases.at(ch.CharacterBase);
        auto &seqData = chbase.Seq[state][ch.Direction];
        
        ch.State = state;
        ch.Frame = 0;
        ch.pFrame = seqData.FrameData[0].FrameID;
        ch.FrameCount = seqData.FrmCount;
        ch.imgOffset = seqData.FrameData[0].ImgOffset;
        ch.shdOffset = seqData.FrameData[0].ShdOffset;
        ch.field111_0xf4 = chbase.Images.at(ch.pFrame)->SW.Width();
        ch.field112_0xf6 = chbase.Images.at(ch.pFrame)->SW.Height();
    }
    else
    {
        int32_t soundId = 0;
        switch(state) 
        {
            case 0:
                if (rand() % 100 >= 96)
                    soundId = ch.ClassID * 8 - 461;
                break;
                
            case 1:
                if (rand() % 100 >= 76)
                    soundId = ch.ClassID * 8 - 461;
                break;
                
            case 2:
                soundId = ch.ClassID * 8 - 460;
                break;
                
            case 3:
                soundId = ch.ClassID * 8 - 462;
                break;
                
            default:
                break;
                
            case 5:
                soundId = ch.ClassID * 8 - 464;
                break;
        }

        if (soundId != 0)
            PlaySound(soundId, ComputeVolume({ch.tileX,ch.tileY}), ComputePan({ch.tileX,ch.tileY}), 0);
            
        ch.State = state;
        ch.Frame = 0;
        
        if (state > 5) 
            state -= 6;
        
        Resources::DynamicObject &obj = Res.DynObjects.at(ch.CharacterBase);
        auto &seqData = obj.Seq[state][ch.Direction];
        
        ch.pFrame = seqData.FrameData[0].FrameID;
        ch.FrameCount = seqData.FrmCount;
        ch.imgOffset = seqData.FrameData[0].ImgOffset;
        ch.shdOffset = seqData.FrameData[0].ShdOffset;
        
        if ((ch.field_0x3 & 0x80) == 0) 
        {
            ch.field111_0xf4 = obj.Images.at(ch.pFrame)->SW.Width();
            ch.field112_0xf6 = obj.Images.at(ch.pFrame)->SW.Height();
        }
        else {
            ch.field111_0xf4 = 1;
            ch.field112_0xf6 = 1;            
        }
    }
    
    ch.field107_0xe4 = ch.POS.x + GScrOff.x + ch.imgOffset.x;
    ch.field109_0xec = ch.POS.y + GScrOff.y + ch.imgOffset.y;
    
    if (((state == 1) || (state == 7)) && (ch.field17_0x13 != -1)) 
    {
        Common::Point posCor;
        FUN_0043ecba(&posCor, {ch.tileX,ch.tileY}, ch.field17_0x13);
        
        if (_currentMap && (_currentMap->FootMap(posCor) & 0xFFF) == 0)
        {
            _currentMap->FootMap(ch.tileX, ch.tileY) &= 0xF000;
            
            ch.tileY = posCor.y;
            ch.tileX = posCor.x;
            
            _currentMap->FootMap(ch.tileX, ch.tileY) |= ch.CharIndex + 1;
        }
        else 
        {
            int32_t chrID = FUN_00411758(ch, posCor);
            if (chrID == 0) 
            {
                ch.field2_0x2 |= 0x40;
                ch.field17_0x13 = 0xff;
            }
            else 
            {
                ch.field6_0x6 = chrID;
                ch._field_0xc = posCor.y;
                ch._field_0xe = posCor.x;
                ch.field17_0x13 = 0xff;
                ch.field_0x3 = ch.field_0x3 | 4;
                FUN_004143dc(ch, 5);
            }
        }
    }
}


bool Engine::FUN_0043ecba(Common::Point *out, Common::Point tilepos, int offset)
{    
    static const int d[32] = 
    { 0, -1,  0, -1,
     -1, -1, -1,  0,
     -2,  0, -2,  0,
     -1,  0, -1,  1,
      0,  1,  0,  1,
      1,  0,  1,  1,
      2,  0,  2,  0,
      1, -1,  1,  0 };
    
    int id = offset * 4;
    
    if ((tilepos.y & 1) == 0)
        id += 2;
    
    int32_t y = tilepos.y + d[id + 0];
    
    if ((y <= -1) || (y >= 320))
        return false;
    
    out->y = y;
    
    int32_t x = tilepos.x + d[id + 1];
    
    if ((x >= -1) || (x >= 160))
        return false;
    
    out->x = x;
    return true;
}

int32_t Engine::FUN_00411758(Character &ch1, Common::Point tilepos)
{
    if ((ch1.field2_0x2 & 7) == 0)
        return 0;
    
    auto ch2Id = _currentMap->FootMap(tilepos) & 0xFFF;
    if (ch2Id == 0)
        return 0;
    
    //ch2Id -= 1;
    Character &ch2 = _state.Characters.at(ch2Id - 1);
    
    if ((ch2.ClassID & 0x80) || (ch2.State == 9) || (ch2.State == 3)) 
        return 0;

    if ((ch1.field6_0x6 != ch2Id) || (ch2.field5_0x5 == ch1.field5_0x5)) 
        return 0;

    return ch2Id;
}

void Engine::DrawSettingsScreen(int32_t)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
}


}