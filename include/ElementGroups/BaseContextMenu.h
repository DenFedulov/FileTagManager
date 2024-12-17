#pragma once
#include "BaseElements/UIBox.h"
#include "ElementGroups/ElementGroup.h"

class BaseContextMenu : public ElementGroup
{
public:
    int x;
    int y;
    int width = 200;
    std::vector<std::shared_ptr<UIElement>> options;
    BaseContextMenu(CommonObjects *comm, std::vector<std::shared_ptr<UIElement>> options, int x, int y);
    std::shared_ptr<UIElement> getElement();
};