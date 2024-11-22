#include "geometry.h"

CoordsArray::CoordsArray(const CoordsVector &coords) : Array2D()
{
    intPair dims = Geometry::getDimentions(coords);
    this->cols = dims.first;
    this->rows = dims.second;
    this->data = std::make_unique<bool[]>(this->cols * this->rows);
    Geometry::coordsVectorFillArray(coords, *this);
}

CoordsArray::CoordsArray(int cols, int rows) : Array2D(cols, rows)
{
}

CoordsArray::CoordsArray()
{
}

CoordsVector Geometry::mirrorCoords(CoordsVector coords, MirrorDirection direction)
{
    for (auto &pair : coords)
    {
        switch (direction)
        {
        case Vertical:
            pair.second = -pair.second;
            break;
        case Horizontal:
            pair.first = -pair.first;
            break;
        case Diagonal:
            int t = pair.first;
            pair.first = pair.second;
            pair.second = t;
            break;
        }
    }
    return coords;
}

CoordsVector Geometry::addToCoords(CoordsVector coords, int x, int y)
{
    for (auto &elem : coords)
    {
        elem.first += x;
        elem.second += y;
    }
    return coords;
}

CoordsVector Geometry::mirrorAndAddCoords(CoordsVector coords, int x, int y, MirrorDirection direction)
{
    for (auto &pair : coords)
    {
        switch (direction)
        {
        case Vertical:
            pair.second = -pair.second;
            pair.second += y;
            break;
        case Horizontal:
            pair.first = -pair.first;
            pair.first += x;
            break;
        case Diagonal:
            int t = pair.first;
            pair.first = pair.second;
            pair.second = t;
            pair.first += x;
            pair.second += y;
            break;
        }
    }
    return coords;
}

CoordsVector Geometry::circleOctant(int r)
{
    CoordsVector coords;
    int x = 0;
    double y = r;
    int r2 = pow(r, 2);

    while (x < y)
    {
        double f = pow(x, 2) + pow(y - 0.5, 2);
        if (f > r2)
        {
            y--;
        }
        coords.push_back(std::pair<int, int>(x, y));
        x++;
    }

    return coords;
}

CoordsVector Geometry::circleQuater(int r)
{
    CoordsVector octate = Geometry::circleOctant(r);
    return Vect::concat<intPair>(octate, Geometry::mirrorCoords(octate, Diagonal));
}

CoordsVector Geometry::circle(int r)
{
    CoordsVector quater = Geometry::circleQuater(r);
    CoordsVector half = Vect::concat<intPair>(quater, Geometry::mirrorCoords(quater, Vertical));
    return Vect::concat<intPair>(half, Geometry::mirrorCoords(half, Horizontal));
}

CoordsVector Geometry::line(intPair start, intPair end)
{
    Setter::swap(start, end, start.first > end.first);

    double xDiff = start.first - end.first;
    double xOffset = xDiff != 0 ? xDiff + (std::abs(xDiff) / xDiff) : 0;

    double yDiff = start.second - end.second;
    double yOffset = yDiff != 0 ? yDiff + (std::abs(yDiff) / yDiff) : 0;

    double xLen = std::abs(xOffset);
    double yLen = std::abs(yOffset);
    double xStep = yOffset != 0 ? (xLen / yLen) : 1;
    double yStep = xOffset != 0 ? (yLen / xLen) : 1;
    xStep *= xDiff != 0 ? -(std::abs(xDiff) / xDiff) : 0;
    yStep *= yDiff != 0 ? -(std::abs(yDiff) / yDiff) : 0;
    xStep = Setter::getInRange(xStep, -1.0, 1.0);
    yStep = Setter::getInRange(yStep, -1.0, 1.0);
    int maxLen = std::max(xLen, yLen);
    double step = closestTo0(xStep, yStep);
    if (step == 0)
    {
        step = 1;
    }
    std::set<intPair> line;
    line.insert(end);
    for (double i = 0; i < maxLen; i += std::abs(step))
    {
        double x = start.first + (i * xStep) + 0.00001;
        double y = start.second + (i * yStep) + 0.00001;
        line.insert({x, y});
    }

    return CoordsVector(line.begin(), line.end());
}

int Geometry::findMaxX(const CoordsVector &coords)
{
    int max = 0;
    for (auto &coord : coords)
    {
        if (coord.first > max)
        {
            max = coord.first;
        }
    }
    return max;
}

int Geometry::findMaxY(const CoordsVector &coords)
{
    int max = 0;
    for (auto &coord : coords)
    {
        if (coord.second > max)
        {
            max = coord.second;
        }
    }
    return max;
}

CoordsVector Geometry::arrayToCoordsVector(Array2D<bool> &arr)
{
    CoordsVector vec;
    for (int j = 0; j < arr.rows; j++)
    {
        for (int i = 0; i < arr.cols; i++)
        {
            if (arr.get(i, j))
            {
                vec.push_back({i, j});
            }
        }
    }
    return vec;
}

intPair Geometry::getDimentions(const CoordsVector &coords)
{
    return {Geometry::findMaxX(coords) + 1, Geometry::findMaxY(coords) + 1};
}

void Geometry::coordsVectorFillArray(const CoordsVector &vec, Array2D<bool> &arr)
{
    for (auto &coord : vec)
    {
        arr.set(coord.first, coord.second, true);
    }
}

intPair Geometry::BorderMaker::getPixelInDirection(int x, int y, int &direction)
{
    switch (this->calcRealCurrentDirection(direction))
    {
    case Direction::Down:
        y++;
        break;
    case Direction::Up:
        y--;
        break;
    case Direction::Left:
        x--;
        break;
    case Direction::Right:
        x++;
        break;
    }
    return {x, y};
}

intPair Geometry::BorderMaker::getPixelFromInTurn(int x, int y, int &direction)
{
    switch (this->calcRealCurrentDirection(direction, this->turnDirection))
    {
    case Direction::Down:
        x--;
        y++;
        break;
    case Direction::Up:
        x++;
        y--;
        break;
    case Direction::Left:
        x--;
        y--;
        break;
    case Direction::Right:
        x++;
        y++;
        break;
    }
    return {x, y};
}

intPair Geometry::BorderMaker::getPixelFromOutTurn(int x, int y, int &currentDirection)
{
    switch (this->calcRealCurrentDirection(currentDirection, this->turnDirection))
    {
    case Direction::Down:
        x++;
        break;
    case Direction::Up:
        x--;
        break;
    case Direction::Left:
        y++;
        break;
    case Direction::Right:
        y--;
        break;
    }
    return {x, y};
}

intPair Geometry::BorderMaker::getFirstPixel()
{
    for (int j = 0; j < this->grid.rows; j++)
    {
        for (int i = 0; i < this->grid.cols; i++)
        {
            if (this->grid.get(i, j))
            {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

intPair Geometry::BorderMaker::pickNextBorderPixel(intPair currentPixel, int &currentDirection)
{
    int subXstart = currentPixel.first - 2;
    int subYstart = currentPixel.second - 2;
    int subSize = 5;
    Array2D<bool> subArray(subSize, subSize);
    getSubArray<bool>(this->grid, subArray, subXstart, subYstart);
    // print2DArray(subArray);

    for (int j = 1; j < subSize - 1; j++)
    {
        for (int i = 1; i < subSize - 1; i++)
        {
            int x = subXstart + i;
            int y = subYstart + j;
            if (!subArray.get(i, j) || pickedCoords.contains({x, y}))
            {
                continue;
            }
            intPair directionPixel = this->getPixelInDirection(i, j, currentDirection);
            intPair inTurnPixel = this->getPixelFromInTurn(i, j, currentDirection);
            intPair outTurnPixel = this->getPixelFromOutTurn(i, j, currentDirection);
            if (!subArray.get(directionPixel.first, directionPixel.second))
            {
                return {x, y};
            }
            else if (inTurnPixel.first == 2 && inTurnPixel.second == 2)
            {
                this->lastTurn = -1;
                currentDirection += this->turnDirection;
                return {x, y};
            }
            else if (outTurnPixel.first == 2 && outTurnPixel.second == 2)
            {
                this->lastTurn = 1;
                currentDirection += this->turnDirection;
                return {x, y};
            }
        }
    }

    return {-1, -1};
}

Geometry::BorderMaker::BorderMaker(const CoordsVector &contour, int width, BorderType borderType) : width(width), borderType(borderType)
{
    this->grid = CoordsArray(contour);
}

Geometry::BorderMaker::BorderMaker(CoordsArray &grid, int width, BorderType borderType) : width(width), borderType(borderType)
{
    this->grid = grid;
}

int Geometry::BorderMaker::calcRealCurrentDirection(int currentDirection, int turn)
{
    return (4 + ((currentDirection + turn) % 4)) % 4;
}

CoordsVector Geometry::BorderMaker::_make(int currentDirection)
{
    this->lastTurn = 0;
    this->pickedCoords.clear();
    intPair nextPixel = this->getFirstPixel();
    std::set<intPair> newCoords;
    if (this->width > 0)
    {
        CoordsVector contour = arrayToCoordsVector(this->grid);
        for (const auto &coord : contour)
        {
            newCoords.insert(coord);
        }
    }
    newCoords;
    for (int i = 0; i < this->grid.rows * this->grid.cols; i++)
    {
        intPair prevPixel = nextPixel;
        int prevDirection = currentDirection;
        nextPixel = this->pickNextBorderPixel(nextPixel, currentDirection);
        if (nextPixel.first < 0)
        {
            break;
        }
        pickedCoords.emplace(nextPixel);
        for (int directionOffset = 1; directionOffset < width; directionOffset++)
        {
            int resultCoord;
            switch (this->calcRealCurrentDirection(currentDirection))
            {
            case Direction::Up:
                resultCoord = nextPixel.second - directionOffset;
                newCoords.insert({nextPixel.first, resultCoord});
                break;
            case Direction::Down:
                resultCoord = nextPixel.second + directionOffset;
                newCoords.insert({nextPixel.first, resultCoord});
                break;
            case Direction::Left:
                resultCoord = nextPixel.first - directionOffset;
                newCoords.insert({resultCoord, nextPixel.second});
                break;
            case Direction::Right:
                resultCoord = nextPixel.first + directionOffset;
                newCoords.insert({resultCoord, nextPixel.second});
                break;
            }

            if (prevDirection == currentDirection || this->lastTurn != 1)
            {
                continue;
            }
            for (int fillOffset = 0; fillOffset <= width; fillOffset++)
            {
                switch (this->calcRealCurrentDirection(currentDirection))
                {
                case Direction::Up:
                    resultCoord = prevPixel.second - directionOffset;
                    newCoords.insert({prevPixel.first + fillOffset, resultCoord});
                    break;
                case Direction::Down:
                    resultCoord = prevPixel.second + directionOffset;
                    newCoords.insert({prevPixel.first - fillOffset, resultCoord});
                    break;
                case Direction::Left:
                    resultCoord = prevPixel.first - directionOffset;
                    newCoords.insert({resultCoord, prevPixel.second - fillOffset});
                    break;
                case Direction::Right:
                    resultCoord = prevPixel.first + directionOffset;
                    newCoords.insert({resultCoord, prevPixel.second + fillOffset});
                    break;
                }
            }
        }
    }

    return CoordsVector(newCoords.begin(), newCoords.end());
}

CoordsVector Geometry::BorderMaker::make()
{
    switch (this->borderType)
    {
    case BorderType::Inner:
        return this->_make();
    case BorderType::Outer:
        return this->_make(Direction::Left);
    case BorderType::Both:
        CoordsVector vec1 = this->_make();
        CoordsVector vec2 = this->_make(Direction::Left);
        vec1.insert(vec1.begin(), vec2.begin(), vec2.end());
        return vec1;
    }
    return CoordsVector();
}
