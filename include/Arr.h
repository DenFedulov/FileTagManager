#pragma once
#include <vector>
#include <cstdint>
#include <memory>
#include <functional>

int calc2DArrayIndex(int col, int row, int colSize, int rowSize);
void iterateWithOffset(int start, int end, std::function<bool(int)> func);

template <typename T>
struct Array2D
{
private:
    void copy(const Array2D &other)
    {
        if (this == &other)
        {
            return;
        }
        this->cols = other.cols;
        this->rows = other.rows;
        this->data = std::make_unique<T[]>(this->getSize());
        for (int i = 0; i < this->getSize(); i++)
        {
            this->data[i] = other.data[i];
        }
    }

public:
    std::unique_ptr<T[]> data;
    int cols = -1;
    int rows = -1;

    Array2D(int cols, int rows) : cols(cols), rows(rows)
    {
        this->data = std::make_unique<T[]>(this->getSize());
    }
    Array2D() {}

    // Array2D(const Array2D &other)
    // {
    //     this->copy(other);
    // }
    Array2D &operator=(const Array2D &other)
    {
        this->copy(other);
        return *this;
    }

    int getSize()
    {
        return this->rows * this->cols;
    }

    bool contains(int x, int y)
    {
        return calc2DArrayIndex(x, y, this->cols, this->rows) > 0;
    }

    T get(int x, int y)
    {
        return this->data[calc2DArrayIndex(x, y, this->cols, this->rows)];
    }

    void set(int x, int y, T val)
    {
        this->data[calc2DArrayIndex(x, y, this->cols, this->rows)] = val;
    }

    void fillFromArray(T *arr)
    {
        for (int i = 0; i < this->getSize(); i++)
        {
            this->data[i] = arr[i];
        }
    }
};
