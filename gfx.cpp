#include <SDL2/SDL_video.h>
#include <GL/gl.h>

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
    //clr = vec4(texture2D(tex2, vec2(clr.r, pl / 256.0)).rgb, clr.g);\n\
    
const std::string StdPShaderText = 
R"(
#version 120
uniform sampler2D tex;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform vec3 cBlend;
uniform bool bBlend;
uniform bool bLight;
uniform int pal;
uniform int drawMode;
uniform float Alpha;
void main()
{
    vec4 clr = texture2D(tex, gl_TexCoord[0].xy);
    
    if (drawMode == 0)
    {
    }
    else if (drawMode == 1)
    {
        if (pal != -1)
        {
            clr = vec4(texture2D(tex2, vec2(clr.r, pal / 256.0)).rgb, clr.g);
        } 
    }
    else if (drawMode == 2)
    {
        clr = vec4(clr.r, clr.r, clr.r, clr.g);
    }
    
    float lightning = 0.0; 
    if (bLight)
    {
        lightning = texture2D(tex3, gl_TexCoord[0].xy).r;
    }
    if (bBlend)
    {
        if (cBlend.r < 0.0)
           clr.r *= 1.0 + clamp(cBlend.r + lightning, -1.0, 0.0);
        else
           clr.r += (1.0 - clr.r) * cBlend.r;
        if (cBlend.g < 0.0)
           clr.g *= 1.0 + clamp(cBlend.g + lightning, -1.0, 0.0);
        else
           clr.g += (1.0 - clr.g) * cBlend.g;
        if (cBlend.b < 0.0)
           clr.b *= 1.0 + cBlend.b;
        else
           clr.b += (1.0 - clr.b) * cBlend.b;
    }
    gl_FragColor = vec4(clr.rgb, clr.a * Alpha);
})";

const std::string StdVShaderText =
R"(
#version 120
void main()
{
    gl_Position=ftransform();
    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
})";
 
    
    
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
    
    
    switch (_mode)
    {
        case MODE_HW:
        case MODE_PS:
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }
            break;
        
        default:
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
        
        _stdTex1Loc = Glext::GLGetUniformLocation(_stdProgID, "tex");
        _stdTex2Loc = Glext::GLGetUniformLocation(_stdProgID, "tex2");
        _stdTex3Loc = Glext::GLGetUniformLocation(_stdProgID, "tex3");
        
        _stdAlphaLoc = Glext::GLGetUniformLocation(_stdProgID, "Alpha");
        
        _stdPalLoc = Glext::GLGetUniformLocation(_stdProgID, "pal");
        _stdDrawModeLoc = Glext::GLGetUniformLocation(_stdProgID, "drawMode");
        
        Glext::GLUniform1i(_stdTex1Loc, 0);
        Glext::GLUniform1i(_stdTex2Loc, 1);
        Glext::GLUniform1i(_stdTex3Loc, 2);
                
        _stdBLightLoc = Glext::GLGetUniformLocation(_stdProgID, "bLight");
        
        
        Glext::GLGenFramebuffers(3, _glBuffer.data());
        glGenTextures(3, _bufferTex.data());
        
        for(int32_t i = 0; i < BUF_MAX; ++i)
            InitBuffer(i);
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

void CDrawer::DrawRect(uint32_t tex, Common::FRect out, float alpha)
{
    switch (_mode)
    {
        default:
            break;
            
        case MODE_HW:
        case MODE_PS:
        {
            _state.Tex = tex;
            _state.Pal = -1;
            _state.DrawMode = 0;
            float tmp = _state.Alpha;
            _state.Alpha = alpha;
            ApplyStates();

            std::array<TVertex, 4> vtx = {
                TVertex( vec3f(out.left,  out.top,    0.0), tUtV(0.0, 0.0) ),
                TVertex( vec3f(out.left,  out.bottom, 0.0), tUtV(0.0, 1.0) ),
                TVertex( vec3f(out.right, out.bottom, 0.0), tUtV(1.0, 1.0) ),
                TVertex( vec3f(out.right, out.top,    0.0), tUtV(1.0, 0.0) )
            };
            
            DrawVtxQuad(vtx);
            _state.Alpha = tmp;
        }
            break;
    }
}

void CDrawer::DrawRect(const Image *img, Common::FRect out)
{
    switch (_mode)
    {
        default:
            break;
            
        case MODE_HW:
        case MODE_PS:
        {
            _state.Tex = img->HW;
            _state.Pal = -1;
            _state.DrawMode = 0;
            ApplyStates();

            std::array<TVertex, 4> vtx = {
                TVertex( vec3f(out.left,  out.top,    0.0), tUtV(0.0, 0.0) ),
                TVertex( vec3f(out.left,  out.bottom, 0.0), tUtV(0.0, 1.0) ),
                TVertex( vec3f(out.right, out.bottom, 0.0), tUtV(1.0, 1.0) ),
                TVertex( vec3f(out.right, out.top,    0.0), tUtV(1.0, 0.0) )
            };
            
            DrawVtxQuad(vtx);
        }
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
            _state.Tex = img->HW;
            _state.Pal = -1;
            _state.DrawMode = 0;
            ApplyStates();

            const int32_t w = img->SW->w;
            const int32_t h = img->SW->h;

            std::array<TVertex, 4> vtx = {
                TVertex( vec3f(out.x,     out.y,     0.0), tUtV(0.0, 0.0) ),
                TVertex( vec3f(out.x,     out.y + h, 0.0), tUtV(0.0, 1.0) ),
                TVertex( vec3f(out.x + w, out.y + h, 0.0), tUtV(1.0, 1.0) ),
                TVertex( vec3f(out.x + w, out.y,     0.0), tUtV(1.0, 0.0) )
            };
            
            DrawVtxQuad(vtx);
        }
            break;
    }
}

void CDrawer::Draw(const PalImage *img, int32_t pal, Common::Point out)
{
    switch (_mode)
    {
        default:
            break;
            
        case MODE_PS:
        {
            _state.Tex = img->HW;
            _state.Pal = pal;
            _state.DrawMode = 1;
            ApplyStates();

            const int32_t w = img->SW.Width();
            const int32_t h = img->SW.Height();

            std::array<TVertex, 4> vtx = {
                TVertex( vec3f(out.x,     out.y,     0.0), tUtV(0.0, 0.0) ),
                TVertex( vec3f(out.x,     out.y + h, 0.0), tUtV(0.0, 1.0) ),
                TVertex( vec3f(out.x + w, out.y + h, 0.0), tUtV(1.0, 1.0) ),
                TVertex( vec3f(out.x + w, out.y,     0.0), tUtV(1.0, 0.0) )
            };
            
            DrawVtxQuad(vtx);
        }
            break;
    }
}

void CDrawer::DrawShadow(const PalImage *img, Common::Point out)
{
    switch (_mode)
    {
        default:
            break;
            
        case MODE_PS:
        {
            _state.Tex = img->HW;
            _state.DrawMode = 2;
            ApplyStates();

            const int32_t w = img->SW.Width();
            const int32_t h = img->SW.Height();

            std::array<TVertex, 4> vtx = {
                TVertex( vec3f(out.x,     out.y,     0.0), tUtV(0.0, 0.0) ),
                TVertex( vec3f(out.x,     out.y + h, 0.0), tUtV(0.0, 1.0) ),
                TVertex( vec3f(out.x + w, out.y + h, 0.0), tUtV(1.0, 1.0) ),
                TVertex( vec3f(out.x + w, out.y,     0.0), tUtV(1.0, 0.0) )
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
            _winSize = res;  
            _resScale = Common::FPoint(1.0 / (float)res.x, 1.0 / (float)res.y);
            
            SetBufferSize(BUF_0, _winSize);
            SetBufferSize(BUF_1, _winSize);
            SetBufferSize(BUF_2, _winSize);
            
            break;
    }
}

void CDrawer::SetScissor(bool mode, Common::Rect rect)
{
    _state.Scissor = mode;
    _state.ScissorRect = rect;
}

SDL_Surface *CDrawer::CreateSWSurface(Common::Point sz)
{
    return SDL_CreateRGBSurfaceWithFormat(0, sz.x, sz.y, _pixfmt->BitsPerPixel, _pixfmt->format);
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
            _state.Tex = img->HW;
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
            _state.Tex = img->HW;
            ApplyStates();
            
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img->SW->w, img->SW->h, _glPixfmt, _glPixtype, img->SW->pixels);            
        }
            break;
    }
    SDL_UnlockSurface(img->SW);
}

PalImage *CDrawer::CreatePalImage(Common::Point sz)
{
    PalImage *img = new PalImage();
    img->SW.Resize(sz);
    
    switch (_mode)
    {
        default:
            break;
            
        case MODE_PS:
        {
            glGenTextures(1, &img->HW);
            _state.Tex = img->HW;
            ApplyStates();
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, sz.x, sz.y, 0, GL_RG, GL_UNSIGNED_BYTE, NULL);
            
        }
            break;
    }
    return img;
}

void CDrawer::UpdatePalImage(PalImage *img)
{
    _state.Tex = img->HW;
    ApplyStates();
    
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img->SW.Width(), img->SW.Height(), GL_RG, GL_UNSIGNED_BYTE, img->SW.data()); 
}

Light *CDrawer::CreateLight(Common::Point sz)
{
    Light *l = new Light();
    l->SW.Resize(sz);
    
    switch (_mode)
    {
        default:
            break;
            
        case MODE_HW:
        case MODE_PS:
        {
            glGenTextures(1, &l->HW);
            _state.Tex2 = l->HW;
            ApplyStates();
            
            glActiveTexture(GL_TEXTURE2);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, l->SW.Width(), l->SW.Height(), 0, GL_RED, GL_UNSIGNED_BYTE, l->SW.data());     
            glActiveTexture(GL_TEXTURE0);
        }
            break;
    }
    return l;
}

void CDrawer::UpdateLight(Light *l)
{
    switch (_mode)
    {
        default:
            break;
            
        case MODE_HW:
        case MODE_PS:
        {
            _state.Tex2 = l->HW;
            ApplyStates();
            
            glActiveTexture(GL_TEXTURE2);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, l->SW.Width(), l->SW.Height(), GL_RED, GL_UNSIGNED_BYTE, l->SW.data());            
            glActiveTexture(GL_TEXTURE0);
        }
            break;
    }
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
        //if (setAll || (newStates->Prog != _lastState.Prog))
        {            
            Glext::GLUseProgram(newStates->Prog);
            
            forceSetShader = true;
        }
    }

    if (setAll || (newStates->Tex != _lastState.Tex))
    {
        if (newStates->Tex)
        {
            glBindTexture(GL_TEXTURE_2D, newStates->Tex);
            
            if (_mode == MODE_PS)
                Glext::GLUniform1i(_stdTex1Loc, 0);
        }
        else
            glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    if (setAll || (newStates->Tex2 != _lastState.Tex2))
    {
        if (newStates->Tex2)
        {
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, newStates->Tex2);
            glActiveTexture(GL_TEXTURE0);
        }
        else
        {
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE0);
        }
        
        if (_mode == MODE_PS)
        {
            if (setAll || (newStates->Tex2 != _lastState.Tex2))
                Glext::GLUniform1i(_stdBLightLoc, (newStates->Tex2 == 0 ? 0 : 2) );
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
        if (setAll || (newStates->Pal != _lastState.Pal))
            Glext::GLUniform1i(_stdPalLoc, newStates->Pal);
        
        if (setAll || (newStates->BBlend != _lastState.BBlend))
            Glext::GLUniform1i(_stdBBLendLoc, newStates->BBlend);
        
        if (setAll || (newStates->CBlend != _lastState.CBlend))
            Glext::GLUniform3f(_stdCBLendLoc, newStates->CBlend.x, newStates->CBlend.y, newStates->CBlend.z);
        
        if (setAll || (newStates->DrawMode != _lastState.DrawMode))
            Glext::GLUniform1i(_stdDrawModeLoc, newStates->DrawMode);
        
        if (setAll || (newStates->Alpha != _lastState.Alpha))
            Glext::GLUniform1f(_stdAlphaLoc, newStates->Alpha);
    }

//    if (setAll || (newStates->Zwrite != _lastState.Zwrite))
//    {
//        if (newStates->Zwrite)
//            glDepthMask(GL_TRUE);
//        else
//            glDepthMask(GL_FALSE);
//    }
    
    if (setAll || (newStates->Scissor != _lastState.Scissor) || (newStates->Scissor && (newStates->ScissorRect != _lastState.ScissorRect)))
    {
        if (newStates->Scissor)
        {
            glEnable(GL_SCISSOR_TEST);
            Common::PointRect t = newStates->ScissorRect;
            t.x = t.x * _winSize.x / _virtResolution.x;
            t.y = t.y * _winSize.y / _virtResolution.y;
            t.w = t.w * _winSize.x / _virtResolution.x;
            t.h = t.h * _winSize.y / _virtResolution.y;
            
            glScissor(t.x, _winSize.y - (t.y + t.h), t.w, t.h);
        }
        else
            glDisable(GL_SCISSOR_TEST);
    }

    if (setAll < 2)
        _lastState = _state;
}

void CDrawer::DrawVtxQuad(const std::array<GFX::CDrawer::TVertex, 4> &vtx)
{
    static const uint16_t indexes[6] = {0, 1, 2, 0, 2, 3};
    
    glVertexPointer(3, GL_FLOAT, sizeof(TVertex), &vtx[0].Pos);
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

void CDrawer::SetBufferSize(int32_t bufid, Common::Point sz)
{
    if (bufid != BUF_OFF)
    {
        glBindTexture(GL_TEXTURE_2D, _bufferTex.at(bufid));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sz.x, sz.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        
        glBindTexture(GL_TEXTURE_2D, 0);
        
        _state.Tex = 0;
    }
}

void CDrawer::InitBuffer(int32_t bufid)
{
    if (bufid != BUF_OFF)
    {
        Glext::GLBindFramebuffer(GL_FRAMEBUFFER, _glBuffer.at(bufid));
        glBindTexture(GL_TEXTURE_2D, _bufferTex.at(bufid));

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        
        Glext::GLFrameBufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _bufferTex.at(bufid), 0);

        glBindTexture(GL_TEXTURE_2D, 0);
        SetOutBuffer(_activeBuffer);
        
        _state.Tex = 0;
    }
}


void CDrawer::SetFade(bool on, const vec3f &blend)
{
    _state.BBlend = on;
    _state.CBlend = blend;
}

void CDrawer::SetLightMask(Light *l)
{
    if (l)
        _state.Tex2 = l->HW;
    else
        _state.Tex2 = 0;
}

void CDrawer::Begin()
{
    if (_mode == MODE_SW)
        return;

    glDisable(GL_CULL_FACE);
    glDisable(GL_DITHER);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDisable(GL_ALPHA_TEST);

    glViewport(0, 0, _winSize.x, _winSize.y);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    
    if (_mode == MODE_PS)
    {
        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _psPalettes);
        glActiveTexture(GL_TEXTURE2);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        
        Glext::GLUniform1i(_stdTex1Loc, 0);
        Glext::GLUniform1i(_stdTex2Loc, 1);
        Glext::GLUniform1i(_stdTex3Loc, 2);
    }
    
    _state = States();
    _state.LinearFilter = true;
    _state.AlphaBlend = true;
    _state.TexBlend = 2;
    _state.SrcBlend = GL_SRC_ALPHA;
    _state.DstBlend = GL_ONE_MINUS_SRC_ALPHA;
    if(_mode == MODE_PS)
        _state.Prog = _stdProgID;

    ApplyStates(1);
            
}


void CDrawer::SetOutBuffer(int32_t bfid)
{
    if (bfid != BUF_OFF)
        Glext::GLBindFramebuffer(GL_FRAMEBUFFER, _glBuffer.at(bfid));
    else
        Glext::GLBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    _activeBuffer = bfid;
}

uint32_t CDrawer::GetBufferTex(int32_t bfid)
{
    if (bfid == BUF_OFF)
        return 0;
    
    return _bufferTex.at(bfid);
}


void CDrawer::SetPalettes(const Common::PlaneArray<SDL_Color, 256, 256> &pals)
{
    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    
    if (!_psPalettes)
        glGenTextures(1, &_psPalettes);
    glBindTexture(GL_TEXTURE_2D, _psPalettes);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, pals.data()); 
    
    glActiveTexture(GL_TEXTURE0);
}

void CDrawer::SetVirtResolution(Common::Point size)
{
    if (size.x == 0 || size.y == 0)
        size = _winSize;

    SetProjectionMatrix( mat4x4f::Ortho(0, size.x, size.y, 0, -1, 1) );
    
    _state.Scissor = false;
    _virtResolution = size;
    
    glMatrixMode(GL_MODELVIEW);
}

int32_t PowerOf2(int32_t n)
{
    for(int32_t i = 2; i < 32; ++i)
    {
        if ( n < (1 << i) )
            return (1 << i);
    }

    return 0;
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