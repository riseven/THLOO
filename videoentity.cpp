#include "videoentity.h"

VideoEntity::VideoEntity(int _x, int _y, int _z, int _angle, int _scaleX, int _scaleY):
    Resource(_z),x(_x), y(_y), angle(_angle), scaleX(_scaleX), scaleY(_scaleY)
{
}

VideoEntity::~VideoEntity(void)
{
}

int
VideoEntity::getX() const
{
    return x ;
}

void
VideoEntity::setX(int X)
{
    x = X ;
}

int
VideoEntity::getY() const
{
    return y ;
}

void 
VideoEntity::setY(int Y)
{
    y = Y ;
}

int 
VideoEntity::getAngle() const
{
    return angle;
}

void
VideoEntity::setAngle(int Angle)
{
    angle = Angle ;
    angle = FixAngle(angle);
}

int
VideoEntity::getScaleX() const
{
    return scaleX ;
}

void
VideoEntity::setScaleX(int ScaleX)
{
    scaleX = ScaleX ;
    scaleX = FixScale(scaleX);
}

int
VideoEntity::getScaleY() const
{
    return scaleY ;
}

void
VideoEntity::setScaleY(int ScaleY)
{
    scaleY = ScaleY ;
    scaleY = FixScale(scaleY);
}

void
VideoEntity::setPos(int X, int Y)
{
    x = X ;
    y = Y ;
}

void 
VideoEntity::setPosRel(int dx, int dy)
{
    x += dx ;
    y += dy ;
}

void
VideoEntity::setAngleRel(int da)
{
    angle += da ;
    angle = FixAngle(angle) ;
}

void
VideoEntity::setScale(int sx, int sy)
{
    scaleX = sx ;
    scaleY = sy ;
}

void
VideoEntity::setScaleRel(int dsx, int dsy)
{
    scaleX += dsx ;
    scaleY += dsy ;
    scaleX = FixScale(scaleX);
    scaleY = FixScale(scaleY);
}



int
VideoEntity::FixAngle(int a)
{
    while (a > 360) a-= 360 ;
    while (a < 0 ) a+= 360 ;
    return a ;
}

int
VideoEntity::FixScale(int s)
{
    if (s < 0) s = 0 ;
    return s ;
}

double
VideoEntity::DegToRad(int deg)
{
    const double PiOver180 = 0.0174532925199;
    return deg*PiOver180;
}

int
VideoEntity::RadToDeg(double rad)
{
    const double PiOver180 = 0.0174532925199;
    return (int)(rad/PiOver180);
}

