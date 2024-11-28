#pragma once
#include "Str.h"
#include "SDL_Helpers.h"
#include "SurfaceEditor.h"
#include "BaseElements/UIDynamicElement.h"

class UIPictureElement : public UIDynamicElement
{
private:
    SDL_Surface *loadPicture(std::string path);
    void updateSurface();

public:
    UIPictureElement(std::string name, CommonObjects *comm);
};