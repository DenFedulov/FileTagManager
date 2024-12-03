#include "AppDB.h"

AppDB::AppDB(std::string dbName)
{
    bool createDbStruct = !std::filesystem::exists(dbName);
    this->_db = std::make_unique<SQLiteClass>(dbName.c_str());
    if (createDbStruct)
    {
        this->_db->exec(DB_STRUCTURE_SQL_SCRIPT);
    }
}