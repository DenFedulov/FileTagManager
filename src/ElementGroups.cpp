#include "ElementGroups.h"

std::shared_ptr<UIElement> ElementGroup::getParentElement()
{
    return this->parentElement;
}

ElementGroup::ElementGroup(FileTagManager *app) : app(app)
{
}

HeaderBar::HeaderBar(FileTagManager *app) : ElementGroup(app)
{
    this->createElementGroup();
}

void HeaderBar::createElementGroup()
{
    int w, h;
    SDL_GetWindowSize(this->app->window, &w, &h);
    this->parentElement = std::make_shared<UIBox>("header", this->app, w, App::HEADER_HEIGHT, 0, RGBA(60, 60, 60));
    this->parentElement->anchors[Direction::Right] = true;
    this->parentElement->displayMode = DisplayMode::Distribute;

    auto title = std::make_shared<UIText>("header title", this->app, App::APP_NAME, 18, RGBA(255, 255, 255));
    title->distPosH = RelPos::Center;
    title->alignPosH = RelPos::Center;

    auto close = std::make_shared<UIPictureElement>(App::IMAGES_PATH + "close.png", this->app);
    close->distPosH = RelPos::End;
    auto closeApp = [this](std::shared_ptr<UIElement> &el, AppEvent &e)
    {
        if (e.mouseEvent.button == Events::MouseButtons::Left)
        {
            this->app->quitSDL();
        }
    };
    close->events.addHandler(AppEvent::mouse_click, closeApp);

    this->parentElement->addChildren({title, close});
}

MainContents::MainContents(FileTagManager *app) : ElementGroup(app)
{
    this->createElementGroup();
}

void MainContents::createElementGroup()
{
    int w, h;
    SDL_GetWindowSize(this->app->window, &w, &h);
    this->parentElement = std::make_shared<UIBox>("mainContent", this->app, w, h - App::HEADER_HEIGHT, 0, RGBA(60, 60, 60));
    this->parentElement->anchors[Direction::Right] = true;
    this->parentElement->anchors[Direction::Down] = true;
    this->parentElement->displayMode = DisplayMode::Distribute;
    this->parentElement->childrenDistPos = RelPos::Start;
    this->parentElement->y = App::HEADER_HEIGHT;

    auto sidebar = std::make_shared<UIBox>("sidebar", this->app, 350, h - App::HEADER_HEIGHT, 10, RGBA(100, 100, 100));
    sidebar->anchors[Direction::Down] = true;

    this->parentElement->addChildren({sidebar});
}