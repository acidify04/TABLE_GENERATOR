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

  private:
    std::vector<Table> tables;
};

struct TableQuery
{
    Semester semester;
    int year;
    std::string name;
    int user_id;
};

#endif
