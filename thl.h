#pragma once
#include <list>

#include "sdl.h"
#include "sdl_mixer.h"

#include "videoentity.h"
#include "animation.h"
#include "basesprite.h"
#include "font.h"
#include "image.h"
#include "region.h"
#include "resource.h"
#include "sprite.h"
#include "text.h"

using namespace std;


/*!
 * \brief Clase base del THL Framework
 *
 * Esta constituido fundamentalmente de metodos estaticos a ser 
 * invocados por otras clases.
 */
class THL
{
public:
    static bool Init(int width, int height, int depth, int _flags);
    static void Quit();
    static bool getInitiated() ;
    static void RegisterResource(Resource *rsc);
    static void UnregisterResource(Resource *rsc);
    static void DrawSurface(SDL_Surface *surface, Region *src, Region *dst);
    static void DrawSurfaceRegion(SDL_Surface *surface, Region *src, Region *dst, Region *region);
    static void DrawScreen();
    static bool Key(int code);
    static void setMousePos(int x, int y);
    static int getMouseX() ;
    static int getMouseY() ;
    static bool getMouseLeft() ;
    static bool getMouseRight() ;
    static bool getMouseMid() ;
    static SDL_Surface * getScreenSurface() ;
    static Region * getScreenRegion() ;
    static void PlayMusic(string FileName, int loop);
    static bool isPlayingMusic() ;
    static void StopMusic() ;
    static void MusicDone() ;
    
    static const int ClearBack = 0x00000001;
private:
    //! Indicia si se ha inicializado el THL
    static bool initiated;
    //! Guarda las flags con las que se inicializó el THL
    static int flags;
    //! Surface que representa la pantalla
    static SDL_Surface *screen ;
    //! Lista de recursos de la THL
    static list <Resource *> resources;
    //! Tabla de estado de teclas
    static Uint8 *Keystate;
    //! Puntero a una región que representa la pantalla entera
    static Region *ScreenRegion ;
    //! Puntero a la musica cargada Con SDL_Mix
    static Mix_Music *Music ;
    //! Variable de estado que vale true si esta sonando la musica
    static bool _isPlayingMusic ;
};


