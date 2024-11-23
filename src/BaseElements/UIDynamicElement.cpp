#include "BaseElements/UIDynamicElement.h"

SDL_Surface *UIDynamicElement::makeSurface()
{
    this->freeSurface();
    return SDL_CreateRGBSurfaceWithFormat(0, this->w, this->h, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32);
}

UIDynamicElement::UIDynamicElement(std::string name, CommonObjects *comm) : UIElement(name, comm)
{
}

UIDynamicElement::~UIDynamicElement()
{
    this->freeSurface();
}

void UIDynamicElement::setW(int w)
{
    this->w = w;
    this->updateSurface();
}

void UIDynamicElement::setH(int h)
{
    this->h = h;
    this->updateSurface();
}
