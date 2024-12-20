#include "BaseElements/UIText.h"

void UIText::updateSurface()
{
    this->freeSurface();
    SDL_Color color = {this->_color.r, this->_color.g, this->_color.b, (uint8_t)((this->_text == L"") ? 80 : 255)};
    std::wstring finalText = this->_text == L"" ? this->_placeholder : this->_text;
    if (finalText == L"")
    {
        finalText = L" ";
    }
    if (this->_editing)
    {
        if (this->_cursorIndex < 0 || this->_text.length() < this->_cursorIndex)
        {
            this->_cursorIndex = this->_text.length();
        }
        finalText.insert(this->_cursorIndex, L"|");
    }
    uint16string finalTextInt = wStrToUInt16(finalText);
    SDL_Surface *defaultSurface;
    if (this->wrap && (this->parentElement != NULL || this->_wrapSize > 0))
    {
        defaultSurface = TTF_RenderUNICODE_Blended_Wrapped(this->_font, finalTextInt.c_str(), color,
                                                           this->_wrapSize > 0 ? this->_wrapSize : this->parentElement->calcW());
    }
    else
    {
        defaultSurface = TTF_RenderUNICODE_Blended(this->_font, finalTextInt.c_str(), color);
    }
    this->_surface = SDL_ConvertSurfaceFormat(defaultSurface, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(defaultSurface);
    this->_w = this->_surface->w;
    this->_h = this->_surface->h;
    this->freeTexture();
    this->_texture = surfaceToTexture(this->comm, this->_surface);
}

int UIText::getFontSize()
{
    return this->_fontSize;
}

std::string UIText::getFontPath()
{
    return this->_fontPath;
}

void UIText::loadFont(std::string path)
{
    this->_font = TTF_OpenFont(path.c_str(), this->_fontSize);
    if (!this->_font)
    {
        this->comm->logger->addErrorLog("Failed to load font: ", TTF_GetError());
    }
}

void UIText::setFont(std::optional<int> fontSize, std::optional<std::string> path)
{
    if (fontSize.has_value())
    {
        this->_fontSize = fontSize.value();
    }
    if (path.has_value())
    {
        this->_fontPath = path.value();
    }
    if (fontSize.has_value() || path.has_value())
    {
        this->loadFont(this->_fontPath);
        this->updateSurface();
    }
}

std::wstring UIText::getPlaceholder()
{
    return this->_placeholder;
}

void UIText::setPlaceholder(std::wstring placeholder)
{
    this->_placeholder = placeholder;
    this->updateSurface();
}

std::wstring UIText::getText()
{
    return this->_text;
}

void UIText::setText(std::wstring text)
{
    this->_text = text;
    if ((this->_historyCursor + 1) < this->_textHistory.size())
    {
        this->_textHistory.erase(this->_textHistory.begin() + this->_historyCursor + 1, this->_textHistory.end());
    }
    this->_historyCursor = this->_textHistory.size();
    this->_textHistory.push_back(text);
    this->updateSurface();
    // std::cout << wstrToHex(this->_text) << '\n';
}

int UIText::getCursorIndex()
{
    return this->_cursorIndex;
}

void UIText::setCursorIndex(int cursorIndex)
{
    this->_cursorIndex = cursorIndex;
    this->updateSurface();
}

int UIText::getWrapSize()
{
    return this->_wrapSize;
}

void UIText::setWrapSize(int wrapSize)
{
    this->_wrapSize = wrapSize;
    this->updateSurface();
}

UIText::UIText(std::string name, CommonObjects *comm, std::wstring text, int fontSize, RGBA color) : UIDynamicElement(name, comm),
                                                                                                     _text(text),
                                                                                                     _fontSize(fontSize)
{
    this->pivotPosH = RelPos::Center;
    this->pivotPosV = RelPos::Center;
    this->_fontPath = this->comm->config->defaultFont;
    this->loadFont(this->_fontPath);
    this->_textHistory.push_back(text);
    this->_color = color;

    auto editEnable = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
        if (!textEl->editable)
        {
            return EventResult<std::shared_ptr<UIElement>>();
        }
        if (el->checkCollision(e.button.x, e.button.y))
        {
            textEl->_cursorIndex = -1;
            textEl->_editing = true;
        }
        else
        {
            textEl->_editing = false;
        }
        textEl->updateSurface();
        return EventResult<std::shared_ptr<UIElement>>();
    };
    this->events.addHandler(SDL_MOUSEBUTTONUP, editEnable);

    auto appEditText = [](std::shared_ptr<UIElement> &el, const std::shared_ptr<AppEvent> &e)
    {
        std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
        if (!textEl->editable)
        {
            return EventResult<std::shared_ptr<UIElement>>();
        }
        textEl->_cursorIndex = -1;
        textEl->_editing = true;
        textEl->updateSurface();
        return EventResult<std::shared_ptr<UIElement>>();
    };
    this->appEvents.addHandler((int)AppEventType::EditText, appEditText);

    auto textInput = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
        if (!textEl->_editing)
        {
            return EventResult<std::shared_ptr<UIElement>>();
        }
        if (std::string(e.text.text).length() > 0)
        {
            std::wstring curText = textEl->getText();
            std::wstring eText = strToWStr(e.text.text);
            curText.insert(textEl->getCursorIndex(), eText);
            textEl->setText(curText);
            textEl->setCursorIndex(textEl->getCursorIndex() + eText.length());
        }
        return EventResult<std::shared_ptr<UIElement>>();
    };
    this->events.addHandler(SDL_TEXTINPUT, textInput);

    auto specialKeysInput = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
        if (!textEl->_editing)
        {
            return EventResult<std::shared_ptr<UIElement>>();
        }
        if (e.key.keysym.sym == SDLK_BACKSPACE && textEl->getText().length() > 0 && textEl->getCursorIndex() > 0)
        {
            std::wstring curText = textEl->getText();
            int charCount = 1;
            if (isKeyPressed(VK_CONTROL))
            {
                charCount = Str::distanceToWSpace(curText, -1, textEl->getCursorIndex());
            }
            curText.erase(textEl->getCursorIndex() - charCount, charCount);
            textEl->setCursorIndex(textEl->getCursorIndex() - charCount);
            textEl->setText(curText);
        }
        if (e.key.keysym.sym == SDLK_DELETE && textEl->getText().length() > 0 && textEl->getCursorIndex() < textEl->getText().length())
        {
            std::wstring curText = textEl->getText();
            int charCount = 1;
            if (isKeyPressed(VK_CONTROL))
            {
                charCount = Str::distanceToWSpace(curText, 1, textEl->getCursorIndex());
            }
            curText.erase(textEl->getCursorIndex(), charCount);
            textEl->setText(curText);
        }
        if (e.key.keysym.sym == SDLK_LEFT && textEl->getCursorIndex() > 0)
        {
            textEl->setCursorIndex(textEl->getCursorIndex() - 1);
        }
        if (e.key.keysym.sym == SDLK_RIGHT)
        {
            textEl->setCursorIndex(textEl->getCursorIndex() + 1);
        }
        if (isKeyPressed(VK_CONTROL) && e.key.keysym.sym == SDLK_v)
        {
            std::wstring curText = textEl->getText();
            std::wstring clipboardText = getClipboardText();
            curText.insert(textEl->getCursorIndex(), clipboardText);
            textEl->setText(curText);
            textEl->setCursorIndex(textEl->getCursorIndex() + clipboardText.length());
        }
        if (isKeyPressed(VK_CONTROL) && e.key.keysym.sym == SDLK_z)
        {
            textEl->undo();
        }
        if (isKeyPressed(VK_CONTROL) && e.key.keysym.sym == SDLK_y)
        {
            textEl->redo();
        }
        return EventResult<std::shared_ptr<UIElement>>();
    };
    this->events.addHandler(SDL_KEYDOWN, specialKeysInput);

    this->updateSurface();
}

void UIText::undo()
{
    if (this->_textHistory.size() <= 1 || this->_historyCursor <= 0)
    {
        return;
    }
    this->_historyCursor--;
    this->_text = this->_textHistory.at(this->_historyCursor);
    this->updateSurface();
}

void UIText::redo()
{
    if ((this->_historyCursor + 1) >= this->_textHistory.size())
    {
        return;
    }
    this->_historyCursor++;
    this->_text = this->_textHistory.at(this->_historyCursor);
    this->updateSurface();
}

UIText::~UIText()
{
    TTF_CloseFont(this->_font);
}
