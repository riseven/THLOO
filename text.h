#pragma once
#include "sprite.h"
#include "font.h"
#include <string>

class Text :
    public Sprite
{
public:
    static Text * Write(Font *fuente, string texto);
    virtual ~Text(void);
    void Rewrite(string texto);
    virtual void ReceiveCallback(Resource *);
    
protected:
    Text(Font *fuente, Image *_img);

    Font *font;
};
