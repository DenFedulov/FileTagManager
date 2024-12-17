#pragma once
#include "BaseElements/UIElement.h"

class ContextMenu
{
protected:
    CommonObjects *comm;

public:
    int optionHeight = 20;
    ContextMenu(CommonObjects *comm);
    virtual ~ContextMenu() {};
    virtual std::vector<std::shared_ptr<UIElement>> make() = 0;
};
