#ifndef _TABLE_H_
#define _TABLE_H_

#include "course.h"
#include "table_db.h"
#include <string>
#include <vector>

class Table
{
  public:
    int get_total_grade();
    void remove_course(Course &course);
    bool insert_course(Course &course);
    std::vector<Course> get_course();
    bool is_satisfy(TableQuery tableQuery);
    std::string encode();
    Table(std::string &string);
    ~Table();

    Semester get_semester();
    int get_year();
    vector<Course> get_courses();
    string get_name();
    int get_user_id();
    int get_id();

  private:
    Semester semester;
    int year;
    std::vector<Course> courses;
    std::string name;
    int user_id;
    int id;
};


#endif