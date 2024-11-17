#pragma once
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>

bool fileWrite(std::string filename, std::string data, bool appendData = true);
std::string fileRead(std::string filename);
std::vector<std::string> scanDir(std::string path);
