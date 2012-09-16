#pragma once
#include "resource.h"
#include "image.h"
#include "sfont.h"

class Font :
    public Resource
{
public:
    static Font * Load(string FileName);
    virtual ~Font(void);
    Image * getImage() ;
    SFont_Font * getSFont() ;
    
protected:
    Font(Image *Img, SFont_Font *SFont);
    
    Image *img;
    SFont_Font *sfont;
};
