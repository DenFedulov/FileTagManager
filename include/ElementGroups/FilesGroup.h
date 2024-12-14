#pragma once
#include "Filesystem.h"
#include "BaseElements/UIBox.h"
#include "ElementGroups/ElementGroup.h"
#include "ElementGroups/FileElement.h"

class FilesGroup : public ElementGroup
{
public:
    std::wstring folderPath = G_App::DEFAULT_PATH;
    FilesGroup(CommonObjects *comm);
    std::shared_ptr<UIElement> getElement();
};