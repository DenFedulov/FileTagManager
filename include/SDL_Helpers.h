#pragma once
#include <string>
#include "SDL.h"
#include "app.h"

SDL_Texture *surfaceToTexture(FileTagManager *app, SDL_Surface *surface, bool freeSurface = false);