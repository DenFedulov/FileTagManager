#include "SDL_Helpers.h"

void initSDL(Logger *logger, CommonObjects *comm)
{
    int result;
    logger->addLog(G_App::APP_NAME + " started");
    logger->dieIf(SDL_Init(SDL_INIT_EVERYTHING) < 0, "Error initializing SDL: ", SDL_GetError());

    logger->addLog("Initializing mixer");
    result = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    logger->dieIf(result != 0, "Error initializing mixer: ", Mix_GetError());

    logger->addLog("Initializing IMG");
    result = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    logger->dieIf(result == 0, "Error initializing IMG: ", IMG_GetError());

    logger->addLog("Initializing TTF");
    result = TTF_Init();
    logger->dieIf(result < 0, "Error initializing TTF: ", TTF_GetError());

    logger->addLog("Creating window");
    comm->window = SDL_CreateWindow(G_App::APP_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, G_App::WIDTH, G_App::HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
    logger->dieIf(!comm->window, "Error Creating window: ", SDL_GetError());
    SDL_SetWindowResizable(comm->window, SDL_TRUE);

    logger->addLog("Creating renderer");
    comm->renderer = SDL_CreateRenderer(comm->window, -1, SDL_RENDERER_ACCELERATED);
    logger->dieIf(!comm->renderer, "Error Creating renderer: ", SDL_GetError());
}

SDL_Texture *surfaceToTexture(CommonObjects *comm, SDL_Surface *surface, bool freeSurface)
{
    if (!surface)
    {
        comm->logger->addErrorLog("Error creating surface: ", (std::string)SDL_GetError() + '\n' + IMG_GetError() + '\n' + TTF_GetError());
        return nullptr;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(comm->renderer, surface);
    if (freeSurface)
    {
        SDL_FreeSurface(surface);
    }
    if (!texture)
    {
        comm->logger->addErrorLog("Error converting surface: ", SDL_GetError());
        return nullptr;
    }
    return texture;
}
