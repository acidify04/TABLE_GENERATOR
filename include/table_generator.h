#ifndef TABLE_GENERATOR_H
#define TABLE_GENERATOR_H

#include <set>
#include <string>
#include "course_db.h"
#include "table_db.h"
#include <unordered_set>

using namespace std;


class TableGenerator
{
  private:
    CourseQuery query;
    CourseDatabase &courseDB; 
    TableDatabase &tableDB;

    unordered_set<string> existCourse;

    int currentGrade = 0; // ���ݱ��� ���� ������ �� ������
    int totalGrade; // ������ �� ������
    vector<CourseTime> time; // ������ �ð�ǥ�� ä���� �ð�

    bool findTime(Course course); // ��ġ�� �ð��ִ��� Ȯ���ϴ� �Լ�
    bool findCourse(Course course);
  public:
    void setTotalGrade(int i); // �� ������ ����

    void setQuery(CourseQuery query); // ���� ���� ����, �г�, �б���� �����մϴ٤�.

    TableGenerator(CourseDatabase &courseDB, TableDatabase &tableDB); // ������ courseDB�� tableDB�� �޽��ϴ�.

    void generateTable(Table &table); // �ð�ǥ�� ����ϴ�.
};
#endif