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
    string department;
};

extern User currentUser;
extern vector<Schedule *> schedules;       // 사용자 시간표 목록
extern vector<string> selectedProfessors;  // 사용자 선택 교수 목록
extern bool isEnglishA;                    // 사용자 영어 A 과목 여부
extern int maxCredits;                     // 사용자 최대 학점
extern unordered_set<string> selectedDays; // 사용자 선택 요일 목록

class Display
{
  private:
    // 의존성 주입
    CourseDatabase courseDatabase;
    TableDatabase tableDatabase;
    TableGenerator tableGenerator;
    Table* table;
    User user;


    public:
    Display(CourseDatabase &courseDatabase, TableDatabase &tableDatabase, TableGenerator &tableGenerator);
    void display();
    void createSchedule();
    void searchAll();
    void searchCourse();
    void setupUser();
    void mainMenu();
};


#endif
