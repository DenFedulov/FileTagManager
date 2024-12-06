#include "ElementGroups/MainParent.h"

MainParent::MainParent(CommonObjects *comm) : ElementGroup(comm)
{
    this->createElementGroup();
}

void MainParent::createElementGroup()
{
    int w, h;
    SDL_GetWindowSize(this->comm->window, &w, &h);
    this->_parentElement = std::make_shared<UIElement>("mainParent", this->comm);
    this->_parentElement->setW(w);
    this->_parentElement->setH(h);
    auto onResize = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            el->setW(e.window.data1);
            el->setH(e.window.data2);
        }
        return EventResult<std::shared_ptr<UIElement>>();
    };
    this->_parentElement->events.addHandler(SDL_WINDOWEVENT, onResize);
    HeaderBar headerBar(this->comm);
    MainContents contents(this->comm);

    this->_parentElement->addChildren({headerBar.getParentElement(), contents.getParentElement()});
}