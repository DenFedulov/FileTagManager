#include "ElementGroups.h"

std::shared_ptr<UIElement> ElementGroup::getParentElement()
{
    return this->_parentElement;
}

ElementGroup::ElementGroup(FileTagManager *app) : _app(app)
{
}

HeaderBar::HeaderBar(FileTagManager *app) : ElementGroup(app)
{
    this->createElementGroup();
}

void HeaderBar::createElementGroup()
{
    int w, h;
    SDL_GetWindowSize(this->_app->window, &w, &h);
    this->_parentElement = std::make_shared<UIBox>("header", this->_app, w, App::HEADER_HEIGHT, 0, RGBA(60, 60, 60));
    this->_parentElement->anchors[Direction::Right] = true;
    this->_parentElement->displayMode = DisplayMode::Distribute;

    auto title = std::make_shared<UIText>("header title", this->_app, App::APP_NAME, 18, RGBA(255, 255, 255));
    title->distPosH = RelPos::Center;
    title->alignPosH = RelPos::Center;

    auto close = std::make_shared<UIPictureElement>(App::IMAGES_PATH + "close.png", this->_app);
    close->distPosH = RelPos::End;
    auto closeApp = [this](std::shared_ptr<UIElement> &el, AppEvent &e)
    {
        if (e.mouseEvent.button == Events::MouseButtons::Left)
        {
            this->_app->quitSDL();
        }
    };
    close->events.addHandler(AppEvent::mouse_click, closeApp);

    this->_parentElement->addChildren({title, close});
}

MainContents::MainContents(FileTagManager *app) : ElementGroup(app)
{
    this->createElementGroup();
}

void MainContents::createElementGroup()
{
    int w, h;
    SDL_GetWindowSize(this->_app->window, &w, &h);
    this->_parentElement = std::make_shared<UIBox>("mainContent", this->_app, w, h - App::HEADER_HEIGHT, 0, RGBA(60, 60, 60));
    this->_parentElement->anchors[Direction::Right] = true;
    this->_parentElement->anchors[Direction::Down] = true;
    this->_parentElement->displayMode = DisplayMode::Distribute;
    this->_parentElement->childrenDistPos = RelPos::Start;
    this->_parentElement->y = App::HEADER_HEIGHT;

    auto sidebar = std::make_shared<UIBox>("sidebar", this->_app, 350, h - App::HEADER_HEIGHT, 10, RGBA(100, 100, 100));
    sidebar->anchors[Direction::Down] = true;

    this->_parentElement->addChildren({sidebar});
}