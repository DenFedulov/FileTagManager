#pragma once
#include <vector>
#include <initializer_list>

namespace Vect
{
    template <typename T>
    std::vector<T> concat(std::initializer_list<std::vector<T>> vectors)
    {
        std::vector<T> result;
        for (const auto &vec : vectors)
        {
            result.reserve(vec.size());
            result.insert(result.end(), vec.begin(), vec.end());
        }
        return result;
    }
}