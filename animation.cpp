#include "animation.h"
#include "basesprite.h"
#include "image.h"
#include <string>
#include <iostream>
using namespace std;


Animation::Animation(void):
    VideoEntity(0, 0, 0, 0, 100, 100), graph(0)
{
}

Animation::~Animation(void)
{
    for ( unsigned i = 0 ; i < sprites.size() ; i++ )
    {
        if ( sprites[i] != NULL )
        {
            sprites[i]->RemoveCallback(this);
        }
    }
}

Animation *
Animation::New(Image *img, int graph_w)
{
    if ( !img )
    {
        cerr << "No se pudo crear la animacion: " << endl ;
        return NULL ;
    }

    Animation *anim = new Animation();

    // Creamos los sprites
    int cant = (img->getWidth())/graph_w;
    
    anim->sprites.resize(cant);
    
    for ( unsigned i = 0 ; i < anim->sprites.size() ; i++ )
    {
        Region *rect = Region::New(i*graph_w, 0, graph_w, img->getHeight());
        anim->sprites[i] = BaseSprite::New(img, rect);
        anim->sprites[i]->AddCallback(anim);
    }
    
    return anim ;
}

void 
Animation::setGraph(int Graph)
{
    graph = Graph ;
}

int
Animation::getGraph()
{
    return graph ;
}

void
Animation::preDrawScreen()
{
    if (graph < 0 || graph > (int)sprites.size())
    {
        cerr << "Error: Codigo de gráfico no valido" << endl ;
        return ;
    }

    sprites[graph]->setPos(x, y);
    sprites[graph]->setAngle(angle);
    sprites[graph]->setScale(scaleX, scaleY);
    sprites[graph]->setControl(controlX, controlY);
    //sprites[graph]->preDrawScreen();
}

void
Animation::onDrawScreen()
{
    if (graph < 0 || graph > (int)sprites.size())
    {
        cerr << "Error: Codigo de gráfico no valido" << endl ;
        return ;
    }

    sprites[graph]->Draw();
}

int
Animation::getNumGraphs()
{
    return (int)sprites.size();
}

void
Animation::setGraphRel(int dg)
{
    graph += dg ;
    while (graph < 0) graph += sprites.size() ;
    while (graph >= sprites.size()) graph -= sprites.size() ;
}

int
Animation::getControlX()
{
    return controlX ;
}

int
Animation::getControlY()
{
    return controlY ;
}

void
Animation::setControl(int cx, int cy)
{
    controlX = cx ;
    controlY = cy ;
}
