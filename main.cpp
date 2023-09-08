#include "system.h"
#include "fsmgr.h"
#include "resources.h"
#include "glfuncs.h"

#include <chrono>
#include <thread>
#include <GL/gl.h>


int main(int argc, char** argv) {
    FSMgr::Mgr::SetBaseDir("");
    System::Init();
    GFXDrawer.Init(GFX::CDrawer::MODE_PS);
    
    GFXDrawer.SetMode({1024,768});
    
    Game::Res.Load();
    
    GFXDrawer.Clear();
    GFXDrawer.Draw( Game::Res.Tiles[304], {100, 100} );
    GFXDrawer.Flip();
    
    auto map = Game::Resources::LoadGameMap(2);
    float xx = 1.0;
    
    for(int8_t k = 2; k < 10; ++k)
    {
        for(int8_t j = 0; j < 8; ++j)
        {
            for(int8_t i = 0; i < Game::Res.CharacterBases[0].Frames[k][j]; ++i)
            {

                Common::Point pnt;
                //printf("%d %d %d %d\n", Game::Res.CharacterBases[0].Seq[k][j][i], k, j, i);
                GFXDrawer.Clear();
                std::this_thread::sleep_for(std::chrono::milliseconds(25));
                
                GFXDrawer.SetFade(true, vec3f(xx, xx, xx));
                
                xx -= 0.04;
                if (xx < -1.0)
                    xx = 1.0;
                
                for(size_t t = 0; t < 160; ++t)
                {
                    for(size_t p = 0; p < 80; ++p)
                    {
                        pnt.x = p * 116; 
                        if ( (t & 1)  )
                            pnt.x += 58;
                        pnt.y = t * 32;
                            
                        if (map->TileMap[t][p][0] != 0)
                        {
                            
                            GFXDrawer.Draw( Game::Res.Tiles[ map->TileMap[t][p][0] - 1 ], pnt);
                        }
                        
                        if (map->TileMap[t][p][1] != 0)              
                            GFXDrawer.Draw( Game::Res.Tiles[ map->TileMap[t][p][1] - 1 ], pnt);
                    }
                }
                
                GFXDrawer.SetFade(false);
                
                pnt.x = 100 + Game::Res.CharacterBases[0].unk3[k][j][i][1].x; 
                pnt.y = 100 + Game::Res.CharacterBases[0].unk3[k][j][i][1].y;
                int32_t si = Game::Res.CharacterBases[0].Seq[k][j][i];
                GFXDrawer.Draw( Game::Res.CharacterBases[0].Images[si], pnt);
                
                pnt.x = 100 + Game::Res.CharacterBases[0].unk2[k][j][i].x + Game::Res.CharacterEquip[4].unk3[k][j][i][1].x; 
                pnt.y = 100 + Game::Res.CharacterBases[0].unk2[k][j][i].y + Game::Res.CharacterEquip[4].unk3[k][j][i][1].y; 
                si = Game::Res.CharacterEquip[4].Seq[k][j][i];
                GFXDrawer.Draw( Game::Res.CharacterEquip[4].Images[si], pnt);
                                
                GFXDrawer.Flip();
            }
        }
    }
    
    return 0;
}

