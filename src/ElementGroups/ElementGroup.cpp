#include "ElementGroups/ElementGroup.h"

std::shared_ptr<UIElement> ElementGroup::getParentElement()
{
    return this->_parentElement;
}

ElementGroup::ElementGroup(CommonObjects *comm) : comm(comm)
{
}