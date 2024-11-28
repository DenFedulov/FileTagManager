#include "ElementGroups/TagElement.h"

TagElement::TagElement(CommonObjects *comm, std::string name) : ElementGroup(comm), _name(name)
{
    this->createElementGroup();
}

TagElement::TagElement(CommonObjects *comm, std::string name, RGBA color) : ElementGroup(comm), _name(name), _color(color)
{
    this->createElementGroup();
}

void TagElement::setColor(RGBA color)
{
    this->_color = color;
    for (const auto &child : this->_parentElement->childElements)
    {
        std::shared_ptr<UIDynamicElement> childEl = std::static_pointer_cast<UIDynamicElement>(child);
        childEl->setColor(color);
    }
}

RGBA TagElement::getColor()
{
    return this->_color;
}

void TagElement::createElementGroup()
{
    this->_parentElement = std::make_shared<UIElement>("tag", this->_comm);
    this->_parentElement->displayMode = DisplayMode::Distribute;
    this->_parentElement->childrenDistPos = RelPos::Start;

    auto tagLeft = std::make_shared<UIPictureElement>(G_App::IMAGES_PATH + "tag_left.png", this->_comm);
    tagLeft->setColor(this->_color);
    tagLeft->setH(this->TAG_HIGHT);
    tagLeft->setW(this->SIDE_WIDTH);

    auto body = std::make_shared<UIBox>("tag_body", this->_comm, this->TAG_HIGHT, this->TAG_HIGHT, 0, this->_color);
    body->displayMode = DisplayMode::Distribute;
    body->childrenDistPos = RelPos::Center;
    body->childrenAlignPos = RelPos::Center;
    auto tagName = std::make_shared<UIText>("tag_name", this->_comm, strToWStr(this->_name.c_str()));
    body->setW(tagName->getW() + this->SIDE_WIDTH);
    body->addChildren({tagName});

    auto tagRight = std::make_shared<UIPictureElement>(G_App::IMAGES_PATH + "tag_right.png", this->_comm);
    tagRight->setColor(this->_color);
    tagRight->setH(this->TAG_HIGHT);
    tagRight->setW(this->SIDE_WIDTH);

    this->_parentElement->addChildren({tagLeft, body, tagRight});
    this->_parentElement->setW(this->_parentElement->getChildWSum());
}