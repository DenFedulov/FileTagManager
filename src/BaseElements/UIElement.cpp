#include "BaseElements/UIElement.h"

UIElement::UIElement(std::string name, CommonObjects *comm) : name(name), comm(comm)
{
    auto editEnable = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        if (el->overflow == OverflowMode::Scroll && el->checkCollision(e.wheel.mouseX, e.wheel.mouseY))
        {
            // std::cout << el->name << '\n';
            // std::cout << e.wheel.mouseX << '\n';
            // std::cout << e.wheel.mouseY << '\n';
            // std::cout << e.wheel.y << '\n';
            int scrollAmount = -e.wheel.y * 50; // TODO: maybe make it configurable/take from windows settings
            switch (el->scrollDirection)
            {
            case Direction::Down:
            case Direction::Up:
                el->moveScrollV(scrollAmount);
                break;
            case Direction::Left:
            case Direction::Right:
                el->moveScrollH(scrollAmount);
                break;
            }
            return EventResult<std::shared_ptr<UIElement>>((int)EventResultType::StopPropagation);
        }
        return EventResult<std::shared_ptr<UIElement>>();
    };
    this->events.addHandler(SDL_MOUSEWHEEL, editEnable);

    auto onResize = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            el->_cropRect = NULL;
        }
        return EventResult<std::shared_ptr<UIElement>>();
    };
    this->events.addHandler(SDL_WINDOWEVENT, onResize);
}

int UIElement::calcCoordRelToParent(
    int baseCoord,
    int pCalc,
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
    return baseCoord + pCalc - calcPivotOffset(pivotPos, pivotDim, pivotDefault);
}

int UIElement::calcCoord(
    int baseCoord,
    Direction anchorDirection,
    int pCalc,
    DistDirection mainDistDirection,
    std::function<int(RelPos)> calcDistPos,
    RelPos distPos,
    std::function<int(RelPos)> calcAlignPos,
    RelPos alignPos,
    RelPos pivotPos,
    int pivotDim, // potential overhang
    int pivotDefault)
{
    int result;
    if (this->anchors[anchorDirection] && this->parentElement != NULL)
    {
        result = pCalc;
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
    return result;
}

bool UIElement::hasCropRect()
{
    return this->parentElement != NULL && this->parentElement->_cropRect != NULL;
}

int UIElement::calcX()
{
    if (!this->visible)
    {
        return 0;
    }
    return this->calcCoord(
               this->x,
               Direction::Left,
               this->parentElement != NULL ? this->parentElement->hitbox->minX : 0,
               DistDirection::column,
               [this](RelPos p)
               { return this->calcDistPosH(p); },
               this->distPosH,
               [this](RelPos p)
               { return this->calcAlignPosV(p); },
               this->alignPosV,
               this->pivotPosH,
               this->_w,
               this->pivot.first) +
           this->margin[Direction::Left];
}

int UIElement::calcY()
{
    if (!this->visible)
    {
        return 0;
    }
    return this->calcCoord(
               this->y,
               Direction::Up,
               this->parentElement != NULL ? this->parentElement->hitbox->minY : 0,
               DistDirection::row,
               [this](RelPos p)
               { return this->calcDistPosV(p); },
               this->distPosV,
               [this](RelPos p)
               { return this->calcAlignPosH(p); },
               this->alignPosH,
               this->pivotPosV,
               this->_h,
               this->pivot.second) +
           this->margin[Direction::Up];
}

void UIElement::setMargin(int m)
{
    for (int i = 0; i < (sizeof(this->margin) / sizeof(this->margin[0])); i++)
    {
        this->margin[i] = m;
    }
}

int UIElement::getScrollH()
{
    return this->_scrollH;
}

void UIElement::setScrollH(int scrollH)
{
    if (this->_cropRect != NULL)
    {
        Setter::setInRange(scrollH,
                           0,
                           this->calcChildRealW() -
                               this->_cropRect->maxX +
                               this->_cropRect->minX +
                               this->margin[Direction::Left] +
                               this->margin[Direction::Right]);
        this->_scrollH = scrollH;
    }
}

void UIElement::moveScrollH(int step)
{
    this->setScrollH(this->_scrollH + step);
}

int UIElement::getScrollV()
{
    return this->_scrollV;
}

void UIElement::setScrollV(int scrollV)
{
    if (this->_cropRect != NULL)
    {
        Setter::setInRange(scrollV,
                           0,
                           this->calcChildRealH() -
                               this->_cropRect->maxY +
                               this->_cropRect->minY +
                               this->margin[Direction::Up] +
                               this->margin[Direction::Down]);
        this->_scrollV = scrollV;
    }
}

void UIElement::moveScrollV(int step)
{
    this->setScrollV(this->_scrollV + step);
}

int UIElement::calcW(bool inner)
{
    if (!this->visible)
    {
        return 0;
    }
    int result = this->_w;
    if (this->anchors[Direction::Right] && this->parentElement != NULL)
    {
        return this->parentElement->hitbox->maxX - this->calcX() - this->margin[Direction::Right];
    }
    return result + (inner ? 0 : this->margin[Direction::Left] + this->margin[Direction::Right]);
}

int UIElement::calcH(bool inner)
{
    if (!this->visible)
    {
        return 0;
    }
    int result = this->_h;
    if (this->anchors[Direction::Down] && this->parentElement != NULL)
    {
        return this->parentElement->hitbox->maxY - this->calcY() - this->margin[Direction::Down];
    }
    return result + (inner ? 0 : this->margin[Direction::Up] + this->margin[Direction::Down]);
}

int UIElement::getW()
{
    return this->_w;
}

int UIElement::getH()
{
    return this->_h;
}

void UIElement::setW(int w)
{
    this->_w = w;
}

void UIElement::setH(int h)
{
    this->_h = h;
}

int UIElement::getZ()
{
    return this->_z;
}

void UIElement::setZ(int z)
{
    this->_z = z;
    for (const auto &child : this->childElements.vec)
    {
        child->setZ(z);
    }
}

int UIElement::calcPivotOffsetH(RelPos p)
{
    return calcPivotOffset(p, this->calcW(), this->pivot.first);
}

int UIElement::calcPivotOffsetV(RelPos p)
{
    return calcPivotOffset(p, this->calcH(), this->pivot.second);
}

int UIElement::getChildWSum(std::optional<int> upTo)
{
    int limit = upTo.value_or(this->childElements.vec.size() - 1);
    int sum = 0;
    for (int i = 0; i <= limit; i++)
    {
        sum += this->childElements.vec[i]->calcW();
    }
    return sum;
}

int UIElement::getChildHSum(std::optional<int> upTo)
{
    int limit = upTo.value_or(this->childElements.vec.size() - 1);
    int sum = 0;
    for (int i = 0; i <= limit; i++)
    {
        sum += this->childElements.vec[i]->calcH();
    }
    return sum;
}

int UIElement::calcChildRealW()
{
    int min = INT_MAX;
    int max = INT_MIN;
    for (const auto &child : this->childElements.vec)
    {
        int x = child->calcX();
        int x2 = x + child->calcW();
        if (min > x)
        {
            min = x;
        }
        if (max < x2)
        {
            max = x2;
        }
    }
    return max - min;
}

int UIElement::calcChildRealH()
{
    int min = INT_MAX;
    int max = INT_MIN;
    for (const auto &child : this->childElements.vec)
    {
        int y = child->calcY();
        int y2 = y + child->calcH();
        if (min > y)
        {
            min = y;
        }
        if (max < y2)
        {
            max = y2;
        }
    }
    return max - min;
}

int UIElement::getMaxChildW()
{
    int max = 0;
    for (const auto &child : this->childElements.vec)
    {
        int width = child->calcW();
        if (max < width)
        {
            max = width;
        }
    }
    return max;
}

int UIElement::getMaxChildH()
{
    int max = 0;
    for (const auto &child : this->childElements.vec)
    {
        int height = child->calcH();
        if (max < height)
        {
            max = height;
        }
    }
    return max;
}

ChildWrappingData UIElement::calcChildWrapping(size_t toChild)
{
    const int directionSize = this->distDirection == DistDirection::column ? this->calcW(true) : this->calcH(true);
    ChildWrappingData result;
    int prevChildMaxDirectionHight = 0;
    for (size_t i = 0; i < this->childElements.vec.size(); i++)
    {
        auto child = this->childElements.vec.at(i);
        int childSize = this->distDirection == DistDirection::column ? child->calcW() : child->calcH();

        int prevChildSize = 0;
        if (i > 0)
        {
            auto prevChild = this->childElements.vec.at(i - 1);
            prevChildSize = this->distDirection == DistDirection::column ? prevChild->calcW() : prevChild->calcH();
            int prevChildDirectionHight = this->distDirection == DistDirection::column ? prevChild->calcH() : prevChild->calcW();
            if (prevChildMaxDirectionHight < prevChildDirectionHight)
            {
                prevChildMaxDirectionHight = prevChildDirectionHight;
            }
        }
        result.mainDirectionOffset += prevChildSize;
        if (i > 0 && directionSize - (result.mainDirectionOffset + childSize) < 0)
        {
            result.mainDirectionOffset = 0;
            result.otherDirectionOffset += prevChildMaxDirectionHight;
            prevChildMaxDirectionHight = 0;
        }
        if (i >= toChild)
        {
            break;
        }
    }

    return result;
}

int UIElement::calcDistPosH(RelPos p)
{
    int childCount = this->parentElement->childElements.vec.size();
    int wOffset = 0;
    int wSum = 0;
    if (!this->anchors[Direction::Down] && this->parentElement->distibutionWrapping)
    {
        wOffset = this->parentElement->calcChildWrapping(this->childIndex).mainDirectionOffset;
        wSum = this->calcW() + wOffset;
    }
    else
    {
        wOffset = this->parentElement->getChildWSum(this->childIndex - 1);
        wSum = this->parentElement->hitbox->w;
    }
    return calcRelPos(
        p,
        this->parentElement->hitbox->minX,
        this->parentElement->hitbox->w,
        this->childIndex,
        childCount,
        wSum,
        wOffset,
        this->pivot.first);
}

int UIElement::calcDistPosV(RelPos p)
{
    int childCount = this->parentElement->childElements.vec.size();
    int hOffset = 0;
    int hSum = 0;
    if (!this->anchors[Direction::Down] && this->parentElement->distibutionWrapping)
    {
        hOffset = this->parentElement->calcChildWrapping(this->childIndex).mainDirectionOffset;
        hSum = this->calcH() + hOffset;
    }
    else
    {
        hOffset = this->parentElement->getChildHSum(this->childIndex - 1);
        hSum = this->parentElement->hitbox->h;
    }
    return calcRelPos(
        p,
        this->parentElement->hitbox->minY,
        this->parentElement->hitbox->h,
        this->childIndex,
        childCount,
        hSum,
        hOffset,
        this->pivot.second);
}

int UIElement::calcAlignPosH(RelPos p)
{
    RelPos pivotPos = this->pivotPosV != RelPos::None ? this->pivotPosV : this->parentElement->childrenPivotPos;
    int maxH = this->parentElement->getMaxChildH();
    int parentOffset = calcPivotOffset(pivotPos, this->parentElement->hitbox->h, this->pivot.second, maxH) + this->parentElement->hitbox->minY;
    int childOffset = 0;
    if (this->parentElement->distibutionWrapping)
    {
        childOffset = this->parentElement->calcChildWrapping(this->childIndex).otherDirectionOffset;
    }
    else
    {
        childOffset = this->parentElement->getChildHSum(this->childIndex - 1);
    }

    int result = calcRelPos(
        p,
        parentOffset,
        this->parentElement->getMaxChildH(),
        0,
        1,
        this->calcH(),
        childOffset,
        this->pivot.second);
    return result;
}

int UIElement::calcAlignPosV(RelPos p)
{
    RelPos pivotPos = this->pivotPosH != RelPos::None ? this->pivotPosH : this->parentElement->childrenPivotPos;
    int maxW = this->parentElement->getMaxChildW();
    int parentOffset = calcPivotOffset(pivotPos, this->parentElement->hitbox->w, this->pivot.first, maxW) + this->parentElement->hitbox->minX;
    int childOffset = 0;
    if (this->parentElement->distibutionWrapping)
    {
        childOffset = this->parentElement->calcChildWrapping(this->childIndex).otherDirectionOffset;
    }
    else
    {
        childOffset = this->parentElement->getChildWSum(this->childIndex - 1);
    }
    int result = calcRelPos(
        p,
        parentOffset,
        maxW,
        0,
        1,
        this->calcW(),
        childOffset,
        this->pivot.first);
    return result;
}

bool UIElement::checkCollision(int x, int y)
{
    if (!this->visible)
    {
        return false;
    }
    int X1 = this->_lastDest.x;
    int X2 = this->_lastDest.x + this->_lastDest.w;
    int Y1 = this->_lastDest.y;
    int Y2 = this->_lastDest.y + this->_lastDest.h;
    if (this->hasCropRect())
    {
        Setter::setInMin(X1, this->parentElement->_cropRect->minX);
        Setter::setInMax(X2, this->parentElement->_cropRect->maxX);
        Setter::setInMin(Y1, this->parentElement->_cropRect->minY);
        Setter::setInMax(Y2, this->parentElement->_cropRect->maxY);
    }
    return x >= X1 &&
           x < X2 &&
           y >= Y1 &&
           y < Y2;
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

void UIElement::setVisible(bool visible)
{
    this->visible = visible;
    for (const auto &child : this->childElements.vec)
    {
        child->setVisible(visible);
    }
}

CommonObjects *UIElement::getCommonObjects()
{
    return this->comm;
}

UIElement::~UIElement()
{
    this->freeTexture();
    this->freeSurface();
}

void UIElement::addChildren(const std::shared_ptr<UIElement> &parentElement, const std::vector<std::shared_ptr<UIElement>> &childElements)
{
    for (auto &childElement : childElements)
    {
        childElement->childId = parentElement->childElements.add(childElement);
        childElement->parentElement = parentElement;
    }
    parentElement->updateChildVec();
    parentElement->setDefaultRenderOrder();
}

void UIElement::updateChildVec()
{
    this->childElements.updateVec();
    for (size_t i = 0; i < this->childElements.vec.size(); i++)
    {
        this->childElements.vec.at(i)->childIndex = i;
    }
}

void UIElement::removeChild(size_t id)
{
    this->childElements.erase(id);
    this->updateChildVec();
}

void UIElement::setDefaultRenderOrder(int order, bool fromMainParent)
{
    if (fromMainParent)
    {
        this->defaultRenderOrder = order;
        for (auto &child : this->childElements.vec)
        {
            child->setDefaultRenderOrder(order + 1, true);
        }
        return;
    }
    if (this->parentElement == nullptr)
    {
        this->setDefaultRenderOrder(0, true);
        return;
    }
    this->parentElement->setDefaultRenderOrder();
}

std::vector<size_t> UIElement::getFamilyIndicies()
{
    std::vector<size_t> result = {this->id};
    for (const auto &child : this->childElements.vec)
    {
        result = Vect::concat<size_t>(result, child->getFamilyIndicies());
    }
    return result;
}

void UIElement::draw()
{
    if (!this->visible)
    {
        return;
    }
    SDL_Rect dimentions;
    dimentions.x = this->calcX();
    dimentions.y = this->calcY();
    dimentions.w = this->calcW();
    dimentions.h = this->calcH();
    this->_lastDest = dimentions;
    if (this->_texture != nullptr)
    {
        SDL_QueryTexture(this->_texture, NULL, NULL, &this->_lastCrop.w, &this->_lastCrop.h);
    }
    else
    {
        this->_lastCrop.w = dimentions.w;
        this->_lastCrop.h = dimentions.h;
    }
    this->_lastCrop.x = 0;
    this->_lastCrop.y = 0;
    if (this->hasCropRect())
    {
        double tWCoef = (double)this->_lastCrop.w / this->_lastDest.w;
        double tHCoef = (double)this->_lastCrop.h / this->_lastDest.h;

        int offsetH = this->_lastDest.x - this->parentElement->_cropRect->minX;
        int scrollOffsetH = Setter::getInMin(this->parentElement->_cropRect->scrollH - offsetH, 0);
        this->_lastDest.x -= Setter::getInMax(this->parentElement->_cropRect->scrollH, offsetH);
        this->_lastCrop.x = scrollOffsetH * tWCoef;

        int offsetV = this->_lastDest.y - this->parentElement->_cropRect->minY;
        int scrollOffsetV = Setter::getInMin(this->parentElement->_cropRect->scrollV - offsetV, 0);
        this->_lastDest.y -= Setter::getInMax(this->parentElement->_cropRect->scrollV, offsetV);
        this->_lastCrop.y = scrollOffsetV * tHCoef;

        this->_lastDest.w = Setter::getInMax(this->_lastDest.x + this->_lastDest.w,
                                             this->parentElement->_cropRect->maxX) -
                            this->_lastDest.x - scrollOffsetH;
        this->_lastDest.h = Setter::getInMax(this->_lastDest.y + this->_lastDest.h,
                                             this->parentElement->_cropRect->maxY) -
                            this->_lastDest.y - scrollOffsetV;
        this->_lastCrop.w = this->_lastDest.w * tWCoef;
        this->_lastCrop.h = this->_lastDest.h * tHCoef;
    }
    int winW, winH;
    SDL_GetWindowSize(this->comm->window, &winW, &winH);
    this->hitbox->minX = Setter::getInMin(dimentions.x, 0);
    this->hitbox->minY = Setter::getInMin(dimentions.y, 0);
    this->hitbox->maxX = Setter::getInMax(this->hitbox->minX + dimentions.w, winW);
    this->hitbox->maxY = Setter::getInMax(this->hitbox->minY + dimentions.h, winH);
    this->hitbox->w = dimentions.w;
    this->hitbox->h = dimentions.h;
    this->hitbox->scrollH = this->_scrollH;
    this->hitbox->scrollV = this->_scrollV;
    this->hitbox = hitbox;
    for (auto &childElement : this->childElements.vec)
    {
        if ((this->hasCropRect()) || this->overflow != OverflowMode::Visible)
        {
            if (this->overflow != OverflowMode::Visible)
            {
                std::shared_ptr<Hitbox> childCrop = std::make_shared<Hitbox>(*hitbox);
                if (this->hasCropRect())
                {
                    Setter::setInRange(childCrop->minX, this->parentElement->_cropRect->minX, this->parentElement->_cropRect->maxX);
                    Setter::setInRange(childCrop->maxX, this->parentElement->_cropRect->minX, this->parentElement->_cropRect->maxX);
                    Setter::setInRange(childCrop->minY, this->parentElement->_cropRect->minY, this->parentElement->_cropRect->maxY);
                    Setter::setInRange(childCrop->maxY, this->parentElement->_cropRect->minY, this->parentElement->_cropRect->maxY);
                }
                this->_cropRect = childCrop;
            }
            else
            {
                this->_cropRect = this->parentElement->_cropRect;
            }
        }
        childElement->draw();
    }
}
void UIElement::render(SDL_Point *rotationPoint, double angle, SDL_RendererFlip flip)
{
    if (!this->visible)
    {
        return;
    }
    if (this->_texture != NULL)
    {
        SDL_RenderCopyEx(this->comm->renderer, this->_texture, &this->_lastCrop, &this->_lastDest, angle, rotationPoint, flip);
    }
    if (this->showHitbox)
    {
        SDL_SetRenderDrawColor(this->comm->renderer, 226, 255, 18, 100);
        SDL_RenderDrawLine(this->comm->renderer,
                           this->_lastDest.x,
                           this->_lastDest.y,
                           this->_lastDest.x + this->_lastDest.w,
                           this->_lastDest.y);
        SDL_RenderDrawLine(this->comm->renderer,
                           this->_lastDest.x,
                           this->_lastDest.y,
                           this->_lastDest.x,
                           this->_lastDest.y + this->_lastDest.h);
        SDL_RenderDrawLine(this->comm->renderer,
                           this->_lastDest.x + this->_lastDest.w,
                           this->_lastDest.y,
                           this->_lastDest.x + this->_lastDest.w,
                           this->_lastDest.y + this->_lastDest.h);
        SDL_RenderDrawLine(this->comm->renderer,
                           this->_lastDest.x,
                           this->_lastDest.y + this->_lastDest.h,
                           this->_lastDest.x + this->_lastDest.w,
                           this->_lastDest.y + this->_lastDest.h);
    }
}