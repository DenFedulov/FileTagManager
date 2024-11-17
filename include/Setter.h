#pragma once
#include <utility>
#include <type_traits>

namespace Setter
{
    template <typename T>
    void swap(T &first, T &second, bool condition = true)
    {
        if (condition)
        {
            std::swap(first, second);
        }
    }

    template <typename T>
    void setIf(T &variable, T value, bool condition = true)
    {
        if (condition)
        {
            variable = value;
        }
    }

    template <typename T>
    typename std::enable_if<std::is_same<T, int>::value || std::is_same<T, double>::value, T>::type
    getInRange(T val, T min, T max)
    {
        if (val < min)
        {
            val = min;
        }
        if (val >= max)
        {
            val = max;
        }
        return val;
    }

    template <typename T>
    typename std::enable_if<std::is_same<T, int>::value || std::is_same<T, double>::value, T>::type
    getInMin(T val, T min)
    {
        if (val < min)
        {
            val = min;
        }
        return val;
    }

    template <typename T>
    typename std::enable_if<std::is_same<T, int>::value || std::is_same<T, double>::value, T>::type
    getInMax(T val, T max)
    {
        if (val >= max)
        {
            val = max;
        }
        return val;
    }

    template <typename T>
    typename std::enable_if<std::is_same<T, int>::value || std::is_same<T, double>::value, void>::type
    setInRange(T &val, T min, T max)
    {
        val = getInRange(val, min, max);
    }
}