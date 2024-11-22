#pragma once
#include "sqlite3.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

struct TableData
{
    std::unordered_map<std::string, int> columns;
    std::vector<std::vector<std::string>> data;
    std::string getCellValue(int column, int row);
    std::string getCellValue(std::string column, int row);
};

typedef std::shared_ptr<TableData> TableDataPtr;

class SQLiteClass
{
private:
    sqlite3 *db;
    char *error = nullptr;
    std::vector<std::string> errorHistory;
    void processExec(int result, const char *sqlQuery);

public:
    SQLiteClass(const char *filename);
    ~SQLiteClass();
    bool exec(const char *sqlQuery);
    TableDataPtr query(const char *sqlQuery);
    
};
