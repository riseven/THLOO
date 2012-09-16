#pragma once
#include "videoentity.h"
#include "basesprite.h"
#include <vector>

class Animation :
    public VideoEntity
{
public:
    static Animation *New(Image *img, int graph_w);
    virtual ~Animation(void);
    void setGraph(int Graph);
    int getGraph();
    virtual void onDrawScreen();
    virtual void preDrawScreen();
    int getNumGraphs();
    void setGraphRel(int dg);
    int getControlX();
    int getControlY();
    void setControl(int cx, int cy);

protected:
    Animation(void);

    vector <BaseSprite *> sprites ;
    int graph ;
    int controlX;
    int controlY;
};
