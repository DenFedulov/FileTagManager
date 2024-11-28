#include "BaseElements/UIPictureElement.h"

UIPictureElement::UIPictureElement(std::string name, CommonObjects *comm) : UIDynamicElement(name, comm)
{
    this->updateSurface();
}

SDL_Surface *UIPictureElement::loadPicture(std::string path)
{
    if (Str::endsWith(path, ".bmp"))
    {
        return SDL_LoadBMP(path.c_str());
    }
    return IMG_Load(path.c_str());
}

void UIPictureElement::updateSurface()
{
    this->freeSurface();
    SDL_Surface *defaultSurface = this->loadPicture(this->name);
    this->_surface = SDL_ConvertSurfaceFormat(defaultSurface, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32, 0);
    this->w = this->_surface->w;
    this->h = this->_surface->h;
    SDL_FreeSurface(defaultSurface);
    SurfaceEditor editor(this->_surface);
    editor.multiply(this->_color);
    this->freeTexture();
    this->_texture = surfaceToTexture(this->_comm, this->_surface);
}
