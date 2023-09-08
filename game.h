#ifndef GAME_H
#define GAME_H

#include "common/common.h"


namespace Game
{
    
class Engine
{
public:
    
public:
    Common::Point _camera;
    float _zoom = 1.0;
    Common::Rect _visibleTiles;
    
    
public:
    static Engine Instance;
};
    
}

static constexpr Game::Engine &GameInst = Game::Engine::Instance;

#endif /* GAME_H */

