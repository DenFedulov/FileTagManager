#pragma once

enum class RelPos
{
    None,
    Start,
    Center,
    End,
    Between,
    Around,
    Even,
    Fixed,
};

enum class DistDirection
{
    column,
    row
};

enum class DisplayMode
{
    Normal,
    Distribute,
};

int calcPivotOffset(RelPos p, int size, int defaultVal, int childSize = 0);
int calcRelPos(RelPos p, int parentOffset, int parentSize, int childIndex, int childCount, int childrenSize, int childOffset, int defaultVal);