#pragma once
#include "BaseElements/UIText.h"
#include "BaseElements/UIPictureElement.h"
#include "ElementGroups/ElementGroup.h"
#include "WindowsUtils.h"

class FileElement : public ElementGroup
{
private:
    void createElementGroup();

public:
    std::wstring filename;
    enum IconType
    {
        Error,
        File,
        Folder,
        Drive,
    };
    IconType iconType = IconType::Error;
    int iconSize = 64;
    int fontSize = 12;
    FileElement(CommonObjects *comm, std::wstring filename);
};