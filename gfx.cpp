#include <SDL2/SDL_video.h>

#include "gfx.h"
#include "system.h"
#include "glfuncs.h"

namespace System{
    void FlipSW();
    void FlipHW();
    
    extern SDL_Surface *screen;
}

namespace GFX
{
    
const std::string StdPShaderText =
"\
#version 120\n\
uniform sampler2D texture;\
uniform vec3 cBlend;\
uniform bool bBlend;\
void main()\
{\
    vec4 clr = texture2D(texture, gl_TexCoord[0].xy);\
    if (bBlend)\
    {\
        if (cBlend.x < 0.0)\
           clr.x *= 1.0 + cBlend.x;\
        else\
           clr.x += (1.0 - clr.x) * cBlend.x;\
        if (cBlend.y < 0.0)\
           clr.y *= 1.0 + cBlend.y;\
        else\
           clr.y += (1.0 - clr.y) * cBlend.y;\
        if (cBlend.z < 0.0)\
           clr.z *= 1.0 + cBlend.z;\
        else\
           clr.z += (1.0 - clr.z) * cBlend.z;\
    }\
    gl_FragColor = clr;\
}";


const std::string StdVShaderText =
"\
#version 120\n\
void main()\
{\
    gl_Position=ftransform();\
    gl_FrontColor = gl_Color;\
    gl_TexCoord[0] = gl_MultiTexCoord0;\
}";
 
    
    
CDrawer CDrawer::Instance;

void CDrawer::Init(int mode)
{   
    _glext = Glext::Init();
    
    _mode = mode;
    
    SDL_DisplayMode curr;
    SDL_GetCurrentDisplayMode(0, &curr);
    
    switch(curr.format)
    {
        case SDL_PIXELFORMAT_RGB888:
        case SDL_PIXELFORMAT_ARGB8888:
            _pixfmt = SDL_AllocFormat( SDL_PIXELFORMAT_ARGB8888 );
            _glPixfmt = GL_BGRA;
            _glPixtype = GL_UNSIGNED_BYTE;
            break;
        
        case SDL_PIXELFORMAT_BGR888:
        case SDL_PIXELFORMAT_ABGR8888:
        default:
            _pixfmt = SDL_AllocFormat( SDL_PIXELFORMAT_ABGR8888 );
            _glPixfmt = GL_RGBA;
            _glPixtype = GL_UNSIGNED_BYTE;
            break;
    }
    
    if (_mode == MODE_PS)
    {
        _stdPsShader = CompileShader(GL_FRAGMENT_SHADER, StdPShaderText);
        _stdVsShader = CompileShader(GL_VERTEX_SHADER,   StdVShaderText);
        _stdProgID = Glext::GLCreateProgram();
        
        Glext::GLAttachShader(_stdProgID, _stdPsShader);
        Glext::GLAttachShader(_stdProgID, _stdVsShader);
        Glext::GLLinkProgram(_stdProgID);
        
        _stdBBLendLoc = Glext::GLGetUniformLocation(_stdProgID, "bBlend");
        _stdCBLendLoc = Glext::GLGetUniformLocation(_stdProgID, "cBlend");
    }
    
    System::EventsAddHandler(EventsWatcher);
}

int CDrawer::EventsWatcher(void *, SDL_Event *event)
{
    switch(event->type)
    {
    case SDL_WINDOWEVENT:
    {
        switch(event->window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            //Instance.UpdateSizes();
            break;
        default:
            break;
        }

    }
    break;
    }
    
    return 1; // This event can be passed to another event watcher
}


void CDrawer::SetProjectionMatrix(const mat4x4f &mat)
{
    glMatrixMode(GL_PROJECTION);
    mat4x4f tmp = mat.Transpose();
    glLoadMatrixf(&tmp.m00);
}

void CDrawer::SetModelViewMatrix(const mat4x4f &mat)
{
    glMatrixMode(GL_MODELVIEW);
    mat4x4f tmp = mat.Transpose();
    glLoadMatrixf(&tmp.m00);
}

void CDrawer::Clear(SDL_Color clr)
{   
    switch (_mode)
    {
        default:
        case MODE_SW:
            {
                SDL_FillRect(System::screen, NULL, SDL_MapRGB(System::screen->format, clr.r, clr.g, clr.b));     
            }
            break;
        
        case MODE_HW:
        case MODE_PS:
            glClearColor(clr.r / 255.0, clr.g / 255.0, clr.b / 255.0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            break;
    }
    
}

void CDrawer::Draw(const Image *img, Common::Point out)
{
    switch (_mode)
    {
        default:
        case MODE_SW:
        {
            SDL_Rect sdlout = out;
            SDL_BlitSurface(img->SW, NULL, System::screen, &sdlout);       
        }
            break;
            
        case MODE_HW:
        case MODE_PS:
        {
            _state.Tex = img;
            ApplyStates();

            const int32_t w = img->SW->w;
            const int32_t h = img->SW->h;

            std::array<TVertex, 4> vtx = {
                TVertex( vec3f(out.x,     out.y,     0.0), tUtV(0.0, 0.0), TGLColor(1.0, 1.0, 1.0, 1.0) ),
                TVertex( vec3f(out.x,     out.y + h, 0.0), tUtV(0.0, 1.0), TGLColor(1.0, 1.0, 1.0, 1.0) ),
                TVertex( vec3f(out.x + w, out.y + h, 0.0), tUtV(1.0, 1.0), TGLColor(1.0, 1.0, 1.0, 1.0) ),
                TVertex( vec3f(out.x + w, out.y,     0.0), tUtV(1.0, 0.0), TGLColor(1.0, 1.0, 1.0, 1.0) )
            };
            
            DrawVtxQuad(vtx);
        }
            break;
    }
}

void CDrawer::Flip()
{
    switch (_mode)
    {
        default:
        case MODE_SW:
            System::FlipSW();
            break;
        
        case MODE_HW:
        case MODE_PS:
            System::FlipHW();
            break;
    }
}

void CDrawer::SetMode(Common::Point res, int windowed)
{
    switch (_mode)
    {
        default:
            break;
            
        case MODE_HW:
        case MODE_PS:
            _state = States();
            _state.LinearFilter = true;
            _state.AlphaBlend = true;
            _state.TexBlend = 2;
            _state.SrcBlend = GL_SRC_ALPHA;
            _state.DstBlend = GL_ONE_MINUS_SRC_ALPHA;
            if(_mode == MODE_PS)
                _state.Prog = _stdProgID;

            ApplyStates(1);

            glDisable(GL_CULL_FACE);
            glDisable(GL_DITHER);
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);
            glDisable(GL_ALPHA_TEST);

            SetProjectionMatrix( mat4x4f::Ortho(0, res.x, res.y, 0, -1, 1) );
            SetModelViewMatrix( mat4x4f() );
            
            glViewport(0, 0, res.x, res.y);
            
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            break;
    }
}

Image *CDrawer::CreateImage(Common::Point sz)
{
    Image *img = new Image();
    img->SW = SDL_CreateRGBSurfaceWithFormat(0, sz.x, sz.y, _pixfmt->BitsPerPixel, _pixfmt->format);
    
    switch (_mode)
    {
        default:
            break;
            
        case MODE_HW:
        case MODE_PS:
        {
            glGenTextures(1, &img->HW);
            _state.Tex = img;
            ApplyStates();
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            if (img->SW->format->format == _pixfmt->format)
            {
                SDL_LockSurface(img->SW);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->SW->w, img->SW->h, 0, _glPixfmt, _glPixtype, img->SW->pixels);
                SDL_UnlockSurface(img->SW);
            }
            else
            {
                SDL_Surface *conv = ConvertSDLSurface(img->SW, _pixfmt);

                SDL_LockSurface(conv);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->SW->w, img->SW->h, 0, _glPixfmt, _glPixtype, conv->pixels);
                SDL_UnlockSurface(conv);

                SDL_FreeSurface(conv);
            }
            
        }
            break;
    }
    return img;
}

int CDrawer::LockImage(Image *img)
{
    return SDL_LockSurface(img->SW);
}

void CDrawer::UnlockImage(Image *img)
{
    switch (_mode)
    {
        default:
            break;
            
        case MODE_HW:
        case MODE_PS:
        {
            _state.Tex = img;
            ApplyStates();
            
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img->SW->w, img->SW->h, _glPixfmt, _glPixtype, img->SW->pixels);            
        }
            break;
    }
    SDL_UnlockSurface(img->SW);
}

void CDrawer::ApplyStates(int setAll)
{
//    static const std::array<int, 4> blends = {GL_ZERO, GL_ONE, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
    States *newStates;
    
    if (setAll < 2)
        newStates = &_state;
    else
        newStates = &_lastState;
    
    bool forceSetShader = false;
    if (setAll)
        forceSetShader = true;
    
    if (_glext)
    {
        if (setAll || (newStates->Prog != _lastState.Prog))
        {            
            Glext::GLUseProgram(newStates->Prog);
            
            forceSetShader = true;
        }
    }

    if (setAll || (newStates->Tex != _lastState.Tex))
    {
        if (newStates->Tex)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, newStates->Tex->HW);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }

    /*if (setAll || (newStates->AlphaTest != _lastState.AlphaTest))
    {
        if (newStates->AlphaTest == false)
        {
            glDisable(GL_ALPHA_TEST);
        }
        else
        {
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0);
        }
    }*/


    if (setAll || (newStates->Tex != _lastState.Tex)
               || (newStates->LinearFilter != _lastState.LinearFilter) )
    {
        if (newStates->Tex)
        {
            if (newStates->LinearFilter)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            }
            else
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            }
        }
    }
    
    if (setAll || (newStates->SrcBlend != _lastState.SrcBlend)
                   || (newStates->DstBlend != _lastState.DstBlend))
    {
        glBlendFunc(newStates->SrcBlend, newStates->DstBlend);
    }

    if (setAll || (newStates->TexBlend != _lastState.TexBlend))
    {
        if (newStates->TexBlend == 0)
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        else if (newStates->TexBlend == 1)
        {
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
            glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
            glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
        }
        else if (newStates->TexBlend == 2)
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }

    if (setAll || (newStates->AlphaBlend != _lastState.AlphaBlend))
    {
        if (newStates->AlphaBlend)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);
    }
    
    if (_mode == MODE_PS)
    {
        if (setAll || (newStates->BBlend != _lastState.BBlend))
            Glext::GLUniform1i(_stdBBLendLoc, newStates->BBlend);
        
        if (setAll || (newStates->CBlend != _lastState.CBlend))
            Glext::GLUniform3f(_stdCBLendLoc, newStates->CBlend.x, newStates->CBlend.y, newStates->CBlend.z);
    }

//    if (setAll || (newStates->Zwrite != _lastState.Zwrite))
//    {
//        if (newStates->Zwrite)
//            glDepthMask(GL_TRUE);
//        else
//            glDepthMask(GL_FALSE);
//    }

    if (setAll < 2)
        _lastState = _state;
}

void CDrawer::DrawVtxQuad(const std::array<GFX::CDrawer::TVertex, 4> &vtx)
{
    static const uint16_t indexes[6] = {0, 1, 2, 0, 2, 3};
    
    glVertexPointer(3, GL_FLOAT, sizeof(TVertex), &vtx[0].Pos);
    glColorPointer(4, GL_FLOAT, sizeof(TVertex), &vtx[0].Color);
    glTexCoordPointer(2, GL_FLOAT, sizeof(TVertex), &vtx[0].TexCoord);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, &indexes);
}

uint32_t CDrawer::CompileShader(int32_t type, const std::string &string)
{
    GLuint sh = Glext::GLCreateShader(type);
    if (!sh)
        return 0;
    
    const GLchar *source = (const GLchar *)string.c_str();
    
    Glext::GLShaderSource(sh, 1, &source, 0);
    Glext::GLCompileShader(sh);
    
    GLint tmpvar;
    Glext::GLGetShaderiv(sh, GL_COMPILE_STATUS, &tmpvar);
    
    if (tmpvar == GL_FALSE)
    {
        Glext::GLGetShaderiv(sh, GL_INFO_LOG_LENGTH, &tmpvar);
        if (tmpvar > 0)
        {
            char *logbuff = new char[tmpvar + 2];
            Glext::GLGetShaderInfoLog(sh, tmpvar, NULL, logbuff);
            printf("Shader error: %s\n", logbuff);
            delete[] logbuff;
        }
        Glext::GLDeleteShader(sh);
        return 0;
    }
    
    return sh;
}

void CDrawer::SetFade(bool on, const vec3f &blend)
{
    _state.BBlend = on;
    _state.CBlend = blend;
}

SDL_Surface * ConvertSDLSurface(SDL_Surface *src, const SDL_PixelFormat * fmt)
{
#if (SDL_COMPILEDVERSION == SDL_VERSIONNUM(2, 0, 12))
    /***
     * Workaround for bug with convertation of surface with palette introduced
     * in SDL2 2.0.12 and fixed soon but after release.
     ***/
    if (src->format->BytesPerPixel == 1)
    {
#if SDL_VERSION_ATLEAST(2,0,5)
        SDL_Surface *tmp = SDL_CreateRGBSurfaceWithFormat(0, src->w, src->h, fmt->BitsPerPixel, fmt->format);
#else
        SDL_Surface *tmp = SDL_CreateRGBSurface(0, src->w, src->h, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask );
#endif
        SDL_BlendMode blend = SDL_BLENDMODE_NONE;
        SDL_GetSurfaceBlendMode(src, &blend);
        SDL_SetSurfaceBlendMode(src, SDL_BLENDMODE_NONE);
        SDL_BlitSurface(src, NULL, tmp, NULL);
        SDL_SetSurfaceBlendMode(src, blend);
        return tmp;
    }
    else
        return SDL_ConvertSurface(src, fmt, 0);
#else
    return SDL_ConvertSurface(src, fmt, 0);
#endif
}

}