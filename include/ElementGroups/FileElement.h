#pragma once
#include "BaseElements/UIText.h"
#include "BaseElements/UIPictureElement.h"
#include "Elements/UIFileElement.h"
#include "ElementGroups/ElementGroup.h"
#include "ElementGroups/BaseContextMenu.h"
#include "ElementGroups/TagList.h"
#include "ContextMenus/FileContextMenu.h"
#include "WindowsUtils.h"
#include "FileExtensions.h"
#include "type_helpers.h"

class FileElement : public ElementGroup
{
public:
    std::wstring filePath;
    enum IconType
    {
        Error,
        File,
        Folder,
        Drive,
        Image,
        Video,
        Audio
    };
    IconType iconType = IconType::Error;
    int iconSize = 64;
    int fontSize = 12;
    FileElement(CommonObjects *comm, std::wstring filePath);
    std::shared_ptr<UIElement> getElement();
};