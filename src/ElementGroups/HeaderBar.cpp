#include "ElementGroups/HeaderBar.h"

HeaderBar::HeaderBar(CommonObjects *comm) : ElementGroup(comm)
{
}

std::shared_ptr<UIElement> HeaderBar::getElement()
{
    if (this->_parentElement != nullptr)
    {
        return this->_parentElement;
    }
    int w, h;
    SDL_GetWindowSize(this->comm->window, &w, &h);
    this->_parentElement = std::make_shared<UIBox>("header", this->comm, w, G_App::HEADER_HEIGHT, 0, RGBA(60, 60, 60));
    this->_parentElement->anchors[Direction::Right] = true;
    this->_parentElement->displayMode = DisplayMode::Distribute;

    auto title = std::make_shared<UIText>("header title", this->comm, strToWStr(G_App::APP_NAME.c_str()), 18, RGBA(255, 255, 255));
    title->distPosH = RelPos::Center;
    title->alignPosH = RelPos::Center;

    auto close = std::make_shared<UIPictureElement>(G_App::IMAGES_PATH + "close.png", this->comm);
    close->setColor(RGBA(255, 200, 200));
    close->distPosH = RelPos::End;
    auto closeApp = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        if (e.button.button == (int)MouseButtons::Left)
        {
            return EventResult<std::shared_ptr<UIElement>>((int)EventResultType::Quit);
        }
        return EventResult<std::shared_ptr<UIElement>>();
    };
    close->events.addHandler((int)CustomEvent::MOUSE_CLICK, closeApp);

    UIElement::addChildren(this->_parentElement, {title, close});
    this->_parentElement->setZ(0);
    return this->_parentElement;
}