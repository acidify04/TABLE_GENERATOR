#include "course_db.h"
#include <iostream>
#include <string>
#include <set>
#include "table_db.h"
#include "table_generator.h"

using namespace std;

int main()
{
    std::cout << "Hello World!" << std::endl;
    CourseDatabase courseDB;
    courseDB.load();
    std::set<std::string> prof = {"ÇÑ¹Ì¶ó"};
    CourseQuery query = {Semester::Fall, 1900, {}, {}, "", {}, prof};
    for (const auto course : courseDB.query(query))
    {
        cout << course.get_name() << endl;
    }

    
    return 0;
}