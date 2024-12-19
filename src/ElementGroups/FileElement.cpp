#include "ElementGroups/FileElement.h"

FileElement::FileElement(CommonObjects *comm, std::wstring filePath) : ElementGroup(comm), filePath(filePath)
{
    if (std::filesystem::is_regular_file(filePath))
    {
        std::string extension = wstrToStr(Str::getTailByChar(filePath, std::wstring(L"."), false));
        if (FileExtensions::image.contains(extension))
        {
            this->iconType = IconType::Image;
        }
        else if (FileExtensions::video.contains(extension))
        {
            this->iconType = IconType::Video;
        }
        else if (FileExtensions::audio.contains(extension))
        {
            this->iconType = IconType::Audio;
        }
        else
        {
            this->iconType = IconType::File;
        }
    }
    else if (std::filesystem::is_directory(filePath))
    {
        this->iconType = IconType::Folder;
        if (filePath.ends_with(L":\\") || filePath.ends_with(L":/"))
        {
            this->iconType = IconType::Drive;
        }
    }
}

std::shared_ptr<UIElement> FileElement::getElement()
{
    if (this->_parentElement != nullptr)
    {
        return this->_parentElement;
    }
    this->_parentElement = std::make_shared<UIFileElement>(wstrToStr(this->filePath), this->comm, this->filePath);
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
    case IconType::Image:
        iconPath = G_App::IMAGES_PATH + "image.png";
        break;
    case IconType::Video:
        iconPath = G_App::IMAGES_PATH + "video.png";
        break;
    case IconType::Audio:
        iconPath = G_App::IMAGES_PATH + "audio.png";
        break;
    }
    auto icon = std::make_shared<UIPictureElement>(iconPath, this->comm);
    icon->setH(this->iconSize);
    icon->setW(this->iconSize);

    std::wstring name = Str::getTailByChar(this->filePath, std::wstring(L"\\/"));
    name = name == L"" ? this->filePath : name;
    auto nameEl = std::make_shared<UIText>("filePath", this->comm, name, this->fontSize);
    nameEl->setWrapSize(this->iconSize);
    nameEl->pivotPosV = RelPos::Start;
    // nameEl->showHitbox = true;

    auto sendNewPath = [eventPath = this->filePath](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        EventResult<std::shared_ptr<UIElement>> result;
        if (e.button.button == (int)MouseButtons::Left)
        {
            if (std::filesystem::is_directory(eventPath))
            {
                std::shared_ptr<AppEvent> newEvent = std::make_shared<AppEvent>(AppEventType::OpenDir);
                newEvent->eventPath = eventPath;
                el->comm->appEventsQueue.push_back(newEvent);
            }
            else
            {
                showFileInExplorer(eventPath);
            }
        }
        else if (e.button.button == (int)MouseButtons::Right)
        {
            FileContextMenu options(el->comm, eventPath);
            BaseContextMenu files(el->comm, {options.make()}, e.button.x, e.button.y);
            result.data = files.getElement();
            result.type = (int)EventResultType::AddElementToMain;
        }
        std::cout << "file event\n";
        return result;
    };
    this->_parentElement->events.addHandler((int)CustomEvent::MOUSE_CLICK, sendNewPath);
    // this->_parentElement->showHitbox = true;

    UIElement::addChildren(this->_parentElement, {icon, nameEl});
    this->_parentElement->setW(this->iconSize);
    this->_parentElement->setH(this->iconSize + nameEl->calcH());

    return this->_parentElement;
}