#include "course_db.h"
#include "display.h"
//#include "schedule_management.h"
//#include "table_generator.h"
#include "table_db.h"
#include <conio.h>
#include <cstdlib> // system() �Լ� ���
#include <iostream>
#include <string>
//#include "table_db.h"
#include "table_generator.h"

using namespace std;

int main()
{
    Table table;
    CourseDatabase courseDatabase;
    TableDatabase tableDatabase;
    courseDatabase.load();


    TableGenerator tableGenerator(courseDatabase, tableDatabase);
    
    Display display(courseDatabase, tableDatabase, tableGenerator);


    display.mainMenu();

    
    return 0;
}
