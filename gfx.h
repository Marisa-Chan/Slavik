#ifndef GFX_H
#define GFX_H

#include <SDL2/SDL_opengl.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/glext.h>
#else
#include <GL/glext.h>
#include <queue>
#endif

#include <SDL2/SDL_surface.h>
#include <array>
#include <string>

#include "common/common.h"
#include "vectors.h"
#include "matrix.h"


namespace GFX
{

struct Image
{
    SDL_Surface *SW = nullptr;
    uint32_t HW = 0;
};

constexpr const std::array<uint8_t, 32> Lkp5To8 = {   0,   8,  16,  24,  32,  41,  49,  57,
                                                     65,  74,  82,  90,  98, 106, 115, 123,
                                                    131, 139, 148, 156, 164, 172, 180, 189,
                                                    197, 205, 213, 222, 230, 238, 246, 255 };

SDL_Surface * ConvertSDLSurface(SDL_Surface *src, const SDL_PixelFormat * fmt);

class CDrawer
{
public:
    struct States
    {
        const Image *Tex = nullptr;
        uint32_t SrcBlend = GL_ONE;
        uint32_t DstBlend = GL_ZERO;
        int8_t TexBlend = 2;
        bool AlphaBlend = false;
        //bool Zwrite = true;
        bool LinearFilter = true;
        
        bool BBlend = false;
        vec3f CBlend; 

        uint32_t Prog = 0;
    };
   
    enum MODE
    {
        MODE_SW = 0,
        MODE_HW = 1,
        MODE_PS = 2
    };
    
    struct tUtV
    {
        float tu = -1.0;
        float tv = -1.0;

        tUtV() = default;
        tUtV(float u, float v): tu(u), tv(v) {};
    };
    
    struct __attribute__((packed)) TGLColor
    {
        float r = 1.0;
        float g = 1.0;
        float b = 1.0;
        float a = 1.0;

        TGLColor() = default;
        TGLColor(float _r, float _g, float _b, float _a = 1.0)
            : r(_r), g(_g), b(_b), a(_a) {};
    };
    
    struct TVertex
    {
        vec3f Pos;
        //vec3f Normal;
        tUtV  TexCoord;
        TGLColor Color;
        TGLColor ComputedColor;
        uint32_t TexCoordId = 0;

        TVertex() = default;
        TVertex(const vec3f &p): Pos(p) {};
        TVertex(const vec3f &p, const tUtV &uv): Pos(p), TexCoord(uv) {};
        TVertex(const vec3f &p, const tUtV &uv, const TGLColor &clr): Pos(p), TexCoord(uv), Color(clr) {};
    };
public:
    CDrawer() {};
    
    void Init(int mode = MODE_SW);
    
    static int EventsWatcher(void *, SDL_Event *event);
    
    void SetProjectionMatrix(const mat4x4f &mat);
    void SetModelViewMatrix(const mat4x4f &mat);
    
    void Clear(SDL_Color clr = {0, 0, 0});
    void Draw(const Image *img, Common::Point out);
    void Flip();
    
    void SetFade(bool on, const vec3f &blend = vec3f());
    
    void SetMode(Common::Point res, int windowed = 1);
    
    Image *CreateImage(Common::Point sz);
    int LockImage(Image *img);
    void UnlockImage(Image *img);
    
    void ApplyStates(int setAll = 0);
    
    void DrawVtxQuad(const std::array<TVertex, 4> &vtx);
    
    uint32_t CompileShader(int32_t type, const std::string &string);

public:
    SDL_PixelFormat *_pixfmt;
    int32_t _glPixfmt, _glPixtype;
    
    int8_t _mode = MODE_SW;
    
    States _state;
    States _lastState; 
    
    bool _glext = false;
    
    uint32_t _stdPsShader = 0;
    uint32_t _stdVsShader = 0;
    uint32_t _stdProgID = 0;
    int32_t _stdBBLendLoc = -1;
    int32_t _stdCBLendLoc = -1;
    
    
public:
    static CDrawer Instance;
};



}

static constexpr GFX::CDrawer &GFXDrawer = GFX::CDrawer::Instance;

#endif /* GFX_H */

