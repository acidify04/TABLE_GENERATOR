#include <iostream>
#include <vector>
#include <string>
#include "table_db.h"

void TableDatabase::insert(Table &table)
{	
	tables.push_back(table);
}

