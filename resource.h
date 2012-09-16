#pragma once

#include <list>
using namespace std;

/*!
 * \brief Clase base de todas las clases del framework.
 *
 * Esta clase se encarga principalmente del control de memoria. Cuando se crea
 * un objeto Resource este se registra automaticamente en la clase THL.
 * Cuando se llama a THL::Quit() se liberan automaticamente todos los recursos.
 * Además, si se intenta instanciar algun recurso antes de llamar a THL::Init()
 * se aborta el programa.
 */
class Resource
{
public:
    virtual ~Resource(void);
    void AddCallback(Resource *);
    void RemoveCallback(Resource *);
    virtual void ReceiveCallback(Resource *);
    virtual void onDrawScreen();
    virtual void preDrawScreen();
    bool operator<(Resource &r);
    bool operator>(Resource &r);
    void setZ(int _z);

protected:
    Resource(int _z=0);

private:
    int z ;
    list <Resource *> Callback;
};
