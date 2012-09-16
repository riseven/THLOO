#include <iostream>
#include "thl.h"
#include "sdl.h"
#include "text.h"

using namespace std;

Text::Text(Font *fuente, Image *_img):
    Sprite(_img, NULL), font(fuente)
{
    font->AddCallback(this);
}

Text::~Text(void)
{
    font->RemoveCallback(this);
}

Text *
Text::Write(Font *fuente, string texto)
{
    if (!fuente)
    {
        cerr << "Error: Fuente no valida para escribir texto" << endl ;
        return NULL;
    }

    // Escribimos el texto
    SDL_Surface *surface1 = SDL_CreateRGBSurface(SDL_HWSURFACE, SFont_TextWidth(fuente->getSFont(), texto.c_str()), SFont_TextHeight(fuente->getSFont()), THL::getScreenSurface()->format->BitsPerPixel, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    if ( !surface1 )
    {
        cerr << "Error: No se puede crear la surface para el texto" << endl ;
        return NULL ;
    }

    SDL_Surface *surface2 = SDL_DisplayFormat(surface1);
    SDL_FreeSurface(surface1);
    if ( !surface2 )
    {
        cerr << "Error: No se puede adaptar surface al Display grafico" << endl ;
        return NULL ;
    }

    SDL_SetColorKey(surface2, SDL_SRCCOLORKEY, fuente->getImage()->getSurface()->format->colorkey);
    SFont_Write(surface2, fuente->getSFont(), 0, 0, texto.c_str() );
    Image *img = Image::New(surface2) ;
    if ( !img )
    {
        cerr << "Error: No se puede crear la imagen a partir de la surface" << endl ;
        SDL_FreeSurface(surface2);
        return NULL;
    }
    Text *t = new Text(fuente, img);
    
    return t ;
}

void
Text::Rewrite(string texto)
{
    if (!font)
    {
        cerr << "Error: No se puede reescribir el texto ya que no tiene fuente asociada" << endl ;
        return ;
    }

    // Escribimos el texto
    SDL_Surface *surface1 = SDL_CreateRGBSurface(SDL_HWSURFACE, SFont_TextWidth(font->getSFont(), texto.c_str()), SFont_TextHeight(font->getSFont()), THL::getScreenSurface()->format->BitsPerPixel, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    if ( !surface1 )
    {
        cerr << "Error: No se puede crear la surface para el texto" << endl ;
        return;
    }

    SDL_Surface *surface2 = SDL_DisplayFormat(surface1);
    SDL_FreeSurface(surface1);
    if ( !surface2 )
    {
        cerr << "Error: No se puede adaptar surface al Display grafico" << endl ;
        return;
    }

    SDL_SetColorKey(surface2, SDL_SRCCOLORKEY, font->getImage()->getSurface()->format->colorkey);
    SFont_Write(surface2, font->getSFont(), 0, 0, texto.c_str() );
    Image *_img = Image::New(surface2) ;
    if ( !_img )
    {
        cerr << "Error: No se puede crear la imagen a partir de la surface" << endl ;
        SDL_FreeSurface(surface2);
        return;
    }

    // La imagen anterior del texto se libera de forma inversa a la dependencia
    // por lo que primero debemos quitar la dependencia para evitar que se destruya
    // el texto.
    img->RemoveCallback(this) ;
    img->ReceiveCallback(this) ;

    // Ponemos la nueva imagen
    img = _img;
}

void
Text::ReceiveCallback(Resource *rsc)
{
    if ( rsc == font )
    {
        font = NULL ;
        return ;
    }

    Sprite::ReceiveCallback(rsc);
}

