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
    std::vector<Course> get_course() const;
    bool is_satisfy(TableQuery tableQuery) const;
    std::string encode();
    Table(std::string &str);
    ~Table();

    Semester get_semester() const;
    int get_year() const;
    const vector<Course>& get_courses() const;
    string get_name() const;
    int get_user_id() const;
    int get_id() const;

  private:
    Semester semester;
    int year;
    std::vector<Course> courses;
    std::string name;
    int user_id;
    int id;
};

#endif