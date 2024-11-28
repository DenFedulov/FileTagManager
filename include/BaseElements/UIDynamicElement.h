#pragma once
#include "BaseElements/UIElement.h"
#include "BaseElements/Dynamic.h"

class UIDynamicElement : public UIElement, Dynamic
{
protected:
    RGBA _color;

public:
    void setColor(RGBA color);
    RGBA getColor();

    UIDynamicElement(std::string name, CommonObjects *comm);
    ~UIDynamicElement() override;
};