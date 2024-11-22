#include "Elements/ElementGroups.h"

std::shared_ptr<UIElement> ElementGroup::getParentElement()
{
    return this->_parentElement;
}

ElementGroup::ElementGroup(CommonObjects *comm) : _comm(comm)
{
}

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

    auto title = std::make_shared<UIText>("header title", this->_comm, G_App::APP_NAME, 18, RGBA(255, 255, 255));
    title->distPosH = RelPos::Center;
    title->alignPosH = RelPos::Center;

    auto close = std::make_shared<UIPictureElement>(G_App::IMAGES_PATH + "close.png", this->_comm);
    close->distPosH = RelPos::End;
    auto closeApp = [this](std::shared_ptr<UIElement> &el, AppEvent &e)
    {
        if (e.mouseEvent.button == Events::MouseButtons::Left)
        {
            // TODO: make event result type
        }
    };
    close->events.addHandler(AppEvent::mouse_click, closeApp);

    this->_parentElement->addChildren({title, close});
}

MainContents::MainContents(CommonObjects *comm) : ElementGroup(comm)
{
    this->createElementGroup();
}

void MainContents::createElementGroup()
{
    int w, h;
    SDL_GetWindowSize(this->_comm->window, &w, &h);
    this->_parentElement = std::make_shared<UIBox>("mainContent", this->_comm, w, h - G_App::HEADER_HEIGHT, 0, RGBA(80, 80, 80));
    this->_parentElement->anchors[Direction::Right] = true;
    this->_parentElement->anchors[Direction::Down] = true;
    this->_parentElement->displayMode = DisplayMode::Distribute;
    this->_parentElement->childrenDistPos = RelPos::Start;
    this->_parentElement->y = G_App::HEADER_HEIGHT;

    auto sidebar = std::make_shared<UIBox>("sidebar", this->_comm, this->SIDEBAR_WIDTH, h - G_App::HEADER_HEIGHT, 0, RGBA(100, 100, 100));
    sidebar->anchors[Direction::Down] = true;

    // auto circle = std::make_shared<UICircle>("circle", this->_comm, 50);
    // sidebar->addChildren({circle});

    this->_parentElement->addChildren({sidebar});
}