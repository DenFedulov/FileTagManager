#include "ElementGroups/FileElement.h"

FileElement::FileElement(CommonObjects *comm, std::wstring filename) : ElementGroup(comm), filename(filename)
{
    if (std::filesystem::is_regular_file(filename))
    {
        this->iconType = IconType::File;
    }
    else if (std::filesystem::is_directory(filename))
    {
        this->iconType = IconType::Folder;
        if (filename.ends_with(L":\\") || filename.ends_with(L":/"))
        {
            this->iconType = IconType::Drive;
        }
    }

    this->createElementGroup();
}

void FileElement::createElementGroup()
{
    this->_parentElement = std::make_shared<UIElement>(wstrToStr(filename), this->comm);
    this->_parentElement->displayMode = DisplayMode::Distribute;
    this->_parentElement->childrenDistPos = RelPos::Center;
    this->_parentElement->childrenAlignPos = RelPos::Start;
    this->_parentElement->setMargin(5);

    std::string iconPath = G_App::IMAGES_PATH + "file.png";
    switch (this->iconType)
    {
    case IconType::Drive:
        iconPath = G_App::IMAGES_PATH + "hard-drive.png";
        break;
    case IconType::Folder:
        iconPath = G_App::IMAGES_PATH + "folder.png";
        break;
    }
    auto icon = std::make_shared<UIPictureElement>(iconPath, this->comm);
    icon->setH(this->iconSize);
    icon->setW(this->iconSize);

    std::wstring name = Str::getTailByChar(this->filename, L"\\/");
    name = name == L"" ? this->filename : name;
    auto nameEl = std::make_shared<UIText>("filename", this->comm, name, this->fontSize);
    nameEl->setWrapSize(this->iconSize);
    nameEl->pivotPosV = RelPos::Start;
    // nameEl->showHitbox = true;

    auto sendNewPath = [newPath = this->filename](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        if (std::filesystem::is_directory(newPath))
        {
            std::shared_ptr<AppEvent> newEvent = std::make_shared<AppEvent>(AppEventType::OpenDir);
            newEvent->newPath = newPath;
            el->comm->appEventsQueue.push_back(newEvent);
        }
        else
        {
            showFileInExplorer(newPath);
        }
        return EventResult<std::shared_ptr<UIElement>>();
    };
    this->_parentElement->events.addHandler((int)CustomEvent::MOUSE_CLICK, sendNewPath);
    // this->_parentElement->showHitbox = true;

    UIElement::addChildren(this->_parentElement, {icon, nameEl});
    this->_parentElement->setW(this->iconSize);
    this->_parentElement->setH(this->iconSize + nameEl->calcH());
}