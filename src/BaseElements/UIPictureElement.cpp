#include "BaseElements/UIPictureElement.h"

UIPictureElement::UIPictureElement(std::string name, CommonObjects *comm) : UIElement(name, comm)
{
    this->_texture = this->loadPictureTexture(name);
    SDL_QueryTexture(this->_texture, NULL, NULL, &this->w, &this->h);
}

SDL_Texture *UIPictureElement::loadPictureTexture(std::string path)
{
    if (this->_texture != nullptr)
    {
        return this->_texture;
    }
    SDL_Texture *texture = surfaceToTexture(this->_comm, Str::endsWith(path, ".bmp") ? SDL_LoadBMP(path.c_str()) : IMG_Load(path.c_str()), true);
    return texture;
}

UIPictureElement::UIPictureElement() : UIElement(name, _comm)
{
}