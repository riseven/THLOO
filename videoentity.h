#pragma once
#include "resource.h"

class VideoEntity :
    public Resource
{
public:
    virtual ~VideoEntity(void);
    int getX() const ;
    void setX(int X);
    int getY() const ;
    void setY(int Y);
    int getAngle() const ;
    void setAngle(int Angle) ;
    int getScaleX() const ;
    void setScaleX(int ScaleX) ;
    int getScaleY() const ;
    void setScaleY(int ScaleY) ;
    void setPos(int X, int Y);
    void setPosRel(int dx, int dy);
    void setAngleRel(int da);
    void setScale(int sx, int sy);
    void setScaleRel(int dsx, int dsy);

    static int FixAngle(int a);
    static int FixScale(int s);
    static double DegToRad(int deg);
    static int RadToDeg(double rad);

protected:
    VideoEntity(int x, int y, int z, int angle, int scaleX, int scaleY);
    int x, y;
    int angle ;
    int scaleX, scaleY ;
    //bool change

private:

};
