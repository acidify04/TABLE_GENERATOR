#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "table_db.h"

using namespace std;

void TableDatabase::insert(Table &table) {	
	tables.push_back(table);
}

void TableDatabase::update(Table &table) {
    auto toUpdate = find_if(tables.begin(), tables.end(), [&table](const Table &t) { 
            return t.get_id() == table.get_id();
    });
  
    if (toUpdate != tables.end()) {
        tables.erase(toUpdate);
        tables.push_back(table);
    }
    else {
        cerr << "Error: There is no table." << endl;
    }
}

void TableDatabase::remove(int id) {
    auto toRemove = find_if(tables.begin(), tables.end(), [id](const Table &t) { 
        return t.get_id() == id; 
    });

    if (toRemove != tables.end()) {
        tables.erase(toRemove);
    }
}

void TableDatabase::load() {

}

void TableDatabase::save() {

}

// getter

vector<Table> TableDatabase::get_tables() const {
    return tables;
}