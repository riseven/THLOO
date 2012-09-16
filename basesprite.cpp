#include "basesprite.h"
#include "sdl.h"
#include <iostream>
#include "thl.h"
using namespace std;

list <Region *>BaseSprite::actualiza;
bool BaseSprite::_dibuja = false ;

BaseSprite::BaseSprite(Image *_img, Region *_rect):
    VideoEntity(0, 0, 0, 0, 100, 100), mod_img(NULL), _angle(0), _scaleX(100), _scaleY(100), _rectDrawn(NULL)
{
    img = _img ;
    
    if ( _rect == NULL )
    {
        img_rect = Region::New(0, 0, img->getWidth(), img->getHeight());
    } else
    { 
        img_rect = Region::Copy(_rect);
    }
    
    img->AddCallback(this);

    // Asignamos el sprite a la Region principal
    region = THL::getScreenRegion() ;
    THL::getScreenRegion()->AddCallback(this);

    // Asignamos el punto de control
    controlX = img->getControlX() ;
    controlY = img->getControlY() ;
}

BaseSprite::~BaseSprite(void)
{
    img->RemoveCallback(this);
    region->RemoveCallback(this);
}

BaseSprite *
BaseSprite::New(Image *img, Region *rect)
{
    if ( img == NULL )
    {
        cerr << "Error: No se puede crear un sprite sin una imagen" << endl ;
        return NULL;
    }

    BaseSprite *bs = new BaseSprite(img, rect);
    return bs ;
}

void
BaseSprite::Draw()
{
    _dibuja = true ; // Ponemos la variable estatica _dibuja a true

    // Calculamos el destino
    Region *drawRegion = Region::New(0,0,0,0) ;
    list <Region *>::iterator i;
    for ( i = actualiza.begin(); i != actualiza.end() ; i++ )
    {
        Region *temp2 = Region::Copy(region)->Interseccion( *i ) ;
        drawRegion->Union(temp2);
        delete temp2 ;
    }

    Region *rect = Region::New(x-controlX,y-controlY,0,0) ;
    if ( angle == 0 && scaleX == 100 && scaleY == 100 )
    {
        img->DrawOnRegion(img_rect, rect, drawRegion);
    }else
    {
        if ( angle != _angle || scaleX != _scaleX || scaleY != _scaleY || !mod_img)
        {
            if ( mod_img != NULL )
                delete mod_img ;

            Image *temp ;
            if ( scaleX != 100 || scaleY != 100 || img_rect->getW() != img->getWidth() || img_rect->getH() != img->getHeight())
            {
                temp = img->ScaleImage(img_rect, scaleX, scaleY) ;
                if ( angle != 0 )
                {
                    mod_img = temp->RotateImage(angle);
                    delete temp ;
                }else
                {
                    mod_img = img->RotateImage(angle);
                }
            }else
            {
                mod_img = img->RotateImage(angle);
            }
        }

        mod_img->DrawOnRegion(NULL, rect, drawRegion);
    }  
    delete drawRegion ;
    delete rect ;
}

void
BaseSprite::setRegion(Region *reg)
{
    region->RemoveCallback(this);
    region = reg ;
    region->AddCallback(this);
}

int
BaseSprite::getControlX()
{
    return controlX; 
}

int 
BaseSprite::getControlY()
{
    return controlY;
}

void
BaseSprite::setControl(int cx, int cy)
{
    controlX = cx ;
    controlY = cy ;
}

void
BaseSprite::AddRedrawRegion(Region *reg)
{
    if ( _dibuja )
    {
        // Si esta es la primera modificacion desde el dibujo, vaciamos la lista
        while ( !actualiza.empty() )
        {
            delete actualiza.front();
            actualiza.pop_front();
        }
        _dibuja = false ;
    }
    actualiza.push_back(reg);
}

Region *
BaseSprite::CalcDrawRegion()
{
    if ( mod_img == NULL )
    {
        int tx = x - controlX ;
        int ty = y - controlY ;
        int tw = img->getWidth() ;
        int th = img->getHeight() ;
        return Region::New(tx, ty, tw, th);
    }

    return NULL;
}

void
BaseSprite::Actualizar()
{
    Region *temp = CalcDrawRegion();
    if (_rectDrawn == NULL )
    {
        Region *temp2 = Region::Copy(temp);
        AddRedrawRegion(temp2);
    }else if (!temp->Equal( _rectDrawn ))
    {
        Region *temp2 = Region::Copy(temp)->Union(_rectDrawn);
        AddRedrawRegion(temp2);
    }
    if ( _rectDrawn )
        delete _rectDrawn ;
    _rectDrawn = temp ;
}





