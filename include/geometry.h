#pragma once
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <memory>
#include <optional>
#include "custom_math.h"
#include "vect.h"
#include "custom_types.h"
#include "Setter.h"
#include "Arr.h"
#include "Debug.h"

struct CoordsArray : public Array2D<bool>
{
    CoordsArray(const CoordsVector &coords);
    CoordsArray(int cols, int rows);
    CoordsArray();
};

namespace Geometry
{
    enum MirrorDirection
    {
        Vertical,
        Horizontal,
        Diagonal
    };
    enum BorderType
    {
        Inner,
        Outer,
        Both
    };
    Direction getOppositeDirection(Direction d);
    CoordsVector arrayToCoordsVector(Array2D<bool> &arr);
    intPair getDimentions(const CoordsVector &coords);

    void coordsVectorFillArray(const CoordsVector &vec, Array2D<bool> &arr);

    CoordsVector mirrorCoords(CoordsVector coords, MirrorDirection direction);
    CoordsVector mirrorAndAddCoords(CoordsVector coords, int x, int y, MirrorDirection direction);
    CoordsVector addToCoords(CoordsVector coords, int x, int y);
    CoordsVector circleOctant(int r);
    CoordsVector circleQuater(int r);
    CoordsVector circle(int r);
    CoordsVector line(intPair start, intPair end);
    int findMaxX(const CoordsVector &coords);
    int findMaxY(const CoordsVector &coords);
    template <typename T>
    void getSubArray(Array2D<T> &array, Array2D<T> &subArray, int x, int y)
    {
        int c = 0;
        for (int j = Setter::getInMax(y, array.rows); j < y + subArray.rows; j++)
        {
            for (int i = Setter::getInMax(x, array.cols); i < x + subArray.cols; i++)
            {
                if (array.contains(i, j))
                {
                    subArray.data[c] = array.get(i, j);
                }
                c++;
            }
        }
    }

    class BorderMaker
    {
    private:
        std::set<intPair> pickedCoords;
        const int turnDirection = 1;
        int lastTurn;
        intPair getPixelInDirection(int x, int y, int &currentDirection);
        intPair getPixelFromInTurn(int x, int y, int &currentDirection);
        intPair getPixelFromOutTurn(int x, int y, int &currentDirection);
        int calcRealCurrentDirection(int currentDirection, int turn = 0);

        intPair getFirstPixel();
        intPair pickNextBorderPixel(intPair currentPixel, int &currentDirection);
        CoordsVector _make(int currentDirection = Direction::Down);

    public:
        CoordsArray grid;
        int width;
        BorderType borderType;
        BorderMaker(const CoordsVector &contour, int width, BorderType borderType = BorderType::Inner);
        BorderMaker(CoordsArray &grid, int width, BorderType borderType = BorderType::Inner);
        CoordsVector make();
    };
}
