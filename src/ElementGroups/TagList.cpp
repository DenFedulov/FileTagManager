#include "ElementGroups/TagList.h"

TagList::TagList(CommonObjects *comm, int w) : ElementGroup(comm), w(w)
{
}

std::shared_ptr<UIElement> TagList::getElement()
{
    if (this->_parentElement != nullptr)
    {
        return this->_parentElement;
    }
    this->_parentElement = std::make_shared<UIElement>("TagList", this->comm);
    this->_parentElement->setW(this->w);
    this->_parentElement->anchors[Direction::Down] = true;
    this->_parentElement->displayMode = DisplayMode::Distribute;
    this->_parentElement->childrenDistPos = RelPos::Start;
    this->_parentElement->childrenAlignPos = RelPos::Start;
    this->_parentElement->margin[Direction::Left] = 5;
    this->_parentElement->margin[Direction::Right] = 5;

    std::vector<std::shared_ptr<UIElement>> tags;
    auto tagData = this->comm->db->getTags();
    for (const auto &row : tagData.data)
    {
        RGBA color = hexStrToRGBA(row.at(1).c_str());
        color.a = 70;
        TagElement tag(this->comm, hexStrToWStr(row.at(0)));
        tag.color = color;
        tags.push_back(tag.getElement());
    }

    UIElement::addChildren(this->_parentElement, tags);

    auto onTagsChange = [](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        EventResult<std::shared_ptr<UIElement>> result;
        std::cout << "deleting old tag list on tag change\n";
        result.type = (int)EventResultType::RemoveElement;
        result.data = el;
        return result;
    };
    this->_parentElement->appEvents.addHandler((int)AppEventType::TagsChanged, onTagsChange);

    return this->_parentElement;
}
