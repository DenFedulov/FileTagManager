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
    if (this->comm->state->selectedTags.size() > 0)
    {
        std::vector<std::string> selectedTags(this->comm->state->selectedTags.begin(), this->comm->state->selectedTags.end());
        TableData taggedFiles = this->comm->db->getFilesWithTags(selectedTags, this->comm->state->tagFilterMode);
        std::unordered_set<std::wstring> foundFilePaths;
        for (const auto &row : taggedFiles.data)
        {
            foundFilePaths.insert(hexStrToWStr(row.at(1)));
        }
        filePaths = std::vector<std::wstring>(foundFilePaths.begin(), foundFilePaths.end());
    }
    else
    {
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
        if (e->eventText != thisPath)
        {
            result.type = (int)EventResultType::RemoveElement;
            result.data = el;
        }
        return result;
    };
    this->_parentElement->appEvents.addHandler((int)AppEventType::OpenDir, onNewPath);

    auto sort = [](std::shared_ptr<UIElement> a, std::shared_ptr<UIElement> b) -> bool
    {
        std::shared_ptr<UIFileElement> fileA = std::static_pointer_cast<UIFileElement>(a);
        std::shared_ptr<UIFileElement> fileB = std::static_pointer_cast<UIFileElement>(b);
        std::wstring extensionA = L"";
        std::wstring extensionB = L"";
        if (std::filesystem::is_regular_file(fileA->filePath))
        {
            extensionA = Str::getTailByChar(fileA->filePath, std::wstring(L"."), false, false);
        }
        if (std::filesystem::is_regular_file(fileB->filePath))
        {
            extensionB = Str::getTailByChar(fileB->filePath, std::wstring(L"."), false, false);
        }
        if (a->comm->state->typeSortMode != 0)
        {
            if (extensionA < extensionB)
            {
                return a->comm->state->typeSortMode == (int)SortMode::Ascending;
            }
            else if (extensionA > extensionB)
            {
                return !(a->comm->state->typeSortMode == (int)SortMode::Ascending);
            }
        }
        return a->comm->state->nameSortMode == (int)SortMode::Ascending
                   ? fileA->filePath < fileB->filePath
                   : fileA->filePath > fileB->filePath;
    };
    this->_parentElement->sortChildVec(sort);
    auto onSortChange = [&sort](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        EventResult<std::shared_ptr<UIElement>> result;
        el->sortChildVec(sort);
        return result;
    };
    this->_parentElement->appEvents.addHandler((int)AppEventType::SortChange, onSortChange);
    auto onTagFilterChange = [](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        EventResult<std::shared_ptr<UIElement>> result;
        std::cout << "deleting old file group on tag change\n";
        result.type = (int)EventResultType::RemoveElement;
        result.data = el;
        return result;
    };
    this->_parentElement->appEvents.addHandler((int)AppEventType::TagFilterChanged, onTagFilterChange);

    return this->_parentElement;
}
