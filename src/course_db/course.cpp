#include <iostream>
#include <stack>

#include "course.h"

// - Getter

int Course::get_grade() const
{
    return times.size();
}

std::string Course::get_name() const
{
    return name;
}

std::string Course::get_professor() const
{
    return professor;
}

CourseType Course::get_type() const
{
    return type;
}

Semester Course::get_semester() const
{
    return semester;
}

int Course::get_year() const
{
    return year;
}

bool Course::is_english_a() const
{
    return english_a;
}

std::set<Department> Course::get_departments() const
{
    return departments;
}

// Decoder

/*
Course Format
<course>
    <id>integer</id>
    <name>string</name>
    <professor>string</professor>
    <type>integer</type>
    <semester>integer</semester>
    <english>integer</english>
    <professor>string</professor>
    <department>integer</department> // support multiple department
    <department>integer</department>
    <time> // support multiple department
        <weekday>integer</weekday>
        <t>integer</t>
        <room>string</room>
        <building>string</building>
    </time>
    <time>
        <weekday>integer</weekday>
        <t>integer</t>
        <room>string</room>
        <building>string</building>
    </time>
</course>
*/

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

Course::Course(const std::string &input)
{
    std::string::const_iterator pt;
    for (pt = input.begin(); pt != input.end();)
    {
        ParseResult result = parse_tag(pt, input.end());
        if (!result.is_success)
            throw std::runtime_error("Fail to decode course.");
        else if (result.tag == "time")
            this->times.push_back(decode_coursetime(result.value));
        else if (result.tag == "department")
            this->departments.insert(decode_department(result.value));
        else if (result.tag == "id")
            id = std::stoi(result.value);
        else if (result.tag == "name")
            name = result.value;
        else if (result.tag == "professor")
            professor = result.value;
        else if (result.tag == "type")
            type = decode_coursetype(result.value);
        else if (result.tag == "semester")
            semester = decode_semester(result.value);
        else if (result.tag == "year")
            year = std::stoi(result.value);
        else if (result.tag == "english")
            english_a = result.value != "0";
        else if (result.tag != "course")
            throw std::runtime_error("Invalid tag error");

        if (pt + 1 == input.end())
            break;
    }
}

CourseTime decode_coursetime(const std::string &input)
{
    CourseTime time;
    std::string::const_iterator pt;
    for (pt = input.begin(); pt != input.end();)
    {
        ParseResult result = parse_tag(pt, input.end());
        if (result.tag == "weekday")
            time.weekday = decode_weekday(result.value);
        else if (result.tag == "t")
            time.time = std::stoi(result.value);
        else if (result.tag == "room")
            time.room = result.value;
        else if (result.tag == "building")
            time.building = result.value;

        if (pt + 1 == input.end())
            break;
    }
    return time;
}

Semester decode_semester(const std::string &input)
{
    int value = std::stoi(input);

    switch (value)
    {
    case 0:
        return Semester::Spring;
    case 1:
        return Semester::Summer;
    case 2:
        return Semester::Fall;
    case 3:
        return Semester::Winter;
    default:
        throw std::exception(); // Invalid semester value.
    }
}

Weekday decode_weekday(const std::string &input)
{
    int value = std::stoi(input);

    switch (value)
    {
    case 0:
        return Weekday::Sun;
    case 1:
        return Weekday::Mon;
    case 2:
        return Weekday::Tue;
    case 3:
        return Weekday::Wed;
    case 4:
        return Weekday::Thu;
    case 5:
        return Weekday::Fri;
    case 6:
        return Weekday::Sat;
    default:
        throw std::exception(); // Invalid semester value.
    }
}

CourseType decode_coursetype(const std::string &input)
{
    int value = std::stoi(input);

    switch (value)
    {
    case 0:
        return CourseType::General_Education;
    case 1:
        return CourseType::CoreCommunication;
    case 2:
        return CourseType::CoreCreativity;
    case 3:
        return CourseType::CoreChallenge;
    case 4:
        return CourseType::CoreTrust;
    case 5:
        return CourseType::ElectiveCommunication;
    case 6:
        return CourseType::ElectiveCreativity;
    case 7:
        return CourseType::ElectiveChallenge;
    case 8:
        return CourseType::ElectiveConvergence;
    case 9:
        return CourseType::Major;
    case 10:
        return CourseType::MajorRequired;
    case 11:
        return CourseType::MajorFundamental;
    default:
        throw std::exception(); // Invalid semester value.
    }
}

Department decode_department(const std::string &input)
{
    int value = std::stoi(input);

    switch (value)
    {
    case 0:
        return Department::ComputerScience;
    default:
        throw std::exception(); // Invalid semester value.
    }
}