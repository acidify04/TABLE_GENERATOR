#include "display.h"
#include "course_db.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <map>

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

void Display::display()
{
    // ���ϰ� �ð��� ���� ���Ǹ� ������ �� (���Ϻ��� �ð� ����)
    std::map<Weekday, std::map<Time, std::string>> schedule;

    vector<Course> courseList = table.get_course();

    // ������ ���ڿ��� ��ȯ�ϱ� ���� �迭
    std::vector<std::string> daysOfWeek = {"Mon", "Tue", "Wed", "Thu", "Fri"};

    // ���� �����͸� �ʿ� �߰�
    for (const auto &course : courseList)
    {
        for (const auto &courseTime : course.get_times())
        {
            Weekday weekday = courseTime.weekday;
            Time time = courseTime.time;
            schedule[weekday][time] = course.get_name(); // ���ϰ� �ð����� ���� �̸� ����
        }
    }

    // �ð�ǥ ���
    std::cout << "====================================================================================================="
                 "=================\n";
    std::cout << "Schedule:\n";

    // ��� ���
    std::cout << "Time ";
    for (size_t i = 0; i < daysOfWeek.size(); ++i) // Mon~Fri�� ���
    {
        std::cout << std::setw(30) << daysOfWeek[i];
    }
    std::cout << '\n';
    std::cout << "====================================================================================================="
                 "=================\n";

    // �ð� ������ ���
    for (Time time = 1; time <= 10; ++time) // �ð��� 1���� 10���� ����
    {
        std::cout << std::setw(4) << time << " "; // �ð� ���

        for (size_t i = 0; i < daysOfWeek.size(); ++i) // Mon~Fri�� ���
        {
            Weekday weekday = static_cast<Weekday>(i);

            if (schedule[weekday].count(time)) // Ư�� �ð��� ���ǰ� �ִ� ���
            {
                std::cout << std::setw(30) << schedule[weekday][time];
            }
            else // ���ǰ� ���� ��� ���� ���
            {
                std::cout << std::setw(30) << " ";
            }
        }
        std::cout << '\n';
    }

    std::cout << "====================================================================================================="
                 "=================\n";

    int input;
    std::cin >> input;
}


void Display::createSchedule()
{
    system("cls");
    vector<Course> courses = courseDatabase.query({});
    CourseQuery query;

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
    }*/  // �ϴ� ����Ʈ�����кθ�

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
            advance(it, input - 1);       // �ε����� 1-based���� 0-based�� ����
            query.professors.insert(*it); // ���� �̸��� ���õ� ���� ��Ͽ� �߰�
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

    display();

}

void Display::searchAll()
{
    CourseQuery query;
    query.user_year = 0;
    query.departments.insert(Department::ComputerScience);
    vector<Course> courseList = courseDatabase.query(query);

    std::cout << std::left // ���� ����
              << std::setw(15) << "���� �ڵ�" << std::setw(50) << "�����" << std::setw(20) << "��� ����"
              << "\n";
    std::cout << std::string(65, '-') << "\n"; // ���м�

    // �� ���� ���� ���
    for (const Course &course : courseList)
    {
        std::cout << std::left << std::setw(15) << course.get_id() << std::setw(50) << course.get_name()
                  << std::setw(20) << course.get_professor() << "\n";
    }

    int input;

    cin >> input;
}

void Display::searchCourse()
{

    CourseQuery query;

    query.user_year = 0;
    query.departments.insert(Department::ComputerScience);

    // ���� �ʵ� �Է�
    std::cout << "Enter course name (or leave blank): ";
    std::cin.ignore(); // ���� ����
    std::getline(std::cin, query.name);

    query.name = "�Ұ�";

    vector<Course> courseList = courseDatabase.query(query);

    std::cout << std::left // ���� ����
              << std::setw(15) << "���� �ڵ�" << std::setw(50) << "�����" << std::setw(20) << "��� ����"
              << "\n";
    std::cout << std::string(65, '-') << "\n"; // ���м�

    // �� ���� ���� ���
    for (const Course &course : courseList)
    {
        std::cout << std::left << std::setw(15) << course.get_id() << std::setw(50) << course.get_name()
                  << std::setw(20) << course.get_professor() << "\n";
    }

    int input;

    cin >> input;
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
    Table table;

    while (true)
    {
        system("cls");
        cout << "[MainMenu]\n1. User Setup\n2. Create Schedule\n3. SearchAll\n4. SearchOne\nSelect: ";
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
        case 3:
            system("cls");
            searchAll();
            break;
        case 4:
            system("cls");
            searchCourse();
            break;
        default:
            cout << "Please select a valid option." << endl;
        }
    }
}
