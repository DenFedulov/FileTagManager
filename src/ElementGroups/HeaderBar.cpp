#include "ElementGroups/HeaderBar.h"

HeaderBar::HeaderBar(CommonObjects *comm) : ElementGroup(comm)
{
    this->createElementGroup();
}

void HeaderBar::createElementGroup()
{
    int w, h;
    SDL_GetWindowSize(this->_comm->window, &w, &h);
    this->_parentElement = std::make_shared<UIBox>("header", this->_comm, w, G_App::HEADER_HEIGHT, 0, RGBA(60, 60, 60));
    this->_parentElement->anchors[Direction::Right] = true;
    this->_parentElement->displayMode = DisplayMode::Distribute;

    auto title = std::make_shared<UIText>("header title", this->_comm, strToWStr(G_App::APP_NAME.c_str()), 18, RGBA(255, 255, 255));
    title->distPosH = RelPos::Center;
    title->alignPosH = RelPos::Center;

    auto close = std::make_shared<UIPictureElement>(G_App::IMAGES_PATH + "close.png", this->_comm);
    close->distPosH = RelPos::End;
    auto closeApp = [this](std::shared_ptr<UIElement> &el, AppEvent &e)
    {
        if (e.mouseEvent.button == Events::MouseButtons::Left)
        {
            return (int)Events::Quit;
        }
        return 0;
    };
    close->events.addHandler(AppEvent::mouse_click, closeApp);

    this->_parentElement->addChildren({title, close});
}