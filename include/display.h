#ifndef DISPLAY_H
#define DISPLAY_H

#include "course.h"
#include "course_db.h"
#include "table.h"
#include "table_db.h"
#include "table_generator.h"
#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

struct Schedule;

struct User
{
    string name;
    int year;
    int student_id;
    Department department;
};

extern User currentUser;
extern vector<Schedule *> schedules;       // ����� �ð�ǥ ���
extern vector<string> selectedProfessors;  // ����� ���� ���� ���
extern bool isEnglishA;                    // ����� ���� A ���� ����
extern int maxCredits;                     // ����� �ִ� ����
extern unordered_set<string> selectedDays; // ����� ���� ���� ���

class Display
{
  private:
    // ������ ����
    CourseDatabase courseDatabase;
    TableDatabase tableDatabase;
    TableGenerator tableGenerator;
    Table* table;
    User user;


    public:
    Display(CourseDatabase &courseDatabase, TableDatabase &tableDatabase, TableGenerator &tableGenerator);
    void display();
    void createSchedule();
    void searchSchedule();
    void searchCourse();
    void setupUser();
    void mainMenu();
};


#endif
