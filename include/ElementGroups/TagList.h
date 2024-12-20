#pragma once
#include "BaseElements/UIBox.h"
#include "ElementGroups/ElementGroup.h"
#include "ElementGroups/FilesGroup.h"
#include "ElementGroups/TagElement.h"
#include "ElementGroups/UIButton.h"
#include "ElementGroups/InputBox.h"
#include "WindowsUtils.h"

class TagList : public ElementGroup
{
public:
    int listSize;
    bool tagsClickable = true;
    Direction anchorDirection = Direction::Down;
    int tagHight = 22;
    int tagFontSize = 14;
    int tagSideWidth = 8;
    std::wstring ofFile = L"";
    TagList(CommonObjects *comm, int listSize, std::wstring ofFile = L"");
    std::shared_ptr<UIElement> getElement();
};