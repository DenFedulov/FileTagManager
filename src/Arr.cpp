#include "Arr.h"

void iterateWithOffset(int start, int end, std::function<bool(int)> func)
{
    int c = 0;
    for (int i = start; i <= end; i++)
    {
        if (i >= end)
        {
            i = 0;
        }
        if (func(i))
        {
            break;
        }
        c++;
        if (c >= end)
        {
            break;
        }
    }
}

int calc2DArrayIndex(int col, int row, int colSize, int rowSize)
{
    if (col < 0 || row < 0 || col >= colSize || row >= rowSize)
    {
        return -1;
    }
    return row * colSize + col;
}
