#include "font.h"
#include <iostream>
using namespace std ;

Font::Font(Image *Img, SFont_Font *SFont):
    img(Img), sfont(SFont)
{
    // Añadimos la dependencia con la imagen
    img->AddCallback(this);
}

Font::~Font(void)
{
    img->RemoveCallback(this);
    SFont_FreeFont(sfont);
}

Font *
Font::Load(string FileName)
{
    // Primero cargamos la imagen de fuente
    Image *_img = Image::Load(FileName);
    if ( _img == NULL )
    {
        cerr << "Error: No se pudo cargar la fuente: " << FileName << endl ;
        return NULL ;
    }

    // Inicializamos la fuente
    SFont_Font *_sfont = SFont_InitFont(_img->getSurface()) ;
    if ( _sfont == NULL )
    {
        cerr << "Error: No se pudo inicializar la fuente: " << FileName << endl ;
        // Liberamos la imagen pues ya no la vamos a necesitar
        delete _img ;
        return NULL;
    }

    Font *_fuente = new Font(_img, _sfont );
    return _fuente ;
}

Image *
Font::getImage()
{
    return img ;
}

SFont_Font *
Font::getSFont()
{
    return sfont ;
}

