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
#include <GL/gl.h>

#include "common/common.h"
#include "vectors.h"
#include "matrix.h"
#include "common/plane.h"
#include "font.h"


namespace GFX
{

struct Image
{
    SDL_Surface *SW = nullptr;
    uint32_t HW = 0;
    
    Common::Point GetSize() const 
    {
        if (!SW)
            return Common::Point();
        return Common::Point(SW->w, SW->h);
    }
    
    ~Image()
    {
        if (SW)
            SDL_FreeSurface(SW);
        
        if (HW)
            glDeleteTextures(1, &HW);
    }
};

struct PalImage
{
    Common::PlaneVector<uint16_t> SW;;
    uint32_t HW = 0;
};

struct Light
{
    Common::PlaneVector<uint8_t> SW;
    uint32_t HW = 0;
};

constexpr const std::array<uint8_t, 32> Lkp5To8 = {   0,   8,  16,  24,  32,  41,  49,  57,
                                                     65,  74,  82,  90,  98, 106, 115, 123,
                                                    131, 139, 148, 156, 164, 172, 180, 189,
                                                    197, 205, 213, 222, 230, 238, 246, 255 };

int32_t PowerOf2(int32_t);

SDL_Surface * ConvertSDLSurface(SDL_Surface *src, const SDL_PixelFormat * fmt);

enum BUFFER
{
    BUF_OFF = -1,
    BUF_0 = 0,
    BUF_1 = 1,
    BUF_2 = 2,
    
    BUF_MAX
};

class CDrawer
{
public:
    struct States
    {
        uint32_t Tex = 0;
        uint32_t Tex2 = 0;
        uint32_t SrcBlend = GL_ONE;
        uint32_t DstBlend = GL_ZERO;
        int8_t TexBlend = 2;
        bool AlphaBlend = false;
        //bool Zwrite = true;
        bool LinearFilter = true;
        
        bool BBlend = false;
        vec3f CBlend; 
        
        vec3f CLight;
        
        int32_t Pal = -1;
        int32_t DrawMode = 0;

        float Alpha = 1.0;
        
        uint32_t Prog = 0;
        
        Common::PointRect ScissorRect;
        bool Scissor;
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

    struct TVertex
    {
        vec3f Pos;
        //vec3f Normal;
        tUtV  TexCoord;
        uint32_t TexCoordId = 0;

        TVertex() = default;
        TVertex(const vec3f &p): Pos(p) {};
        TVertex(const vec3f &p, const tUtV &uv): Pos(p), TexCoord(uv) {};
    };
public:
    CDrawer() {};
    
    void Init(int mode = MODE_SW);
    
    static int EventsWatcher(void *, SDL_Event *event);
        
    void SetProjectionMatrix(const mat4x4f &mat);
    void SetModelViewMatrix(const mat4x4f &mat);
    
    void Begin();
    
    void SetOutBuffer(int32_t bfid = BUF_OFF);
    uint32_t GetBufferTex(int32_t bfid = BUF_OFF);
    
    void SetPalettes(const Common::PlaneArray<SDL_Color, 256, 256> &pals);
    
    void Clear(SDL_Color clr = {0, 0, 0});
        
    void DrawRect(uint32_t tex, Common::FRect out, float alpha = 1.0);
    void DrawRect(const Image *img, Common::FRect out);
    void Draw(const Image *img, Common::Point out);
    
    void Draw(const PalImage *img, int32_t pal, Common::Point out);
    void DrawShadow(const PalImage *img, Common::Point out);
    void Flip();
    
    void SetFade(bool on, const vec3f &blend);
    void SetFade(bool on) {_state.BBlend = on;};
    
    void SetLightMask(Light *l);
    
    void SetLightColor(const vec3f &color)
    { _state.CLight = color; }
    
    void SetMode(Common::Point res, int windowed = 1);
    
    void SetScissor(bool mode, Common::Rect rect = Common::Rect());
    
    SDL_Surface *CreateSWSurface(Common::Point sz);
    
    Image *CreateImage(Common::Point sz);
    int LockImage(Image *img);
    void UnlockImage(Image *img);
    
    PalImage *CreatePalImage(Common::Point sz);
    void UpdatePalImage(PalImage *img);
    
    Light *CreateLight(Common::Point sz);
    void UpdateLight(Light *l);
    
    void ApplyStates(int setAll = 0);
    
    void DrawVtxQuad(const std::array<TVertex, 4> &vtx);
    
    uint32_t CompileShader(int32_t type, const std::string &string);
    
    void SetBufferSize(int32_t bufid, Common::Point sz);
    void InitBuffer(int32_t bufid);
    
    Font *LoadFont(const std::string &fname);
    void DrawText(const std::string &txt, Font *fnt, Common::Point pos);
    
    void SetVirtResolution(Common::Point size = Common::Point());

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
    int32_t _stdBLightLoc = -1;
    int32_t _stdCLightLoc = -1;
    int32_t _stdPalLoc = -1;
    int32_t _stdDrawModeLoc = -1;
    int32_t _stdTex1Loc = -1;
    int32_t _stdTex2Loc = -1;
    int32_t _stdTex3Loc = -1;
    int32_t _stdAlphaLoc = -1;
   
    
    int32_t _activeBuffer = 0;
    
    std::array<uint32_t, BUF_MAX> _glBuffer;
    std::array<uint32_t, BUF_MAX> _bufferTex;
    std::array<Common::Point, BUF_MAX> _bufferSize;
    
    uint32_t _psPalettes = 0;
    
    Common::Point _virtResolution;
    Common::Point _winSize;
    Common::FPoint _resScale;
    
public:
    static CDrawer Instance;
};



}

static constexpr GFX::CDrawer &GFXDrawer = GFX::CDrawer::Instance;

#endif /* GFX_H */

