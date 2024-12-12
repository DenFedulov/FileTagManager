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

std::vector<std::wstring> scanDir(std::wstring path)
{
    std::vector<std::wstring> result;
    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        std::wstring filepath = entry.path().wstring();
        auto attr = GetFileAttributesW(filepath.c_str());
        auto disallow = attr & FILE_ATTRIBUTE_SYSTEM;
        if (!disallow)
        {
            result.push_back(filepath);
        }
    }
    return result;
}
