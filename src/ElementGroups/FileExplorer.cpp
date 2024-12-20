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
    pathControls->setH(this->controlsHight);
    pathControls->displayMode = DisplayMode::Distribute;
    pathControls->childrenDistPos = RelPos::Start;

    auto backwardsButton = std::make_shared<UIElement>("backwardsButton", this->comm);
    auto backwardsIcon = std::make_shared<UIPictureElement>(G_App::IMAGES_PATH + "left_arrow.png", this->comm);
    backwardsButton->setW(this->controlsHight);
    backwardsIcon->setW(this->controlsHight);
    backwardsIcon->setH(this->controlsHight);
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
    forwardsButton->setW(this->controlsHight);
    forwardsIcon->setW(this->controlsHight);
    forwardsIcon->setH(this->controlsHight);
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
    folderUpButton->setW(this->controlsHight);
    folderUpIcon->setW(this->controlsHight);
    folderUpIcon->setH(this->controlsHight);
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
        newEvent->eventText = textEl->getText();
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
        newEvent->eventText = textEl->getText();
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
            newEvent->eventText = G_App::DEFAULT_PATH;
        }
        else
        {
            newEvent->eventText = Str::cutTailByChar(textEl->getText(), L"\\/", false);
            if (!newEvent->eventText.ends_with(L":\\") && !newEvent->eventText.ends_with(L":/"))
            {
                newEvent->eventText = newEvent->eventText.substr(0, newEvent->eventText.length() - 1);
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
        std::wcout << "new text path is: " << e->eventText << '\n';
        std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
        if (e->eventText != textEl->getText())
        {
            textEl->setText(e->eventText);
        }
        FilesGroup files(el->comm);
        files.folderPath = e->eventText;
        UIElement::addChildren(el->groupParentElement, {files.getElement()});
        result.data = files.getElement();
        result.type = (int)EventResultType::AddElement;
        return result;
    };
    currentPathText->appEvents.addHandler((int)AppEventType::OpenDir, onNewPath);

    UIElement::addChildren(currentPath, {currentPathText});
    UIElement::addChildren(pathControls, {backwardsButton, forwardsButton, folderUpButton, currentPath});

    FilesGroup files(this->comm);

    auto viewControls = std::make_shared<UIBox>("viewControls", this->comm, 1, 1, 0, RGBA(50, 50, 50));
    viewControls->anchors[Direction::Right] = true;
    viewControls->setH(this->controlsHight);
    viewControls->displayMode = DisplayMode::Distribute;
    viewControls->childrenDistPos = RelPos::Start;

    UIButton sortTitle(this->comm, L"Sort by", 12, RGBA(0, 0, 0, 0));
    UIButton sortByTypeButton(this->comm, L"Type", 12, RGBA(100, 100, 100), 5, 2, RGBA(0, 255, 0, 100));
    auto sortByType = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        std::shared_ptr<UIBox> boxEl = std::static_pointer_cast<UIBox>(el);
        switch (el->comm->state->typeSortMode)
        {
        case (int)SortMode::Ascending:
            el->comm->state->typeSortMode = (int)SortMode::Descending;
            boxEl->setBorderColor(RGBA(255, 0, 0, 100));
            break;
        case (int)SortMode::Descending:
            el->comm->state->typeSortMode = (int)SortMode::None;
            boxEl->setBorderColor(RGBA(0, 0, 0, 0));
            break;
        case (int)SortMode::None:
            el->comm->state->typeSortMode = (int)SortMode::Ascending;
            boxEl->setBorderColor(RGBA(0, 255, 0, 100));
            break;
        }
        el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::SortChange));
        return EventResult<std::shared_ptr<UIElement>>();
    };
    sortByTypeButton.getElement()->events.addHandler((int)CustomEvent::MOUSE_CLICK, sortByType);

    UIButton sortByNameButton(this->comm, L"Name", 12, RGBA(100, 100, 100), 5, 2, RGBA(0, 255, 0, 100));
    auto sortByName = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        std::shared_ptr<UIBox> boxEl = std::static_pointer_cast<UIBox>(el);
        switch (el->comm->state->nameSortMode)
        {
        case (int)SortMode::Ascending:
            el->comm->state->nameSortMode = (int)SortMode::Descending;
            boxEl->setBorderColor(RGBA(255, 0, 0, 100));
            break;
        case (int)SortMode::Descending:
            el->comm->state->nameSortMode = (int)SortMode::Ascending;
            boxEl->setBorderColor(RGBA(0, 255, 0, 100));
            break;
        }
        el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::SortChange));
        return EventResult<std::shared_ptr<UIElement>>();
    };
    sortByNameButton.getElement()->events.addHandler((int)CustomEvent::MOUSE_CLICK, sortByName);

    auto filterElement = std::make_shared<FilterElement>("filterElement", this->comm, L"", 14, RGBA(0, 0, 0));
    InputBox filterInput(this->comm, 4000, filterElement);
    filterInput.borderRadius = 0;
    filterInput.borderWidth = 0;
    filterInput.getElement()->anchors[Direction::Right] = true;
    filterInput.getInputElement()->setPlaceholder(L"filter");

    auto onTagFilterChange = [currentPathText = currentPathText](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        EventResult<std::shared_ptr<UIElement>> result;
        std::cout << "creating new file group on tag change\n";
        FilesGroup files(el->comm);
        files.folderPath = currentPathText->getText();
        UIElement::addChildren(el, {files.getElement()});
        result.data = files.getElement();
        result.type = (int)EventResultType::AddElement;

        return result;
    };
    this->_parentElement->appEvents.addHandler((int)AppEventType::TagFilterChanged, onTagFilterChange);

    UIElement::addChildren(viewControls, {sortTitle.getElement(), sortByTypeButton.getElement(), sortByNameButton.getElement(), filterInput.getElement()});

    UIElement::addChildren(this->_parentElement, {pathControls, viewControls, files.getElement()});
    return this->_parentElement;
}