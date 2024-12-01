#include "course_db.h"
#include <iostream>
#include "table_db.h"

using namespace std;

int main()
{
    std::cout << "Hello World!" << std::endl;
    CourseDatabase db;
    db.load();
    return 0;
}