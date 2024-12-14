#pragma once
#include "WindowsUtils.h"
#include "BaseElements/UIText.h"
#include "Elements/UICircle.h"
#include "ElementGroups/ElementGroup.h"
#include "ElementGroups/MainContents.h"
#include "ElementGroups/HeaderBar.h"

class MainParent : public ElementGroup
{
public:
    MainParent(CommonObjects *comm);
    std::shared_ptr<UIElement> getElement();
};