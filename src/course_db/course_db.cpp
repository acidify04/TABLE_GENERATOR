#include "course_db.h"
#include "parser.h"
#include <fstream>
#include <iostream>

bool IndexKey::operator==(const IndexKey &key) const
{
    return year == key.year && semester == key.semester && time == key.time && weekday == key.weekday;
}

std::size_t IndexKeyHash::operator()(const IndexKey &key) const
{
    return std::hash<int>()(key.year) ^ (std::hash<std::string>()(encode_semester(key.semester)) << 1) ^
           (std::hash<std::string>()(encode_weekday(key.weekday)) << 2) ^ (std::hash<int>()(key.time) << 3);
}

CourseDatabase::CourseDatabase() {};

void CourseDatabase::load()
{
    load_courses();
    load_index();
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
            Course course(line_str);
            int id = course.get_id();
            courses[id] = course;
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
indexing format
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

void CourseDatabase::load_index()
{
    std::ifstream file("index.txt");

    if (!file.is_open())
    {
        file.close();
        indexing();
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
            IndexKey key;
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
            indexed_courses[key] = course_ptrs;
        }
    }

    file.close();
}

void CourseDatabase::indexing()
{
    for (auto &index : courses)
    {
        Course &course = index.second;
        for (const auto time : course.get_times())
        {
            IndexKey key = {course.get_year(), course.get_semester(), time.weekday, time.time};
            indexed_courses[key].insert(&course);
        }
    }

    std::ofstream file("index.txt");

    std::string raw_value;
    for (auto index : indexed_courses)
    {
        const IndexKey key = index.first;
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
        std::cout << raw_value << std::endl;
        raw_value.clear();
    }

    file.close();
}

std::vector<Course> CourseDatabase::query(CourseQuery condition) const
{
    Courses matched_courses;

    // check department condition
    for (const auto &index : indexed_courses)
    {
        const IndexKey &key = index.first;
        if (key.semester != condition.semester || key.year != condition.year)
            continue;

        bool is_matched = true;

        if (!condition.times.empty())
            is_matched = is_matched && condition.times.count(key.time);

        if (!condition.weekdays.empty())
            is_matched = is_matched && condition.weekdays.count(key.weekday);

        if (is_matched)
            matched_courses.insert(index.second.begin(), index.second.end());
    }

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

        // name 설정
    }

    matched_courses.extract(unmatched_courses.begin());

    std::vector<Course> result;
    for (auto course_ptr : matched_courses)
    {
        result.push_back(*course_ptr);
    }

    return result;
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