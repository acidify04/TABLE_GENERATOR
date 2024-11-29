#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "table.h"

using namespace std;

int Table::get_total_grade() {
    int total_grade = 0;
    vector<Course>::iterator p;

    for (p = courses.begin(); p != courses.end(); ++p) {   
        total_grade += p->get_grade();
    }

    return total_grade;
}

void Table::remove_course(Course &course) {
    auto toRemove = find(courses.begin(), courses.end(), course);
    if (toRemove != courses.end()) {
        courses.erase(toRemove);
    } else {
        cerr << "선택한 강의 번호가 시간표에 없습니다. 다시 선택해주세요." << endl;
    }
}

bool Table::insert_course(Course &course) {
    try {
        courses.push_back(course);
    }
    catch (const exception &e) {
        return false;
    }
    return true;   
}

vector<Course> Table::get_course() const {
    return courses;
}

bool Table::is_satisfy(TableQuery tableQuery) const {
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
    string raw_value;
    raw_value.append("<Table>");
    raw_value.append("<id>" + to_string(id) + "</id>");
    raw_value.append("<user_id>" + to_string(user_id) + "</user_id>");
    raw_value.append("<year>" + to_string(year) + "</year>");
    raw_value.append("<semester>" + encode_semester(semester) + "</semester>");
    raw_value.append("<name>" + name + "</name>");

    for (auto course : courses) {
        raw_value.append("<course>" + course.encode() + "</course>");
    }
    raw_value.append("</Table>");
    return raw_value;
}

string encode_semester(Semester semester) {
    if (semester == Semester::Spring) {
        return "Spring";
    }
    else if (semester == Semester::Summer) {
        return "Summer";
    }
    else if (semester == Semester::Fall)
    {
        return "Fall";
    }
    else if (semester == Semester::Winter)
    {
        return "Winter";
    }
}

Table::Table(string &string){

}

Table::~Table() { }

// getter

Semester Table::get_semester() const {
    return semester;
}

int Table::get_year() const {
    return year;
}

const vector<Course>& Table::get_courses() const {
    return courses;
}

string Table::get_name() const {
    return name;
}

int Table::get_user_id() const {
    return user_id;
}

int Table::get_id() const {
    return id;
}