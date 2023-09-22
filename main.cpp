#include "system.h"
#include "fsmgr.h"
#include "resources.h"
#include "glfuncs.h"
#include "game.h"

#include <GL/gl.h>


int main(int argc, char** argv) {
    FSMgr::Mgr::SetBaseDir("");
    System::Init();
    GFXDrawer.Init(GFX::CDrawer::MODE_PS);
    
    GFXDrawer.SetMode({1024,768});

    GameInst.Init(GFX::CDrawer::MODE_PS);
    GameInst.LoadGameState(0);
    GameInst.LoadMap(3);
    
    while(true)
    {
        if ( System::ProcessEvents() )
            break;
        
        if ( GameInst.Process() )
            break;
    }
    
    return 0;
}

