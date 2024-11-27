#include "type_helpers.h"

uint16string wStrToUInt16(const std::wstring &wStr)
{
    uint16string result(wStr.begin(), wStr.end());
    return result;
}

std::wstring strToWStr(const std::string &str)
{
    std::wstring result(str.begin(), str.end());
    return result;
}