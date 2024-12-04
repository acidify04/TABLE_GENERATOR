#include "course_db.h"
#include <iostream>
#include <string>
#include "table_db.h"
#include "table_generator.h"

using namespace std;

int main()
{
    std::cout << "Hello World!" << std::endl;
    CourseDatabase courseDB;
    courseDB.load();
    TableDatabase db;
    CourseQuery query = {Semester::Fall, 2024, {}, {}, "", {}, {}};

    Table table("sample id");
    TableGenerator table_generator(courseDB, db);
    table_generator.setTotalGrade(20);
    table_generator.setQuery(query);
    table_generator.generateTable(table);
    
    return 0;
}