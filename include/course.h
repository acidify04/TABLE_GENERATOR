#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <set>
#include <optional>
#include <vector>

typedef int Time;

enum class Weekday
{
    Sun, Mon, Tue, Wed, Thu, Fri, Sat
};

enum class CourseType
{
    General_Education, // ���뱳��
    CoreCommunication, // �ٽ�-����
    CoreCreativity, // �ٽ�-â��
    CoreChallenge, // �ٽ�-����
    CoreConvergence, // �ٽ�-����
    CoreTrust, // �ٽ�-�ŷ�
    ElectiveCommunication, // ����-����
    ElectiveCreativity, // ����-â��
    ElectiveChallenge, // ����-����
    ElectiveConvergence, // ����-����
    ElectiveTrust, // ����-�ŷ�
    Major, // ����
    MajorRequired, // �����ʼ�
    MajorFundamental // ��������
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

    bool operator==(const CourseTime&) const;
};

class Course {
int id;
std::string name;
CourseType type;
int year;
Semester semester;
bool english_a;
std::string professor;
std::set<Department> departments;
std::vector<CourseTime> times;
int minimum_year;

public:
int grade;
    Course();
    // Decode from string
    Course(const std::string&);
    // Encode to string
    std::string encode() const;

    int get_id() const;
    // �������� ����ϴ�.
    int get_grade() const;
    // �̼������� �ּ��г��Դϴ�. 0�̸� ��� �г��� �̼������մϴ�.
    int get_minimum_year() const;
    std::vector<CourseTime> get_times() const;
    std::string get_name() const;
    std::string get_professor() const;
    // ���������� �μ��� ����ϴ�.
    std::set<Department> get_departments() const;
    CourseType get_type() const;
    // ���ǰ� ������ �б��Դϴ�.
    Semester get_semester() const;
    // ���ǰ� ������ �����Դϴ�.
    int get_year() const;
    bool is_english_a() const;

    bool operator==(const Course&) const;
};

// if fail to decode, error will be thrown
CourseTime decode_coursetime(const std::string&);
std::string encode_coursetime(const CourseTime&);

// if fail to decode, error will be thrown
Semester decode_semester(const std::string&);
std::string encode_semester(Semester);

// if fail to decode, error will be thrown
Weekday decode_weekday(const std::string&);
std::string encode_weekday(Weekday);

// if fail to decode, error will be thrown
CourseType decode_coursetype(const std::string&);
std::string encode_coursetype(const CourseType);

// if fail to decode, error will be thrown
Department decode_department(const std::string&);
std::string encode_department(const Department);
#endif