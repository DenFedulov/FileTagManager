#include "BaseElements.h"

UIElement::UIElement(std::string name, FileTagManager *app, bool isMainElement) : name(name), _app(app)
{
    if (isMainElement)
    {
        int width, hight;
        SDL_GetWindowSize(this->_app->window, &width, &hight);
        this->w = width;
        this->h = hight;
    }
}

int UIElement::calcCoordRelToParent(
    int baseCoord,
    std::function<int()> pCalc,
    DistDirection mainDistDirection,
    std::function<int(RelPos)> calcDistPos,
    RelPos distPos,
    std::function<int(RelPos)> calcAlignPos,
    RelPos alignPos,
    RelPos pivotPos,
    int pivotDim,
    int pivotDefault)
{
    if (this->parentElement->displayMode == DisplayMode::Distribute)
    {
        if (this->parentElement->distDirection == mainDistDirection)
        {
            if (distPos != RelPos::None ||
                (distPos == RelPos::None && this->parentElement->childrenDistPos != RelPos::None))
            {
                return calcDistPos(
                    distPos != RelPos::None ? distPos : this->parentElement->childrenDistPos);
            }
        }
        else
        {
            if (alignPos != RelPos::None ||
                (alignPos == RelPos::None && this->parentElement->childrenAlignPos != RelPos::None))
            {
                return calcAlignPos(
                    alignPos != RelPos::None ? alignPos : this->parentElement->childrenAlignPos);
            }
        }
    }
    return baseCoord + pCalc() - calcPivotOffset(pivotPos, pivotDim, pivotDefault);
}

int UIElement::calcCoord(
    int baseCoord,
    Direction anchorDirection,
    std::function<int()> pCalc,
    DistDirection mainDistDirection,
    std::function<int(RelPos)> calcDistPos,
    RelPos distPos,
    std::function<int(RelPos)> calcAlignPos,
    RelPos alignPos,
    RelPos pivotPos,
    int pivotDim, // potential overhang
    int pivotDefault,
    Direction marginDirection)
{
    int result;
    if (this->anchors[anchorDirection] && this->parentElement != NULL)
    {
        result = pCalc();
    }
    else if (this->parentElement != NULL)
    {
        result = this->calcCoordRelToParent(
            baseCoord,
            pCalc,
            mainDistDirection,
            calcDistPos,
            distPos,
            calcAlignPos,
            alignPos,
            pivotPos,
            pivotDim,
            pivotDefault);
    }
    else
    {
        result = baseCoord - calcPivotOffset(pivotPos, pivotDim, pivotDefault);
    }
    return result + this->margin[marginDirection];
}

int UIElement::calcX()
{
    return this->calcCoord(
        this->x,
        Direction::Left,
        [this]()
        { return this->parentElement != NULL ? this->parentElement->calcX() : 0; },
        DistDirection::column,
        [this](RelPos p)
        { return this->calcDistPosH(p); },
        this->distPosH,
        [this](RelPos p)
        { return this->calcAlignPosV(p); },
        this->alignPosV,
        this->pivotPosH,
        this->calcW(),
        this->pivot.first,
        Direction::Left);
}

int UIElement::calcY()
{
    return this->calcCoord(
        this->y,
        Direction::Up,
        [this]()
        { return this->parentElement != NULL ? this->parentElement->calcY() : 0; },
        DistDirection::row,
        [this](RelPos p)
        { return this->calcDistPosV(p); },
        this->distPosV,
        [this](RelPos p)
        { return this->calcAlignPosH(p); },
        this->alignPosH,
        this->pivotPosV,
        this->calcH(),
        this->pivot.second,
        Direction::Left);
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

int UIElement::calcPivotOffsetH(RelPos p)
{
    return calcPivotOffset(p, this->calcW(), this->pivot.first);
}

int UIElement::calcPivotOffsetV(RelPos p)
{
    return calcPivotOffset(p, this->calcH(), this->pivot.second);
}

int UIElement::getChildWSum(int upTo)
{
    if (upTo < -9)
    {
        upTo = this->childElements.size() - 1;
    }
    int sum = 0;
    for (int i = 0; i <= upTo; i++)
    {
        sum += this->childElements[i]->w;
    }
    return sum;
}

int UIElement::getChildHSum(int upTo)
{
    if (upTo < -9)
    {
        upTo = this->childElements.size() - 1;
    }
    int sum = 0;
    for (int i = 0; i <= upTo; i++)
    {
        sum += this->childElements[i]->h;
    }
    return sum;
}

int UIElement::getChildMaxW()
{
    int max = 0;
    for (const auto &child : this->childElements)
    {
        int width = child->calcW();
        if (max < width)
        {
            max = width;
        }
    }
    return max;
}

int UIElement::getChildMaxH()
{
    int max = 0;
    for (const auto &child : this->childElements)
    {
        int height = child->calcH();
        if (max < height)
        {
            max = height;
        }
    }
    return max;
}

int UIElement::calcDistPosH(RelPos p)
{
    int childCount = this->parentElement->childElements.size();
    int wSum = this->parentElement->getChildWSum();
    int wOffset = this->parentElement->getChildWSum(this->childIndex - 1);
    return calcRelPos(
        p,
        this->parentElement->calcX(),
        this->parentElement->calcW(),
        this->childIndex,
        childCount,
        wSum,
        wOffset,
        this->pivot.first);
}

int UIElement::calcDistPosV(RelPos p)
{
    int childCount = this->parentElement->childElements.size();
    int hSum = this->parentElement->getChildHSum();
    int hOffset = this->parentElement->getChildHSum(this->childIndex - 1);
    return calcRelPos(
        p,
        this->parentElement->calcY(),
        this->parentElement->calcH(),
        this->childIndex,
        childCount,
        hSum,
        hOffset,
        this->pivot.second);
}

int UIElement::calcAlignPosH(RelPos p)
{
    RelPos pivotPos = this->pivotPosV != RelPos::None ? this->pivotPosV : this->parentElement->childrenPivotPos;
    int maxH = this->parentElement->getChildMaxH();
    int parentOffset = calcPivotOffset(pivotPos, this->parentElement->calcH(), this->pivot.second, maxH) + this->parentElement->calcY();

    int result = calcRelPos(
        p,
        parentOffset,
        this->parentElement->getChildMaxH(),
        0,
        1,
        this->calcH(),
        0,
        this->pivot.second);
    return result;
}

int UIElement::calcAlignPosV(RelPos p)
{
    RelPos pivotPos = this->pivotPosH != RelPos::None ? this->pivotPosH : this->parentElement->childrenPivotPos;
    int maxW = this->parentElement->getChildMaxW();
    int parentOffset = calcPivotOffset(pivotPos, this->parentElement->calcW(), this->pivot.first, maxW) + this->parentElement->calcX();

    int result = calcRelPos(
        p,
        parentOffset,
        maxW,
        0,
        1,
        this->calcW(),
        0,
        this->pivot.first);
    return result;
}

bool UIElement::checkCollision(int x, int y)
{
    return x >= this->calcX() && x < this->calcX() + this->calcW() && y >= this->calcY() && y < this->calcY() + this->calcH();
}

void UIElement::freeSurface()
{
    if (this->_surface != nullptr)
    {
        SDL_FreeSurface(this->_surface);
        this->_surface = nullptr;
    }
}

SDL_Texture *UIElement::getTexture()
{
    return this->_texture;
}

void UIElement::freeTexture()
{
    if (this->_texture != nullptr)
    {
        SDL_DestroyTexture(this->_texture);
        this->_texture = nullptr;
    }
}

FileTagManager *UIElement::getApp()
{
    return this->_app;
}

UIElement::~UIElement()
{
    this->freeTexture();
}

void UIElement::addChildren(const std::vector<std::shared_ptr<UIElement>> &childElements)
{
    for (auto &childElement : childElements)
    {
        childElement->childIndex = this->childElements.size();
        childElement->parentElement = std::shared_ptr<UIElement>(this);
        this->childElements.push_back(childElement);
    }
}

void UIElement::removeChild(int id)
{
    if (id >= this->childElements.size())
    {
        return;
    }
    auto childElement = this->childElements.at(id);
    this->childElements.erase(this->childElements.begin() + id);
    this->parentElement = this->_app->mainElement;
}

std::shared_ptr<UIElement> UIElement::getChild(int id)
{
    if (id < this->childElements.size())
    {
        return this->childElements.at(id);
    }
    return nullptr;
}

void UIElement::draw(SDL_Point *rotationPoint, double angle, SDL_RendererFlip flip)
{
    if (!this->visible)
    {
        return;
    }
    if (this->_texture != NULL)
    {
        SDL_Rect dest;
        dest.x = this->calcX();
        dest.y = this->calcY();
        dest.w = this->calcW();
        dest.h = this->calcH();
        SDL_RenderCopyEx(this->_app->renderer, this->_texture, NULL, &dest, angle, rotationPoint, flip);
    }
    if (this->z > -1)
    {
        return;
    }
    for (auto &childElement : this->childElements)
    {
        childElement->draw(rotationPoint, angle, flip);
    }
}

void UIText::updateSurface()
{
    this->freeSurface();
    SDL_Color color = {this->_color.r, this->_color.g, this->_color.b};
    std::string finalText = this->_text;
    if (finalText == "")
    {
        finalText = " ";
    }
    if (this->editable)
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
    this->_texture = surfaceToTexture(this->_app, this->_surface);
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
        this->_app->logger->addErrorLog("Failed to load font: ", TTF_GetError());
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

UIText::UIText(std::string name, FileTagManager *app, std::string text, int fontSize, RGBA color) : UIElement(name, app),
                                                                                                    _text(text),
                                                                                                    _fontSize(fontSize),
                                                                                                    _color(color)
{
    this->pivotPosH = RelPos::Center;
    this->pivotPosV = RelPos::Center;
    this->_fontPath = this->_app->config->defaultFont;
    this->loadFont(this->_fontPath);

    auto editEnable = [](std::shared_ptr<UIElement> &el, AppEvent &e)
    {
        std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
        if (el->checkCollision(e.mouseEvent.pos.first, e.mouseEvent.pos.second))
        {
            textEl->_cursorIndex = -1;
            textEl->editable = true;
        }
        else
        {
            textEl->editable = false;
        }
        textEl->updateSurface();
    };
    this->events.addHandler(AppEvent::mouse_button_down, editEnable);
    auto textInput = [](std::shared_ptr<UIElement> &el, AppEvent &e)
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
        }
    };
    this->events.addHandler(AppEvent::text_input, textInput);
    auto specialKeysInput = [](std::shared_ptr<UIElement> &el, AppEvent &e)
    {
        std::shared_ptr<UIText> textEl = std::static_pointer_cast<UIText>(el);
        if (!textEl->editable)
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

UIPictureElement::UIPictureElement(std::string name, FileTagManager *app) : UIElement(name, app)
{
    this->_texture = this->loadPictureTexture(name);
    SDL_QueryTexture(this->_texture, NULL, NULL, &this->w, &this->h);
}

SDL_Texture *UIPictureElement::loadPictureTexture(std::string path)
{
    if (this->_texture != nullptr)
    {
        return this->_texture;
    }
    SDL_Texture *texture = surfaceToTexture(this->_app, Str::endsWith(path, ".bmp") ? SDL_LoadBMP(path.c_str()) : IMG_Load(path.c_str()), true);
    return texture;
}

UIPictureElement::UIPictureElement() : UIElement(name, _app)
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
    this->_radius = radius;
    Setter::setInRange(this->_radius, 0, this->getMaxRadius() - 1);
    this->updateSurface();
}

int UIBox::getRadius()
{
    return this->_radius;
}

void UIBox::setColor(RGBA color)
{
    this->_color = color;
    this->updateSurface();
}

RGBA UIBox::getColor()
{
    return this->_color;
}

void UIBox::setBorderWidth(int borderWidth)
{
    this->_borderWidth = borderWidth;
    this->updateSurface();
}

int UIBox::getBorderWidth()
{
    return this->_borderWidth;
}

void UIBox::setBorderColor(RGBA borderColor)
{
    this->_borderColor = borderColor;
    this->updateSurface();
}

RGBA UIBox::getBorderColor()
{
    return this->_borderColor;
}

void UIBox::updateSurface()
{
    this->_surface = this->makeSurface();
    SurfaceEditor editor(this->_surface);
    editor.setDrawColor(this->_color);
    editor.fillRectangle(this->_radius, 0, this->w - this->_radius, this->h);
    editor.fillRectangle(0, this->_radius, this->w, this->h - this->_radius);
    CoordsVector topLeft = Geometry::mirrorAndAddCoords(
        Geometry::mirrorAndAddCoords(
            Geometry::circleQuater(this->_radius), this->_radius, this->_radius, Geometry::Horizontal),
        this->_radius, this->_radius, Geometry::Vertical);
    CoordsVector topRight = Geometry::mirrorAndAddCoords(Geometry::circleQuater(this->_radius), this->_radius, this->_radius, Geometry::Vertical);
    CoordsVector bottomLeft = Geometry::mirrorAndAddCoords(Geometry::circleQuater(this->_radius), this->_radius, this->_radius, Geometry::Horizontal);
    CoordsVector bottomRight = Geometry::circleQuater(this->_radius);

    editor.fillWithCoordsVector(topLeft, this->_radius, true, 0, 0);
    editor.fillWithCoordsVector(topRight, this->_radius, true, this->w - 1 - this->_radius, 0);
    editor.fillWithCoordsVector(bottomLeft, this->h - 1 - this->_radius, true, 0, this->h - 1 - this->_radius);
    editor.fillWithCoordsVector(bottomRight, this->h - 1 - this->_radius, true, this->w - 1 - this->_radius, this->h - 1 - this->_radius);

    topRight = Geometry::addToCoords(topRight, this->w - 1 - this->_radius, 0);
    bottomLeft = Geometry::addToCoords(bottomLeft, 0, this->h - 1 - this->_radius);
    bottomRight = Geometry::addToCoords(bottomRight, this->w - 1 - this->_radius, this->h - 1 - this->_radius);
    CoordsVector top = Geometry::line({this->_radius, 0}, {this->w - 1 - this->_radius, 0});
    CoordsVector left = Geometry::line({0, this->_radius}, {0, this->h - 1 - this->_radius});
    CoordsVector right = Geometry::line({this->w - 1, this->_radius}, {this->w - 1, this->h - 1 - this->_radius});
    CoordsVector down = Geometry::line({this->_radius, this->h - 1}, {this->w - 1 - this->_radius, this->h - 1});

    CoordsVector countor = Vect::concat<intPair>(topLeft, topRight, bottomLeft, bottomRight, top, left, right, down);
    Geometry::BorderMaker borderMaker(countor, this->_borderWidth);
    CoordsVector border = borderMaker.make();
    editor.setDrawColor(this->_borderColor);
    editor.drawWithCoordsVector(border);

    this->freeTexture();
    this->_texture = surfaceToTexture(this->_app, this->_surface);
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
    this->_radius = radius;
    Setter::setInRange(this->_radius, 0, this->getMaxRadius() - 1);
    this->_color = color;
    this->_borderWidth = borderWidth;
    this->_borderColor = borderColor;
    this->updateSurface();
}
