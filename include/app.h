#pragma once
#include <unordered_map>
#include <bitset>
#include <optional>
#include <memory>
#include <algorithm>
#include "AppElementsCommon.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "logger.h"
#include "Str.h"
#include "geometry.h"
#include "BaseElements.h"
#include "ElementGroups.h"
#include "config.h"
#include "EventManager.h"
#include "Layout.h"

namespace App
{
    const int WIDTH = 1280, HEIGHT = 720;
    const int HEADER_HEIGHT = 32;
    const int RESIZE_PADDING = 6;
    const std::string APP_NAME = "FileTagManager";
    const std::string RESOURCES_PATH = "resources/";
    const std::string IMAGES_PATH = RESOURCES_PATH + "images/";
    const std::string AUDIO_PATH = RESOURCES_PATH + "audio/";
    const std::string FONTS_PATH = RESOURCES_PATH + "fonts/";
    const std::string CONFIG_FILENAME = "config.json";
}

struct MouseSelection
{
    std::optional<int> x1, y1, x2, y2;
};

class FileTagManager
{
private:
    bool running = true;
    MouseSelection mSelection;
    void triggerMouseEvent(AppEvent::Type eventEnum, SDL_Event sdlE);
    void triggerKeyEvent(AppEvent::Type eventEnum, SDL_Event sdlE);
    std::unordered_map<std::string, Mix_Chunk *> loadedSounds;
    std::unordered_map<std::string, Mix_Music *> loadedMusic;
    std::vector<std::shared_ptr<UIElement>> loadedElements;
    void sortLoadedElements();

public:
    Logger *logger;
    Config *config;

    std::shared_ptr<UIElement> mainElement;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    FileTagManager(Logger *logger, Config *config);

    void initSDL();
    void initResize();
    void initElements();
    void addElements(std::vector<std::shared_ptr<UIElement>> elements, bool sortElements = true);
    std::shared_ptr<UIElement> getElement(int id);
    Mix_Chunk *getSound(std::string filename);
    Mix_Music *getMusic(std::string filename);
    void drawCoordsVector(CoordsVector coords, int xC, int yC, bool fill = false);
    bool loop();
    void quitSDL();
};
