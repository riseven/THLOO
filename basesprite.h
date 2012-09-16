#pragma once
#include "videoentity.h"
#include "image.h"
#include "region.h"
#include "sdl.h"
#include <list>
using namespace std;

class BaseSprite :
    public VideoEntity
{
public:
    static BaseSprite * New(Image *img, Region *rect=NULL);
    virtual ~BaseSprite(void);
    void Draw();
    void setRegion(Region *reg);
    int getControlX();
    int getControlY();
    void setControl(int cx, int cy);
    Region * CalcDrawRegion(); 
    void Actualizar();
    static void AddRedrawRegion(Region *reg);

protected:
    BaseSprite(Image *_img, Region *_rect);
    
    Image *img ;
    Image *mod_img ;
    Region *img_rect;
    Region *region;

    // Valores del frame anterior, se establecen en Actualizar
    Region *_rectDrawn ; // Rectangulo que ocupaba el basesprite
    bool _formChange ; // Indica si se ha modificado angle o escale

    // Valores de la imagen
    int _angle ;
    int _scaleX ;
    int _scaleY ;

    int controlX ;
    int controlY ;

    static list <Region *>actualiza ;
    static bool _dibuja ;; // Indica si la ultima operacion ha sido de dibujado o de actualizacion

private:

};
