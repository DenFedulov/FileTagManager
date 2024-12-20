#pragma once
#include <string>
#include <filesystem>
#include <locale>
#include "AppGlobals.h"
#include "SQLiteClass.h"
#include "db_structure.h"
#include "type_helpers.h"

class AppDB
{
private:
    std::unique_ptr<SQLiteClass> _db;

public:
    AppDB(std::string dbName);
    bool addTag(std::wstring tagName, RGBA color);
    bool addTag(std::wstring tagName, std::string color);
    bool deleteTag(std::wstring tagName);
    TableData getTags();
    TableData getFileTags(const std::vector<std::string> &tags, bool tagFilterMode);
    bool addTagToFile(std::wstring tagName, std::wstring filePath);
};
