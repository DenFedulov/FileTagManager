#include "BaseElements/UIText.h"

void UIText::updateSurface()
{
    this->freeSurface();
    SDL_Color color = {this->_color.r, this->_color.g, this->_color.b};
    std::string finalText = this->_text;
    if (finalText == "")
    {
        finalText = " ";
    }
    if (this->_editing)
    {
        if (this->_cursorIndex < 0 || finalText.length() < this->_cursorIndex)
        {
            this->_cursorIndex = finalText.length();
        }
        finalText.insert(this->_cursorIndex, "|");
    }
    SDL_Surface *defaultSurface;
    if (this->parentElement != NULL)
    {
        defaultSurface = TTF_RenderUTF8_Blended_Wrapped(this->_font, finalText.c_str(), color, this->parentElement->getW());
    }
    else
    {
        defaultSurface = TTF_RenderUTF8_Blended(this->_font, finalText.c_str(), color);
    }
    this->_surface = SDL_ConvertSurfaceFormat(defaultSurface, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(defaultSurface);
    this->w = this->_surface->w;
    this->h = this->_surface->h;
    this->freeTexture();
    this->_texture = surfaceToTexture(this->_comm, this->_surface);
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
        this->_comm->logger->addErrorLog("Failed to load font: ", TTF_GetError());
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

std::string UIText::getText()
{
    return this->_text;
}

void UIText::setText(std::string text)
{
    this->_text = text;
    this->updateSurface();
}

RGBA UIText::getColor()
{
    return this->_color;
}

void UIText::setColor(RGBA color)
{
    this->_color = color;
    this->updateSurface();
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

UIText::UIText(std::string name, CommonObjects *comm, std::string text, int fontSize, RGBA color) : UIElement(name, comm),
                                                                                                    _text(text),
                                                                                                    _fontSize(fontSize),
                                                                                                    _color(color)
{
    this->pivotPosH = RelPos::Center;
    this->pivotPosV = RelPos::Center;
    this->_fontPath = this->_comm->config->defaultFont;
    this->loadFont(this->_fontPath);

    auto editEnable = [](std::shared_ptr<UIElement> &el, AppEvent &e)
    {
        std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
        if(!textEl->editable){
            return;
        }
        if (el->checkCollision(e.mouseEvent.pos.first, e.mouseEvent.pos.second))
        {
            textEl->_cursorIndex = -1;
            textEl->_editing = true;
        }
        else
        {
            textEl->_editing = false;
        }
        textEl->updateSurface();
    };
    this->events.addHandler(AppEvent::mouse_button_down, editEnable);
    auto textInput = [](std::shared_ptr<UIElement> &el, AppEvent &e)
    {
        std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
        if (!textEl->_editing)
        {
            return;
        }
        if (e.keyEvent.text.length() > 0)
        {
            std::string curText = textEl->getText();
            curText.insert(textEl->getCursorIndex(), e.keyEvent.text);
            textEl->setText(curText);
            textEl->setCursorIndex(textEl->getCursorIndex() + e.keyEvent.text.length());
        }
    };
    this->events.addHandler(AppEvent::text_input, textInput);
    auto specialKeysInput = [](std::shared_ptr<UIElement> &el, AppEvent &e)
    {
        std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
        if (!textEl->_editing)
        {
            return;
        }
        if (e.keyEvent.keycode == SDLK_BACKSPACE && textEl->getText().length() > 0 && textEl->getCursorIndex() > 0)
        {
            std::string curText = textEl->getText();
            curText.erase(textEl->getCursorIndex() - 1, 1);
            textEl->setCursorIndex(textEl->getCursorIndex() - 1);
            textEl->setText(curText);
        }
        if (e.keyEvent.keycode == SDLK_DELETE && textEl->getText().length() > 0 && textEl->getCursorIndex() < textEl->getText().length())
        {
            std::string curText = textEl->getText();
            curText.erase(textEl->getCursorIndex(), 1);
            textEl->setText(curText);
        }
        if (e.keyEvent.keycode == 1073741904 && textEl->getCursorIndex() > 0)
        {
            textEl->setCursorIndex(textEl->getCursorIndex() - 1);
        }
        if (e.keyEvent.keycode == 1073741903)
        {
            textEl->setCursorIndex(textEl->getCursorIndex() + 1);
        }
    };
    this->events.addHandler(AppEvent::keyboard_button_down, specialKeysInput);

    this->updateSurface();
}

UIText::~UIText()
{
    TTF_CloseFont(this->_font);
}
