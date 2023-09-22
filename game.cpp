#include <regex>
#include <GL/gl.h>
#include <thread>
#include <chrono>

#include "game.h"
#include "system.h"
#include "resources.h"
#include "fmt/core.h"

namespace Game
{
 
Engine Engine::Instance;
    
bool Engine::Process()
{
    
    if (_KeyState[KEYFN_MAPLEFT])
        _camera.x -= 10;
    if (_KeyState[KEYFN_MAPRIGHT])
        _camera.x += 10;
    if (_KeyState[KEYFN_MAPUP])
        _camera.y -= 10;
    if (_KeyState[KEYFN_MAPDOWN])
        _camera.y += 10;
    
    
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    
    Draw();
    
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
        }
        break;

        case SDL_MOUSEBUTTONUP:
        {
        }
        break;

        case SDL_MOUSEMOTION:
        {
        }
        break;
    }
    
    return 1; // This event can be passed to another event watcher
}

static bool incr = false;

void Engine::Draw()
{
    GFXDrawer.Begin();
    GFXDrawer.Clear();
    
    static float xx = 1.0;
    
    float yy = Common::MAX<float>(-1.0, xx);
    GFXDrawer.SetFade(true, vec3f(yy, yy, yy));

    xx += (incr ? 1 : -1) * 0.01;
    if (xx < -0.5)
    {
        xx = -0.5;
        incr = !incr;
    }
    else if (xx > 0.5)
    {
        xx = 0.5;
        incr = !incr;
    }
    
    DrawMap();
    
    GFXDrawer.SetFade(false);
    
    static int k = 0, j = 0, i = 0;
                
    Common::Point pnt;
    pnt.x = 100 + Res.CharacterBases[0].unk3[k][j][i][1].x; 
    pnt.y = 100 + Res.CharacterBases[0].unk3[k][j][i][1].y;
    int32_t si = Res.CharacterBases[0].Seq[k][j][i];
    GFXDrawer.Draw( Res.CharacterBases[0].Images[si], Res.CharBasePal[0], pnt);

    pnt.x = 100 + Res.CharacterBases[0].unk2[k][j][i].x + Res.CharacterEquip[4].unk3[k][j][i][1].x; 
    pnt.y = 100 + Res.CharacterBases[0].unk2[k][j][i].y + Res.CharacterEquip[4].unk3[k][j][i][1].y; 
    si = Res.CharacterEquip[4].Seq[k][j][i];
    GFXDrawer.Draw( Res.CharacterEquip[4].Images[si], Res.CharEquipPal[4], pnt);
    
    i++;
    if (i == Res.CharacterBases[0].Frames[k][j])
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
    if (i2 == Res.DynObjects[14].Frames[k2][j2])
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
    
    si = Res.DynObjects[14].Seq[k2][j2][i2];
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
                si = dynobj.Seq[ch.State][ch.Direction][0];
                printf("%d %d %d %d %d %d\n", &obj - _state.MapChar_ARRAY.data(), obj.CharacterIndex, obj.field_0x8, obj.field_0xc, obj.field_0x10, obj.field_0x14);
                pnt.x = obj.field_0x14 * 58 + (2 - (obj.field_0xc & 1)) * 29;
                pnt.y = obj.field_0xc * 16 + 16;
                GFXDrawer.Draw(dynobj.Images[si], ch.paletteOffset / 512, pnt);
            }
        }
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
    mat4x4f Translate;
    Translate.setTranslate(vec3f(-_camera.x, -_camera.y, 0));
    GFXDrawer.SetModelViewMatrix(Translate);
    
    if (_currentMap)
    {
        for(size_t y = 0; y < 160; ++y)
        {
            for(size_t x = 0; x < 80; ++x)
            {
                Common::Point pnt(x * 116, y * 32);
                if ( (y & 1)  )
                    pnt.x += 58;

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
                Common::Point pnt(obj.unk1 * 58, obj.unk2 * 16);
                if (obj.unk2 & 1)
                    pnt.x -= 29;
                pnt += Res.SimpleObjects[obj.ObjId].unk[0][1];
                GFXDrawer.Draw( Res.SimpleObjects[obj.ObjId].Images[0], pnt);
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
    


    
}