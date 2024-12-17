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
    std::vector<std::shared_ptr<UIElement>> tags;
    auto tagData = this->comm->db->getTags();
    for (const auto &row : tagData.data)
    {
        RGBA color = hexStrToRGBA(row.at(1).c_str());
        color.a = 120;
        TagElement tag(this->comm, hexStrToWStr(row.at(0)));
        tag.color = color;
        tags.push_back(tag.getElement());
    }

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
        el->comm->db->addTag(nameTextEl->getText(), wstrToStr(colorTextEl->getText()));
        return EventResult<std::shared_ptr<UIElement>>();
    };
    addTagButton.getElement()->events.addHandler((int)CustomEvent::MOUSE_CLICK, addTag);
    UIElement::addChildren(addTagField, {addTagButton.getElement(), addTagInputName.getElement(), addTagInputColor.getElement()});

    auto tagActionModeField = std::make_shared<UIElement>("tagActionModeField", this->comm);
    tagActionModeField->setH(this->controlsFieldHight);
    tagActionModeField->anchors[Direction::Right] = true;
    tagActionModeField->displayMode = DisplayMode::Distribute;
    tagActionModeField->childrenDistPos = RelPos::Start;
    tagActionModeField->childrenAlignPos = RelPos::Start;

    UIButton currentActionTitle(this->comm, L"Current tag action ->", 12, RGBA(0, 0, 0, 0), 5);
    InputBox currentActionName(this->comm, 140);
    currentActionName.getInputElement()->editable = false;
    currentActionName.getInputElement()->setText(G_App::TAG_ACTION_MODE_NAMES.at(this->comm->state->tagActionMode));
    auto onActionChange = [](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
        textEl->setText(G_App::TAG_ACTION_MODE_NAMES.at(el->comm->state->tagActionMode));
        return EventResult<std::shared_ptr<UIElement>>();
    };
    currentActionName.getInputElement()->appEvents.addHandler((int)AppEventType::TagActionChange, onActionChange);

    UIElement::addChildren(tagActionModeField, {currentActionTitle.getElement(), currentActionName.getElement()});

    UIElement::addChildren(controls, {addTagField, tagActionModeField});
    controls->setH(controls->getChildHSum());

    auto tagList = std::make_shared<UIElement>("tagList", this->comm);
    tagList->setW(this->width);
    tagList->anchors[Direction::Down] = true;
    tagList->displayMode = DisplayMode::Distribute;
    tagList->childrenDistPos = RelPos::Start;
    tagList->childrenAlignPos = RelPos::Start;
    tagList->margin[Direction::Left] = 5;
    tagList->margin[Direction::Right] = 5;
    UIElement::addChildren(tagList, tags);

    UIElement::addChildren(this->_parentElement, {controls, tagList});
    return this->_parentElement;
}
