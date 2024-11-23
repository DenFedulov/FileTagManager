#pragma once
#include "Str.h"
#include "SDL_Helpers.h"
#include "BaseElements/UIElement.h"

class UIPictureElement : public UIElement
{
private:
    SDL_Texture *loadPictureTexture(std::string filename);

public:
    UIPictureElement();
    UIPictureElement(std::string name, CommonObjects *comm);
};