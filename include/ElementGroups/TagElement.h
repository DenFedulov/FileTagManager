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
    bool clickable = true;
    int tagHight = 22;
    int tagFontSize = 14;
    int tagSideWidth = 8;
    TagElement(CommonObjects *comm, std::wstring name);
    std::shared_ptr<UIElement> getElement();

    void setColor(RGBA color);
    RGBA getColor();
};