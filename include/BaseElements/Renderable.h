#pragma once
#include "SDL.h"
#include "Layout.h"

class Renderable
{
public:
    virtual ~Renderable() {};
    virtual void draw() = 0;
    virtual void render(SDL_Point *rotationPoint = nullptr, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE) = 0;
};