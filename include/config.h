#pragma once
#include "Filesystem.h"
#include "json.hpp"

class Config
{
private:
    nlohmann::json data;
    template <typename T>
    T get(std::string propName)
    {
        return (T)this->data[propName];
    }

public:
    Config(std::string filename);
    std::string defaultFont = "resources/fonts/roboto/Roboto-Black.ttf";
};