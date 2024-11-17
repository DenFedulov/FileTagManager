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
        else if (y < App::HEADER_HEIGHT)
        {
            result = SDL_HITTEST_DRAGGABLE;
        }

        app->currentHittest = result;
        return result;
    };
    SDL_SetWindowHitTest(this->window, HitTestCallback, this);
}

void FileTagManager::initElements()
{
    this->mainElement = std::make_shared<UIElement>("main", this, true);
    int w, h;
    SDL_GetWindowSize(this->window, &w, &h);
    auto element = std::make_shared<UIBox>("button", this, w, App::HEADER_HEIGHT + 50, 0, RGBA(), 10, RGBA(100, 100, 100));
    auto text = std::make_shared<UIText>("text", this, "test text", element);
    element->childElements.push_back(text);
    text->pivotPositionV = PivotPosition::End;
    // element->events.addHandler(AppEvent::mouse_button_down, [](std::shared_ptr<UIElement> &el, AppEvent &e)
    // 						   { Mix_PlayChannel(-1, el->getApp()->getSound("failsound.mp3"), 0); });
    auto setTextToCoords = [text](std::shared_ptr<UIElement> &el, AppEvent &e)
    {
        text->setText(std::to_string(e.mouseEvent.pos.first) + "," + std::to_string(e.mouseEvent.pos.second));
    };
    element->events.addHandler(AppEvent::mouse_button_down, setTextToCoords);
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
    for (auto &[name, element] : this->loadedElements)
    {
        AppEvent e;
        e.type = eventEnum;
        e.mouseEvent.pos = {sdlE.button.x, sdlE.button.y};
        e.mouseEvent.pressState = sdlE.button.state;
        e.mouseEvent.which = sdlE.button.which;
        element->events.triggerEvent(eventEnum, element, e);
        if (eventEnum == AppEvent::mouse_button_down && element->checkCollision(sdlE.button.x, sdlE.button.y))
        {
            element->events.triggerEvent(AppEvent::mouse_click, element, e);
        }
    }
}

void FileTagManager::triggerKeyEvent(AppEvent::Type eventEnum, SDL_Event sdlE)
{
    for (auto &[name, element] : this->loadedElements)
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
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->renderer);
    SDL_Event evt;
    SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
    while (SDL_PollEvent(&evt))
    {
        switch (evt.type)
        {
        case SDL_QUIT:
            this->quitSDL();
            return false;
        case SDL_KEYDOWN:
            this->triggerKeyEvent(AppEvent::keyboard_button_down, evt);
            std::cout << evt.key.keysym.sym << '\n';
            break;
        case SDL_KEYUP:
            this->triggerKeyEvent(AppEvent::keyboard_button_up, evt);
            std::cout << evt.key.keysym.sym << '\n';
            break;
        case SDL_MOUSEBUTTONDOWN:
            std::cout << evt.button.button << '\n';
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
        default:
            break;
        }
    }
    this->mainElement->draw();
    SDL_RenderPresent(this->renderer);

    return true;
}

std::shared_ptr<UIElement> FileTagManager::getElement(std::string name)
{
    if (this->loadedElements.contains(name))
    {
        return this->loadedElements.at(name);
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
