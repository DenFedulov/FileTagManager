#pragma once
#include "Logger.h"
#include "Config.h"
#include "SDL.h"
#include "AppDB.h"

struct CommonObjects
{
    Logger *logger = NULL;
    Config *config = NULL;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    AppDB *db = NULL;
};
