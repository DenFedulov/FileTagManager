#include "ElementGroups/TagElement.h"

TagElement::TagElement(CommonObjects *comm, std::wstring name) : ElementGroup(comm), name(name)
{
}

void TagElement::setColor(RGBA color)
{
    this->color = color;
    for (const auto &child : this->_parentElement->childElements.vec)
    {
        std::shared_ptr<UIDynamicElement> childEl = std::static_pointer_cast<UIDynamicElement>(child);
        childEl->setColor(color);
    }
}

std::shared_ptr<UIElement> TagElement::getElement()
{
    if (this->_parentElement != nullptr)
    {
        return this->_parentElement;
    }
    this->_parentElement = std::make_shared<UIElement>("tag", this->comm);
    this->_parentElement->displayMode = DisplayMode::Distribute;
    this->_parentElement->childrenDistPos = RelPos::Start;
    this->_parentElement->margin[Direction::Up] = 4;

    RGBA actualColor = this->color;
    if (this->comm->state->selectedTags.contains(wstrToHex(this->name)))
    {
        actualColor.a = 255;
    }

    auto tagLeft = std::make_shared<UIPictureElement>(G_App::IMAGES_PATH + "tag_left.png", this->comm);
    tagLeft->setColor(actualColor);
    tagLeft->setH(this->TAG_HIGHT);
    tagLeft->setW(this->SIDE_WIDTH);

    auto body = std::make_shared<UIBox>("tag_body", this->comm, this->TAG_HIGHT, this->TAG_HIGHT, 0, actualColor);
    body->displayMode = DisplayMode::Distribute;
    body->childrenDistPos = RelPos::Center;
    body->childrenAlignPos = RelPos::Center;
    auto tagName = std::make_shared<UIText>("tag_name", this->comm, this->name, this->TAG_FONT_SIZE);
    body->setW(tagName->getW() + this->SIDE_WIDTH);
    UIElement::addChildren(body, {tagName});

    auto tagRight = std::make_shared<UIPictureElement>(G_App::IMAGES_PATH + "tag_right.png", this->comm);
    tagRight->setColor(actualColor);
    tagRight->setH(this->TAG_HIGHT);
    tagRight->setW(this->SIDE_WIDTH);

    UIElement::addChildren(this->_parentElement, {tagLeft, body, tagRight});
    this->_parentElement->setH(this->TAG_HIGHT);
    this->_parentElement->setW(this->_parentElement->getChildWSum());
    auto action = [tagName = this->name](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        EventResult<std::shared_ptr<UIElement>> result;
        auto tagHexName = wstrToHex(tagName);
        if (el->comm->state->tagActionMode == (int)TagActionMode::Select)
        {
            bool selected = !el->comm->state->selectedTags.contains(tagHexName);
            if (selected)
            {
                el->comm->state->selectedTags.insert(tagHexName);
            }
            else
            {
                el->comm->state->selectedTags.erase(tagHexName);
            }
            el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::TagFilterChanged));
        }
        else if (el->comm->state->tagActionMode == (int)TagActionMode::AddToFile)
        {
            bool addResult = el->comm->db->addTagToFile(tagName, el->comm->state->addTagPath);
            std::cout << tagHexName << (addResult ? " added\n" : " exists\n");
            el->comm->state->tagActionMode = (int)TagActionMode::Select;
            el->comm->state->addTagPath = L"";
            el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::TagActionChange));
        }
        else if (el->comm->state->tagActionMode == (int)TagActionMode::Delete)
        {
            bool addResult = el->comm->db->deleteTag(tagName);
            std::cout << tagHexName << (addResult ? " deleted\n" : " does not exist\n");
            el->comm->state->selectedTags.erase(tagHexName);
            el->comm->state->tagActionMode = (int)TagActionMode::Select;
            el->comm->state->addTagPath = L"";
            el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::TagActionChange));
            el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::TagsChanged));
            el->comm->appEventsQueue.push_back(std::make_shared<AppEvent>(AppEventType::TagFilterChanged));
        }
        return result;
    };
    this->_parentElement->events.addHandler((int)CustomEvent::MOUSE_CLICK, action);

    auto onTagFilterChange = [tagName = this->name, color = this->color](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        auto tagHexName = wstrToHex(tagName);
        EventResult<std::shared_ptr<UIElement>> result;
        for (const auto &child : el->childElements.vec)
        {
            RGBA newColor(color);
            std::shared_ptr<UIDynamicElement> dynEl = std::static_pointer_cast<UIDynamicElement>(child);
            if (el->comm->state->selectedTags.contains(tagHexName))
            {
                newColor.a = 255;
            }
            dynEl->setColor(newColor);
        }

        return result;
    };
    this->_parentElement->appEvents.addHandler((int)AppEventType::TagFilterChanged, onTagFilterChange);

    return this->_parentElement;
}