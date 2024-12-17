#include "ElementGroups/FilesGroup.h"

FilesGroup::FilesGroup(CommonObjects *comm) : ElementGroup(comm)
{
}

std::shared_ptr<UIElement> FilesGroup::getElement()
{
    if (this->_parentElement != nullptr)
    {
        return this->_parentElement;
    }
    this->_parentElement = std::make_shared<UIElement>("file group", this->comm);
    this->_parentElement->anchors[Direction::Right] = true;
    this->_parentElement->anchors[Direction::Down] = true;
    this->_parentElement->displayMode = DisplayMode::Distribute;
    this->_parentElement->childrenDistPos = RelPos::Start;
    this->_parentElement->childrenAlignPos = RelPos::Start;
    this->_parentElement->overflow = OverflowMode::Scroll;

    std::vector<std::wstring> filePaths;
    if (this->folderPath != G_App::DEFAULT_PATH)
    {
        filePaths = scanDir(this->folderPath);
    }
    else
    {
        for (const auto &disk : getDrivesList())
        {
            filePaths.push_back(strToWStr(disk.c_str()));
        }
    }
    std::vector<std::shared_ptr<UIElement>> files;
    for (const auto &filePath : filePaths)
    {
        FileElement fileEl(this->comm, filePath);
        files.push_back(fileEl.getElement());
    }
    UIElement::addChildren(this->_parentElement, files);
    auto onNewPath = [thisPath = this->folderPath](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        EventResult<std::shared_ptr<UIElement>> result;
        if (e->eventPath != thisPath)
        {
            result.type = (int)EventResultType::RemoveElement;
            result.data = el;
        }
        return result;
    };
    this->_parentElement->appEvents.addHandler((int)AppEventType::OpenDir, onNewPath);
    return this->_parentElement;
}
