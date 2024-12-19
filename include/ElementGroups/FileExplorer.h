#pragma once
#include "BaseElements/UIBox.h"
#include "ElementGroups/ElementGroup.h"
#include "ElementGroups/FilesGroup.h"
#include "ElementGroups/InputBox.h"
#include "Elements/FilterElement.h"
#include "WindowsUtils.h"

class FileExplorer : public ElementGroup
{
public:
    int controlsHight = 24;
    int elementsGap = 4;
    FileExplorer(CommonObjects *comm);
    std::shared_ptr<UIElement> getElement();
};