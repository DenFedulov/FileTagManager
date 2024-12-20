#include "ElementGroups/Sidebar.h"

Sidebar::Sidebar(CommonObjects *comm) : ElementGroup(comm)
{
}

std::shared_ptr<UIElement> Sidebar::getElement()
{
    if (this->_parentElement != nullptr)
    {
        return this->_parentElement;
    }
    this->_parentElement = std::make_shared<UIBox>("sidebar", this->comm, this->width, 1, 0, RGBA(100, 100, 100));
    this->_parentElement->anchors[Direction::Down] = true;
    this->_parentElement->overflow = OverflowMode::Scroll;
    this->_parentElement->displayMode = DisplayMode::Distribute;
    this->_parentElement->distDirection = DistDirection::row;
    this->_parentElement->childrenDistPos = RelPos::Start;
    this->_parentElement->childrenAlignPos = RelPos::Start;

    auto controls = std::make_shared<UIBox>("sidebarControls", this->comm, this->width, 1, 0, RGBA(40, 40, 40));
    controls->displayMode = DisplayMode::Distribute;
    controls->distDirection = DistDirection::row;
    controls->childrenDistPos = RelPos::Start;
    controls->childrenAlignPos = RelPos::Start;

    auto addTagField = std::make_shared<UIElement>("addTagField", this->comm);
    addTagField->setH(this->controlsFieldHight);
    addTagField->anchors[Direction::Right] = true;
    addTagField->displayMode = DisplayMode::Distribute;
    addTagField->childrenDistPos = RelPos::Start;
    addTagField->childrenAlignPos = RelPos::Start;

    InputBox addTagInputName(this->comm, 140);
    InputBox addTagInputColor(this->comm, 70);
    addTagInputName.getInputElement()->setPlaceholder(L"tag name");
    addTagInputColor.getInputElement()->setPlaceholder(L"hex color");
    UIButton addTagButton(this->comm, L"Add tag", 12, RGBA(100, 100, 100), 5);
    auto addTag = [nameTextEl = addTagInputName.getInputElement(), colorTextEl = addTagInputColor.getInputElement()](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        if (el->comm->db->addTag(nameTextEl->getText(), wstrToStr(colorTextEl->getText())))
        {
            el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::TagsChanged));
        }
        return EventResult<std::shared_ptr<UIElement>>();
    };
    addTagButton.getElement()->events.addHandler((int)CustomEvent::MOUSE_CLICK, addTag);

    UIButton deleteTagButton(this->comm, L"Delete tag", 12, RGBA(100, 100, 100), 5);
    auto deteleTag = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        el->comm->state->tagActionMode = (int)TagActionMode::Delete;
        el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::TagActionChange));
        return EventResult<std::shared_ptr<UIElement>>();
    };
    deleteTagButton.getElement()->events.addHandler((int)CustomEvent::MOUSE_CLICK, deteleTag);
    UIElement::addChildren(addTagField, {addTagButton.getElement(), addTagInputName.getElement(), addTagInputColor.getElement(), deleteTagButton.getElement()});

    auto tagModsField = std::make_shared<UIElement>("tagModsField", this->comm);
    tagModsField->setH(this->controlsFieldHight);
    tagModsField->anchors[Direction::Right] = true;
    tagModsField->displayMode = DisplayMode::Distribute;
    tagModsField->childrenDistPos = RelPos::Start;
    tagModsField->childrenAlignPos = RelPos::Start;

    UIButton currentFilterTitle(this->comm, L"Tag filter mode", 12, RGBA(0, 0, 0, 0), 5);
    UIButton currentFilterName(this->comm, L"Any", 12, RGBA(100, 100, 100));
    auto changeTagFilterMode = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        el->comm->state->tagFilterMode = !el->comm->state->tagFilterMode;
        std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el->childElements.vec.at(0));
        textEl->setText(el->comm->state->tagFilterMode ? L"All" : L"Any");
        if (el->comm->state->selectedTags.size() > 1)
        {
            el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::TagFilterChanged));
            std::cout << "TagFilterChanged\n";
        }
        std::cout << "changed filter mode\n";
        return EventResult<std::shared_ptr<UIElement>>();
    };
    currentFilterName.getElement()->events.addHandler((int)CustomEvent::MOUSE_CLICK, changeTagFilterMode);
    UIElement::addChildren(tagModsField, {currentFilterTitle.getElement(), currentFilterName.getElement()});

    UIButton currentActionTitle(this->comm, L"Tag action", 12, RGBA(0, 0, 0, 0), 5);
    InputBox currentActionName(this->comm, 80);
    currentActionName.getInputElement()->editable = false;
    currentActionName.getInputElement()->setText(G_App::TAG_ACTION_MODE_NAMES.at(this->comm->state->tagActionMode));
    auto onActionChange = [](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
        textEl->setText(G_App::TAG_ACTION_MODE_NAMES.at(el->comm->state->tagActionMode));
        return EventResult<std::shared_ptr<UIElement>>();
    };
    currentActionName.getInputElement()->appEvents.addHandler((int)AppEventType::TagActionChange, onActionChange);
    UIButton resetTagActionButton(this->comm, L"Reset", 12, RGBA(100, 100, 100), 5);
    auto resetTag = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        el->comm->state->tagActionMode = (int)TagActionMode::Select;
        el->comm->state->addTagPath = L"";
        el->comm->state->selectedTags.clear();
        el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::TagActionChange));
        el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::TagFilterChanged));
        return EventResult<std::shared_ptr<UIElement>>();
    };
    resetTagActionButton.getElement()->events.addHandler((int)CustomEvent::MOUSE_CLICK, resetTag);

    UIElement::addChildren(tagModsField, {currentActionTitle.getElement(), currentActionName.getElement(), resetTagActionButton.getElement()});

    UIElement::addChildren(controls, {addTagField, tagModsField});
    controls->setH(controls->getChildHSum());

    TagList tagList(this->comm, this->width);

    auto onTagsChange = [width = this->width](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        EventResult<std::shared_ptr<UIElement>> result;
        std::cout << "creating new tag list on tag change\n";
        TagList files(el->comm, width);
        UIElement::addChildren(el, {files.getElement()});
        result.data = files.getElement();
        result.type = (int)EventResultType::AddElement;

        return result;
    };
    this->_parentElement->appEvents.addHandler((int)AppEventType::TagsChanged, onTagsChange);

    UIElement::addChildren(this->_parentElement, {controls, tagList.getElement()});
    return this->_parentElement;
}
