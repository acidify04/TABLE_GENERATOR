#include "course_db.h"
#include "parser.h"
#include <fstream>
#include <iostream>

#define TokenLength 2

bool DateIndexKey::operator==(const DateIndexKey &key) const
{
    return year == key.year && semester == key.semester && time == key.time && weekday == key.weekday;
}

std::size_t DateIndexKeyHash::operator()(const DateIndexKey &key) const
{
    return std::hash<int>()(key.year) ^ (std::hash<std::string>()(encode_semester(key.semester)) << 1) ^
           (std::hash<std::string>()(encode_weekday(key.weekday)) << 2) ^ (std::hash<int>()(key.time) << 3);
}

CourseDatabase::CourseDatabase() {};

void CourseDatabase::load()
{
    load_courses();
    load_date_index();
    load_name_index();
}

void CourseDatabase::load_courses()
{
    std::ifstream file("courses.txt");
    if (!file.is_open())
    {
        std::cout << "Failed to open courses.txt" << std::endl;
        return;
    }

    size_t current_line = 0;
    std::string line_str;
    while (std::getline(file, line_str))
    {
        try
        {
            std::string::const_iterator pt = line_str.begin();
            ParseResult result = parse_tag(pt, line_str.end());
            if (result.tag == "course")
            {
                Course course(result.value);
                int id = course.get_id();
                courses[id] = course;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "Fail to decode course (line:" << current_line + 1 << ")" << std::endl;
        }
        current_line++;
    }
    file.close();
}

/*
indexing_by_date format
<index>
    <year>integer</year>
    <semester>integer</semester>
    <time>integer</time>
    <courses>
        <id>integer</id>
        <id>integer</id> // course id
    </courses>
</index>
*/

void CourseDatabase::load_date_index()
{
    std::ifstream file("date_index.txt");

    if (!file.is_open())
    {
        file.close();
        indexing_by_date();
        return;
    }

    std::string line_str;
    while (std::getline(file, line_str))
    {
        std::string::const_iterator begin = line_str.begin();
        ParseResult result = parse_tag(begin, line_str.end());
        if (result.is_success && result.tag == "index")
        {
            begin = result.value.begin();
            DateIndexKey key;
            Courses course_ptrs;
            for (; begin != result.value.end(); begin++)
            {
                ParseResult property = parse_tag(begin, result.value.end());
                if (property.tag == "year")
                    key.year = std::stoi(property.value);
                else if (property.tag == "semester")
                    key.semester = decode_semester(property.value);
                else if (property.tag == "time")
                    key.time = std::stoi(property.value);
                else if (property.tag == "weekday")
                    key.weekday = decode_weekday(property.value);
                else if (property.tag == "courses")
                {
                    std::string ids = property.value;
                    std::string::const_iterator ids_ptr = ids.begin();
                    for (; ids_ptr != ids.end(); ids_ptr++)
                    {
                        ParseResult id_property = parse_tag(ids_ptr, ids.end());
                        if (id_property.tag == "id")
                        {
                            int id = std::stoi(id_property.value);
                            course_ptrs.insert(&courses[id]);
                        }
                    }
                }
            }
            date_index[key].insert(course_ptrs.begin(), course_ptrs.end());
        }
    }

    file.close();
}

void CourseDatabase::indexing_by_date()
{
    for (auto &index : courses)
    {
        Course &course = index.second;
        for (const auto time : course.get_times())
        {
            DateIndexKey key = {course.get_year(), course.get_semester(), time.weekday, time.time};
            date_index[key].insert(&course);
        }
    }

    std::ofstream file("date_index.txt");

    std::string raw_value;
    for (auto index : date_index)
    {
        const DateIndexKey key = index.first;
        raw_value.append("<index>");
        raw_value.append("<year>" + std::to_string(key.year) + "</year>");
        raw_value.append("<semester>" + encode_semester(key.semester) + "</semester>");
        raw_value.append("<time>" + std::to_string(key.time) + "</time>");
        raw_value.append("<weekday>" + encode_weekday(key.weekday) + "</weekday>");
        raw_value.append("<courses>");
        for (auto course : index.second)
            raw_value.append("<id>" + std::to_string(course->get_id()) + "</id>");
        raw_value.append("</courses>");
        raw_value.append("</index>");
        file << raw_value << std::endl;
        raw_value.clear();
    }

    file.close();
}

/*
indexing by name format
<index>
    <name>string</name>
    <courses>
        <id>integer</id>
    </courses>
</index>
*/

void CourseDatabase::indexing_by_name()
{
    for (const auto &element : courses)
    {
        const Course &course = element.second;
        std::string name = course.get_name();
        std::set<std::string> tokens = seperate_str(name.begin(), name.end(), TokenLength);
        for (const auto &token : tokens)
            name_index[token].insert(&course);
    }

    std::ofstream file("name_index.txt");
    std::string raw_value;
    for (const auto &element : name_index)
    {
        raw_value.append("<index>");
        raw_value.append("<name>" + element.first + "</name>");
        raw_value.append("<courses>");
        for (const auto &course : element.second)
        {
            raw_value.append("<id>" + std::to_string(course->get_id()) + "</id>");
        }
        raw_value.append("</courses>");
        raw_value.append("</index>");

        file << raw_value << std::endl;
        raw_value.clear();
    }
    file.close();
}

void CourseDatabase::load_name_index()
{
    std::ifstream file("name_index.txt");

    if (!file.is_open())
    {
        file.close();
        indexing_by_name();
        return;
    }

    std::string line_str;
    while (std::getline(file, line_str))
    {
        std::string::const_iterator begin = line_str.begin();
        ParseResult index = parse_tag(begin, line_str.end());

        if (index.is_success && index.tag == "index")
        {
            begin = index.value.begin();
            std::string key;
            Courses course_ptrs;
            for (; begin != index.value.end(); begin++)
            {
                ParseResult property = parse_tag(begin, index.value.end());
                if (property.tag == "name")
                    key = property.value;
                else if (property.tag == "courses")
                {
                    std::string ids = property.value;
                    std::string::const_iterator ids_begin = ids.begin();
                    for (; ids_begin != ids.end(); ids_begin++)
                    {
                        ParseResult id_tag = parse_tag(ids_begin, ids.end());
                        if (id_tag.is_success && id_tag.tag == "id")
                        {
                            int id = std::stoi(id_tag.value);
                            course_ptrs.insert(&courses[id]);
                        }
                    }
                }
            }
            name_index[key] = course_ptrs;
        }
    }

    file.close();
}

std::set<std::string> seperate_str(std::string::iterator begin, std::string::iterator end, int by)
{
    std::set<std::string> result;
    std::string token;
    std::string::iterator current = begin;
    std::string::iterator point = begin;
    int count = 0;

    for (; point != end; current++)
    {
        point = current;
        for (count = 0; count < by;)
        {
            if (*point != ' ' && *point != '\n')
            {
                token.push_back(*point);
                count++;
            }
            point++;
        }
        result.insert(token);
        token.clear();
    }

    return result;
}

std::vector<Course> CourseDatabase::query(CourseQuery condition) const
{
    std::set<Weekday> week_query;
    std::set<Time> time_query;

    Courses matched_courses;

    // query by using date index.
    Courses date_matched_courses;
    if (condition.weekdays.empty())
        week_query = {Weekday::Sun, Weekday::Mon, Weekday::Tue, Weekday::Wed, Weekday::Thu, Weekday::Fri, Weekday::Sat};
    else
        week_query = condition.weekdays;

    if (condition.times.empty())
        time_query = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    else
        time_query = condition.times;

    for (auto time : time_query)
    {
        for (auto week : week_query)
        {
            const DateIndexKey &key = {condition.year, condition.semester, week, time};
            if (date_index.count(key) > 0)
                date_matched_courses.insert(date_index.at(key).begin(), date_index.at(key).end());
        }
    }

    // query by name index
    if (!condition.name.empty())
    {
        std::string::iterator name_begin = condition.name.begin();
        std::set<std::string> search_tokens = seperate_str(name_begin, condition.name.end(), TokenLength);
        Courses name_matched_courses;
        for (const auto &token : search_tokens)
        {
            name_matched_courses.insert(name_index.at(token).begin(), name_index.at(token).end());
        }
        std::set_intersection(date_matched_courses.begin(), date_matched_courses.end(), name_matched_courses.begin(),
                              name_matched_courses.end(), std::inserter(matched_courses, matched_courses.begin()));
    }
    else
        matched_courses = date_matched_courses;

    // query by departments, professors
    Courses unmatched_courses;

    for (const auto course_ptr : matched_courses)
    {
        bool is_matched = true;

        if (!condition.departments.empty())
        {
            std::set<Department> departments = course_ptr->get_departments();
            bool is_included = std::includes(condition.departments.begin(), condition.departments.end(),
                                             departments.begin(), departments.end());
            if (!is_included)
            {
                unmatched_courses.insert(course_ptr);
                continue;
            }
        }

        if (!condition.professors.empty())
        {
            bool is_empty =
                std::find(condition.professors.begin(), condition.professors.end(), course_ptr->get_professor())
                    ->empty();
            if (is_empty)
            {
                unmatched_courses.insert(course_ptr);
                continue;
            }
        }
    }

    std::vector<Course> result;
    for (auto course_ptr : matched_courses)
        if (unmatched_courses.count(course_ptr) == 0)
            result.push_back(*course_ptr);

    return result;
}
