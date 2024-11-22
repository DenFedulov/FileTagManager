#pragma once
#include "SDL.h"

class Drawable
{
public:
    virtual ~Drawable() {};
    virtual void draw(SDL_Point *rotationPoint = nullptr, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE) = 0;
};