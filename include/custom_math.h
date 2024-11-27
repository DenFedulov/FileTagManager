#pragma once
#include <cmath>

template <typename T>
typename std::enable_if<std::is_same<T, int>::value || std::is_same<T, double>::value, T>::type
closestTo0(T first, T second)
{
    T absFirst = std::abs(first);
    T absSecond = std::abs(second);
    T result = ((std::min)(absFirst, absSecond) == absFirst) ? first : second;
    return result;
}

template <typename T>
typename std::enable_if<std::is_same<T, int>::value || std::is_same<T, double>::value, bool>::type
isInRange(const T &value, const T &from, const T &to)
{
    return !(value < from || value > to);
}