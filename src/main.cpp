#include "course_db.h"
//#include "display.h"
//#include "schedule_management.h"
//#include "table_generator.h"
#include "table_db.h"
//#include <conio.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    //std::cout << "Hello World!" << std::endl;
    CourseDatabase courseDB;
    courseDB.load();
    TableDatabase db;
    //CourseQuery query = {Semester::Fall, 2024, {}, {}, "ACT", {}, {}};

    /*Table table("sample id");
    TableGenerator table_generator(courseDB, db);
    table_generator.setTotalGrade(20);
    table_generator.setQuery(query);
    table_generator.generateTable(table);*/
    db.load();

    for (Course course : db.get_tables()[0].get_course())
    {
        cout << course.get_name() << endl;
    }
    
    //mainMenu();
    return 0;
}
