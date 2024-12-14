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

    auto tagLeft = std::make_shared<UIPictureElement>(G_App::IMAGES_PATH + "tag_left.png", this->comm);
    tagLeft->setColor(this->color);
    tagLeft->setH(this->TAG_HIGHT);
    tagLeft->setW(this->SIDE_WIDTH);

    auto body = std::make_shared<UIBox>("tag_body", this->comm, this->TAG_HIGHT, this->TAG_HIGHT, 0, this->color);
    body->displayMode = DisplayMode::Distribute;
    body->childrenDistPos = RelPos::Center;
    body->childrenAlignPos = RelPos::Center;
    auto tagName = std::make_shared<UIText>("tag_name", this->comm, this->name, this->TAG_FONT_SIZE);
    tagName->editable = true;
    body->setW(tagName->getW() + this->SIDE_WIDTH);
    UIElement::addChildren(body, {tagName});

    auto tagRight = std::make_shared<UIPictureElement>(G_App::IMAGES_PATH + "tag_right.png", this->comm);
    tagRight->setColor(this->color);
    tagRight->setH(this->TAG_HIGHT);
    tagRight->setW(this->SIDE_WIDTH);

    UIElement::addChildren(this->_parentElement, {tagLeft, body, tagRight});
    this->_parentElement->setH(this->TAG_HIGHT);
    this->_parentElement->setW(this->_parentElement->getChildWSum());
    return this->_parentElement;
}