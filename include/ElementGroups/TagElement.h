#pragma once
#include "BaseElements/UIText.h"
#include "BaseElements/UIPictureElement.h"
#include "BaseElements/UIBox.h"
#include "ElementGroups/ElementGroup.h"
#include "type_helpers.h"

class TagElement : public ElementGroup
{
private:
    void createElementGroup();
    RGBA _color;
    std::string _name;

public:
    const int TAG_HIGHT = 40;
    const int SIDE_WIDTH = 15;
    TagElement(CommonObjects *comm, std::string name);
    TagElement(CommonObjects *comm, std::string name, RGBA color);

    void setColor(RGBA color);
    RGBA getColor();
};