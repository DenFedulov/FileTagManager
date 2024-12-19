#include "ElementGroups/UIButton.h"

UIButton::UIButton(CommonObjects *comm, std::wstring buttonText, int fontSize, RGBA color, int borderRadius, int borderWidth, RGBA borderColor)
    : ElementGroup(comm),
      buttonText(buttonText),
      fontSize(fontSize),
      borderRadius(borderRadius),
      borderWidth(borderWidth),
      borderColor(borderColor),
      color(color)
{
}

std::shared_ptr<UIElement> UIButton::getElement()
{
    if (this->_parentElement != nullptr)
    {
        return this->_parentElement;
    }
    this->_parentElement = std::make_shared<UIBox>("button", this->comm, 1, 1, this->borderRadius, this->color, this->borderWidth, this->borderColor);
    this->_parentElement->displayMode = DisplayMode::Distribute;
    this->_parentElement->childrenDistPos = RelPos::Center;
    this->_parentElement->childrenAlignPos = RelPos::Center;

    auto buttonText = std::make_shared<UIText>("buttonText", this->comm, this->buttonText, this->fontSize);
    this->_parentElement->setH(buttonText->getH() + 8);
    this->_parentElement->setW(buttonText->getW() + 8);
    UIElement::addChildren(this->_parentElement, {buttonText});
    return this->_parentElement;
}
