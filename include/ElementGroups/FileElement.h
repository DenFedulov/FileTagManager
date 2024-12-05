#pragma once
#include "BaseElements/UIText.h"
#include "BaseElements/UIPictureElement.h"
#include "ElementGroups/ElementGroup.h"

class FileElement : public ElementGroup
{
private:
    void createElementGroup();

public:
    std::string filename;
    enum IconType
    {
        Default,
        Drive,
        Image,
        Vidoe
    };
    IconType iconType = IconType::Default;
    int iconSize = 64;
    int fontSize = 18;
    FileElement(CommonObjects *comm, std::string filename);
};