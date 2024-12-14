#pragma once
#include "BaseElements/UIText.h"
#include "BaseElements/UIPictureElement.h"
#include "BaseElements/UIBox.h"
#include "ElementGroups/ElementGroup.h"
#include "type_helpers.h"

class TagElement : public ElementGroup
{
private:

public:
    RGBA color;
    std::wstring name;
    const int TAG_HIGHT = 22;
    const int TAG_FONT_SIZE = 14;
    const int SIDE_WIDTH = 8;
    TagElement(CommonObjects *comm, std::wstring name);
    std::shared_ptr<UIElement> getElement();

    void setColor(RGBA color);
    RGBA getColor();
};