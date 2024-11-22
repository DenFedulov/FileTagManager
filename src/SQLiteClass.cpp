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
        std::string message = "Error while executing query '" + std::string(sqlQuery) + "': " + std::string(this->_errorMassage);
        this->_errorHistory.push_back(message);
        sqlite3_free(this->_errorMassage);
    }
}

SQLiteClass::SQLiteClass(const char *filename)
{
    sqlite3_open(filename, &this->_db);
    this->exec("PRAGMA foreign_keys = ON;");
}

SQLiteClass::~SQLiteClass()
{
    sqlite3_close(this->_db);
}

bool SQLiteClass::exec(const char *sqlQuery)
{
    int result = sqlite3_exec(this->_db, sqlQuery, nullptr, nullptr, &this->_errorMassage);
    processExec(result, sqlQuery);
    return result == SQLITE_OK;
}

TableData SQLiteClass::query(const char *sqlQuery)
{
    TableData tableData;
    auto callback = [](void *param, int columnCount, char **data, char **colName) -> int
    {
        TableData *tableData = static_cast<TableData *>(param);
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
    processExec(sqlite3_exec(this->_db, sqlQuery, callback, &tableData, &this->_errorMassage), sqlQuery);
    return tableData;
}
