#pragma once
#include <iostream>
#include <memory>
#include "Arr.h"

template <typename T>
void print2DArray(Array2D<T> &arr)
{
    std::cout << "     ";
    for (int i = 0; i < arr.cols; i++)
    {
        std::cout << i << ((i > 9) ? " " : "  ");
    }
    std::cout << '\n';
    for (int j = 0; j < arr.rows; j++)
    {
        std::cout << j << ((j > 9) ? " | " : "  | ");
        for (int i = 0; i < arr.cols; i++)
        {
            std::cout << (arr.get(i, j) ? "o" : "-") << "  ";
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}