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

bool AppDB::deleteTag(std::wstring tagName)
{
    std::string query = std::format("DELETE FROM tags WHERE tag_name='{}'",
                                    wstrToHex(tagName));
    return this->_db->exec(query.c_str());
}

TableData AppDB::getTags(std::vector<std::string> tags)
{
    std::string where = "";
    for (size_t i = 0; i < tags.size(); i++)
    {
        if (i == 0)
        {
            where = " WHERE";
        }
        else
        {
            where += " OR";
        }
        where += " tag_name = '" + tags.at(i) + "'";
    }
    std::string query = "SELECT * FROM tags" + where;
    return this->_db->query(query.c_str());
}

TableData AppDB::getFilesWithTags(const std::vector<std::string> &tags, bool tagFilterMode)
{
    std::string where = "";
    for (size_t i = 0; i < tags.size(); i++)
    {
        if (i == 0)
        {
            where = " WHERE";
        }
        else
        {
            where += " OR";
        }
        where += " tag_name = '" + tags.at(i) + "'";
    }
    std::string query = "SELECT *, COUNT(filepath) as c FROM file_tags" + where + " GROUP BY filepath" +
                        (tagFilterMode ? " HAVING c = " + std::to_string(tags.size()) : "");
    return this->_db->query(query.c_str());
}

TableData AppDB::getTagsOfFile(std::wstring file)
{
    std::string query = std::format("SELECT * FROM file_tags WHERE filepath='{}'",
                                    wstrToHex(file));
    return this->_db->query(query.c_str());
}

bool AppDB::addTagToFile(std::wstring tagName, std::wstring filePath)
{
    std::string query = std::format("INSERT INTO file_tags (tag_name,filepath) VALUES('{}', '{}');",
                                    wstrToHex(tagName), wstrToHex(filePath));
    return this->_db->exec(query.c_str());
}

bool AppDB::deleteTagFromFile(std::wstring tagName, std::wstring filePath)
{
    std::string query = std::format("DELETE FROM file_tags WHERE tag_name='{}' AND filepath='{}'",
                                    wstrToHex(tagName), wstrToHex(filePath));
    return this->_db->exec(query.c_str());
}
