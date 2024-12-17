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
        mainParent.getElement(),
    });
}

void FileTagManager::updateLoadedElements()
{
    this->_loadedElements.updateVec();
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
    std::sort(this->_loadedElements.vec.begin(), this->_loadedElements.vec.end(), sortByZ);
}

void FileTagManager::addElements(const std::vector<std::shared_ptr<UIElement>> &elements, bool updateElements)
{
    for (const auto &element : elements)
    {
        element->id = this->_loadedElements.add(element);
        std::cout << "added element " << element->name << " with id " << element->id << '\n';
        this->addElements(element->childElements.vec, false);
    }
    if (updateElements)
    {
        this->updateLoadedElements();
    }
}

void FileTagManager::removeElements(std::vector<size_t> indexes)
{
    std::cout << "removing elements\n";
    for (auto id : indexes)
    {
        this->_loadedElements.erase(id);
    }
    this->updateLoadedElements();
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
    std::vector<EventResult<std::shared_ptr<UIElement>>> allResults;
    for (auto &element : std::ranges::reverse_view(this->_loadedElements.vec))
    {
        std::vector<EventResult<std::shared_ptr<UIElement>>> results;
        if (event.type == SDL_MOUSEBUTTONUP && element->checkCollision(event.button.x, event.button.y))
        {
            results = Vect::concat<EventResult<std::shared_ptr<UIElement>>>(results, element->events.triggerEvent((int)CustomEvent::MOUSE_CLICK, element, event));
        }

        results = Vect::concat<EventResult<std::shared_ptr<UIElement>>>(results, element->events.triggerEvent(event.type, element, event));
        allResults = Vect::concat<EventResult<std::shared_ptr<UIElement>>>(allResults, results);
        if (this->checkStopPropagation(results))
        {
            break;
        }
    }
    this->processEventResults(allResults);
}

void FileTagManager::triggerEvent(const std::shared_ptr<AppEvent> &event)
{
    std::vector<EventResult<std::shared_ptr<UIElement>>> allResults;
    for (auto &element : std::ranges::reverse_view(this->_loadedElements.vec))
    {
        std::vector<EventResult<std::shared_ptr<UIElement>>> results;
        results = element->appEvents.triggerEvent((int)event->type, element, event);
        allResults = Vect::concat<EventResult<std::shared_ptr<UIElement>>>(allResults, results);
        if (this->checkStopPropagation(results))
        {
            break;
        }
    }
    this->processEventResults(allResults);
}

void FileTagManager::processEventResults(const std::vector<EventResult<std::shared_ptr<UIElement>>> &results)
{
    for (auto result : results)
    {
        switch (result.type)
        {
        case (int)EventResultType::Quit:
            this->quitSDL();
            break;
        case (int)EventResultType::RemoveElement:
            result.data->parentElement->removeChild(result.data->childId);
            this->removeElements(result.data->getFamilyIndicies());
            break;
        case (int)EventResultType::AddElement:
            this->addElements({result.data}, false);
            break;
        case (int)EventResultType::AddElementToMain:
            UIElement::addChildren(this->_loadedElements.map.at(0), {result.data});
            this->addElements({result.data});
            break;
        }
    }
}

bool FileTagManager::checkStopPropagation(const std::vector<EventResult<std::shared_ptr<UIElement>>> &results)
{
    for (const auto &result : results)
    {
        if (result.type == (int)EventResultType::StopPropagation)
        {
            return true;
        }
    }
    return false;
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

    while (!this->comm->appEventsQueue.empty())
    {
        this->triggerEvent(this->comm->appEventsQueue.front());
        this->comm->appEventsQueue.pop_front();
    }

    if (this->_loadedElements.map.size() > 0)
    {
        this->_loadedElements.map.at(0)->draw();
    }
    for (auto &element : this->_loadedElements.vec)
    {
        element->render();
    }
    SDL_RenderPresent(this->comm->renderer);

    return true;
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
