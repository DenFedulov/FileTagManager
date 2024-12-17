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

bool AppDB::addTag(std::wstring tagName, RGBA color)
{
    std::string query = std::format("INSERT INTO tags (tag_name,color) VALUES('{}', '{}');",
                                    wstrToHex(tagName), RGBAToHexStr(color));
    return this->_db->exec(query.c_str());
}

bool AppDB::addTag(std::wstring tagName, std::string color)
{
    std::string query = std::format("INSERT INTO tags (tag_name,color) VALUES('{}', '{}');",
                                    wstrToHex(tagName), color);
    return this->_db->exec(query.c_str());
}

TableData AppDB::getTags()
{
    return this->_db->query("SELECT * FROM tags");
}

bool AppDB::addTagToFile(std::wstring tagName, std::wstring filePath)
{
    std::string query = std::format("INSERT INTO file_tags (tag_name,filepath) VALUES('{}', '{}');",
                                    wstrToHex(tagName), wstrToHex(filePath));
    return this->_db->exec(query.c_str());
}
