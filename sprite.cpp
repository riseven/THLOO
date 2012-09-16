#include "sprite.h"
#include <iostream>
using namespace std;

Sprite::Sprite(Image *_img, Region *_rect):
    BaseSprite(_img, _rect)
{
}

Sprite::~Sprite(void)
{
}

Sprite *
Sprite::New(Image *img, Region *rect)
{
    if ( img == NULL )
    {
        cerr << "Error: No se puede crear un sprite sin una imagen" << endl ;
        return NULL;
    }

    Sprite *s = new Sprite(img, rect);
    return s ;
}

void
Sprite::preDrawScreen()
{
    Actualizar();
}


void
Sprite::onDrawScreen()
{
    Draw();
}


