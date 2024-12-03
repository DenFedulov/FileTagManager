#include "App.h"

FileTagManager::FileTagManager(CommonObjects *comm) : comm(comm)
{
}

void FileTagManager::initResize()
{
    auto HitTestCallback = [](SDL_Window *win, const SDL_Point *area, void *data) -> SDL_HitTestResult
    {
        int w, h;
        SDL_GetWindowSize(win, &w, &h);
        int x = area->x, y = area->y;
        SDL_HitTestResult result = SDL_HITTEST_NORMAL;
        if (y < G_App::RESIZE_PADDING)
        {
            if (x < G_App::RESIZE_PADDING)
            {
                result = SDL_HITTEST_RESIZE_TOPLEFT;
            }
            else if (x > w - G_App::RESIZE_PADDING)
            {
                result = SDL_HITTEST_RESIZE_TOPRIGHT;
            }
            else
            {
                result = SDL_HITTEST_RESIZE_TOP;
            }
        }
        else if (y > h - G_App::RESIZE_PADDING)
        {
            if (x < G_App::RESIZE_PADDING)
            {
                result = SDL_HITTEST_RESIZE_BOTTOMLEFT;
            }
            else if (x > w - G_App::RESIZE_PADDING)
            {
                result = SDL_HITTEST_RESIZE_BOTTOMRIGHT;
            }
            else
            {
                result = SDL_HITTEST_RESIZE_BOTTOM;
            }
        }
        else if (x < G_App::RESIZE_PADDING)
        {
            result = SDL_HITTEST_RESIZE_LEFT;
        }
        else if (x > w - G_App::RESIZE_PADDING)
        {
            result = SDL_HITTEST_RESIZE_RIGHT;
        }
        else if (x > w - G_App::HEADER_HEIGHT)
        {
            result = SDL_HITTEST_NORMAL;
        }
        else if (y < G_App::HEADER_HEIGHT)
        {
            result = SDL_HITTEST_DRAGGABLE;
        }

        return result;
    };
    SDL_SetWindowHitTest(this->comm->window, HitTestCallback, 0);
}

void FileTagManager::initElements()
{
    MainParent mainParent(this->comm);

    this->addElements({
        mainParent.getParentElement(),
    });
}

void FileTagManager::sortLoadedElements()
{
    auto sortByZ = [](std::shared_ptr<UIElement> a, std::shared_ptr<UIElement> b) -> bool
    {
        if (a->getZ() < b->getZ())
        {
            return true;
        }
        else if (a->getZ() == b->getZ())
        {
            return a->defaultRenderOrder < b->defaultRenderOrder;
        }
        return false;
    };
    std::sort(this->_loadedElements.begin(), this->_loadedElements.end(), sortByZ);
}

void FileTagManager::addElements(const std::vector<std::shared_ptr<UIElement>> &elements, int order, bool sortElements)
{
    for (const auto &element : elements)
    {
        if (element->id > -1)
        {
            continue;
        }
        int id = this->_loadedElements.size();
        element->defaultRenderOrder = order;
        element->id = id;
        this->_loadedElements.push_back(element);
        this->addElements(element->childElements, order + 1, false);
    }
    if (sortElements)
    {
        this->sortLoadedElements();
    }
}

void FileTagManager::drawCoordsVector(const CoordsVector &coords, int xC, int yC, bool fill)
{
    CoordsVector addedCoords = Geometry::addToCoords(coords, xC, yC);
    for (auto pair : addedCoords)
    {
        if (fill)
        {
            SDL_RenderDrawLine(this->comm->renderer, pair.first, pair.second, xC, pair.second);
        }
        else
        {
            SDL_RenderDrawPoint(this->comm->renderer, pair.first, pair.second);
        }
    }
}

void FileTagManager::triggerEvent(const SDL_Event &event)
{
    for (auto &element : std::ranges::reverse_view(this->_loadedElements))
    {
        std::vector<int> results;
        if (event.type == SDL_MOUSEBUTTONUP && element->checkCollision(event.button.x, event.button.y))
        {
            results = Vect::concat<int>(results, element->events.triggerEvent((int)CustomEvent::MOUSE_CLICK, element, event));
        }

        results = Vect::concat<int>(results, element->events.triggerEvent(event.type, element, event));
        if (this->processEventResults(results))
        {
            break;
        }
    }
}

bool FileTagManager::processEventResults(const std::vector<int> &results)
{
    int stopPropagation = false;
    for (int result : results)
    {
        switch (result)
        {
        case (int)EventResult::Quit:
            this->quitSDL();
            break;
        case (int)EventResult::StopPropagation:
            stopPropagation = true;
            break;
        }
    }
    return stopPropagation;
}

bool FileTagManager::loop()
{
    if (!this->_running)
    {
        return false;
    }
    SDL_RenderClear(this->comm->renderer);
    SDL_Event evt;
    while (SDL_PollEvent(&evt))
    {
        switch (evt.type)
        {
        case SDL_QUIT:
            this->quitSDL();
            return false;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEWHEEL:
        case SDL_MOUSEMOTION:
        case SDL_TEXTINPUT:
        case SDL_WINDOWEVENT:
            this->triggerEvent(evt);
            break;
        }
    }

    if (this->_loadedElements.size() > 0)
    {
        this->_loadedElements.at(0)->draw();
    }
    for (auto &element : this->_loadedElements)
    {
        element->render();
    }
    SDL_RenderPresent(this->comm->renderer);

    return true;
}

std::shared_ptr<UIElement> FileTagManager::getElement(int id)
{
    if (id < this->_loadedElements.size())
    {
        return this->_loadedElements.at(id);
    }
    return nullptr;
}

Mix_Chunk *FileTagManager::getSound(std::string filename)
{
    std::string path = G_App::AUDIO_PATH + filename;
    if (this->_loadedSounds.contains(filename))
    {
        return this->_loadedSounds.at(filename);
    }
    Mix_Chunk *sound = Mix_LoadWAV(path.c_str());
    if (!sound)
    {
        this->comm->logger->addErrorLog("Error loading sound: ", Mix_GetError());
        return nullptr;
    }
    this->_loadedSounds.emplace(filename, sound);
    return sound;
}

Mix_Music *FileTagManager::getMusic(std::string filename)
{
    if (this->_loadedMusic.contains(filename))
    {
        return this->_loadedMusic.at(filename);
    }
    Mix_Music *music = Mix_LoadMUS((G_App::AUDIO_PATH + filename).c_str());
    if (!music)
    {
        this->comm->logger->addErrorLog("Error loading music: ", Mix_GetError());
        return nullptr;
    }
    this->_loadedMusic.emplace(filename, music);
    return music;
}

void FileTagManager::quitSDL()
{
    this->_running = false;
    for (auto const &[filename, sound] : this->_loadedSounds)
    {
        Mix_FreeChunk(sound);
    }
    for (auto const &[filename, music] : this->_loadedMusic)
    {
        Mix_FreeMusic(music);
    }
    SDL_DestroyWindow(this->comm->window);
    SDL_DestroyRenderer(this->comm->renderer);
    IMG_Quit();
    SDL_Quit();
}
