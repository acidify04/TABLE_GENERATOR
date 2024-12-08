#include "table_generator.h"
#include "course_db.h"
#include "table_db.h"
#include <random>
#include <vector>

using namespace std;

TableGenerator::TableGenerator(CourseDatabase &courseDB, TableDatabase &tableDB)
    : courseDB(courseDB), tableDB(tableDB), totalGrade(0)
{
}

void TableGenerator::generateTable(Table &table) // query�� ó���� courseDB�� �ִ� course�� greedyAloghrithm���� �ð�ǥ ����
{
    vector<Course> courses = courseDB.query(query); // ���Ǹ�� ����

    random_device rd;
    mt19937 g(rd()); // ����

    this->currentGrade = 0;
    this->existCourse.clear();
    this->time.clear(); // ���� ���� ����, �ð� �ʱ�ȭ

    shuffle(courses.begin(), courses.end(), g); // course�� ������ ����

    if (!query.name.empty() && !courses.empty()) // Ư�������� �ִ� ��� (vector�� ������� ���� ���)
    {
        table.insert_course(courses[0]);
        this->existCourse.insert(courses[0].get_name());
        this->currentGrade += courses[0].get_grade();
    }

    query.name.clear();
    courses = this->courseDB.query(this->query);
    shuffle(courses.begin(), courses.end(), g);

    if (!query.professors.empty() && !courses.empty()) // �켱���� ���� ó��
    {
        for (Course &course : courses)
        {
            if (totalGrade >= currentGrade + course.get_grade())
            {
                if (this->findTime(course) && findCourse(course))
                {
                    table.insert_course(course);
                    this->existCourse.insert(course.get_name());
                    this->currentGrade += course.get_grade();
                }
            }
        }
        query.professors.clear();
    }

    courses = this->courseDB.query(this->query);

    shuffle(courses.begin(), courses.end(), g);
    for (Course &course : courses) // ������ ���� ó��
    {
        if (totalGrade >=
            currentGrade + course.get_grade()) // ������ �� ���� �� ���� ���� ���� + course�� ������ �� �۰ų� ���� ���
        {
            if (this->findTime(course) && findCourse(course)) // �ð��� ��ġ�� ���� ���
            {
                table.insert_course(course); // �ð�ǥ�� ���� �߰�
                this->existCourse.insert(course.get_name());
                this->currentGrade += course.get_grade(); // ���� �� �������� ������ ������ŭ ����
            }
        }
    }

    this->tableDB.insert(table);
}

void TableGenerator::setQuery(CourseQuery query)
{
    this->query = query;
}

void TableGenerator::setTotalGrade(int grade) // ������ �ð�ǥ�� �� ������ ����
{
    this->totalGrade = grade;
}

bool TableGenerator::findTime(Course course) // ��ġ�� �ð� �ִ��� Ȯ��
{
    for (const CourseTime availableTime : course.get_times())
    {
        for (const CourseTime existingTime : this->time)
        {
            if (availableTime.weekday == existingTime.weekday &&
                availableTime.time == existingTime.time) // ��ġ�� �ð��� �ִ� ���
            {
                return false;
            }
        }
    }

    for (const CourseTime availableTime : course.get_times()) // �ð�ǥ�� �Ҵ�� �ð��� ���
    {
        this->time.push_back(availableTime);
    }

    return true;
}

bool TableGenerator::findCourse(Course course) // ���� ���� �ȵ����� ����
{
    if (this->existCourse.find(course.get_name()) != this->existCourse.end())
    {
        return false;
    }
    return true;
}