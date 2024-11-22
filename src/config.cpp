#include "Config.h"

Config::Config(std::string filename)
{
    using json = nlohmann::json;
    if (!std::filesystem::exists(filename))
    {
        throw std::runtime_error("Config file " + filename + " doesn't exist");
    }
    std::string fileJsonData = fileRead(filename);
    if(fileJsonData.empty()){
        throw std::runtime_error("Config file " + filename + " is empty");
    }
    this->_data = json::parse(fileJsonData);
    this->defaultFont = this->get<std::string>("defaultFont");
}
