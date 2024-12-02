#include "BaseElements/UIElement.h"

UIElement::UIElement(std::string name, CommonObjects *comm) : name(name), _comm(comm)
{
    auto editEnable = [](std::shared_ptr<UIElement> &el, const SDL_Event &e)
    {
        if (el->overflow == OverflowMode::Scroll && el->checkCollision(e.wheel.mouseX, e.wheel.mouseY))
        {
            std::cout << el->name << '\n';
            std::cout << e.wheel.mouseX << '\n';
            std::cout << e.wheel.mouseY << '\n';
            std::cout << e.wheel.y << '\n';
            int scrollAmount = -e.wheel.y * 10; // TODO: maybe make it configurable/take from windows settings
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
            return (int)EventResult::StopPropagation;
        }
        return 0;
    };
    this->events.addHandler(SDL_MOUSEWHEEL, editEnable);
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
    Direction scrollDirection)
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
                     pivotDefault) -
                 (scrollDirection == Direction::Right ? this->parentElement->_scrollH : this->parentElement->_scrollV);
    }
    else
    {
        result = baseCoord - calcPivotOffset(pivotPos, pivotDim, pivotDefault);
    }
    return result;
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
               Direction::Right) +
           this->margin[Direction::Left];
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
               Direction::Down) +
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
    Setter::setInRange(scrollH,
                       0,
                       this->calcChildRealW() -
                           this->_cropRect->maxX +
                           this->margin[Direction::Left] +
                           this->margin[Direction::Right]);
    this->_scrollH = scrollH;
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
    Setter::setInRange(scrollV,
                       0,
                       this->calcChildRealH() - this->_cropRect->maxY +
                           this->margin[Direction::Up] +
                           this->margin[Direction::Down]);
    this->_scrollV = scrollV;
}

void UIElement::moveScrollV(int step)
{
    this->setScrollV(this->_scrollV + step);
}

int UIElement::calcW()
{
    int result = this->_w;
    if (this->anchors[Direction::Right] && this->parentElement != NULL)
    {
        result = this->parentElement->calcW();
    }
    return result + this->margin[Direction::Left] + this->margin[Direction::Right];
}

int UIElement::calcH()
{
    int result = this->_h;
    if (this->anchors[Direction::Down] && this->parentElement != NULL)
    {
        result = this->parentElement->calcH();
    }
    return result + this->margin[Direction::Up] + this->margin[Direction::Down];
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
    for (const auto &child : this->childElements)
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
    int limit = upTo.value_or(this->childElements.size() - 1);
    int sum = 0;
    for (int i = 0; i <= limit; i++)
    {
        sum += this->childElements[i]->calcW();
    }
    return sum;
}

int UIElement::getChildHSum(std::optional<int> upTo)
{
    int limit = upTo.value_or(this->childElements.size() - 1);
    int sum = 0;
    for (int i = 0; i <= limit; i++)
    {
        sum += this->childElements[i]->calcH();
    }
    return sum;
}

int UIElement::calcChildRealW()
{
    int min = INT_MAX;
    int max = INT_MIN;
    for (const auto &child : this->childElements)
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
    for (const auto &child : this->childElements)
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

int UIElement::getMaxChildH()
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

intPair UIElement::calcChildWrapping(int childInd)
{
    const int directionSize = this->distDirection == DistDirection::column ? this->calcW() : this->calcH();
    int directionOrder = 0;
    int offset = 0;
    for (size_t i = 0; i < this->childElements.size(); i++)
    {
        auto child = this->childElements.at(i);
        int childSize = this->distDirection == DistDirection::column ? child->calcW() : child->calcH();
        int prevChildSize = 0;
        if (i > 0)
        {
            auto prevChild = this->childElements.at(i - 1);
            prevChildSize = this->distDirection == DistDirection::column ? prevChild->calcW() : prevChild->calcH();
        }
        offset += prevChildSize;
        if (i > 0 && directionSize - (offset + childSize) < 0)
        {
            directionOrder++;
            offset = 0;
        }
        if (child->childIndex == childInd)
        {
            break;
        }
    }

    return {directionOrder, offset};
}

int UIElement::calcDistPosH(RelPos p)
{
    int childCount = this->parentElement->childElements.size();
    int wSum = this->parentElement->getChildWSum();
    int wOffset = this->parentElement->calcChildWrapping(this->childIndex).second;
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
    int hOffset = this->parentElement->calcChildWrapping(this->childIndex).second;
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
    int maxH = this->parentElement->getMaxChildH();
    int parentOffset = calcPivotOffset(pivotPos, this->parentElement->calcH(), this->pivot.second, maxH) + this->parentElement->calcY();
    int childOffset = this->parentElement->calcChildWrapping(this->childIndex).first * maxH;

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
    int parentOffset = calcPivotOffset(pivotPos, this->parentElement->calcW(), this->pivot.first, maxW) + this->parentElement->calcX();
    int childOffset = this->parentElement->calcChildWrapping(this->childIndex).first * maxW;

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
    int elemX = this->calcX();
    int elemY = this->calcY();
    int X1 = elemX;
    int X2 = elemX + this->calcW();
    int Y1 = elemY;
    int Y2 = elemY + this->calcH();
    if (this->_cropRect != NULL)
    {
        Setter::setInMin(X1, this->_cropRect->minX);
        Setter::setInMax(X2, this->_cropRect->maxX);
        Setter::setInMin(Y1, this->_cropRect->minY);
        Setter::setInMax(Y2, this->_cropRect->maxY);
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

CommonObjects *UIElement::getCommonObjects()
{
    return this->_comm;
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
    this->parentElement = NULL;
}

std::shared_ptr<UIElement> UIElement::getChild(int id)
{
    if (id < this->childElements.size())
    {
        return this->childElements.at(id);
    }
    return nullptr;
}

void UIElement::draw()
{
    if (!this->visible)
    {
        return;
    }
    if (this->_texture != NULL)
    {
        this->_lastDest.x = this->calcX();
        this->_lastDest.y = this->calcY();
        this->_lastDest.w = this->calcW();
        this->_lastDest.h = this->calcH();
        int tW, tH;
        SDL_QueryTexture(this->_texture, NULL, NULL, &tW, &tH);
        this->_lastCrop.x = 0;
        this->_lastCrop.y = 0;
        this->_lastCrop.w = tW;
        this->_lastCrop.h = tH;
        if (this->_cropRect != NULL)
        {
            double tWCoef = (double)tW / this->_lastDest.w;
            double tHCoef = (double)tH / this->_lastDest.h;
            this->_lastDest.w = Setter::getInMax(this->_lastDest.x + this->_lastDest.w,
                                                 this->_cropRect->maxX) -
                                this->_lastDest.x;
            this->_lastDest.h = Setter::getInMax(this->_lastDest.y + this->_lastDest.h,
                                                 this->_cropRect->maxY) -
                                this->_lastDest.y;
            this->_lastCrop.w = this->_lastDest.w * tWCoef;
            this->_lastCrop.h = this->_lastDest.h * tHCoef;
        }
    }
    for (auto &childElement : this->childElements)
    {
        if (this->_cropRect != NULL || this->overflow != OverflowMode::Visible)
        {
            if (this->overflow != OverflowMode::Visible)
            {
                std::shared_ptr<LimitRect> childCrop = std::make_shared<LimitRect>();
                childCrop->minX = this->calcX();
                childCrop->minY = this->calcY();
                childCrop->maxX = childCrop->minX + this->calcW();
                childCrop->maxY = childCrop->minY + this->calcH();
                if (this->_cropRect != NULL)
                {
                    Setter::setInRange(childCrop->minX, this->_cropRect->minX, this->_cropRect->maxX);
                    Setter::setInRange(childCrop->maxX, this->_cropRect->minX, this->_cropRect->maxX);
                    Setter::setInRange(childCrop->minY, this->_cropRect->minY, this->_cropRect->maxY);
                    Setter::setInRange(childCrop->maxY, this->_cropRect->minY, this->_cropRect->maxY);
                }
                childElement->_cropRect = childCrop;
                this->_cropRect = childCrop;
            }
            else
            {
                childElement->_cropRect = this->_cropRect;
            }
        }
        childElement->draw();
    }
}
void UIElement::render(SDL_Point *rotationPoint, double angle, SDL_RendererFlip flip)
{
    if (!this->visible || this->_texture == NULL)
    {
        return;
    }
    SDL_RenderCopyEx(this->_comm->renderer, this->_texture, &this->_lastCrop, &this->_lastDest, angle, rotationPoint, flip);
}