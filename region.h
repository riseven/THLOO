#pragma once
#include "resource.h"
#include "sdl.h"

class Region :
    public Resource
{
public:
    static Region * New(int x, int y, int w, int h);
    static Region * Copy(Region *);
    virtual ~Region(void);
    void setRect(int x, int y, int w, int h);
    void setRect(SDL_Rect rect);
    SDL_Rect getRect() ;
    int getX();
    int getY();
    int getW();
    int getH();
    bool isVacio();
    Region * Union(Region *reg);
    Region * Interseccion(Region *reg);
    bool Equal(Region *reg);

protected:
    Region(int x, int y, int w, int h);
    
    SDL_Rect rect;
};
