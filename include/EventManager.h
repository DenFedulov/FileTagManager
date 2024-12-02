#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>

enum class MouseButtons
{
    None,
    Left,
    Middle,
    Right,
    Back,
    Forward
};

enum class EventResult
{
    Nothing,
    Quit,
    StopPropagation,
};

enum class CustomEvent
{
    MOUSE_CLICK = -1
};

template <typename... Args>
class EventManager
{
    typedef std::unordered_map<int, std::function<int(Args...)>> CallbackList;
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
    int addHandler(const int &eventTypeEnum, std::function<int(Args...)> func)
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
    std::vector<int> triggerEvent(const int &eventTypeEnum, Args... args)
    {
        std::vector<int> results;
        CallbackList &callbacks = this->getCallbackList(eventTypeEnum);
        for (auto &[i, func] : callbacks)
        {
            results.push_back(func(std::forward<Args>(args)...));
        }
        return results;
    }
};