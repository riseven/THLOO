#include "thl.h"
#include "SDL_mixer.h"
#include <iostream>
#include <string>
#include "sdl_mixer.h"
using namespace std;


bool THL::initiated = false ;
int THL::flags = 0 ;
SDL_Surface * THL::screen = NULL ;
list <Resource *> THL::resources ;
Uint8 * THL::Keystate = NULL ;
Region * THL::ScreenRegion = NULL ;
Mix_Music * THL::Music = NULL ;
bool THL::_isPlayingMusic = false ;

/*!
 * \brief Inicializa el framework
 *
 * Inicializa todos los sitemas de bajo nivel necesarios para el funcionamiento
 * del Framework. El valor de Flags puede ser una combinacion OR de:
 * THL::ClearBack : Se borra el contenido de la pantalla en cada frame
 * 
 * \param width Ancho de la resolución deseada
 * \param height Alto de la resolución deseada
 * \param depth Profundidad de color de la resolución deseada
 * \param flags Flags de inicialización del Framework
 * \return true en caso de que todo vaya bien, false si no se pudo inicializar el THL
 */
bool 
THL::Init(int width, int height, int depth, int _flags)
{
    if ( initiated )
    {
        cerr << "Error: No se puede inicializar dos veces el THL" << endl ;
        return false ;
    }

    // Primero guardamos una copia de los flags
    flags = _flags ;

    // Inicializamos el SDL
    if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER) == -1 )
    {
        // Si no se puede inicializar el SDL es un error crítico
        cerr << "Error crítico: No se pudo inicializar SDL" << endl ;
        exit(-1);
    }

    // Inicializamos la pantalla
    if ( (screen = SDL_SetVideoMode(width, height, depth, SDL_HWSURFACE|SDL_DOUBLEBUF)) == NULL )
    {
        if ( (screen = SDL_SetVideoMode(width, height, depth, SDL_DOUBLEBUF)) == NULL )
        {
            cerr << "Error crítico: No se pudo inicializar el modo gráfico" << endl ;
            exit(-1);
        }
    }

    // Inicializamos el puntero al array de teclas
    Keystate = SDL_GetKeyState(NULL);

    // Inicializamos el mixer
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096);

    // Ponemos la variable Initiated a cierto
    initiated = true ;
    // Tras poner initiated a true se pueden instanciar Resources

    // Creamos la region principal de pantalla, 
    ScreenRegion = Region::New(0, 0, screen->w, screen->h );

    return true;
}

/*!
 * \brief Libera los recursos del THL y termina la ejecución del programa
 */
void
THL::Quit()
{
    if ( !initiated )
    {
        return ;
    }

    while( !resources.empty() )
    {
        delete resources.front() ;
        resources.pop_front();
    }

    // Cerramos el mixer de sonido
    Mix_CloseAudio();
    
    // Cerramos el SDL
    SDL_Quit();

    // Terminamos la ejecución del programa
    exit(0);
}

/*!
 * \brief Indica si el THL ha sido inicializado
 *
 * \return True si el THL ha sido inicializado, false en caso contrario
 */
bool
THL::getInitiated()
{
    return initiated ;
}

/*!
 * \brief Registra un recurso en el THL
 *
 * Esta función es llamada por la clase Resource en su constructor para registrar
 * el recurso en el THL de modo que este sea liberado con la llamada a THL::Quit()
 * \param rsc Puntero al recurso que se quiere registrar
 */
void
THL::RegisterResource(Resource *rsc)
{
    // Buscamos la posicion
    list <Resource *>::iterator i ;
    i = resources.begin();
    while ( true )
    {
        if ( i == resources.end() ) break ;
        if ( *rsc > **i ) break ;
        i++ ;
    }
    resources.insert(i, rsc);
}

/*!
 * \brief Borra el registro de un recurso del THL
 *
 * Esta función es llamada por el desctructor de la clase Resources para indicar
 * al THL que ya no exite tal recurso y por tanto no debe liberarlo con la llamada
 * a THL::Quit().
 * Aunque también se llama a esta función durante el proceso de THL::Quit() tras
 * la destrucción de cada recurso.
 * \param rsc El puntero al recurso que ha dejado de existir
 */
void
THL::UnregisterResource(Resource *rsc)
{
    resources.remove(rsc);
}

void
THL::DrawSurface(SDL_Surface *surface, Region *src, Region *dst)
{
    if ( !initiated )
    {
        cerr << "Error: No se puede dibujar en la pantalla antes de inicializar el THL" << endl ;
        return ;
    }
    SDL_Rect rsrc = src->getRect();
    SDL_Rect rdst = dst->getRect();

    SDL_BlitSurface(surface, &rsrc, screen, &rdst);
}

void
THL::DrawSurfaceRegion(SDL_Surface *surface, Region *src, Region *dst, Region *region)
{
    if ( !initiated )
    {
        cerr << "Error: No se puede dibujar en la pantalla antes de inicializar el THL" << endl ;
        return ;
    }

    SDL_Rect rsrc = src->getRect();
    SDL_Rect rdst = dst->getRect();

    SDL_SetClipRect(screen, &region->getRect());
    SDL_BlitSurface(surface, &rsrc, screen, &rdst);
    SDL_SetClipRect(screen, &ScreenRegion->getRect());
}

void
THL::DrawScreen()
{
    if ( !initiated )
    {
        cerr << "Error: No se puede dibujar en la pantalla antes de inicializar el THL" << endl ;
        return ;
    }

    list <Resource *>::iterator i ;
    for ( i = resources.begin(); i != resources.end() ; i++ )
    {
        (*i)->preDrawScreen();
    }

    // Primero comprobamos los eventos
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:  
            Quit();
            break;
        }
    }
    
    for ( i = resources.begin(); i != resources.end() ; i++ )
    {
        (*i)->onDrawScreen();
    }
    SDL_Flip(screen);
}


SDL_Surface *
THL::getScreenSurface()
{
    return screen ;
}

bool
THL::Key(int code)
{
    if ( !initiated )
    {
        cerr << "Error: No se puede acceder al teclado antes de inicializar el THL" << endl ;
        return false ;
    }

    if ( Keystate == NULL )
    {
        cerr << "Error: El array de teclas no esta inicializado" << endl ;
        return false ;
    }

    return (Keystate[code]==1) ;
}

void
THL::setMousePos(int x, int y)
{
    if ( !initiated )
    {
        cerr << "Error: No se puede acceder al raton antes de inicializar el THL" << endl ;
        return ;
    }

    SDL_WarpMouse( x, y );
}

int
THL::getMouseX()
{
    if ( !initiated )
    {
        cerr << "Error: No se puede acceder al raton antes de inicializar el THL" << endl ;
        return 0 ;
    }

    int x ;
    SDL_GetMouseState(&x, NULL);
    return x ;
}

int
THL::getMouseY()
{
    if ( !initiated )
    {
        cerr << "Error: No se puede acceder al raton antes de inicializar el THL" << endl ;
        return 0 ;
    }

    int y ;
    SDL_GetMouseState(NULL, &y);
    return y ;
}

bool
THL::getMouseLeft()
{
    if ( !initiated )
    {
        cerr << "Error: No se puede acceder al raton antes de inicializar el THL" << endl ;
        return false ;
    }

    return (SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(1))==1 ;
}

bool
THL::getMouseRight()
{
    if ( !initiated )
    {
        cerr << "Error: No se puede acceder al raton antes de inicializar el THL" << endl ;
        return false ;
    }

    return (SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(3))==1 ;
}

bool
THL::getMouseMid()
{
    if ( !initiated )
    {
        cerr << "Error: No se puede acceder al raton antes de inicializar el THL" << endl ;
        return false ;
    }

    return (SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(2))==1 ;
}

Region *
THL::getScreenRegion()
{
    if ( !initiated )
    {
        cerr << "Error: No se puede acceder al raton antes de inicializar el THL" << endl ;
        return false ;
    }

    return ScreenRegion ;
}

void THL::PlayMusic(string FileName, int loop)
{
    if ( !initiated )
    {
        cerr << "Error: No se puede reproducir sonido antes de inicializar el THL" << endl ;
        return ;
    }

    if ( Music != NULL )
    {
        Mix_HaltMusic();
        Mix_FreeMusic(Music);
        Music = NULL ;
    }

    Music=Mix_LoadMUS(FileName.c_str());
    if(!Music) 
    {
        cerr << "Error: No se puede reproducir el sonido: " << FileName << endl ;
        return ;
    }

    Mix_PlayMusic(Music, loop);  
    Mix_HookMusicFinished(THL::MusicDone);
    _isPlayingMusic = true ;
}

bool THL::isPlayingMusic()
{
    return _isPlayingMusic;
}

void
THL::StopMusic()
{
    Mix_HaltMusic();
    Mix_FreeMusic(Music);
    Music = NULL ;
    _isPlayingMusic = false ;
}

void
THL::MusicDone()
{
    Mix_FreeMusic(Music);
    Music = NULL ;
    _isPlayingMusic = false ;
}


