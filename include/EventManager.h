#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>

template <typename EventResultType, typename... Args>
class EventManager
{
    typedef std::unordered_map<int, std::function<EventResultType(Args...)>> CallbackList;
    typedef std::unordered_map<int, CallbackList> EventHandlers;

private:
    EventHandlers _eventHandlers;
    CallbackList &getCallbackList(const int &eventTypeEnum)
    {
        if (!this->_eventHandlers.contains(eventTypeEnum))
        {
            this->_eventHandlers.emplace(eventTypeEnum, CallbackList());
        }
        return this->_eventHandlers.at(eventTypeEnum);
    }

public:
    int addHandler(const int &eventTypeEnum, std::function<EventResultType(Args...)> func)
    {
        CallbackList &callbacks = this->getCallbackList(eventTypeEnum);
        int index = callbacks.size();
        callbacks.emplace(index, func);
        return index;
    }
    bool deleteHandler(const int &eventTypeEnum, int i)
    {
        CallbackList *callbacks = this->getCallbackList(eventTypeEnum);
        if (callbacks->contains(i))
        {
            callbacks->erase(i);
            return true;
        }
        return false;
    }
    void clearHandlers()
    {
        this->_eventHandlers.clear();
    }
    std::vector<EventResultType> triggerEvent(const int &eventTypeEnum, Args... args)
    {
        std::vector<EventResultType> results;
        CallbackList &callbacks = this->getCallbackList(eventTypeEnum);
        for (const auto &[i, func] : callbacks)
        {
            results.push_back(func(std::forward<Args>(args)...));
        }
        return results;
    }
};