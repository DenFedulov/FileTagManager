#include "Str.h"

bool Str::endsWith(std::string const &subject, std::string const &ending)
{
    if (subject.length() >= ending.length())
    {
        return (0 == subject.compare(subject.length() - ending.length(), ending.length(), ending));
    }
    else
    {
        return false;
    }
}

int Str::distanceToWSpace(const std::wstring &str, int step, int offset)
{
    int distance = 1;
    Setter::setInRange(offset, 0, (int)(str.length() - 1));
    for (size_t i = offset; i < str.length(); i += step)
    {
        if (std::iswspace(str.at(i)))
        {
            break;
        }
        distance++;
    }
    Setter::setInRange(distance, 1, (int)(str.length()));
    return distance;
}

std::string Str::getTailByChar(const std::string &str, const char *c, bool excludeChar)
{
    size_t foundIndex = str.find_last_of(c);
    if (foundIndex < 0)
    {
        return str;
    }
    return str.substr(foundIndex + excludeChar);
}

std::wstring Str::getTailByChar(const std::wstring &str, const wchar_t *c, bool excludeChar)
{
    size_t foundIndex = str.find_last_of(c);
    if (foundIndex < 0)
    {
        return str;
    }
    return str.substr(foundIndex + excludeChar);
}

std::wstring Str::cutTailByChar(const std::wstring &str, const wchar_t *c, bool excludeChar)
{
    size_t foundIndex = str.find_last_of(c);
    if (foundIndex < 0)
    {
        return str;
    }
    return str.substr(0, foundIndex + 1 - excludeChar);
}
