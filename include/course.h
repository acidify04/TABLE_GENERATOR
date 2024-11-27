#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <set>

typedef int Time;

enum class Weekday
{
    Sun, Mon, Tue, Wed, Thu, Fri, Sat
};

enum class CourseType
{
    General_Education,
    CoreCommunication,
    CoreCreativity,
    CoreChallenge,
    CoreConvergence,
    CoreTrust,
    ElectiveCommunication,
    ElectiveCreativity,
    ElectiveChallenge,
    ElectiveConvergence,
    Major,
    MajorRequired,
    MajorFundamental
};

enum class Department
{
    ComputerScience
};

enum class Semester
{
    Spring, Summer, Fall, Winter
};

struct CourseTime
{
    Weekday weekday;
    Time time;
    std::string room;
    std::string building;
};

class Course {
int id;
std::string name;
CourseType type;
int year;
Semester semester;
bool english_a;
std::string professor;
std::vector<CourseTime> times;

public:
    // Decode from string
    Course(const std::string&);
    // Encode to string
    std::string encode() const;

    int get_grade() const;
    std::string get_name() const;
    std::string get_professor() const;
    CourseType get_type() const;
    Semester get_semester() const;
    int get_year() const;
    bool is_english_a() const;
};

#endif