#pragma once
#include "BaseElements/UIBox.h"
#include "ElementGroups/ElementGroup.h"
#include "ElementGroups/FilesGroup.h"
#include "ElementGroups/TagElement.h"
#include "ElementGroups/UIButton.h"
#include "ElementGroups/InputBox.h"
#include "ElementGroups/TagList.h"
#include "WindowsUtils.h"

class Sidebar : public ElementGroup
{
public:
    const int width = 350;
    const int controlsFieldHight = 24;
    Sidebar(CommonObjects *comm);
    std::shared_ptr<UIElement> getElement();
};