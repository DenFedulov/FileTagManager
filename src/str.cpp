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
