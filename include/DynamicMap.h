#pragma once
#include "vect.h"
#include <deque>
#include <unordered_map>

template <typename T>
class DynamicMap
{
private:
    size_t _nextId = 0;
    std::deque<size_t> _avalableIds;

public:
    std::unordered_map<size_t, T> map;
    std::vector<T> vec;

    size_t add(T value)
    {
        size_t addedId;
        if (!this->_avalableIds.empty())
        {
            addedId = this->_avalableIds.front();
            this->map.emplace(addedId, value);
            this->_avalableIds.pop_front();
            return addedId;
        }
        addedId = this->_nextId;
        this->map.emplace(addedId, value);
        this->_nextId++;
        return addedId;
    }

    bool erase(size_t id)
    {
        if (!this->map.contains(id))
        {
            return false;
        }
        this->map.erase(id);
        this->_avalableIds.push_back(id);
        return true;
    }

    std::vector<T> updateVec()
    {
        this->vec = Vect::vecFromUnmap(this->map);
        return this->vec;
    }
};
