#include "Layout.h"

int calcPivotOffset(RelPos p, int size, int defaultVal, int childSize)
{
    switch (p)
    {
    case RelPos::Start:
        return 0;
    case RelPos::Center:
        return size / 2 - (childSize / 2);
    case RelPos::End:
        return size - childSize;
    default:
        return defaultVal;
    }
}

int calcRelPos(RelPos p, int parentOffset, int parentSize, int childIndex, int childCount, int childrenSize, int childOffset, int defaultVal)
{
    switch (p)
    {
    case RelPos::Start:
        return parentOffset + childOffset;
    case RelPos::Center:
        return parentOffset + (parentSize / 2) - (childrenSize / 2) + childOffset;
    case RelPos::End:
        return parentOffset + parentSize - childrenSize + childOffset;
    case RelPos::Between:
    {
        if (childCount < 2)
        {
            return parentOffset + childOffset;
        }
        int spaceBetween = (parentSize - childrenSize) / (childCount - 1);
        return parentOffset + childIndex * spaceBetween + childOffset;
    }
    case RelPos::Around:
    {
        if (childCount < 2)
        {
            return parentOffset + (parentSize / 2) - (childrenSize / 2) + childOffset;
        }
        int spaceBetween = (parentSize - childrenSize) / (childCount * 2);
        return parentOffset + spaceBetween + childIndex * (spaceBetween * 2) + childOffset;
    }
    case RelPos::Even:
    {
        if (childCount < 2)
        {
            return parentOffset + (parentSize / 2) - (childrenSize / 2) + childOffset;
        }
        int spaceBetween = (parentSize - childrenSize) / (childCount + 1);
        return parentOffset + spaceBetween + childIndex * spaceBetween + childOffset;
    }
    default:
        return parentOffset + defaultVal;
    }
}
