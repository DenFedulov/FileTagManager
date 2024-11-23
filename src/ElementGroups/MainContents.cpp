#include "ElementGroups/MainContents.h"

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

    this->_parentElement->addChildren({sidebar});
}