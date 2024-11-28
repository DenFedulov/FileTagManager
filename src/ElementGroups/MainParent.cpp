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
    auto onResize = [](std::shared_ptr<UIElement> &el, AppEvent &e)
    {
        el->setW(e.windowEvent.data1);
        el->setH(e.windowEvent.data2);
        return 0;
    };
    this->_parentElement->events.addHandler(AppEvent::window_resized, onResize);
    HeaderBar headerBar(this->_comm);
    MainContents contents(this->_comm);

    this->_parentElement->addChildren({headerBar.getParentElement(), contents.getParentElement()});
}