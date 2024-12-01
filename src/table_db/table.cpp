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
        cerr << "������ ���� ��ȣ�� �ð�ǥ�� �����ϴ�. �ٽ� �������ּ���." << endl;
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
    if (semester != decode_semester(tableQuery.semester)) {
        return false;
    }

    if (year != stoi(tableQuery.year)) {
        return false;
    }

    if (name != tableQuery.name) {
        return false;    
    }

    if (user_id != stoi(tableQuery.user_id)) {
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
    ParseResult result;
    ParseResult course_result;
    auto it = str.begin();
    while (it != str.end()) {
        result = parse_tag(it, str.end());
        if (result.is_success) {
            if (result.tag == "id") {
                id = stoi(result.value);
            }
            else if (result.tag == "name") {
                name = stoi(result.value);
            }
            else if (result.tag == "user_id") {
                user_id = stoi(result.value);
            }
            else if (result.tag == "year") {
                year = stoi(result.value);
            }
            else if (result.tag == "Semester") {
                semester = decode_semester(result.value);
            }
            else if (result.tag == "Course") {
                Course course(result.value);
                courses.push_back(course);
            }
        }
    }
}

// AllArgsConstructor
Table::Table(int iid, int iuser_id, std::string iname, std::vector<Course> icourses, int iyear, Semester isemester) {
    id = iid;
    user_id = iuser_id;
    name = iname;
    courses = icourses;
    year = iyear;
    semester = isemester;
}

Table::~Table() { }

// getter

Semester Table::get_semester() const {
    return semester;
}

int Table::get_year() const {
    return year;
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
    string tag;
    string value;
};

// Parse tag and tag's contents
ParseResult parse_tag(string::const_iterator &pt, const string::const_iterator &end)
{
    ParseResult result;

    std::string target_token;
    std::string temp_token;
    bool is_success = false;
    bool is_course = false;
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