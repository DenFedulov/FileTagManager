#include "Elements.h"

UIElement::UIElement(std::string name, FileTagManager *app, bool isMainElement) : name(name), app(app)
{
    if (isMainElement)
    {
        int width, hight;
        SDL_GetWindowSize(this->app->window, &width, &hight);
        this->visible = false;
        this->w = width;
        this->h = hight;
    }
}

int UIElement::calcX()
{
    if (this->anchors[Direction::Left] && this->parentElement != NULL)
    {
        return this->parentElement->calcX();
    }
    return this->x - this->calcPivotPosition(this->pivotPositionH, false);
}

int UIElement::calcY()
{
    if (this->anchors[Direction::Up] && this->parentElement != NULL)
    {
        return this->parentElement->calcY();
    }
    return this->y - this->calcPivotPosition(this->pivotPositionV, true);
}

int UIElement::calcW()
{
    if (this->anchors[Direction::Right] && this->parentElement != NULL)
    {
        return this->parentElement->calcW();
    }
    return this->w;
}

int UIElement::calcH()
{
    if (this->anchors[Direction::Down] && this->parentElement != NULL)
    {
        return this->parentElement->calcH();
    }
    return this->h;
}

int UIElement::getW()
{
    return this->w;
}

int UIElement::getH()
{
    return this->h;
}

void UIElement::setW(int w)
{
    this->w = w;
}

void UIElement::setH(int h)
{
    this->h = h;
}

int UIElement::calcPivotPosition(PivotPosition p, bool vertical)
{
    switch (p)
    {
    case PivotPosition::Start:
        return 0;
    case PivotPosition::Center:
        if (!vertical)
        {
            return this->w / 2;
        }
        else
        {
            return this->h / 2;
        }
        break;
    case PivotPosition::End:
        if (!vertical)
        {
            return this->w - 1;
        }
        else
        {
            return this->h - 1;
        }
    default:
        if (!vertical)
        {
            return this->pivot.first;
        }
        else
        {
            return this->pivot.second;
        }
    }
}

bool UIElement::checkCollision(int x, int y)
{
    return x >= this->calcX() && x < this->calcX() + this->w && y >= this->calcY() && y < this->calcY() + this->h;
}

void UIElement::freeSurface()
{
    if (this->surface != nullptr)
    {
        SDL_FreeSurface(this->surface);
        this->surface = nullptr;
    }
}

SDL_Texture *UIElement::getTexture()
{
    return this->texture;
}

void UIElement::freeTexture()
{
    if (this->texture != nullptr)
    {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }
}

FileTagManager *UIElement::getApp()
{
    return this->app;
}

UIElement::~UIElement()
{
    this->freeTexture();
}

void UIElement::fillChildDrawElements()
{
    this->childDrawElements.clear();
    for (const auto &childPair : this->childElements)
    {
        this->childDrawElements.push_back(childPair.second);
    }
}

void UIElement::addChild(std::shared_ptr<UIElement> childElement, bool firstCall)
{
    if (!this->childElements.contains(childElement->name))
    {
        this->childElements.emplace(childElement->name, childElement);
        this->fillChildDrawElements();
    }
    if (firstCall)
    {
        childElement->setParent(std::shared_ptr<UIElement>(this), false);
    }
}

void UIElement::removeChild(std::string name, bool firstCall)
{
    if (!this->childElements.contains(name))
    {
        return;
    }
    auto childElement = this->childElements.at(name);
    this->childElements.erase(name);
    this->fillChildDrawElements();
    if (firstCall)
    {
        childElement->setParent(this->app->mainElement, false);
    }
}

void UIElement::setParent(std::shared_ptr<UIElement> parent, bool firstCall)
{
    this->parentElement = parent;
    if (firstCall)
    {
        this->parentElement->addChild(std::shared_ptr<UIElement>(this), false);
    }
}

std::shared_ptr<UIElement> UIElement::getParent()
{
    return this->parentElement;
}

std::shared_ptr<UIElement> UIElement::getChild(std::string name)
{
    if (this->childElements.contains(name))
    {
        return this->childElements.at(name);
    }
    return nullptr;
}

void UIElement::draw(SDL_Point *rotationPoint, double angle, SDL_RendererFlip flip)
{
    if (this->texture != NULL && this->visible)
    {
        SDL_Rect dest;
        dest.x = this->calcX();
        dest.y = this->calcY();
        dest.w = this->calcW();
        dest.h = this->calcH();
        SDL_RenderCopyEx(this->app->renderer, this->texture, NULL, &dest, angle, rotationPoint, flip);
    }
    for (auto &childElement : this->childDrawElements)
    {
        childElement->draw(rotationPoint, angle, flip);
    }
}

void UIText::updateSurface()
{
    this->freeSurface();
    SDL_Color color = {this->color.r, this->color.g, this->color.b};
    std::string finalText = this->text;
    if (finalText == "")
    {
        finalText = " ";
    }
    if (this->editable)
    {
        if (this->cursorIndex < 0 || finalText.length() < this->cursorIndex)
        {
            this->cursorIndex = finalText.length();
        }
        finalText.insert(this->cursorIndex, "|");
    }
    SDL_Surface *defaultSurface;
    if (this->parentElement != NULL)
    {
        defaultSurface = TTF_RenderUTF8_Blended_Wrapped(this->font, finalText.c_str(), color, this->parentElement->getW());
    }
    else
    {
        defaultSurface = TTF_RenderUTF8_Blended(this->font, finalText.c_str(), color);
    }
    this->surface = SDL_ConvertSurfaceFormat(defaultSurface, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(defaultSurface);
    this->w = this->surface->w;
    this->h = this->surface->h;
    this->freeTexture();
    this->texture = surfaceToTexture(this->app, this->surface);
}

int UIText::calcTextX()
{
    if (this->parentElement == NULL)
    {
        return this->calcX();
    }
    else
    {
        return this->parentElement->calcX() + this->parentElement->calcPivotPosition(this->pivotPositionH, false) - this->calcPivotPosition(this->pivotPositionH, false);
    }
}

int UIText::calcTextY()
{
    if (this->parentElement == NULL)
    {
        return this->calcY();
    }
    else
    {
        return this->parentElement->calcY() + this->parentElement->calcPivotPosition(this->pivotPositionV, true) - this->calcPivotPosition(this->pivotPositionV, true);
    }
}

int UIText::getFontSize()
{
    return this->fontSize;
}

std::string UIText::getFontPath()
{
    return this->fontPath;
}

void UIText::loadFont(std::string path)
{
    this->font = TTF_OpenFont(path.c_str(), this->fontSize);
    if (!this->font)
    {
        this->app->logger->addErrorLog("Failed to load font: ", TTF_GetError());
    }
}

void UIText::setFont(std::optional<int> fontSize, std::optional<std::string> path)
{
    if (fontSize.has_value())
    {
        this->fontSize = fontSize.value();
    }
    if (path.has_value())
    {
        this->fontPath = path.value();
    }
    if (fontSize.has_value() || path.has_value())
    {
        this->loadFont(this->fontPath);
        this->updateSurface();
    }
}

std::string UIText::getText()
{
    return this->text;
}

void UIText::setText(std::string text)
{
    this->text = text;
    this->updateSurface();
}

RGBA UIText::getColor()
{
    return this->color;
}

void UIText::setColor(RGBA color)
{
    this->color = color;
    this->updateSurface();
}

bool UIText::checkCollision(int x, int y)
{
    return x >= this->calcTextX() && x < this->calcTextX() + this->w && y >= this->calcTextY() && y < this->calcTextY() + this->h;
}

int UIText::getCursorIndex()
{
    return this->cursorIndex;
}

void UIText::setCursorIndex(int cursorIndex)
{
    this->cursorIndex = cursorIndex;
    this->updateSurface();
}

UIText::UIText(std::string name, FileTagManager *app, std::string text, int fontSize, RGBA color) : UIElement(name, app),
                                                                                                    text(text),
                                                                                                    fontSize(fontSize),
                                                                                                    color(color)
{
    this->pivotPositionH = PivotPosition::Center;
    this->pivotPositionV = PivotPosition::Center;
    this->fontPath = this->app->config->defaultFont;
    this->loadFont(this->fontPath);

    this->events.addHandler(AppEvent::mouse_button_down, [](std::shared_ptr<UIElement> &el, AppEvent &e)
                            { 
                                std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
                                if (el->checkCollision(e.mouseEvent.pos.first, e.mouseEvent.pos.second)){
                                    textEl->cursorIndex = -1;
                                    textEl->editable = true;
                            } else {
                                    textEl->editable = false;
                            } });
    this->events.addHandler(AppEvent::text_input, [](std::shared_ptr<UIElement> &el, AppEvent &e)
                            {
                                std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
                                if (!textEl->editable)
                                {
                                    return;
                                }
                                if (e.keyEvent.text.length() > 0)
                                {
                                    std::string curText = textEl->getText();
                                    curText.insert(textEl->getCursorIndex(), e.keyEvent.text);
                                    textEl->setText(curText);
                                    textEl->setCursorIndex(textEl->getCursorIndex() + e.keyEvent.text.length());
                                } });
    this->events.addHandler(AppEvent::keyboard_button_down, [](std::shared_ptr<UIElement> &el, AppEvent &e)
                            { 
                                std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
                                if(!textEl->editable){
                                    return;
                                }
                                if(e.keyEvent.keycode == SDLK_BACKSPACE && textEl->getText().length() > 0 && textEl->getCursorIndex() > 0){
                                    std::string curText = textEl->getText();
                                    curText.erase(textEl->getCursorIndex() - 1, 1);
                                    textEl->setCursorIndex(textEl->getCursorIndex() - 1);
                                    textEl->setText(curText);
                                } 
                                if(e.keyEvent.keycode == SDLK_DELETE && textEl->getText().length() > 0 && textEl->getCursorIndex() < textEl->getText().length()){
                                    std::string curText = textEl->getText();
                                    curText.erase(textEl->getCursorIndex(), 1);
                                    textEl->setText(curText);
                                } 
                                if(e.keyEvent.keycode == 1073741904 && textEl->getCursorIndex() > 0){
                                    textEl->setCursorIndex(textEl->getCursorIndex() - 1);
                                } 
                                if(e.keyEvent.keycode == 1073741903){
                                    textEl->setCursorIndex(textEl->getCursorIndex() + 1);
                                } });

    this->updateSurface();
}

UIText::UIText(std::string name, FileTagManager *app, std::string text, std::shared_ptr<UIElement> parentElement, int fontSize, RGBA color)
    : UIText(name, app, text, fontSize, color)
{
    this->setParent(parentElement);
}

void UIText::draw(SDL_Point *rotationPoint, double angle, SDL_RendererFlip flip)
{
    if (!this->visible)
    {
        return;
    }
    SDL_Rect dest;
    dest.x = this->calcTextX();
    dest.y = this->calcTextY();
    dest.w = this->calcW();
    dest.h = this->calcH();
    SDL_RenderCopyEx(this->app->renderer, this->texture, NULL, &dest, angle, rotationPoint, flip);
}

UIText::~UIText()
{
    TTF_CloseFont(this->font);
}

UIPictureElement::UIPictureElement(std::string name, FileTagManager *app) : UIElement(name, app)
{
    this->texture = this->loadPictureTexture(name);
    SDL_QueryTexture(this->texture, NULL, NULL, &this->w, &this->h);
}

SDL_Texture *UIPictureElement::loadPictureTexture(std::string path)
{
    if (this->texture != nullptr)
    {
        return this->texture;
    }
    SDL_Texture *texture = surfaceToTexture(this->app, Str::endsWith(path, ".bmp") ? SDL_LoadBMP(path.c_str()) : IMG_Load(path.c_str()), true);
    return texture;
}

UIPictureElement::UIPictureElement() : UIElement(name, app)
{
}

SDL_Surface *UIDynamicElement::makeSurface()
{
    this->freeSurface();
    return SDL_CreateRGBSurfaceWithFormat(0, this->w, this->h, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32);
}

UIDynamicElement::UIDynamicElement(std::string name, FileTagManager *app) : UIElement(name, app)
{
}

UIDynamicElement::~UIDynamicElement()
{
    this->freeSurface();
}

void UIDynamicElement::setW(int w)
{
    this->w = w;
    this->updateSurface();
}

void UIDynamicElement::setH(int h)
{
    this->h = h;
    this->updateSurface();
}

void UIBox::setRadius(int radius)
{
    this->radius = radius;
    Setter::setInRange(this->radius, 0, this->getMaxRadius() - 1);
    this->updateSurface();
}

int UIBox::getRadius()
{
    return this->radius;
}

void UIBox::setColor(RGBA color)
{
    this->color = color;
    this->updateSurface();
}

RGBA UIBox::getColor()
{
    return this->color;
}

void UIBox::setBorderWidth(int borderWidth)
{
    this->borderWidth = borderWidth;
    this->updateSurface();
}

int UIBox::getBorderWidth()
{
    return this->borderWidth;
}

void UIBox::setBorderColor(RGBA borderColor)
{
    this->borderColor = borderColor;
    this->updateSurface();
}

RGBA UIBox::getBorderColor()
{
    return this->borderColor;
}

void UIBox::updateSurface()
{
    this->surface = this->makeSurface();
    SurfaceEditor editor(this->surface);
    editor.setDrawColor(this->color);
    editor.fillRectangle(this->radius, 0, this->w - this->radius, this->h);
    editor.fillRectangle(0, this->radius, this->w, this->h - this->radius);
    CoordsVector topLeft = Geometry::mirrorAndAddCoords(
        Geometry::mirrorAndAddCoords(
            Geometry::circleQuater(this->radius), this->radius, this->radius, Geometry::Horizontal),
        this->radius, this->radius, Geometry::Vertical);
    CoordsVector topRight = Geometry::mirrorAndAddCoords(Geometry::circleQuater(this->radius), this->radius, this->radius, Geometry::Vertical);
    CoordsVector bottomLeft = Geometry::mirrorAndAddCoords(Geometry::circleQuater(this->radius), this->radius, this->radius, Geometry::Horizontal);
    CoordsVector bottomRight = Geometry::circleQuater(this->radius);

    editor.fillWithCoordsVector(topLeft, this->radius, true, 0, 0);
    editor.fillWithCoordsVector(topRight, this->radius, true, this->w - 1 - this->radius, 0);
    editor.fillWithCoordsVector(bottomLeft, this->h - 1 - this->radius, true, 0, this->h - 1 - this->radius);
    editor.fillWithCoordsVector(bottomRight, this->h - 1 - this->radius, true, this->w - 1 - this->radius, this->h - 1 - this->radius);

    topRight = Geometry::addToCoords(topRight, this->w - 1 - this->radius, 0);
    bottomLeft = Geometry::addToCoords(bottomLeft, 0, this->h - 1 - this->radius);
    bottomRight = Geometry::addToCoords(bottomRight, this->w - 1 - this->radius, this->h - 1 - this->radius);
    CoordsVector top = Geometry::line({this->radius, 0}, {this->w - 1 - this->radius, 0});
    CoordsVector left = Geometry::line({0, this->radius}, {0, this->h - 1 - this->radius});
    CoordsVector right = Geometry::line({this->w - 1, this->radius}, {this->w - 1, this->h - 1 - this->radius});
    CoordsVector down = Geometry::line({this->radius, this->h - 1}, {this->w - 1 - this->radius, this->h - 1});
    CoordsVector countor = Vect::concat({topLeft, topRight, bottomLeft, bottomRight, top, left, right, down});
    Geometry::BorderMaker borderMaker(countor, this->borderWidth);
    CoordsVector border = borderMaker.make();
    // CoordsArray arr2d(this->w, this->h);
    // Geometry::coordsVectorFillArray(border, arr2d);
    // print2DArray(arr2d);
    editor.setDrawColor(this->borderColor);
    editor.drawWithCoordsVector(border);
    this->freeTexture();
    this->texture = surfaceToTexture(this->app, this->surface);
}

int UIBox::getMaxRadius()
{
    return std::min(this->w / 2, this->h / 2);
}

UIBox::UIBox(std::string name, FileTagManager *app, int w, int h,
             int radius, RGBA color,
             int borderWidth, RGBA borderColor) : UIDynamicElement(name, app)
{
    this->w = w;
    this->h = h;
    this->radius = radius;
    Setter::setInRange(this->radius, 0, this->getMaxRadius() - 1);
    this->color = color;
    this->borderWidth = borderWidth;
    this->borderColor = borderColor;
    this->updateSurface();
}
