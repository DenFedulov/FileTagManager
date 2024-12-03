#pragma once
#include <unordered_map>
#include <bitset>
#include <memory>
#include <algorithm>
#include <filesystem>
#include <ranges>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "Geometry.h"
#include "BaseElements/UIElement.h"
#include "ElementGroups/MainParent.h"
#include "Config.h"
#include "EventManager.h"
#include "Layout.h"
#include "CommonObjects.h"
#include "AppGlobals.h"

struct MouseSelection
{
    std::optional<int> x1, y1, x2, y2;
};

class FileTagManager
{
private:
    bool _running = true;
    MouseSelection _mSelection;
    std::unordered_map<std::string, Mix_Chunk *> _loadedSounds;
    std::unordered_map<std::string, Mix_Music *> _loadedMusic;
    std::vector<std::shared_ptr<UIElement>> _loadedElements;

    void triggerEvent(const SDL_Event &sdlE);
    bool processEventResults(const std::vector<int> &results);
    void sortLoadedElements();

public:
    CommonObjects *comm = NULL;

    FileTagManager(CommonObjects *comm);

    void initResize();
    void initElements();
    void addElements(const std::vector<std::shared_ptr<UIElement>> &elements, int order = 0, bool sortElements = true);
    std::shared_ptr<UIElement> getElement(int id);
    Mix_Chunk *getSound(std::string filename);
    Mix_Music *getMusic(std::string filename);
    void drawCoordsVector(const CoordsVector &coords, int xC, int yC, bool fill = false);
    bool loop();
    void quitSDL();
};
