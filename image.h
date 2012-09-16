#pragma once
#include "SDL.h"
#include "resource.h"
#include "region.h"
#include <string>

class Image :
    public Resource
{
public:
    static Image *Load(string FileName) ;
    static Image *New(SDL_Surface *surface) ;
    virtual ~Image(void);
    void Draw(Region *src, Region *dst);
    void DrawOnRegion(Region *src, Region *dst, Region *reg);
    int getWidth();
    int getHeight();
    SDL_Surface *getSurface();
    Image *ScaleImage(Region *rect, int scx, int scy) ;
    Image *RotateImage(int angle);
    int getControlX();
    int getControlY();
    void setControl(int cx, int cy);

protected:
    Image(SDL_Surface *_surface);
    
    SDL_Surface *surface;
    int controlX, controlY;

private:

};
