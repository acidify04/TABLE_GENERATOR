#include "course_db.h"
#include <iostream>

int main()
{
    std::cout << "Hello World!" << std::endl;
    CourseDatabase db;
    db.load();
    return 0;
}