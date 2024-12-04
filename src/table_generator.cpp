#include "table_generator.h"
#include "table_db.h"
#include <vector>
#include "course_db.h"

using namespace std;

TableGenerator::TableGenerator(CourseDatabase &courseDB, TableDatabase &tableDB)
    : courseDB(courseDB), tableDB(tableDB), totalGrade(0)
{
}


void TableGenerator::generateTable(Table &table) //query로 처리된 courseDB에 있는 course중 greedyAloghrithm으로 시간표 생성
{
    vector<Course> courses = this->courseDB.query(this->query);
    cout << courses.size() << endl;
    for (Course &course : courses) // course의 수만큼 반복
    {
        if (totalGrade >= currentGrade + course.get_grade()) // 설정한 총 학점 수 보다 현재 학점 + course의 학점이 더 작거나 같을 경우
        {
            if (this->findTime(course) && isConflict(course)) // 시간이 겹치지 않을 경우
            {
                table.insert_course(course);// 시간표에 강의 추가
                this->existCourse.push_back(course.get_name());
                this->currentGrade += course.get_grade(); // 현재 총 학점수를 강의의 학점만큼 증가
                cout << "add : " << course.get_name() << endl;
                for (CourseTime ctime : course.get_times())
                {
                    cout << "Day : ";
                    switch (ctime.weekday)
                    {
                    case Weekday::Mon:
                        cout << "Mon " << ctime.time << endl;
                        break;
                    case Weekday::Tue:
                        cout << "Tue " << ctime.time << endl;
                        break;
                    case Weekday::Wed:
                        cout << "Wed " << ctime.time << endl;
                        break;
                    case Weekday::Thu:
                        cout << "Thu " << ctime.time << endl;
                        break;
                    case Weekday::Fri:
                        cout << "Fri " << ctime.time << endl;
                        break;
                    case Weekday::Sat:
                        cout << "Sat " << ctime.time << endl;
                        break;
                    case Weekday::Sun:
                        cout << "Sun " << ctime.time << endl;
                        break;
                    }
                }
            }
        }     
    }
    cout << "total : " << totalGrade << "current : " << currentGrade << endl;
    this->tableDB.insert(table);
}

void TableGenerator::setQuery(CourseQuery query) // table generator 생성
{
    this->query = query;
}

void TableGenerator::setTotalGrade(int grade) // 생성할 시간표의 총 학점수 설정
{
    this->totalGrade = grade;
}

bool TableGenerator::findTime(Course course) // 겹치는 시간 있는지 확인
{
    for (const CourseTime availableTime : course.get_times())
    {
        for (const CourseTime existingTime : this->time)
        {
            if (availableTime.weekday == existingTime.weekday && availableTime.time == existingTime.time) // 겹치는 시간이 있는 경우
            {
                return false;
            }
        }
    }

    for (const CourseTime availableTime : course.get_times())
    {
        this->time.push_back(availableTime);
    }

    return true; // 성공적으로 추가됨
}

bool TableGenerator::isConflict(Course course)
{
    for (string name : this->existCourse)
    {
        if (name == course.get_name())
        {
            return false;
        }
    }

    return true;
}