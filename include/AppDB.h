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
    TableData getTags();
    bool addTagToFile(std::wstring tagName, std::wstring filePath);
};
