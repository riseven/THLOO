#include "region.h"

Region::Region(int _x, int _y, int _w, int _h)
{
    rect.x = _x ;
    rect.y = _y ;
    rect.w = _w ;
    rect.h = _h ;
}

Region::~Region(void)
{
}

Region *
Region::New(int x, int y, int w, int h)
{
    Region *r = new Region(x, y, w, h);
    return r ;
}

Region *
Region::Copy(Region *reg)
{
    Region *nueva = new Region(reg->getX(), reg->getY(), reg->getW(), reg->getH());
    return nueva ;
}

bool
Region::isVacio()
{
    return (rect.w < 1 || rect.h < 1);
}

void
Region::setRect(int _x, int _y, int _w, int _h)
{
    rect.x = _x ;
    rect.y = _y ;
    rect.w = _w ;
    rect.h = _h ;
}

void
Region::setRect(SDL_Rect _rect)
{
    rect.x = _rect.x ;
    rect.y = _rect.y ;
    rect.w = _rect.w ;
    rect.h = _rect.h ;
}

SDL_Rect
Region::getRect()
{
    return rect ;
}

int
Region::getX()
{
    return rect.x ;
}

int
Region::getY()
{
    return rect.y ;
}

int
Region::getW()
{
    return rect.w ;
}

int
Region::getH()
{
    return rect.h ;
}


Region *
Region::Union(Region *reg)
{
    // Union rapida
    if ( isVacio() )
    {
        setRect(reg->getRect());
        return this ;
    }
    if ( reg->isVacio() )
        return this ;

    // Realizamos la union entre dos rects
    int ax0 = rect.x ;
    int ay0 = rect.y ;
    int ax1 = ax0 + rect.w ;
    int ay1 = ay0 + rect.h ;

    int bx0 = reg->rect.x ;
    int by0 = reg->rect.y ;
    int bx1 = bx0 + reg->rect.w ;
    int by1 = by0 + reg->rect.h ;

    if ( bx0 < ax0 ) ax0 = bx0 ;
    if ( by0 < ay0 ) ay0 = by0 ;
    if ( bx1 > ax1 ) ax1 = bx1 ;
    if ( by1 > ay1 ) ay1 = by1 ;

    rect.x = ax0 ;
    rect.y = ay0 ;
    rect.w = ax1 - ax0 ;
    rect.h = ay1 - ay0 ;

    return this ;
}

Region *
Region::Interseccion(Region *reg)
{
    // Union rapida
    if ( isVacio() )
    {
        return this ;
    }
    if ( reg->isVacio() )
    {
        setRect(reg->getRect());
        return this ;
    }

    // Realizamos la union entre dos rects
    int ax0 = rect.x ;
    int ay0 = rect.y ;
    int ax1 = ax0 + rect.w ;
    int ay1 = ay0 + rect.h ;

    int bx0 = reg->rect.x ;
    int by0 = reg->rect.y ;
    int bx1 = bx0 + reg->rect.w ;
    int by1 = by0 + reg->rect.h ;

    if ( bx0 > ax0 ) ax0 = bx0 ;
    if ( by0 > ay0 ) ay0 = by0 ;
    if ( bx1 < ax1 ) ax1 = bx1 ;
    if ( by1 < ay1 ) ay1 = by1 ;

    rect.x = ax0 ;
    rect.y = ay0 ;
    rect.w = ax1 - ax0 ;
    rect.h = ay1 - ay0 ;

    return this ;
}

bool
Region::Equal(Region *reg)
{
    if (rect.x != reg->rect.x || rect.y != reg->rect.y ||
        rect.w != reg->rect.w || rect.h != reg->rect.h)
    {
        return false ;
    }
    return true ;
}