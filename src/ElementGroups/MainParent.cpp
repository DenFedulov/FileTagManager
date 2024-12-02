#include "ElementGroups/MainParent.h"

MainParent::MainParent(CommonObjects *comm) : ElementGroup(comm)
{
    this->createElementGroup();
}

void MainParent::createElementGroup()
{
    int w, h;
    SDL_GetWindowSize(this->_comm->window, &w, &h);
    this->_parentElement = std::make_shared<UIElement>("mainParent", this->_comm);
    this->_parentElement->setW(w);
    this->_parentElement->setH(h);
    auto onResize = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        el->setW(e.window.data1);
        el->setH(e.window.data2);
        return 0;
    };
    this->_parentElement->events.addHandler(SDL_WINDOWEVENT, onResize);
    HeaderBar headerBar(this->_comm);
    MainContents contents(this->_comm);

    this->_parentElement->addChildren({headerBar.getParentElement(), contents.getParentElement()});
}