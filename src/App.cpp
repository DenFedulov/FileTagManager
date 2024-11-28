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
    int w, h;
    SDL_GetWindowSize(this->comm->window, &w, &h);

    MainParent mainParent(this->comm);

    // auto element = std::make_shared<UIBox>("box", this, w, 400, 0, RGBA(), 10, RGBA(100, 100, 100));
    // element->displayMode = DisplayMode::Distribute;
    // element->y = G_App::HEADER_HEIGHT;
    // // auto text = std::make_shared<UIText>("text", this, "test text", element);
    // // text->distPosH = RelPos::Center;
    // // text->distPosV = RelPos::Center;
    // // // element->events.addHandler(AppEvent::mouse_button_down, [](std::shared_ptr<UIElement> &el, AppEvent &e)
    // // // 						   { Mix_PlayChannel(-1, el->getApp()->getSound("failsound.mp3"), 0); });
    // // auto setTextToCoords = [text](std::shared_ptr<UIElement> &el, AppEvent &e)
    // // {
    // //     text->setText(std::to_string(e.mouseEvent.pos.first) + "," + std::to_string(e.mouseEvent.pos.second));
    // // };
    // // element->events.addHandler(AppEvent::mouse_button_down, setTextToCoords);
    // // element->childrenDistPos = RelPos::Start;
    // // element->childrenAlignPos = RelPos::End;
    // element->distDirection = DistDirection::row;
    // element->childrenDistPos = RelPos::Start;
    // element->childrenAlignPos = RelPos::Center;
    // element->childrenPivotPos = RelPos::End;
    // auto child1 = std::make_shared<UIBox>("child1", this, 200, 80, 0, RGBA(255, 0, 0), 5);
    // auto child2 = std::make_shared<UIBox>("child2", this, 100, 30, 0, RGBA(0, 255, 0), 5);
    // auto child3 = std::make_shared<UIBox>("child3", this, 50, 70, 0, RGBA(0, 0, 255), 5);
    // element->addChildren({child1, child2, child3});

    this->addElements({
        mainParent.getParentElement(),
        // element,
        // text,
    });
}

void FileTagManager::sortLoadedElements()
{
    auto sortByZ = [](std::shared_ptr<UIElement> a, std::shared_ptr<UIElement> b) -> bool
    {
        return a->z < b->z;
    };
    std::sort(this->_loadedElements.begin(), this->_loadedElements.end(), sortByZ);
}

void FileTagManager::addElements(const std::vector<std::shared_ptr<UIElement>> &elements, bool sortElements)
{
    for (const auto &element : elements)
    {
        if (element->id > -1)
        {
            continue;
        }
        int id = this->_loadedElements.size();
        element->id = id;
        this->_loadedElements.push_back(element);
        this->addElements(element->childElements, false);
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

void FileTagManager::triggerEvent(AppEvent::Type eventEnum, const SDL_Event &sdlE)
{
    std::vector<int> results;
    std::vector<int> fR;
    std::vector<int> sR;
    for (auto &element : this->_loadedElements)
    {
        AppEvent e;
        e.type = eventEnum;
        switch (eventEnum)
        {
        case AppEvent::mouse_button_down:
        case AppEvent::mouse_button_up:
        case AppEvent::mouse_click:
        case AppEvent::mouse_move:
            e.mouseEvent.pos = {sdlE.button.x, sdlE.button.y};
            e.mouseEvent.pressState = sdlE.button.state;
            e.mouseEvent.button = sdlE.button.button;
            fR = element->events.triggerEvent(eventEnum, element, e);
            if (eventEnum == AppEvent::mouse_button_up && element->checkCollision(sdlE.button.x, sdlE.button.y))
            {
                sR = element->events.triggerEvent(AppEvent::mouse_click, element, e);
            }
            results = Vect::concat<int>(fR, sR);
            break;
        case AppEvent::keyboard_button_down:
        case AppEvent::keyboard_button_up:
        case AppEvent::text_input:
            e.keyEvent.keycode = sdlE.key.keysym.sym;
            e.keyEvent.pressState = sdlE.key.state;
            e.keyEvent.text = sdlE.text.text;
            results = element->events.triggerEvent(eventEnum, element, e);
            break;
        case AppEvent::window_resized:
            e.windowEvent.window = this->comm->window;
            e.windowEvent.data1 = sdlE.window.data1;
            e.windowEvent.data2 = sdlE.window.data2;
            results = element->events.triggerEvent(eventEnum, element, e);
            break;
        }
    }
    this->processEventResults(results);
}

void FileTagManager::processEventResults(const std::vector<int> &results)
{
    for (int result : results)
    {
        switch (result)
        {
        case Events::Result::Quit:
            this->quitSDL();
            break;
        }
    }
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
            this->triggerEvent(AppEvent::keyboard_button_down, evt);
            // std::cout << evt.key.keysym.sym << '\n';
            break;
        case SDL_KEYUP:
            this->triggerEvent(AppEvent::keyboard_button_up, evt);
            // std::cout << evt.key.keysym.sym << '\n';
            break;
        case SDL_MOUSEBUTTONDOWN:
            // std::cout << evt.button.button << '\n';
            this->triggerEvent(AppEvent::mouse_button_down, evt);
            break;
        case SDL_MOUSEBUTTONUP:
            this->triggerEvent(AppEvent::mouse_button_up, evt);
            this->_mSelection.x1.reset();
            this->_mSelection.y1.reset();
            break;
        case SDL_MOUSEMOTION:
            this->triggerEvent(AppEvent::mouse_move, evt);
            break;
        case SDL_TEXTINPUT:
            this->triggerEvent(AppEvent::text_input, evt);
            break;
        case SDL_WINDOWEVENT:
            // std::cout << (int)evt.window.event << ' ' << evt.window.data1 << ' ' << evt.window.data2 << '\n';
            if (evt.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                this->triggerEvent(AppEvent::window_resized, evt);
            }
        default:
            break;
        }
    }
    for (auto &element : this->_loadedElements)
    {
        element->draw();
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
