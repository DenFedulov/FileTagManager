#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

namespace DATE_FORMATS
{
    const std::string fullFormat = "%Y-%m-%d %H:%M:%S%z";
    const std::string shortFormat = "%Y-%m-%d";
}

std::string getCurrentDate(std::string format = DATE_FORMATS::fullFormat);