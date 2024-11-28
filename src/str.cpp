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
    return distance;
}
