#include "SQLiteClass.h"

std::string TableData::getCellValue(int column, int row)
{
    return data.at(row).at(column);
}

std::string TableData::getCellValue(std::string column, int row)
{
    return this->getCellValue(this->columns.at(column), row);
}

void SQLiteClass::processExec(int result, const char *sqlQuery)
{
    if (result != SQLITE_OK)
    {
        std::string message = "Error while executing query '" + std::string(sqlQuery) + "': " + std::string(this->error);
        this->errorHistory.push_back(message);
        sqlite3_free(this->error);
    }
}

SQLiteClass::SQLiteClass(const char *filename)
{
    sqlite3_open(filename, &this->db);
    this->exec("PRAGMA foreign_keys = ON;");
}

SQLiteClass::~SQLiteClass()
{
    sqlite3_close(this->db);
}

bool SQLiteClass::exec(const char *sqlQuery)
{
    int result = sqlite3_exec(this->db, sqlQuery, nullptr, nullptr, &this->error);
    processExec(result, sqlQuery);
    return result == SQLITE_OK;
}

TableDataPtr SQLiteClass::query(const char *sqlQuery)
{
    TableDataPtr tableData = std::make_unique<TableData>();
    auto callback = [](void *param, int columnCount, char **data, char **colName) -> int
    {
        TableDataPtr tableData = *static_cast<TableDataPtr *>(param);
        std::vector<std::string> dataRow;
        if (tableData->columns.empty())
        {
            for (int i = 0; i < columnCount; i++)
            {
                tableData->columns.emplace(colName[i], i);
            }
        }
        for (int i = 0; i < columnCount; i++)
        {
            char *val = data[i];
            if (val == nullptr)
            {
                continue;
            }
            dataRow.insert(dataRow.begin() + i, val);
        }
        tableData->data.push_back(dataRow);
        return 0;
    };
    processExec(sqlite3_exec(this->db, sqlQuery, callback, &tableData, &this->error), sqlQuery);
    return tableData;
}
