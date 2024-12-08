#include "course_db.h"
#include "display.h"
//#include "schedule_management.h"
//#include "table_generator.h"
#include "table_db.h"
//#include <conio.h>
#include <iostream>
#include <string>
//#include "table_db.h"
#include "table_generator.h"

using namespace std;

int main()
{
    CourseDatabase courseDatabase;
    TableDatabase tableDatabase;
    TableGenerator tableGenerator(courseDatabase, tableDatabase);

    //Display display(courseDatabase, tableDatabase, tableGenerator);

    mainMenu();
    
    return 0;
}
