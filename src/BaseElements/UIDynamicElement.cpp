#include "BaseElements/UIDynamicElement.h"

UIDynamicElement::UIDynamicElement(std::string name, CommonObjects *comm) : UIElement(name, comm)
{
}

void UIDynamicElement::setColor(RGBA color)
{
    this->_color = color;
    this->updateSurface();
}

RGBA UIDynamicElement::getColor()
{
    return this->_color;
}
