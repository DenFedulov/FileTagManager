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
    int w;
    TagList(CommonObjects *comm, int w);
    std::shared_ptr<UIElement> getElement();
};