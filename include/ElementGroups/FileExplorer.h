#pragma once
#include "BaseElements/UIBox.h"
#include "ElementGroups/ElementGroup.h"
#include "ElementGroups/FilesGroup.h"
#include "WindowsUtils.h"

class FileExplorer : public ElementGroup
{
private:
    void createElementGroup();

public:
    int pathControlsHight = 24;
    int elementsGap = 4;
    FileExplorer(CommonObjects *comm);
};