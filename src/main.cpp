#include "course_db.h"
#include <iostream>
#include <string>
#include "table_db.h"

using namespace std;

int main()
{
    std::cout << "Hello World!" << std::endl;
    TableDatabase db;
    Table table("<table><id>101</id><name>Park</name><user_id>20231234</user_id><year>2</year><Semester>Fall</Semester><course><id>10000</id><name>OOP</name><professor>Kim</professor><type>10</type><semester>2</semester><english>1</english><professor>string</professor><department>integer</department><department>integer</department><time><weekday>integer</weekday><t>3</t><room>727</room><building>310</building></time><time><weekday>4</weekday><t>4</t><room>728</room><building>310</building></time></course></table>");
    db.insert(table);
    db.save();
    db.load();

    return 0;
}