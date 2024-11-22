#pragma once
#include <string>
#include "SDL.h"
#include "App.h"
#include "CommonObjects.h"

void initSDL(Logger *logger, CommonObjects *comm);
SDL_Texture *surfaceToTexture(CommonObjects *comm, SDL_Surface *surface, bool freeSurface = false);