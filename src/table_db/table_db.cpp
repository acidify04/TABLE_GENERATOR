#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "table_db.h"

using namespace std;

vector<Table> TableDatabase::query(TableQuery tableQuery) {
    vector<Table> result;
    if (!tableQuery.id.empty()) {
        copy_if(tables.begin(), tables.end(), back_inserter(result), 
            [&](const Table& table) { return table.get_id() == stoi(tableQuery.id);});
    }
    if (!tableQuery.name.empty()) {
        copy_if(result.begin(), result.end(), back_inserter(result),
                [&](const Table &table) { return table.get_name() == tableQuery.name; });
    }
    if (!tableQuery.semester.empty()) {
        copy_if(result.begin(), result.end(), back_inserter(result),
                [&](const Table &table) { return table.get_semester() == decode_semester(tableQuery.semester); });
    }
    if (!tableQuery.year.empty()) {
        copy_if(result.begin(), result.end(), back_inserter(result),
                [&](const Table &table) { return table.get_year() == stoi(tableQuery.year); });
    }
}

void TableDatabase::insert(Table &table) {	
	tables.push_back(table);
}

void TableDatabase::update(Table &table) {
    auto toUpdate = find_if(tables.begin(), tables.end(), [&table](const Table &t) { 
            return t.get_id() == table.get_id();
    });
  
    if (toUpdate != tables.end()) {
        tables.erase(toUpdate);
        tables.push_back(table);
    }
    else {
        cerr << "���ǿ� �´� �ð�ǥ�� �����ϴ�. �ٽ� �˻����ּ���." << endl;
    }
}

void TableDatabase::remove(int id) {
    auto toRemove = find_if(tables.begin(), tables.end(), [id](const Table &t) { 
        return t.get_id() == id; 
    });

    if (toRemove != tables.end()) {
        tables.erase(toRemove);
    }
}

void TableDatabase::load() {
    fstream file("tables.txt");
    string line;
    bool isTable = false;
    bool isCourse = false;
    int index = 0;

    int tableId;
    int year;
    Semester semester;
    int user_id;
    string name;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (line._Starts_with("<Table ")) {
                isTable = true;
            }
            else if (isTable) {
                if (index == 0) {
                    tableId = stoi(line);
                }
                else if (index == 1)
                {
                    string word;
                    istringstream iss(line);
                    int order = 0;
                    while (iss >> word)
                    {
                        if (order == 0)
                        {
                            year = stoi(word);
                        }
                        else if (order == 1)
                        {  
                            semester = decode_semester(word);
                        }
                        order++;
                    }
                }
                else if (index == 2) {
                    string word;
                    istringstream iss(line);
                    int order = 0;
                    while (iss >> word)
                    {
                        if (order == 0)
                        {
                            user_id = stoi(word);
                        }
                        else if (order == 1)
                        {
                            name = word;
                        }
                        order++;
                    }
                }
                index++;
                // TODO: course �о����
            }
        }
    }
    else {
        cerr << "������ �� �� �����ϴ�." << endl;
    }

}

void TableDatabase::save() {
    fstream file("tables.txt");

    if (file.is_open()) {
        for (Table table : tables) {
            file << table.encode();
        }
        file.close();
    } else {
        cout << "Failed to open tables.txt" << endl;
    }
}

// getter

vector<Table> TableDatabase::get_tables() const {
    return tables;
}

string encode_semester(Semester semester)
{
    if (semester == Semester::Spring)
    {
        return "Spring";
    }
    else if (semester == Semester::Summer)
    {
        return "Summer";
    }
    else if (semester == Semester::Fall)
    {
        return "Fall";
    }
    else if (semester == Semester::Winter)
    {
        return "Winter";
    }
}

string encode_course_type(CourseType course_type) {
    /*
    * General_Education,
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
    */

    if (course_type == CourseType::General_Education)
        return "General Eduaction";
    else if (course_type == CourseType::CoreCommunication)
        return "Core Communication";
    else if (course_type == CourseType::CoreCreativity)
        return "Core Creativity";
    else if (course_type == CourseType::CoreChallenge)
        return "Core Challenge";
    else if (course_type == CourseType::CoreConvergence)
        return "Core Convergencw";
    else if (course_type == CourseType::CoreTrust)
        return "Core Trust";
    else if (course_type == CourseType::ElectiveCommunication)
        return "Elective Communication";
    else if (course_type == CourseType::ElectiveCreativity)
        return "Elective Creativity";
    else if (course_type == CourseType::ElectiveChallenge)
        return "Elective Challenge";
    else if (course_type == CourseType::ElectiveConvergence)
        return "Elective Convergencw";
    else if (course_type == CourseType::Major)
        return "Major";
    else if (course_type == CourseType::MajorRequired)
        return "Major Requied";
    else if (course_type == CourseType::MajorFundamental)
        return "Major Fundamental";
}