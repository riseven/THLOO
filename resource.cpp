#include <iostream>
#include "resource.h"
#include "thl.h"
using namespace std;



/*!
 * \brief Registra el recurso en la clase THL
 *
 * Si se invoca el constructor de la clase Resource y el THL no ha sido 
 * inicializado provocará la finalización del programa. Si el THL ha sido
 * inicializado instancia el recurso y lo registra en la clase THL.
 * Cuando se invoque THL::Quit() todos los recursos serán automáticamente
 * eliminados.
 * El constructor es privato para impedir que se puedan crear objetos
 * estaticos de esta clase. Todas las clases derivadas de esta deberían
 * tener el constructor privado junto con un constructor static que cree
 * el objeto dinamicamente.
 */
Resource::Resource(int _z):
    z(_z)
{
    if ( !THL::getInitiated() )
    {
        cerr << "Error crítico: No se pueden instanciar recursos antes de inicializar el THL" << endl ;
        exit(-1);
    }

    // Registramos el recurso en el THL
    THL::RegisterResource(this);
}

/*!
 * \brief Elimina el registro del recurso en el THL
 *
 * Primero realiza Callbacks a todos los recursos que dependen de este recurso,
 * despues borra el registro del recurso en el THL.
 */
Resource::~Resource(void)
{
    Resource *r;
    while ( !Callback.empty() )
    {
        r = Callback.front() ;
        delete r ;
        if ( r == Callback.front() )
        {
            Callback.remove(Callback.front());
        }
    }

    THL::UnregisterResource(this);
}


void
Resource::AddCallback(Resource *rsc)
{
    Callback.push_back(rsc);
}

void
Resource::RemoveCallback(Resource *rsc)
{
    Callback.remove(rsc);
}

void
Resource::ReceiveCallback(Resource *rsc)
{
    delete this ;
}

void
Resource::onDrawScreen()
{
}

void
Resource::preDrawScreen()
{
}

bool
Resource::operator <(Resource &r)
{
    if ( z < r.z )
        return true ;
    return false ;
}

bool
Resource::operator >(Resource &r)
{
    if ( z > r.z )
        return true ;
    return false ;
}

void
Resource::setZ(int _z)
{
    z = _z ;
    THL::UnregisterResource(this);
    THL::RegisterResource(this);
}



