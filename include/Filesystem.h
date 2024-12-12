#pragma once
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <windows.h>
#include "type_helpers.h"

bool fileWrite(std::string filename, std::string data, bool appendData = true);
std::string fileRead(std::string filename);
std::vector<std::wstring> scanDir(std::wstring path);
