#pragma once
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "AppGlobals.h"
#include "CommonObjects.h"

void initSDL(Logger *logger, CommonObjects *comm);
SDL_Texture *surfaceToTexture(CommonObjects *comm, SDL_Surface *surface, bool freeSurface = false);