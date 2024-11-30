#ifndef _TABLE_DB_H_
#define _TABLE_DB_H_

#include "table.h"
#include <string>
#include <vector>

class TableDatabase
{
  public:
    std::vector<Table> query(TableQuery tableQuery);
    void insert(Table &table);
    void update(Table &table);
    void remove(int id);
    void load();
    void save();

    std::vector<Table> get_tables() const;

  private:
    std::vector<Table> tables;
};

struct TableQuery
{
    std::string id;
    std::string department;
    std::string semester;
    std::string year;
    std::string name;
    std::string user_id;
};

#endif
