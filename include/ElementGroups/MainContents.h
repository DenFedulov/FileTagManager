#pragma once
#include "WindowsUtils.h"
#include "BaseElements/UIText.h"
#include "Elements/UICircle.h"
#include "ElementGroups/ElementGroup.h"
#include "ElementGroups/TagElement.h"
#include "ElementGroups/FileExplorer.h"
#include "ElementGroups/Sidebar.h"

class MainContents : public ElementGroup
{
public:
    MainContents(CommonObjects *comm);
    std::shared_ptr<UIElement> getElement();
};