#include "SDL_Helpers.h"

SDL_Texture *surfaceToTexture(FileTagManager *app, SDL_Surface *surface, bool freeSurface)
{
    if (!surface)
    {
        app->logger->addErrorLog("Error creating surface: ", (std::string)SDL_GetError() + '\n' + IMG_GetError() + '\n' + TTF_GetError());
        return nullptr;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app->renderer, surface);
    if (freeSurface)
    {
        SDL_FreeSurface(surface);
    }
    if (!texture)
    {
        app->logger->addErrorLog("Error converting surface: ", SDL_GetError());
        return nullptr;
    }
    return texture;
}