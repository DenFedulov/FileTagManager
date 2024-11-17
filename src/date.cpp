#include "date.h"

std::string getCurrentDate(std::string format)
{
    time_t t = std::time(nullptr);
    tm tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, format.c_str());
    return oss.str();
}