#pragma once
#include "Str.h"
#include "SDL_Helpers.h"
#include "SurfaceEditor.h"
#include "BaseElements/Dynamic.h"
#include "BaseElements/UIElement.h"

class UIPictureElement : public UIElement, Dynamic
{
private:
    RGBA _color;

    SDL_Surface *loadPicture(std::string path);
    void updateSurface();

public:
    void setColor(RGBA color);
    RGBA getColor();

    UIPictureElement(std::string name, CommonObjects *comm);
};