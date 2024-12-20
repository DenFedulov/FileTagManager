#include "ElementGroups/BaseContextMenu.h"

BaseContextMenu::BaseContextMenu(CommonObjects *comm, std::vector<std::shared_ptr<UIElement>> options, int x, int y)
    : ElementGroup(comm),
      x(x),
      y(y),
      options(options)
{
}

std::shared_ptr<UIElement> BaseContextMenu::getElement()
{
    if (this->_parentElement != nullptr)
    {
        return this->_parentElement;
    }
    int winW, winH;
    SDL_GetWindowSize(this->comm->window, &winW, &winH);
    this->_parentElement = std::make_shared<UIBox>("context menu", this->comm, this->width, 1, 0, RGBA(60, 60, 60), 2, RGBA(140, 140, 140));
    this->_parentElement->posMode = PositionMode::Absolute;
    this->_parentElement->displayMode = DisplayMode::Distribute;
    this->_parentElement->distDirection = DistDirection::row;
    this->_parentElement->childrenDistPos = RelPos::Start;
    this->_parentElement->childrenAlignPos = RelPos::Start;

    UIElement::addChildren(this->_parentElement, this->options);
    this->_parentElement->setZ(1);
    this->_parentElement->setH(this->_parentElement->getChildHSum());
    this->_parentElement->x = (this->x + this->width < winW ? this->x : this->x - this->width);
    this->_parentElement->y = (this->y + this->_parentElement->getH() < winH ? this->y : this->y - this->_parentElement->getH());
    auto clickRemoveMenu = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        EventResult<std::shared_ptr<UIElement>> result;
        if (el->checkCollision(e.button.x, e.button.y))
        {
            result.type = (int)EventResultType::StopPropagation;
        }
        else
        {
            result.type = (int)EventResultType::RemoveElement;
            result.data = el;
        }
        return result;
    };
    this->_parentElement->events.addHandler(SDL_MOUSEBUTTONUP, clickRemoveMenu);
    auto removeMenu = [](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        EventResult<std::shared_ptr<UIElement>> result;
        result.type = (int)EventResultType::RemoveElement;
        result.data = el;
        return result;
    };
    this->_parentElement->appEvents.addHandler((int)AppEventType::CloseContextMenu, removeMenu);
    return this->_parentElement;
}