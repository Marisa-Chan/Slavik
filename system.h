#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <SDL2/SDL.h>
#include "common/common.h"

namespace  System 
{
    struct ResRatio
    {
        int32_t W = 0;
        int32_t H = 0;
        float   R = 1.0;

        ResRatio();
        ResRatio(const Common::Point &p);
        ResRatio(int32_t w, int32_t h);
        bool operator==(const ResRatio &b) const;
        bool operator!=(const ResRatio &b) const;
        void Set(int32_t w, int32_t h);
        
        operator Common::Point() const;
    };
    
    void Init();
    void Deinit();
    
    void EventsAddHandler(SDL_EventFilter func, bool first = true);
    void EventsDeleteHandler(SDL_EventFilter func);
    
    // Events update cycle
    bool ProcessEvents();
    
    void PostQuitMessage();
    
    void SetVideoMode(const Common::Point &sz, uint32_t full, SDL_DisplayMode *mode);
    
    // Return drawable area sizes
    ResRatio GetRResolution();
    Common::Point GetResolution();
    
    int rand();
}


#endif /* SYSTEM_H */

