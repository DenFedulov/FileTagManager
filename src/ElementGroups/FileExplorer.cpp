#include "ElementGroups/FileExplorer.h"

FileExplorer::FileExplorer(CommonObjects *comm) : ElementGroup(comm)
{
    this->createElementGroup();
}

void FileExplorer::createElementGroup()
{
    auto drives = getDrivesList();
    this->_parentElement = std::make_shared<UIElement>("file explorer", this->comm);
    this->_parentElement->anchors[Direction::Right] = true;
    this->_parentElement->anchors[Direction::Down] = true;
    this->_parentElement->displayMode = DisplayMode::Distribute;
    this->_parentElement->distDirection = DistDirection::row;
    this->_parentElement->childrenDistPos = RelPos::Start;

    auto pathControls = std::make_shared<UIBox>("pathControls", this->comm, 1, 1, 0, RGBA(50, 50, 50));
    pathControls->anchors[Direction::Right] = true;
    pathControls->setH(this->pathControlsHight);
    pathControls->displayMode = DisplayMode::Distribute;
    pathControls->distDirection = DistDirection::column;
    pathControls->childrenDistPos = RelPos::Start;

    auto backwardsButton = std::make_shared<UIElement>("backwardsButton", this->comm);
    auto backwardsIcon = std::make_shared<UIPictureElement>(G_App::IMAGES_PATH + "left_arrow.png", this->comm);
    backwardsButton->setW(this->pathControlsHight);
    backwardsIcon->setW(this->pathControlsHight);
    backwardsIcon->setH(this->pathControlsHight);
    backwardsButton->margin[Direction::Right] = this->elementsGap;
    auto addBackward = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::Backward));
        return EventResult<std::shared_ptr<UIElement>>();
    };
    backwardsIcon->events.addHandler((int)CustomEvent::MOUSE_CLICK, addBackward);
    backwardsButton->addChildren({backwardsIcon});

    auto forwardsButton = std::make_shared<UIElement>("forwardsButton", this->comm);
    auto forwardsIcon = std::make_shared<UIPictureElement>(G_App::IMAGES_PATH + "right_arrow.png", this->comm);
    forwardsButton->setW(this->pathControlsHight);
    forwardsIcon->setW(this->pathControlsHight);
    forwardsIcon->setH(this->pathControlsHight);
    forwardsButton->margin[Direction::Right] = this->elementsGap;
    auto addForward = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::Forward));
        return EventResult<std::shared_ptr<UIElement>>();
    };
    forwardsIcon->events.addHandler((int)CustomEvent::MOUSE_CLICK, addForward);

    forwardsButton->addChildren({forwardsIcon});

    auto folderUpButton = std::make_shared<UIElement>("folderUpButton", this->comm);
    auto folderUpIcon = std::make_shared<UIPictureElement>(G_App::IMAGES_PATH + "up_arrow.png", this->comm);
    folderUpButton->setW(this->pathControlsHight);
    folderUpIcon->setW(this->pathControlsHight);
    folderUpIcon->setH(this->pathControlsHight);
    folderUpButton->margin[Direction::Right] = this->elementsGap;
    auto addFolderUp = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::FolderUp));
        return EventResult<std::shared_ptr<UIElement>>();
    };
    folderUpIcon->events.addHandler((int)CustomEvent::MOUSE_CLICK, addFolderUp);

    folderUpButton->addChildren({folderUpIcon});

    auto currentPath = std::make_shared<UIBox>("currentPath", this->comm, 1, 1, 0, RGBA(20, 20, 20));
    currentPath->anchors[Direction::Right] = true;
    currentPath->anchors[Direction::Down] = true;
    currentPath->displayMode = DisplayMode::Distribute;
    currentPath->childrenDistPos = RelPos::Start;
    currentPath->childrenAlignPos = RelPos::Start;
    currentPath->overflow = OverflowMode::Scroll;
    currentPath->scrollDirection = Direction::Right;

    auto currentPathText = std::make_shared<UIText>("currentPathText", this->comm, strToWStr("<select disk>"), 14);
    currentPathText->wrap = false;
    auto onBackward = [](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        std::cout << "text onBackward called" << '\n';
        return EventResult<std::shared_ptr<UIElement>>();
    };
    currentPathText->appEvents.addHandler((int)AppEventType::Backward, onBackward);
    auto onForward = [](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        std::cout << "text onForward called" << '\n';
        return EventResult<std::shared_ptr<UIElement>>();
    };
    currentPathText->appEvents.addHandler((int)AppEventType::Forward, onForward);
    auto onFolderUp = [](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        std::cout << "text onFolderUp called" << '\n';
        return EventResult<std::shared_ptr<UIElement>>();
    };
    currentPathText->appEvents.addHandler((int)AppEventType::FolderUp, onFolderUp);
    currentPath->addChildren({currentPathText});

    pathControls->addChildren({backwardsButton, forwardsButton, folderUpButton, currentPath});

    auto fileElements = std::make_shared<UIElement>("fileElements", this->comm);
    fileElements->anchors[Direction::Right] = true;
    fileElements->anchors[Direction::Down] = true;
    fileElements->displayMode = DisplayMode::Distribute;
    fileElements->childrenDistPos = RelPos::Start;

    std::vector<std::shared_ptr<UIElement>> files;
    for (const auto &drive : drives)
    {
        FileElement fileEl(this->comm, drive);
        files.push_back(fileEl.getParentElement());
    }
    fileElements->addChildren(files);

    this->_parentElement->addChildren({pathControls, fileElements});
}