#pragma once
#include "Logger.h"
#include "Config.h"
#include "SDL.h"

struct CommonObjects
{
    Logger *logger = NULL;
    Config *config = NULL;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
};
