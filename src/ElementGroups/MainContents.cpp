#include "ElementGroups/MainContents.h"

MainContents::MainContents(CommonObjects *comm) : ElementGroup(comm)
{
}

std::shared_ptr<UIElement> MainContents::getElement()
{
    if (this->_parentElement != nullptr)
    {
        return this->_parentElement;
    }
    int w, h;
    SDL_GetWindowSize(this->comm->window, &w, &h);
    this->_parentElement = std::make_shared<UIBox>("mainContent", this->comm, w, h - G_App::HEADER_HEIGHT, 0, RGBA(80, 80, 80));
    this->_parentElement->anchors[Direction::Right] = true;
    this->_parentElement->anchors[Direction::Down] = true;
    this->_parentElement->displayMode = DisplayMode::Distribute;
    this->_parentElement->childrenDistPos = RelPos::Start;
    this->_parentElement->y = G_App::HEADER_HEIGHT;

    Sidebar sidebar(this->comm);

    FileExplorer explorer(this->comm);

    UIElement::addChildren(this->_parentElement, {sidebar.getElement(),
                                                  explorer.getElement()});
    return this->_parentElement;
}