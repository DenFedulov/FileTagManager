#include "ElementGroups/TagList.h"

TagList::TagList(CommonObjects *comm, int listSize, std::wstring ofFile)
    : ElementGroup(comm),
      ofFile(ofFile),
      listSize(listSize)
{
}

std::shared_ptr<UIElement> TagList::getElement()
{
    if (this->_parentElement != nullptr)
    {
        return this->_parentElement;
    }
    this->_parentElement = std::make_shared<UIElement>("TagList", this->comm);
    if (this->anchorDirection == Direction::Down)
    {
        this->_parentElement->setW(this->listSize);
    }
    else
    {
        this->_parentElement->setH(this->listSize);
    }
    this->_parentElement->anchors[this->anchorDirection] = true;
    this->_parentElement->displayMode = DisplayMode::Distribute;
    this->_parentElement->childrenDistPos = RelPos::Start;
    this->_parentElement->childrenAlignPos = RelPos::Start;
    this->_parentElement->margin[Direction::Left] = 5;
    this->_parentElement->margin[Direction::Right] = 5;

    std::vector<std::shared_ptr<UIElement>> tags;
    std::vector<std::string> tagNames;
    TableData tagData;
    if (this->ofFile != L"" && std::filesystem::exists(this->ofFile))
    {
        TableData fileTags = this->comm->db->getTagsOfFile(this->ofFile);
        for (const auto &row : fileTags.data)
        {
            tagNames.push_back(row.at(0));
        }
        if (tagNames.size() > 0)
        {
            tagData = this->comm->db->getTags(tagNames);
        }
    }
    else
    {
        tagData = this->comm->db->getTags();
    }
    for (const auto &row : tagData.data)
    {
        RGBA color = hexStrToRGBA(row.at(1).c_str());
        color.a = 70;
        TagElement tag(this->comm, hexStrToWStr(row.at(0)));
        tag.clickable = this->tagsClickable;
        tag.tagHight = this->tagHight;
        tag.tagFontSize = this->tagFontSize;
        tag.tagSideWidth = this->tagSideWidth;
        tag.color = color;
        tags.push_back(tag.getElement());
    }

    UIElement::addChildren(this->_parentElement, tags);

    if (this->anchorDirection != Direction::Down)
    {
        this->_parentElement->setH(this->_parentElement->calcChildRealH());
    }

    auto onTagsChange = [filePath = this->ofFile](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        EventResult<std::shared_ptr<UIElement>> result;
        if (e->type == AppEventType::FileTagChanged && e->eventText2 != filePath)
        {
            return result;
        }
        std::cout << "deleting old tag list on tag change\n";
        result.type = (int)EventResultType::RemoveElement;
        result.data = el;
        return result;
    };
    this->_parentElement->appEvents.addHandler((int)AppEventType::TagsChanged, onTagsChange);
    this->_parentElement->appEvents.addHandler((int)AppEventType::FileTagChanged, onTagsChange);

    return this->_parentElement;
}
