#ifndef COURSE_DB_H
#define COURSE_DB_H

#include <set>
#include <string>
#include <iostream>
#include "course.h"

typedef std::set<const Course*> Courses;

std::set<std::string> seperate_str(std::string::iterator, std::string::iterator, int);

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
    std::set<std::string> professors;
};

struct DateIndexKey
{
    int year;
    Semester semester;
    Weekday weekday;
    Time time;

    bool operator==(const DateIndexKey&) const;
};

struct DateIndexKeyHash { // for using index key in unordered map's key.
    std::size_t operator()(const DateIndexKey&) const;
};

class CourseDatabase
{
std::unordered_map<int, Course> courses;
std::unordered_map<DateIndexKey, Courses, DateIndexKeyHash> date_index;
std::unordered_map<std::string, Courses> name_index;

public:
CourseDatabase();
std::vector<Course> query(CourseQuery) const;
void load();

private:
void load_courses();
void load_date_index();
void indexing_by_date();
void load_name_index();
void indexing_by_name();
};
#endif