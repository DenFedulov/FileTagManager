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

    auto close = std::make_shared<UIPictureElement>(App::IMAGES_PATH + "close.png", this->app);
    close->distPosH = RelPos::End;
    this->parentElement->addChildren({close});
    auto closeApp = [this](std::shared_ptr<UIElement> &el, AppEvent &e)
    {
        if (e.mouseEvent.button == Events::MouseButtons::Left)
        {
            this->app->quitSDL();
        }
    };
    close->events.addHandler(AppEvent::mouse_click, closeApp);
}
