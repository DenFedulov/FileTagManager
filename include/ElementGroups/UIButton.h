#pragma once
#include "Filesystem.h"
#include "BaseElements/UIBox.h"
#include "BaseElements/UIText.h"
#include "ElementGroups/ElementGroup.h"

class UIButton : public ElementGroup
{
public:
    std::wstring buttonText;
    int fontSize;
    RGBA color;
    int borderRadius;
    int borderWidth;
    RGBA borderColor;
    UIButton(CommonObjects *comm, std::wstring buttonText, int fontSize, RGBA color, int borderRadius = 0, int borderWidth = 0, RGBA borderColor = RGBA(100, 100, 100));
    std::shared_ptr<UIElement> getElement();
};