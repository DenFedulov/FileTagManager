#include "ElementGroups/FileExplorer.h"

FileExplorer::FileExplorer(CommonObjects *comm) : ElementGroup(comm)
{
}

std::shared_ptr<UIElement> FileExplorer::getElement()
{
    if (this->_parentElement != nullptr)
    {
        return this->_parentElement;
    }
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
    UIElement::addChildren(backwardsButton, {backwardsIcon});

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

    UIElement::addChildren(forwardsButton, {forwardsIcon});

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

    UIElement::addChildren(folderUpButton, {folderUpIcon});

    auto currentPath = std::make_shared<UIBox>("currentPath", this->comm, 1, 1, 0, RGBA(20, 20, 20));
    currentPath->anchors[Direction::Right] = true;
    currentPath->anchors[Direction::Down] = true;
    currentPath->displayMode = DisplayMode::Distribute;
    currentPath->childrenDistPos = RelPos::Start;
    currentPath->childrenAlignPos = RelPos::Start;
    currentPath->overflow = OverflowMode::Scroll;
    currentPath->scrollDirection = Direction::Right;

    auto currentPathText = std::make_shared<UIText>("currentPathText", this->comm, G_App::DEFAULT_PATH, 14);
    currentPathText->wrap = false;
    auto onBackward = [](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        std::cout << "text onBackward called" << '\n';
        std::shared_ptr<AppEvent> newEvent = std::make_shared<AppEvent>(AppEventType::OpenDir);
        std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
        textEl->undo();
        newEvent->newPath = textEl->getText();
        el->comm->appEventsQueue.push_back(newEvent);
        return EventResult<std::shared_ptr<UIElement>>();
    };
    currentPathText->appEvents.addHandler((int)AppEventType::Backward, onBackward);
    auto onForward = [](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        std::cout << "text onForward called" << '\n';
        std::shared_ptr<AppEvent> newEvent = std::make_shared<AppEvent>(AppEventType::OpenDir);
        std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
        textEl->redo();
        newEvent->newPath = textEl->getText();
        el->comm->appEventsQueue.push_back(newEvent);
        return EventResult<std::shared_ptr<UIElement>>();
    };
    currentPathText->appEvents.addHandler((int)AppEventType::Forward, onForward);
    auto onFolderUp = [](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        std::cout << "text onFolderUp called" << '\n';
        std::shared_ptr<AppEvent> newEvent = std::make_shared<AppEvent>(AppEventType::OpenDir);
        std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
        if (textEl->getText() == G_App::DEFAULT_PATH)
        {
            return EventResult<std::shared_ptr<UIElement>>();
        }
        if (textEl->getText().ends_with(L":\\") || textEl->getText().ends_with(L":/"))
        {
            newEvent->newPath = G_App::DEFAULT_PATH;
        }
        else
        {
            newEvent->newPath = Str::cutTailByChar(textEl->getText(), L"\\/", false);
            if (!newEvent->newPath.ends_with(L":\\") && !newEvent->newPath.ends_with(L":/"))
            {
                newEvent->newPath = newEvent->newPath.substr(0, newEvent->newPath.length() - 1);
            }
        }
        el->comm->appEventsQueue.push_back(newEvent);
        return EventResult<std::shared_ptr<UIElement>>();
    };
    currentPathText->appEvents.addHandler((int)AppEventType::FolderUp, onFolderUp);
    currentPathText->groupParentElement = this->_parentElement;
    auto onNewPath = [](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        EventResult<std::shared_ptr<UIElement>> result;
        std::wcout << "new text path is: " << e->newPath << '\n';
        std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
        if (e->newPath != textEl->getText())
        {
            textEl->setText(e->newPath);
        }
        FilesGroup files(el->comm);
        files.folderPath = e->newPath;
        UIElement::addChildren(el->groupParentElement, {files.getElement()});
        result.data = files.getElement();
        result.type = (int)EventResultType::AddElement;
        return result;
    };
    currentPathText->appEvents.addHandler((int)AppEventType::OpenDir, onNewPath);
    UIElement::addChildren(currentPath, {currentPathText});

    UIElement::addChildren(pathControls, {backwardsButton, forwardsButton, folderUpButton, currentPath});

    FilesGroup files(this->comm);

    UIElement::addChildren(this->_parentElement, {pathControls, files.getElement()});
    return this->_parentElement;
}