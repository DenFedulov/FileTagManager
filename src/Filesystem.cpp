#include "Filesystem.h"

bool fileWrite(std::string filename, std::string data, bool appendData)
{
    std::fstream file;
    std::ios::openmode flag;
    flag = appendData ? (std::ios::app) : (std::ios::out | std::ios::trunc);
    file.open(filename, flag);
    file << data;
    file.close();
    return true;
}

std::string fileRead(std::string filename)
{
    std::string line;
    std::string output;
    std::fstream file;
    file.open(filename);
    while (std::getline(file, line))
    {
        output += line;
    }
    file.close();
    return output;
}

std::vector<std::string> scanDir(std::string path)
{
    std::vector<std::string> result;
    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        result.push_back(entry.path().string());
    }
    return result;
}
