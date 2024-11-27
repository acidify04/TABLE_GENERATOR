#ifndef COURSE_DB_H
#define COURSE_DB_H

#include <set>
#include <string>
#include <iostream>
#include "course.h"

struct CourseQuery
{
    // Semester is required.
    Semester semester;
    // Year is required
    int year;
    // Set weekdays that you want to query. if empty, all weekday's courses will be quried.
    std::set<Weekday> weekdays;
    // Set times you want to query. if empty, all times will be quried.
    std::set<Time> times;
    // Query by course name. Query support n-gram query. if empty, all courses will be quried.
    std::string name;
    // Query by departments. if empty, all department's courses will be quried.
    std::set<Department> departments;
    // Query by professor. if empty, all courses will be quried.
    std::string professor;
};

class CourseDatabase
{
std::vector<Course> courses;

public:
std::vector<Course> query(CourseQuery) const;

private:
void indexing_by_name();
};
#endif