#include "thl.h"
#include "image.h"
#include "videoentity.h"
#include "SDL_Image.h"
#include "sdl.h"
#include <iostream>
#include <cmath>

using namespace std;

Image::Image(SDL_Surface *_surface):
    surface(_surface)
{
    if ( surface )
    {
        controlX = surface->w / 2 ;
        controlY = surface->h / 2 ;
    }
}

Image::~Image(void)
{
    if ( surface != NULL )
    {
        SDL_FreeSurface(surface) ;
        surface = NULL ;
    }
}

Image *
Image::Load(string FileName)
{
    SDL_Surface *temp = IMG_Load(FileName.c_str());
    if (!temp)
    {
        // No se pudo cargar la imagen
        cerr << "Error: No se pudo cargar la imagen: " << FileName << endl ;
        return NULL;
    }

    SDL_Surface *temp2 = SDL_DisplayFormatAlpha(temp) ;
    SDL_FreeSurface(temp);
    if (!temp2)
    {
        cerr << "Error: No se pudo cargar la imagen: " << FileName << endl ;
        return NULL ;
    }
    Image *img = new Image(temp2);

    return img ;
}

Image *
Image::New(SDL_Surface *_surface)
{
    if ( !_surface )
    {
        cerr << "Error: La surface para crear la imagen no es valida" << endl ;
        return NULL ;
    }

    Image *img = new Image(_surface);

    return img ;
}


void
Image::Draw(Region *src, Region *dst)
{
    THL::DrawSurface(surface, src, dst);
}

void
Image::DrawOnRegion(Region *src, Region *dst, Region *reg)
{
    THL::DrawSurfaceRegion(surface, src, dst, reg);
}
    
int
Image::getWidth()
{
    return surface->w ;
}

int
Image::getHeight()
{
    return surface->h ;
}

SDL_Surface *
Image::getSurface()
{
    return surface ;
}

Image *
Image::ScaleImage(Region *rect, int scx, int scy)
{
    scx = VideoEntity::FixScale(scx);
    scy = VideoEntity::FixScale(scy);

    double fx = scx / 100.0 ;
    double fy = scy / 100.0 ;

    int nw = (int)(rect->getW() * fx) ;
    int nh = (int)(rect->getH() * fy) ;

    SDL_Surface *nueva1 = SDL_CreateRGBSurface(SDL_HWSURFACE, nw, nh, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
    if ( !nueva1 )
    {
        cerr << "Error: No se pudo escalar la imagen" << endl ;
        return NULL ;
    }
    SDL_Surface *nueva2 = SDL_DisplayFormat(nueva1);
    SDL_FreeSurface(nueva1) ;
    if ( !nueva2 )
    {
        cerr << "Error: No se pudo escalar la imagen" << endl ;
        return NULL ;
    }

    SDL_SetColorKey(nueva2, SDL_SRCCOLORKEY, surface->format->colorkey );

    if ( SDL_LockSurface(nueva2))
    {
        cerr << "Error: No se pudo escalar la imagen" << endl ;
        SDL_FreeSurface( nueva2 );
        return NULL ;
    }

    if ( SDL_LockSurface(surface))
    {
        cerr << "Error: No se pudo escalar la imagen" << endl ;
        SDL_UnlockSurface( nueva2 ) ;
        SDL_FreeSurface( nueva2 ) ;
        return NULL ;
    }

    for ( int y = 0 ; y < nh ; y++ )
    {
        for ( int x = 0 ; x < nw ; x++ )
        {
            Uint8  *abits, *nbits;
            Uint32 Bpp;


            Bpp = surface->format->BytesPerPixel;
            abits = ((Uint8 *)surface->pixels)+((int)(y/fy)+rect->getY())*surface->pitch+((int)(x/fx)+rect->getX())*Bpp;
            nbits = ((Uint8 *)nueva2->pixels)+y*nueva2->pitch+x*Bpp;

            switch(Bpp) 
            {
                case 1:
                    *nbits = *abits;
                    break;
                case 2:
                    *((Uint16 *)nbits) = *((Uint16 *)abits);
                    break;
                case 3: 
                    *((nbits)+nueva2->format->Rshift/8) = *((abits)+surface->format->Rshift/8);
                    *((nbits)+nueva2->format->Gshift/8) = *((abits)+surface->format->Gshift/8);
                    *((nbits)+nueva2->format->Bshift/8) = *((abits)+surface->format->Bshift/8);
                    break;
                case 4:
                    *((Uint32 *)nbits) = *((Uint32 *)abits);
                    break;
            }
        }
    }

    SDL_UnlockSurface(nueva2);
    SDL_UnlockSurface(surface);

    Image *img = Image::New(nueva2);
    if (!img)
    {
        cerr << "Error: No se pudo escalar la imagen" << endl ;
        SDL_FreeSurface(nueva2);
        return NULL ;
    }

    return img ;
}

Image *
Image::RotateImage(int angle)
{
    const double PIOVER180 = 0.0174532925199;
    const double PIOVER2   = 1.5707963267948;

    int deg = angle ;
    angle = VideoEntity::FixAngle(angle);
    double rad = VideoEntity::DegToRad(angle);

    int nw = (int)(surface->w * fabs(cos( rad )) + surface->h * fabs(sin( rad )));
    int nh = (int)(surface->w * fabs(sin( rad )) + surface->h * fabs(cos( rad )));

    SDL_Surface *nueva1 = SDL_CreateRGBSurface(SDL_HWSURFACE, nw, nh, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
    if ( !nueva1 )
    {
        cerr << "Error: No se pudo rotar la imagen" << endl ;
        return NULL ;
    }
    SDL_Surface *nueva2 = SDL_DisplayFormat(nueva1);
    SDL_FreeSurface(nueva1);
    if ( !nueva2 )
    {
        cerr << "Error: No se pudo rotar la imagen" << endl ;
        return NULL ;
    }

    SDL_SetColorKey(nueva2, SDL_SRCCOLORKEY, surface->format->colorkey );

    if ( SDL_LockSurface(nueva2))
    {
        cerr << "Error: No se pudo rotar la imagen" << endl ;
        SDL_FreeSurface( nueva2 );
        return NULL ;
    }

    if ( SDL_LockSurface(surface))
    {
        cerr << "Error: No se pudo rotar la imagen" << endl ;
        SDL_UnlockSurface( nueva2 ) ;
        SDL_FreeSurface( nueva2 ) ;
        return NULL ;
    }

    double ty0, nx0, ny0, xdx, xdy, ydx, ydy ;

    // Ahora Calculamos la coordenada n(0,0) en a
    if ( deg >= 0 && deg < 90 )
    {
        ty0 = surface->w * sin( rad ) ;

        nx0 = ty0 * sin( rad ) ;
        ny0 = -ty0 * cos( rad ) ;

    }else if ( deg >= 90 && deg < 180 )
    {
        ty0 = surface->h * sin( rad-PIOVER2 );

        nx0 = surface->w + ty0*cos( rad-PIOVER2 );
        ny0 = ty0 * sin( rad-PIOVER2 );
    }else if ( deg >= 180 && deg < 270 )
    {
        ty0 = surface->w * sin( rad - 2*PIOVER2 );

        nx0 = surface->w - ty0 * sin( rad - 2*PIOVER2 );
        ny0 = surface->h + ty0 * cos( rad - 2*PIOVER2 );
    }else
    {
        ty0 = surface->h * sin( rad - 3*PIOVER2 );

        nx0 = -ty0 * cos( rad - 3*PIOVER2 );
        ny0 = surface->h - ty0*sin( rad - 3*PIOVER2);
    }

    xdx = cos(rad) ;
    xdy = sin(rad) ;

    ydx = -sin(rad) ;
    ydy = cos(rad) ;

    int x,y;
    double brx,bry,rx,ry;

    for ( y = 0 , brx = nx0, bry = ny0 ; y < nueva2->h ; y++, brx+=ydx, bry+=ydy )
    {
        for ( x = 0 , rx = brx, ry = bry ; x < nueva2->w ; x++, rx+=xdx, ry+=xdy )
        {
            if ( rx >= 0 && rx < surface->w && ry >= 0 && ry < surface->h )
            {

                Uint8  *abits, *nbits;
                Uint32 Bpp;

                Bpp = surface->format->BytesPerPixel;
                abits = ((Uint8 *)surface->pixels)+((int)ry)*surface->pitch+((int)rx)*Bpp;
                nbits = ((Uint8 *)nueva2->pixels)+y*nueva2->pitch+x*Bpp;
                
                switch(Bpp) 
                {
                    case 1:
                        *nbits = *abits;
                        break;
                    case 2:
                        *((Uint16 *)nbits) = *((Uint16 *)abits);
                        break;
                    case 3: 
                        *((nbits)+nueva2->format->Rshift/8) = *((abits)+surface->format->Rshift/8);
                        *((nbits)+nueva2->format->Gshift/8) = *((abits)+surface->format->Gshift/8);
                        *((nbits)+nueva2->format->Bshift/8) = *((abits)+surface->format->Bshift/8);
                        break;
                    case 4:
                        *((Uint32 *)nbits) = *((Uint32 *)abits);
                        break;
                }

            }else
            {
                Uint8  *bits;
                Uint32 Bpp;
   
                Bpp = surface->format->BytesPerPixel;
                bits = ((Uint8 *)nueva2->pixels)+y*nueva2->pitch+x*Bpp;



                switch(Bpp) 
                {
                    case 1:
                        *bits = surface->format->colorkey;
                        break;
                    case 2:
                        *((Uint16 *)bits) = nueva2->format->colorkey;
                        break;
                    case 3: 
                        *((bits)+0) = (nueva2->format->colorkey&0x0000FF);
                        *((bits)+1) = (nueva2->format->colorkey&0x00FF00)>>8;
                        *((bits)+2) = (nueva2->format->colorkey&0xFF0000)>>16;
                        break;
                    case 4:
                        *((Uint32 *)bits) = nueva2->format->colorkey;
                        break;
                }
            }
        }
    }

    SDL_UnlockSurface(nueva2);
    SDL_UnlockSurface(surface);

    Image *img = Image::New(nueva2);
    if (!img)
    {
        cerr << "Error: No se pudo escalar la imagen" << endl ;
        SDL_FreeSurface(nueva2);
        return NULL ;
    }

    return img ;
}

int
Image::getControlX()
{
    return controlX;
}

int
Image::getControlY()
{
    return controlY;
}

void
Image::setControl(int cx, int cy)
{
    controlX = cx ;
    controlY = cy ;
}




