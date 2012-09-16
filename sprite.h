#pragma once
#include "basesprite.h"

class Sprite :
    public BaseSprite
{
public:
    static Sprite * New(Image *img, Region *rect=NULL);
    virtual ~Sprite(void);
    virtual void preDrawScreen();
    virtual void onDrawScreen();    

protected:
    Sprite(Image *_img, Region *_rect);

};
