#include "app.h"

FileTagManager::FileTagManager(Logger *logger, Config *config)
{
    this->logger = logger;
    this->config = config;
}

void FileTagManager::initSDL()
{
    int result;
    this->logger->addLog(App::APP_NAME + " started");
    this->logger->dieIf(SDL_Init(SDL_INIT_EVERYTHING) < 0, "Error initializing SDL: ", SDL_GetError());

    this->logger->addLog("Initializing mixer");
    result = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    this->logger->dieIf(result != 0, "Error initializing mixer: ", Mix_GetError());

    this->logger->addLog("Initializing IMG");
    result = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    this->logger->dieIf(result == 0, "Error initializing IMG: ", IMG_GetError());

    this->logger->addLog("Initializing TTF");
    result = TTF_Init();
    this->logger->dieIf(result < 0, "Error initializing TTF: ", TTF_GetError());

    this->logger->addLog("Creating window");
    this->window = SDL_CreateWindow(App::APP_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, App::WIDTH, App::HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
    this->logger->dieIf(!this->window, "Error Creating window: ", SDL_GetError());
    SDL_SetWindowResizable(this->window, SDL_TRUE);

    this->logger->addLog("Creating renderer");
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
    this->logger->dieIf(!this->renderer, "Error Creating renderer: ", SDL_GetError());
}

void FileTagManager::initResize()
{
    auto HitTestCallback = [](SDL_Window *win, const SDL_Point *area, void *data) -> SDL_HitTestResult
    {
        int w, h;
        FileTagManager *app = static_cast<FileTagManager *>(data);
        SDL_GetWindowSize(win, &w, &h);
        int x = area->x, y = area->y;
        SDL_HitTestResult result = SDL_HITTEST_NORMAL;
        if (y < App::RESIZE_PADDING)
        {
            if (x < App::RESIZE_PADDING)
            {
                result = SDL_HITTEST_RESIZE_TOPLEFT;
            }
            else if (x > w - App::RESIZE_PADDING)
            {
                result = SDL_HITTEST_RESIZE_TOPRIGHT;
            }
            else
            {
                result = SDL_HITTEST_RESIZE_TOP;
            }
        }
        else if (y > h - App::RESIZE_PADDING)
        {
            if (x < App::RESIZE_PADDING)
            {
                result = SDL_HITTEST_RESIZE_BOTTOMLEFT;
            }
            else if (x > w - App::RESIZE_PADDING)
            {
                result = SDL_HITTEST_RESIZE_BOTTOMRIGHT;
            }
            else
            {
                result = SDL_HITTEST_RESIZE_BOTTOM;
            }
        }
        else if (x < App::RESIZE_PADDING)
        {
            result = SDL_HITTEST_RESIZE_LEFT;
        }
        else if (x > w - App::RESIZE_PADDING)
        {
            result = SDL_HITTEST_RESIZE_RIGHT;
        }
        else if (x > w - App::HEADER_HEIGHT)
        {
            result = SDL_HITTEST_NORMAL;
        }
        else if (y < App::HEADER_HEIGHT)
        {
            result = SDL_HITTEST_DRAGGABLE;
        }

        return result;
    };
    SDL_SetWindowHitTest(this->window, HitTestCallback, this);
}

void FileTagManager::initElements()
{
    this->mainElement = std::make_shared<UIElement>("main", this, true);
    int w, h;
    SDL_GetWindowSize(this->window, &w, &h);

    auto header = std::make_shared<UIBox>("header", this, w, App::HEADER_HEIGHT, 0, RGBA(60, 60, 60));
    header->anchors[Direction::Right] = true;
    header->displayMode = DisplayMode::Distribute;
    auto close = std::make_shared<UIPictureElement>(App::IMAGES_PATH + "close.png", this);
    close->distPosH = RelPos::End;
    close->setParent(header);
    auto closeApp = [this](std::shared_ptr<UIElement> &el, AppEvent &e)
    {
        if (e.mouseEvent.button == Events::MouseButtons::Left)
        {
            this->quitSDL();
        }
    };
    close->events.addHandler(AppEvent::mouse_click, closeApp);

    auto element = std::make_shared<UIBox>("box", this, w, 400, 0, RGBA(), 10, RGBA(100, 100, 100));
    element->displayMode = DisplayMode::Distribute;
    element->y = App::HEADER_HEIGHT;
    // auto text = std::make_shared<UIText>("text", this, "test text", element);
    // text->distPosH = RelPos::Center;
    // text->distPosV = RelPos::Center;
    // // element->events.addHandler(AppEvent::mouse_button_down, [](std::shared_ptr<UIElement> &el, AppEvent &e)
    // // 						   { Mix_PlayChannel(-1, el->getApp()->getSound("failsound.mp3"), 0); });
    // auto setTextToCoords = [text](std::shared_ptr<UIElement> &el, AppEvent &e)
    // {
    //     text->setText(std::to_string(e.mouseEvent.pos.first) + "," + std::to_string(e.mouseEvent.pos.second));
    // };
    // element->events.addHandler(AppEvent::mouse_button_down, setTextToCoords);
    // element->childrenDistPos = RelPos::Start;
    // element->childrenAlignPos = RelPos::End;
    element->distDirection = DistDirection::row;
    element->childrenDistPos = RelPos::Start;
    element->childrenAlignPos = RelPos::Center;
    element->childrenPivotPos = RelPos::End;
    auto child1 = std::make_shared<UIBox>("child1", this, 200, 80, 0, RGBA(255, 0, 0), 5);
    auto child2 = std::make_shared<UIBox>("child2", this, 100, 30, 0, RGBA(0, 255, 0), 5);
    auto child3 = std::make_shared<UIBox>("child2", this, 50, 70, 0, RGBA(0, 0, 255), 5);
    element->addChildren({child1, child2, child3});
    this->addElements({
        this->mainElement,
        header,
        close,
        element,
        child1,
        child2,
        child3,
        // text,
    });
}

void FileTagManager::sortLoadedElements()
{
    auto sortByZ = [](std::shared_ptr<UIElement> a, std::shared_ptr<UIElement> b) -> bool
    {
        return a->z < b->z;
    };
    std::sort(this->loadedElements.begin(), this->loadedElements.end(), sortByZ);
}

void FileTagManager::addElements(std::vector<std::shared_ptr<UIElement>> elements)
{
    for (const auto &element : elements)
    {
        int id = this->loadedElements.size();
        element->id = id;
        if (element->getParent() == NULL && element->name != "main")
        {
            element->setParent(this->mainElement);
        }
        this->loadedElements.push_back(element);
    }
    this->sortLoadedElements();
}

void FileTagManager::drawCoordsVector(CoordsVector coords, int xC, int yC, bool fill)
{
    coords = Geometry::addToCoords(coords, xC, yC);
    for (auto pair : coords)
    {
        if (fill)
        {
            SDL_RenderDrawLine(this->renderer, pair.first, pair.second, xC, pair.second);
        }
        else
        {
            SDL_RenderDrawPoint(this->renderer, pair.first, pair.second);
        }
    }
}

void FileTagManager::triggerMouseEvent(AppEvent::Type eventEnum, SDL_Event sdlE)
{
    for (auto &element : this->loadedElements)
    {
        AppEvent e;
        e.type = eventEnum;
        e.mouseEvent.pos = {sdlE.button.x, sdlE.button.y};
        e.mouseEvent.pressState = sdlE.button.state;
        e.mouseEvent.button = sdlE.button.button;
        element->events.triggerEvent(eventEnum, element, e);
        if (eventEnum == AppEvent::mouse_button_up && element->checkCollision(sdlE.button.x, sdlE.button.y))
        {
            element->events.triggerEvent(AppEvent::mouse_click, element, e);
        }
    }
}

void FileTagManager::triggerKeyEvent(AppEvent::Type eventEnum, SDL_Event sdlE)
{
    for (auto &element : this->loadedElements)
    {
        AppEvent e;
        e.type = eventEnum;
        e.keyEvent.keycode = sdlE.key.keysym.sym;
        e.keyEvent.pressState = sdlE.key.state;
        e.keyEvent.text = sdlE.text.text;
        element->events.triggerEvent(eventEnum, element, e);
    }
}

bool FileTagManager::loop()
{
    if (!this->running)
    {
        return false;
    }
    SDL_RenderClear(this->renderer);
    SDL_Event evt;
    while (SDL_PollEvent(&evt))
    {
        switch (evt.type)
        {
        case SDL_QUIT:
            this->quitSDL();
            return false;
        case SDL_KEYDOWN:
            this->triggerKeyEvent(AppEvent::keyboard_button_down, evt);
            // std::cout << evt.key.keysym.sym << '\n';
            break;
        case SDL_KEYUP:
            this->triggerKeyEvent(AppEvent::keyboard_button_up, evt);
            // std::cout << evt.key.keysym.sym << '\n';
            break;
        case SDL_MOUSEBUTTONDOWN:
            // std::cout << evt.button.button << '\n';
            this->triggerMouseEvent(AppEvent::mouse_button_down, evt);
            break;
        case SDL_MOUSEBUTTONUP:
            this->triggerMouseEvent(AppEvent::mouse_button_up, evt);
            this->mSelection.x1.reset();
            this->mSelection.y1.reset();
            break;
        case SDL_MOUSEMOTION:
            this->triggerMouseEvent(AppEvent::mouse_move, evt);
            break;
        case SDL_TEXTINPUT:
            this->triggerKeyEvent(AppEvent::text_input, evt);
            break;
        case SDL_WINDOWEVENT:
            // std::cout << (int)evt.window.event << ' ' << evt.window.data1 << ' ' << evt.window.data2 << '\n';
            if (evt.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                this->mainElement->setW(evt.window.data1);
                this->mainElement->setH(evt.window.data2);
            }
        default:
            break;
        }
    }
    for (auto &element : this->loadedElements)
    {
        element->draw();
    }
    SDL_RenderPresent(this->renderer);

    return true;
}

std::shared_ptr<UIElement> FileTagManager::getElement(int id)
{
    if (id < this->loadedElements.size())
    {
        return this->loadedElements.at(id);
    }
    return nullptr;
}

Mix_Chunk *FileTagManager::getSound(std::string filename)
{
    std::string path = App::AUDIO_PATH + filename;
    if (this->loadedSounds.contains(filename))
    {
        return this->loadedSounds.at(filename);
    }
    Mix_Chunk *sound = Mix_LoadWAV(path.c_str());
    if (!sound)
    {
        this->logger->addErrorLog("Error loading sound: ", Mix_GetError());
        return nullptr;
    }
    this->loadedSounds.emplace(filename, sound);
    return sound;
}

Mix_Music *FileTagManager::getMusic(std::string filename)
{
    if (this->loadedMusic.contains(filename))
    {
        return this->loadedMusic.at(filename);
    }
    Mix_Music *music = Mix_LoadMUS((App::AUDIO_PATH + filename).c_str());
    if (!music)
    {
        this->logger->addErrorLog("Error loading music: ", Mix_GetError());
        return nullptr;
    }
    this->loadedMusic.emplace(filename, music);
    return music;
}

void FileTagManager::quitSDL()
{
    this->running = false;
    for (auto const &[filename, sound] : this->loadedSounds)
    {
        Mix_FreeChunk(sound);
    }
    for (auto const &[filename, music] : this->loadedMusic)
    {
        Mix_FreeMusic(music);
    }
    SDL_DestroyWindow(this->window);
    SDL_DestroyRenderer(this->renderer);
    IMG_Quit();
    SDL_Quit();
}
