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

template <typename T>
T Str::getTailByChar(const T &str, const T &c, bool excludeChar, bool returnSource)
{
    size_t foundIndex = str.find_last_of(c);
    if (foundIndex == T::npos)
    {
        return returnSource ? str : T();
    }
    return str.substr(foundIndex + excludeChar);
}
template std::string Str::getTailByChar(const std::string &str, const std::string &c, bool excludeChar, bool returnSource);
template std::wstring Str::getTailByChar(const std::wstring &str, const std::wstring &c, bool excludeChar, bool returnSource);

std::wstring Str::cutTailByChar(const std::wstring &str, const wchar_t *c, bool excludeChar)
{
    size_t foundIndex = str.find_last_of(c);
    if (foundIndex == std::wstring::npos)
    {
        return str;
    }
    return str.substr(0, foundIndex + 1 - excludeChar);
}

template std::string Str::toLowerCase(std::string str);
template std::wstring Str::toLowerCase(std::wstring str);
template <typename T>
T Str::toLowerCase(T str)
{
    std::transform(str.begin(), str.end(), str.begin(),
                   [](uint8_t c)
                   { return std::tolower(c); });
    return str;
}

template std::vector<std::string> Str::explode(const std::string &str, const std::string &separator);
template std::vector<std::wstring> Str::explode(const std::wstring &str, const std::wstring &separator);
template <typename T>
std::vector<T> Str::explode(const T &str, const T &separator)
{
	std::vector<T> result;
	T remainder = str;
	size_t foundIndex = std::wstring::npos;
	do
	{
		foundIndex = remainder.find(separator);
		if (foundIndex != std::wstring::npos)
		{
			result.push_back(remainder.substr(0, foundIndex));
			remainder = (foundIndex + 1) < remainder.size() ? remainder.substr(foundIndex + 1) : T();
		}
		else
		{
			result.push_back(remainder);
		}
	} while (foundIndex != std::wstring::npos);

	return result;
}