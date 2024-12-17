#pragma once
#include <deque>
#include "AppEvents.h"
#include "Logger.h"
#include "Config.h"
#include "SDL.h"
#include "AppDB.h"
#include "AppState.h"

struct CommonObjects
{
    Logger *logger = NULL;
    Config *config = NULL;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    AppDB *db = NULL;
    AppState *state = NULL;
    std::deque<std::shared_ptr<AppEvent>> appEventsQueue;
};
