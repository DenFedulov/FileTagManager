#pragma once
#include "ContextMenus/ContextMenu.h"
#include "ElementGroups/UIButton.h"

class FileContextMenu : public ContextMenu
{
private:
    std::wstring _filePath;
public:
    FileContextMenu(CommonObjects *comm, std::wstring filePath);
    std::vector<std::shared_ptr<UIElement>> make();
};
