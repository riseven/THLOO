#include "thl.h"
#include "image.h"
#include "sprite.h"
#include "text.h"
#include "animation.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    THL::Init(640,480,24,NULL);

    Image *inave = Image::Load("nave.bmp");
    Image *ifondo = Image::Load("fondo.jpg");

    Sprite *fondo = Sprite::New(ifondo);
    fondo->setControl(0,0);

    Sprite *nave = Sprite::New(inave);

    int velH = 0 ;
    int velV = 0 ;
    
    while(true)
    {

        THL::DrawScreen();

        if ( THL::Key(SDLK_UP))
        {
            velV = velV - 1 ;
        }
        if ( THL::Key(SDLK_DOWN))
        {
            velV = velV + 1 ;
        }
        if ( THL::Key(SDLK_RIGHT))
        {
            velH = velH +1 ;
        }
        if ( THL::Key(SDLK_LEFT))
        {
            velH = velH -1 ;
        }

        nave->setPosRel(velH, velV);

        
    }

    THL::Quit();
    return 0;
}
