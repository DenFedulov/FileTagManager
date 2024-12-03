#pragma once
#include <string>
#include <filesystem>
#include "AppGlobals.h"
#include "SQLiteClass.h"
#include "db_structure.h"

class AppDB
{
private:
    std::unique_ptr<SQLiteClass> _db;

public:
    AppDB(std::string dbName);
};
