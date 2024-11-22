#pragma once
#include <vector>
#include <initializer_list>

namespace Vect
{
    template <typename T, typename... Vectors>
    std::vector<T> concat(const Vectors &...vectors)
    {
        size_t total_size = (vectors.size() + ...);

        std::vector<T> result;
        result.reserve(total_size);

        ([&result](const auto &vec)
         { result.insert(result.end(), vec.begin(), vec.end()); }(vectors), ...);

        return result;
    }
}