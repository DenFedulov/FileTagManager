#pragma once
#include "BaseElements/UIElement.h"
#include "AppGlobals.h"

class ElementGroup
{
protected:
    CommonObjects *comm;
    std::shared_ptr<UIElement> _parentElement = NULL;

public:
    ElementGroup(CommonObjects *comm);
    virtual ~ElementGroup() {};
    virtual std::shared_ptr<UIElement> getElement() = 0;
};
