#include <limits>

#include "system.h"
#include "game.h"
#include "fmt/printf.h"

namespace Game {
    

const uint8_t Engine::PathGo[8][8]  =
{
    {0xff, 0xff, 0xff,    1, 0x80,    7, 0xff, 0xff},
    {0xff, 0xff, 0xff,    2,    3, 0x80,    7,    0},
    {0xff, 0xff, 0xff, 0xff, 0xff,    3, 0x80,    1},
    {   1,    2, 0xff, 0xff, 0xff,   4,     5, 0x80},
    {0x80,    3, 0xff, 0xff, 0xff, 0xff, 0xff,    5},
    {   7, 0x80,    3,    4, 0xff, 0xff, 0xff,    6},
    {0xff,    7, 0x80,    5, 0xff, 0xff, 0xff, 0xff},
    {0xff,    0,    1, 0x80,    5,    6, 0xff, 0xff}
};

int Engine::FUN_0043edb7(Character *pchar)
{
    auto &FootMap = _currentMap->FootMap;
    
    if (pchar->MoveTile == pchar->Tile)
        return 0;
        
    Common::Point bpozy;
    
    int32_t chID = FootMap.At(pchar->MoveTile).ID;
    if (chID) 
    {
        if (chID == 0xfff)
            return 0;
        
        if ((pchar->field2_0x2 & 7) == 0)
            return 0;
        
        if (pchar->EnemyCharID != chID)
            return 0;
        
        bpozy = pchar->Tile;
    }
    
    Common::Point mv = pchar->MoveTile;
    
    const Common::Point mv_nx = mv.Add(-1, 0);
    const Common::Point mv_ax = mv.Add( 1, 0);
    const Common::Point mv_n2y = mv.Add(0, -2);
    const Common::Point mv_a2y = mv.Add(0, 2);
    
    if ( (mv.x < 1    || (FootMap.At(mv_nx).ID != 0 && mv_nx != bpozy))
      && (mv.x >= 159 || (FootMap.At(mv_ax).ID != 0 && mv_ax != bpozy))
      && (mv.y < 2    || (FootMap.At(mv_n2y).ID != 0 && mv_n2y != bpozy))
      && (mv.y >= 318 || (FootMap.At(mv_a2y).ID != 0 && mv_a2y != bpozy)) )
    {
        const Common::Point mv_ny = mv.Add(0, -1);
        
        if (mv.y & 1)
        {
            const Common::Point mv_nxy = mv.Add(-1, -1);
            if ( mv.y < 1 || (FootMap.At(mv_ny).ID != 0  && mv_ny  != bpozy 
              && (mv.x < 1 || (FootMap.At(mv_nxy).ID != 0 && mv_nxy != bpozy))) ) 
            {
                if (mv.y >= 319)
                    return 0;
                
                const Common::Point mv_ay = mv.Add(0, 1);
                if (FootMap.At(mv_ay).ID != 0 && mv_ay != bpozy)
                {
                    if (mv.x < 1)
                        return 0;
                    
                    const Common::Point mv_aynx = mv.Add(-1, 1);
                    if (FootMap.At(mv_aynx).ID != 0 && mv_aynx != bpozy)
                        return 0;
                }
            }
        }
        else
        {
            const Common::Point mv_nyax = mv.Add(1, -1);
            if ( mv.y  < 1 || (FootMap.At(mv_ny).ID != 0
              && (mv.x >= 159 || FootMap.At(mv_nyax).ID != 0)) )
            {
                if (mv.y >= 319)
                    return 0;
                
                const Common::Point mv_ay = mv.Add( 0, 1);
                if (FootMap.At(mv_ay).ID != 0)
                {
                    if (mv.x >= 159)
                        return 0;
                    
                    const Common::Point mv_ayax = mv.Add(1, 1);
                    if (FootMap.At(mv_ayax).ID != 0)
                        return 0;
                }
            }
        }
    }
    
    bool atTarget = false;
    const Common::Point Target = pchar->MoveTile;
    
    pchar->field17_0x13.fill(0xff);
    
    HardComputeCount++;
    
    
    struct DirCost
    {
        uint32_t Cost = std::numeric_limits<int32_t>::max();
        int8_t Dir = 0xff;
    };    
    static Common::PlaneArray<DirCost, 160, 320> costMap;
    
    
    struct PF_PT
    {
        Common::Point pt;
        uint32_t cost = 0;
        
        PF_PT(Common::Point _t, uint32_t _w): pt(_t), cost(_w) {};
    };
    static std::deque< PF_PT > CheckPaths;
    
    
    static int32_t BaseCost = 0x7F7F0000;
    
    BaseCost -= 0x10000;
    
    if (BaseCost <= 0)
    {
        costMap.fill(DirCost());
        BaseCost = 0x7F7F0000;
    }
        
    //Clear next check routes
    CheckPaths.clear();
    int32_t chkCount = 0;
    
    PF_PT curPos(pchar->Tile, BaseCost);
    
    while(true) 
    {
        if (chkCount >= MAXPATHITER)
        {
            printf("WARNING pathfind too long! %d\n", pchar->Index);
            
            return 0;
        }
        
        curPos.cost += 10;
        if (curPos.pt.y & 1)
        {
            if (curPos.pt.y > 0) 
            {
                Common::Point ny = curPos.pt.Add(0, -1);
                DirCost *dcost = &costMap.At(ny);
                
                /*  .
                   /  */
                if (ny == Target)
                {
                    if (curPos.cost < dcost->Cost) 
                    {
                        atTarget = true;
                        dcost->Cost = curPos.cost;
                        dcost->Dir = 3;
                    }
                }
                else if ( FootMap.At(ny).ID == 0 && !atTarget && 
                         curPos.cost < dcost->Cost ) 
                {
                    CheckPaths.push_back( {ny, curPos.cost} );

                    dcost->Cost = curPos.cost;
                    dcost->Dir = 3;
                }
                
                /*. 
                   \ */
                if (curPos.pt.x > 0)
                {
                    Common::Point nxy = ny.Add(-1, 0);
                    dcost = &costMap.At(nxy);
                    
                    if (nxy == Target) 
                    {
                        if (curPos.cost < dcost->Cost) 
                        {
                            atTarget = true;
                            dcost->Cost = curPos.cost;
                            dcost->Dir = 1;
                        }
                    }
                    else if (FootMap.At(nxy).ID == 0 && !atTarget && 
                         curPos.cost < dcost->Cost )
                    {
                        CheckPaths.push_back( {nxy, curPos.cost} );

                        dcost->Cost = curPos.cost;
                        dcost->Dir = 1;
                    }
                }
            }
            
            if (curPos.pt.y < 319) 
            {
                Common::Point ay = curPos.pt.Add(0, 1);
                DirCost *dcost = &costMap.At(ay);
                /* \ 
                    ' */
                if (ay == Target) 
                {
                    if (curPos.cost < dcost->Cost)
                    {
                        atTarget = true;
                        dcost->Cost = curPos.cost;
                        dcost->Dir = 5;
                    }
                }
                else if (FootMap.At(ay).ID == 0 && !atTarget && 
                         curPos.cost < dcost->Cost )
                {
                    CheckPaths.push_back( {ay, curPos.cost} );

                    dcost->Cost = curPos.cost;
                    dcost->Dir = 5;
                }
                
                /*  / 
                   '  */
                if (curPos.pt.x > 0)
                {
                    Common::Point aynx = ay.Add(-1, 0);
                    dcost = &costMap.At(aynx);
                    if (aynx == Target)
                    {
                        if (curPos.cost < dcost->Cost)
                        {
                            atTarget = true;
                            dcost->Cost = curPos.cost;
                            dcost->Dir = 7;
                        }
                    }
                    else if (FootMap.At(aynx).ID == 0 && !atTarget && 
                                curPos.cost < dcost->Cost) 
                    {
                        CheckPaths.push_back( {aynx, curPos.cost} );
                        
                        dcost->Cost = curPos.cost;
                        dcost->Dir = 7;
                    }
                }
            }
        }
        else
        {
            if (curPos.pt.y > 0)
            {
                Common::Point ny = curPos.pt.Add(0, -1);
                DirCost *dcost = &costMap.At(ny);
                /* . 
                    \ */
                if (ny == Target)
                {
                    if (curPos.cost < dcost->Cost)
                    {
                        atTarget = true;
                        dcost->Cost = curPos.cost;
                        dcost->Dir = 1;
                    }
                }
                else if (FootMap.At(ny).ID == 0 && !atTarget && 
                                curPos.cost < dcost->Cost)
                {
                    CheckPaths.push_back( {ny, curPos.cost} );
                    
                    dcost->Cost = curPos.cost;
                    dcost->Dir = 1;
                }
                
                /*  . 
                   /  */
                if (curPos.pt.x < 159) 
                {
                    Common::Point axny = ny.Add(1, 0);
                    dcost = &costMap.At(axny);
                    if (axny == Target)
                    {
                        if (curPos.cost < dcost->Cost)
                        {
                            atTarget = true;
                            dcost->Cost = curPos.cost;
                            dcost->Dir = 3;
                        }
                    }
                    else if (FootMap.At(axny).ID == 0 && !atTarget &&
                                curPos.cost < dcost->Cost)
                    {
                        CheckPaths.push_back( {axny, curPos.cost} );
                        
                        dcost->Cost = curPos.cost;
                        dcost->Dir = 3;
                    }
                }
            }
            
            if (curPos.pt.y < 319)
            {
                Common::Point ay = curPos.pt.Add(0, 1);
                DirCost *dcost = &costMap.At(ay);
                
                if (ay == Target)
                {
                    if (curPos.cost < dcost->Cost)
                    {
                        atTarget = true;
                        dcost->Cost = curPos.cost;
                        dcost->Dir = 7;
                    }
                }
                else if (FootMap.At(ay).ID == 0 && !atTarget && 
                                curPos.cost < dcost->Cost)
                {
                    CheckPaths.push_back( {ay, curPos.cost} );
                    
                    dcost->Cost = curPos.cost;
                    dcost->Dir = 7;
                }
                
                if (curPos.pt.x < 159)
                {
                    Common::Point axy = ay.Add(1, 0);
                    dcost = &costMap.At(axy);
                    if (axy == Target)
                    {
                        if (curPos.cost < dcost->Cost)
                        {
                            atTarget = true;
                            dcost->Cost = curPos.cost;
                            dcost->Dir = 5;
                        }
                    }
                    else if (FootMap.At(axy).ID == 0 && !atTarget && 
                                curPos.cost < dcost->Cost)
                    {
                        CheckPaths.push_back( {axy, curPos.cost} );
                        
                        dcost->Cost = curPos.cost;
                        dcost->Dir = 5;
                    }
                }
            }
        }
            
        curPos.cost += 4;
        if (curPos.pt.x > 0)
        {
            Common::Point nx = curPos.pt.Add(-1, 0);
            DirCost *dcost = &costMap.At(nx);
            
            if (nx == Target)
            {
                if (curPos.cost < dcost->Cost)
                {
                    atTarget = true;
                    dcost->Cost = curPos.cost;
                    dcost->Dir = 0;
                }
            }
            else if (FootMap.At(nx).ID == 0 && !atTarget && 
                        curPos.cost < dcost->Cost)
            {
                CheckPaths.push_back( {nx, curPos.cost} );
                
                dcost->Cost = curPos.cost;
                dcost->Dir = 0;
            }
        }
        
        if (curPos.pt.x < 159)
        {
            Common::Point ax = curPos.pt.Add(1, 0);
            DirCost *dcost = &costMap.At(ax);
            
            if (ax == Target)
            {
                if (curPos.cost < dcost->Cost)
                {
                    atTarget = true;
                    dcost->Cost = curPos.cost;
                    dcost->Dir = 4;
                }
            }
            else if (FootMap.At(ax).ID == 0 && !atTarget && 
                        curPos.cost < dcost->Cost)
            {
                CheckPaths.push_back( {ax, curPos.cost} );
                
                dcost->Cost = curPos.cost;
                dcost->Dir = 4;
            }
        }
        
        if (curPos.pt.y > 1)
        {
            Common::Point ny = curPos.pt.Add(0, -2);
            DirCost *dcost = &costMap.At(ny);
            
            if (ny == Target)
            {
                if (curPos.cost < dcost->Cost)
                {
                    atTarget = true;
                    dcost->Cost = curPos.cost;
                    dcost->Dir = 2;
                }
            }
            else if (FootMap.At(ny).ID == 0 && !atTarget && 
                        curPos.cost < dcost->Cost)
            {
                CheckPaths.push_back( {ny, curPos.cost} );
                
                dcost->Cost = curPos.cost;
                dcost->Dir = 2;
            }
        }
        
        if (curPos.pt.y < 318) 
        {
            Common::Point ay = curPos.pt.Add(0, 2);
            DirCost *dcost = &costMap.At(ay);
            
            if (ay == Target)
            {
                if (curPos.cost < dcost->Cost)
                {
                    atTarget = true;
                    dcost->Cost = curPos.cost;
                    dcost->Dir = 6;
                }
            }
            else if (FootMap.At(ay).ID == 0 && !atTarget && 
                        curPos.cost < dcost->Cost)
            {
                CheckPaths.push_back( {ay, curPos.cost} );
                
                dcost->Cost = curPos.cost;
                dcost->Dir = 6;
            }
        }
        
        if (CheckPaths.empty()) 
            break;
            
        curPos = CheckPaths.front();
        CheckPaths.pop_front();
        
        chkCount++;
    }
    
    if (!atTarget)
        return 0;

    std::vector<int32_t> pathmoves;
    pathmoves.reserve(32);

    for (Common::Point pos = Target; pos != pchar->Tile; ) 
    {
        int32_t dir = costMap.At(pos).Dir;

        pathmoves.push_back(dir);

        if (dir >= 4)
            dir -= 4;
        else
            dir += 4;

        static const Common::Point dxdy[8][2] =
        {
            {{-1,  0}, {-1,  0}},
            {{ 0, -1}, {-1, -1}},
            {{ 0, -2}, { 0, -2}},
            {{ 1, -1}, { 0, -1}},
            {{ 1,  0}, { 1,  0}},
            {{ 1,  1}, { 0,  1}},
            {{ 0,  2}, { 0,  2}},
            {{ 0,  1}, {-1,  1}}
        };

        pos += dxdy[dir][pos.y & 1];
    }

    int32_t dist = pathmoves.size();
    if (dist > 31)
        dist = 31;

    for(int32_t i = 0; i < dist; ++i)
    {
        pchar->field17_0x13[i] = pathmoves.back();
        pathmoves.pop_back();
    }

    return dist + pathmoves.size();
}


}
