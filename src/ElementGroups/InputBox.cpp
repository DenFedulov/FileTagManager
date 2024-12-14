#include "ElementGroups/InputBox.h"

InputBox::InputBox(CommonObjects *comm, int w)
    : ElementGroup(comm),
      w(w)
{
}

std::shared_ptr<UIElement> InputBox::getElement()
{
    if (this->_parentElement != nullptr)
    {
        return this->_parentElement;
    }
    this->_parentElement = std::make_shared<UIBox>("inputBox", this->comm, this->w, this->h, 5, RGBA(), 2);
    this->_parentElement->displayMode = DisplayMode::Distribute;
    this->_parentElement->childrenDistPos = RelPos::Start;
    this->_parentElement->childrenAlignPos = RelPos::Center;

    auto inputWrap = std::make_shared<UIElement>("inputWrap", this->comm);
    inputWrap->setW(this->w - 8);
    inputWrap->setH(this->h);
    inputWrap->displayMode = DisplayMode::Distribute;
    inputWrap->childrenDistPos = RelPos::Start;
    inputWrap->childrenAlignPos = RelPos::Center;
    inputWrap->margin[Direction::Left] = 4;

    this->_inputElement = std::make_shared<UIText>("input", this->comm, L"", this->fontSize, RGBA(0, 0, 0));
    this->_inputElement->setPlaceholder(this->placeholder);
    this->_inputElement->editable = true;
    UIElement::addChildren(inputWrap, {this->_inputElement});
    UIElement::addChildren(this->_parentElement, {inputWrap});
    auto editinput = [input = this->_inputElement](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        auto inputEl = std::static_pointer_cast<UIElement>(input);
        input->appEvents.triggerEvent((int)AppEventType::EditText, inputEl, std::make_shared<AppEvent>(AppEventType::EditText));
        return EventResult<std::shared_ptr<UIElement>>();
    };
    this->_parentElement->events.addHandler((int)CustomEvent::MOUSE_CLICK, editinput);
    return this->_parentElement;
}
std::shared_ptr<UIText> InputBox::getInputElement()
{
    if (this->_parentElement == nullptr)
    {
        this->getElement();
    }
    return this->_inputElement;
}