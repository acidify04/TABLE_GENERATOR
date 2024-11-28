#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "table.h"

using namespace std;

int Table::get_total_grade()
{
    int total_grade = 0;
    vector<Course>::iterator p;

    for (p = courses.begin(); p != courses.end(); ++p)
    {   
        total_grade += p->get_grade();
    }

    return total_grade;
}

void Table::remove_course(Course &course)
{
    auto toRemove = find(courses.begin(), courses.end(), course);
    if (toRemove != courses.end())
    {
        courses.erase(toRemove);
    }
    else
    {
        cerr << "Error: There is no course." << endl;
    }
}

bool Table::insert_course(Course &course)
{
    try {
        courses.push_back(course);
    }
    catch (const exception &e)
    {
        return false;
    }
    return true;   
}

vector<Course> Table::get_course()
{
    return courses;
}

bool Table::is_satisfy(TableQuery tableQuery){
    if (semester != tableQuery.semester) {
        return false;
    }

    if (year != tableQuery.year) {
        return false;
    }

    if (name != tableQuery.name) {
        return false;    
    }

    if (user_id != tableQuery.user_id) {
        return false;
    }
    return true;
}

string Table::encode(){

}

Table::Table(string &string){

}

Table::~Table() { }

// getter, setter

Semester Table::get_semester() {
    return semester;
}

int Table::get_year() {
    return year;
}

vector<Course> Table::get_courses() {
    return courses;
}

string Table::get_name() {
    return name;
}

int Table::get_user_id() {
    return user_id;
}

int Table::get_id() const {
    return id;
}