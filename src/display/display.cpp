#include "display.h"
#include "course_db.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <unordered_set>

using namespace std;

vector<Schedule *> schedules;
CourseDatabase courseDb;

Display::Display(
    CourseDatabase& courseDatabase,
    TableDatabase& tableDatabase,
    TableGenerator& tableGenerator)
    :
    courseDatabase(courseDatabase), tableDatabase(tableDatabase), tableGenerator(tableGenerator)
{

}

//void Display::addCourse(const string &course)
//{
//    courses.push_back(course);
//}
//
//void Display::addDay(const string &day)
//{
//    days.push_back(day);
//}
//
//void Display::removeCourse(int index)
//{
//    if (index >= 0 && index < courses.size())
//    {
//        courses.erase(courses.begin() + index);
//    }
//}
//
//void Display::removeDay(int index)
//{
//    if (index >= 0 && index < days.size())
//    {
//        days.erase(days.begin() + index);
//    }
//}
//
//void Display::display() const
//{
//    cout << "===========================================================\n";
//    cout << "Schedule ID: " << id << "\n";
//    cout << "Year: " << year << ", Semester: " << semester << ", Department: " << department << "\n";
//    cout << "Courses:\n";
//
//    for (const auto &course : courses)
//    {
//        cout << "  - " << course << "\n";
//    }
//
//    cout << "Days to Avoid: ";
//    for (const auto &day : days)
//    {
//        cout << day << " ";
//    }
//    cout << "\n";
//
//    vector<string> daysOfWeek = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
//    vector<vector<string>> scheduleTable(7);
//    unordered_set<string> addedCourses;
//
//    for (const auto &course : courses)
//    {
//        for (const auto &courseObj : courseDb.query({}))
//        {
//            if (courseObj.get_name() == course && addedCourses.find(course) == addedCourses.end())
//            {
//                for (const auto &courseTime : courseObj.get_times())
//                {
//                    int dayIndex = static_cast<int>(courseTime.weekday);
//                    if (find(days.begin(), days.end(), daysOfWeek[dayIndex]) == days.end() &&
//                        find(days.begin(), days.end(), daysOfWeek[dayIndex]) == days.end())
//                    {
//                        scheduleTable[dayIndex].push_back(course);
//                    }
//                }
//                addedCourses.insert(course);
//            }
//        }
//    }
//
//    cout << "\n===========================================================\n";
//    cout << "Weekly Schedule:\n";
//    for (int i = 0; i < 7; ++i)
//    {
//        cout << daysOfWeek[i] << " | ";
//        for (const auto &course : scheduleTable[i])
//        {
//            cout << course << " | ";
//        }
//        cout << "\n";
//    }
//    cout << "===========================================================\n";
//}

void Display::createSchedule()
{
    system("cls");
    courseDatabase.load();
    vector<Course> courses = courseDatabase.query({});
    CourseQuery query;
    Table table;

    int input;

    if (courses.empty())
    {
        cout << "No courses found in the database. Please check the database." << endl;
        return;
    }

    cout << "Enter the year for the schedule (e.g., 2023, 2024): ";
    cin >> input;
    table.set_year(input);
    query.year = input;
    query.user_year = user.year;
    cin.ignore();
    system("cls");

    cout << "You are currently in year " << user.year << " (as set in User Setup). Press Enter to continue...";
    cin.ignore();
    system("cls");

    while (true)
    {
        cout << "Select a semester:\n1. Spring\n2. Summer\n3. Fall\n4. Winter\nSelect: ";
        cin >> input;
        cin.ignore();
        if (input >= 1 && input <= 4)
        {
            break;
        }
        else
        {
            cout << "Invalid choice. Please select a number between 1 and 4." << endl;
        }
    }
    switch (input)
    {
    case 1:
        table.set_semester(Semester::Spring);
        query.semester = Semester::Spring;
        break;
    case 2:
        table.set_semester(Semester::Summer);
        query.semester = Semester::Summer;
        break;
    case 3:
        table.set_semester(Semester::Fall);
        query.semester = Semester::Fall;
        break;
    case 4:
        table.set_semester(Semester::Winter);
        query.semester = Semester::Winter;
        break;
    }

    system("cls");

   /* set<string> departments;

    for (const auto &course : courses)
    {
        for (const auto &dept : course.get_departments())
        {
            departments.insert(static_cast<string>(encode_department(dept)));
        }
    }

    if (departments.empty())
    {
        cout << "No departments found. Please check the course data." << endl;
        return;
    }

    vector<string> departmentList(departments.begin(), departments.end());

    while (true)
    {
        cout << "Select a department: " << endl;
        int idx = 1;
        for (const auto &dept : departmentList)
        {
            cout << idx++ << ". ComputerScience" << endl;
        }

        cin >> input;
        cin.ignore();
        if (input >= 1 && input <= departmentList.size())
        {
            break;
        }
        else
        {
            cout << "Invalid choice. Please select a valid department number." << endl;
        }
    }*/  // 일단 소프트웨어학부만

    query.departments.insert(Department::ComputerScience);

    cout << "Select a preferred professor. You can add multiple professor. press -1 to exit";

    int index = 1;
    set<string> professorList;

    for (const auto &course : courses)
    {
        professorList.insert(course.get_professor());
    }

    for (const auto &professor : professorList)
    {
        cout << index++ << "." << professor << '\n';
    }

    while (true)
    {
        cin >> input;

        if (input == -1)
        {
            break;
        }

        if (input >= 1 && input <= professorList.size())
        {
            auto it = professorList.begin();
            advance(it, input - 1);       // 인덱스를 1-based에서 0-based로 조정
            query.professors.insert(*it); // 교수 이름을 선택된 교수 목록에 추가
        }
        else
        {
            cout << "Invalid selection. Please try again." << endl;
        }

        cout << "Would you like to select another professor? (Press -1 to finish): ";
    }

    cout << "Please select the day(s) you prefer for a free period. \n";

    cout << "1. Monday \n";
    cout << "2. Tuesday \n";
    cout << "3. Wednesday \n";
    cout << "4. Thrusday \n";
    cout << "5. Friday \n";
    cout << "6. None \n";

    while (true)
    {
        cin >> input;

        if (input < 1 && input > 6)
            cout << "Invalid Choice! \n";
        else
            break;
    }

    switch (input)
    {
    case 1:
        for (int i = 1; i < 6; i++)
        {
            Weekday day = static_cast<Weekday>(i);
            if (day != Weekday::Mon)
            {
                query.weekdays.insert(day);
            }
        }
        break;
    case 2:
        for (int i = 1; i < 6; i++)
        {
            Weekday day = static_cast<Weekday>(i);
            if (day != Weekday::Tue)
            {
                query.weekdays.insert(day);
            }
        }
        break;
    case 3:
        for (int i = 1; i < 6; i++)
        {
            Weekday day = static_cast<Weekday>(i);
            if (day != Weekday::Wed)
            {
                query.weekdays.insert(day);
            }
        }
        break;
    case 4:
        for (int i = 1; i < 6; i++)
        {
            Weekday day = static_cast<Weekday>(i);
            if (day != Weekday::Thu)
            {
                query.weekdays.insert(day);
            }
        }
        break;
    case 5:
        for (int i = 1; i < 6; i++)
        {
            Weekday day = static_cast<Weekday>(i);
            if (day != Weekday::Fri)
            {
                query.weekdays.insert(day);
            }
        }
        break;
    default:
        break;
    }

    cout << "total credit? \n";
    cin >> input;

    tableGenerator.setTotalGrade(input);
    tableGenerator.setQuery(query);
    tableGenerator.generateTable(table);

}


void searchAndModifySchedule()
{
    // 시간표 검색 후 수정 부분은 아직 미구현
}

void Display::setupUser()
{
    system("cls");
    cout << "Enter user name: ";
    cin >> user.name;

    while (true)
    {
        cout << "Enter user year (1-4): ";
        cin >> user.year;
        if (user.year >= 1 && user.year <= 4)
        {
            break;
        }
        else
        {
            cout << "Invalid input. Please enter a number between 1 and 4." << endl;
        }
    }

    while (true)
    {
        cout << "Enter user student ID (8-digit number): ";
        cin >> user.student_id;
        if (user.student_id >= 10000000 && user.student_id <= 99999999)
        {
            break;
        }
        else
        {
            cout << "Invalid input. Please enter an 8-digit number." << endl;
        }
    }
    cout << "Enter user department: ";
    cin >> user.department;

    cin.ignore();
}

void Display::mainMenu()
{
    system("cls");
    int choice;

    while (true)
    {
        system("cls");
        cout << "[MainMenu]\n1. User Setup\n2. Create Schedule\n3. Search and Modify Schedule\n4. Exit\nSelect: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            system("cls");
            setupUser();
            break;
        case 2:
            system("cls");
            createSchedule();
            break;
        //case 3:
        //    system("cls");
        //    searchAndModifySchedule();
        //    break;
        //case 4:
        //    system("cls");
        //    cout << "Exiting the program." << endl;
        //    return;
        default:
            cout << "Please select a valid option." << endl;
        }
    }
}
