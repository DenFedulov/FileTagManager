#pragma once
#include <vector>
#include <iterator>
#include <algorithm>
#include <initializer_list>
#include <unordered_map>

namespace Vect
{
    template <typename T, typename Y>
    std::vector<T> vecFromUnmap(const std::unordered_map<Y, T> map)
    {
        std::vector<T> result;
        result.reserve(map.size());
        for (const auto &[key, val] : map)
        {
            result.push_back(val);
        }
        return result;
    }

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

    template <typename T>
    std::vector<T> eraseIndexes(const std::vector<T> &data, std::vector<size_t> &indexesToDelete)
    {
        if (indexesToDelete.empty())
            return data;

        std::vector<T> result;
        result.reserve(data.size() - indexesToDelete.size());
        std::sort(indexesToDelete.begin(), indexesToDelete.end());

        auto blockBegin = data.begin();
        for (const auto &index : indexesToDelete)
        {
            auto blockEnd = data.begin() + index;
            if (blockBegin != blockEnd)
            {
                std::copy(blockBegin, blockEnd, std::back_inserter(result));
            }
            blockBegin = blockEnd + 1;
        }
        if (blockBegin != data.end())
        {
            std::copy(blockBegin, data.end(), std::back_inserter(result));
        }

        return result;
    }
}