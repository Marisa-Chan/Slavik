#include <string>
#include <list>

#include "system.h"


namespace System
{
    
ResRatio::ResRatio()
{}

ResRatio::ResRatio(const Common::Point &p)
: W(p.x), H(p.y)
{
    R = (float)W / (float)H;
}

ResRatio::ResRatio(int32_t w, int32_t h)
: W(w), H(h)
{
    R = (float)W / (float)H;
}

bool ResRatio::operator==(const ResRatio &b) const
{
    if (W != b.W || H != b.H)
        return false;
    return true;
}

bool ResRatio::operator!=(const ResRatio &b) const
{
    if (W == b.W || H == b.H)
        return false;
    return true;
}

ResRatio::operator Common::Point() const
{
    return Common::Point(W, H);
}

void ResRatio::Set(int32_t w, int32_t h)
{
    W = w;
    H = h;
    R = (float)W / (float)H;
}
    




static SDL_Window *window = NULL;
static SDL_GLContext cont = NULL;
static std::list< SDL_EventFilter > EventHandlers;
static ResRatio winRes(1024, 768);

SDL_Surface *screen = NULL;


void Init()
{
    EventHandlers.clear();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS ) < 0)
    {
        printf("Couldn't initialize SDL: %s", SDL_GetError());
        return;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    window = SDL_CreateWindow("Slavik", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winRes.W, winRes.H, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    if (!window)
    {
        printf("Couldn't create window and renderer: %s", SDL_GetError());
        return;
    }
    
    screen = SDL_GetWindowSurface(window);

    
    cont = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, cont);

    //TTF_Init();

    //ScanFonts();

    //IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);

    //SDLNet_Init();
}

void Deinit()
{
    //IMG_Quit();

    //FontsList.clear();

    if (!cont)
        SDL_GL_DeleteContext(cont);

    if (!window)
        SDL_DestroyWindow(window);

    SDL_Quit();
}


void EventsAddHandler(SDL_EventFilter func, bool first)
{
    if (first)
        EventHandlers.push_front(func);
    else
        EventHandlers.push_back(func);
}

void EventsDeleteHandler(SDL_EventFilter func)
{
    EventHandlers.remove(func);
}

bool ProcessEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_WINDOWEVENT:
        {
            switch(event.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:

                int w, h;
                SDL_GL_MakeCurrent(window, cont);
                SDL_GL_GetDrawableSize(window, &w, &h);
                winRes = ResRatio(w, h);
                break;

            case SDL_WINDOWEVENT_CLOSE:
                return true;
            
            case SDL_WINDOWEVENT_DISPLAY_CHANGED:
                SDL_GL_MakeCurrent(window, cont);
                break;

            default:
                break;
            }

        }
        break;
        
        case SDL_QUIT:
            return true;

        default:
            break;
        }
        
        for(SDL_EventFilter &e : EventHandlers)
        {
            if ( (e)(NULL, &event) == 0 )
                break;
        }
    }

    return false;
}

void PostQuitMessage()
{
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);
}

void SetVideoMode(const Common::Point &sz, uint32_t full, SDL_DisplayMode *mode)
{
    if (window)
    {
        SDL_SetWindowFullscreen(window, full);

        if (full != 0 && mode)
        {
            SDL_Delay(250);
            SDL_SetWindowDisplayMode(window, mode);
        }
        else
        {
            if (winRes != sz)
            {
                if (SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED)
                {
                    SDL_RestoreWindow(window);
                    SDL_Delay(250);
                    
                    SDL_SetWindowSize(window, sz.x, sz.y);
                    
                    SDL_MaximizeWindow(window);
                    SDL_Delay(250);
                }
                else
                    SDL_SetWindowSize(window, sz.x, sz.y);
            }
        }
        
        int w, h;
        SDL_GL_GetDrawableSize(window, &w, &h);
        winRes = ResRatio(w, h);
    }
}

ResRatio GetRResolution()
{
    return winRes;
}

Common::Point GetResolution()
{
    return Common::Point(winRes.W, winRes.H);
}

void FlipSW()
{
    SDL_UpdateWindowSurface(window);
}

void FlipHW()
{
    SDL_GL_SwapWindow(window);
}

int rand() 
{ 
    return ::rand(); 
};

}