#pragma once
#include "Filesystem.h"
#include "BaseElements/UIBox.h"
#include "ElementGroups/ElementGroup.h"
#include "ElementGroups/FileElement.h"

class FilesGroup : public ElementGroup
{
private:
    void createElementGroup();

public:
    std::wstring folderPath;
    FilesGroup(CommonObjects *comm, std::wstring folderPath = G_App::DEFAULT_PATH);
};