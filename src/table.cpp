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

// semester to string
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

Table::Table(string &str){
    /*size_t id_start = str.find("<id>") + 4;
    size_t id_end = str.find("</id>");

    if (id_start != string::npos && id_end != string::npos && id_start != id_end) {
        id = stoi(str.substr(id_start, id_end - id_start));
    }

    size_t user_id_start = str.find("<user_id>") + 9;
    size_t user_id_end = str.find("</user_id>");

    if (user_id_start != string::npos && user_id_end != string::npos && user_id_start != user_id_end) {
        user_id = stoi(str.substr(user_id_start, user_id_end - user_id_start));
    }

    size_t year_start = str.find("<year>") + 6;
    size_t year_end = str.find("</year>");

    if (year_start != string::npos && year_end != string::npos && year_start != year_end) {
        year = stoi(str.substr(year_start, year_end - year_start));
    }

    size_t sem_start = str.find("<semester>") + 10;
    size_t sem_end = str.find("</semester>");

    if (sem_start != string::npos && sem_end != string::npos && sem_start != sem_end) {
        semester = decode_semester(str.substr(sem_start, sem_end - sem_start));
    }

    size_t name_start = str.find("<name>") + 6;
    size_t name_end = str.find("</name>");

    if (name_start != string::npos && name_end != string::npos && name_start != name_end)
    {
        name = str.substr(name_start, name_end - name_start);
    }*/
    
    ParseResult result = parse_tag(str.begin(), str.end());

}

// string to Semester
Semester decode_semester(string str) {
    if (str == "Spring") {
        return Semester::Spring;
    }
    else if (str == "Summer") {
        return Semester::Summer;
    }
    else if (str == "Fall") {
        return Semester::Fall;
    }
    else if (str == "Winter") {
        return Semester::Winter;
    }
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

struct ParseResult
{
    bool is_success;
    std::string tag;
    std::string value;
};

// Parse tag and tag's contents
ParseResult parse_tag(std::string::const_iterator &pt, const std::string::const_iterator &end)
{
    ParseResult result;

    std::string target_token;
    std::string temp_token;
    bool is_success = false;
    bool is_token_content = false;
    bool is_token = false;
    bool is_closed = false;
    std::string value_str;

    for (; pt != end; ++pt)
    {
        if (is_token)
        {
            if (*pt == '/')
                is_closed = true;
            else if (*pt == '>')
            {
                bool is_target_exit = target_token.size() > 0;
                if (is_closed)
                {
                    if (is_target_exit && target_token == temp_token)
                    {
                        is_success = true;
                        break;
                    }
                    else
                        value_str.append("</" + temp_token + ">");
                    is_closed = false;
                }
                else if (!is_target_exit)
                {
                    target_token = temp_token;
                    is_token_content = true;
                }
                else
                    value_str.append("<" + temp_token + ">");
                is_token = false;
                temp_token.clear();
            }
            else
                temp_token.push_back(*pt);
        }
        else if (*pt == '<')
            is_token = true;
        else if (*pt != '\n' && is_token_content)
            value_str.push_back(*pt);
    }

    return ParseResult{is_success, target_token, value_str};
}
