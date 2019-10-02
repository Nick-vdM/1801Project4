//
// Created by nick on 25/9/19.
//

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <cstring>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <fstream>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

/*
 * ============SOURCES==========
 * ORDER OF NOTATION SOURCE:
 *  https://github.com/gibsjose/cpp-cheat-sheet/blob/master/Data%20Structures%20and%20Algorithms.md#15-map-stdmap-and-stdunordered_map
 * All notations for structures of structure complexities were taken from there
 * MAP VS UNORDERED MAP ON LARGE SETS OF DATA:
 * https://pdfs.semanticscholar.org/a5a2/5b5ea9c8219fd43b3de87d14df31c0e89b23.pdf
 *
 * ======LOADING IN DATA========
 * While data is being initially loaded in, it does not have to be in any specific order. However, multiple
 * lines contain the same student, which means a student must be tested whether they already exist or not, and
 * if they do the grade can just get appended. As such, using a hash on the name would be the fastest way to
 * access this, as finding a student name is O(c) with that - meaning a type of map should be used. The biggest downside
 * is hashing, which could take up to O(n), but this is not called often.
 *      The other two properties that can be added is making it unordered and/or a multimap. First of all, this data
 * does not need to be unordered, meaning grabbing the O(c) time from unordered_map is worthwhile. However, having
 * multiple elements with the same key is not useful even if it could speed up access times as we don't want redundant
 * data. Getting around that would require extra implementations that is not worthwhile.
 *      An arguable downside of an unordered map is that the data gets scrambled after something is inserted. However,
 * this does not actually impact what the iterators point to. Another negative thing is
 * that it uses up more memory than a map to store things, however, the insert and access time being O(1) is more
 * valuable. Additionally, in that research paper its proven that in large datasets std::map is only better at
 * deleting things even in large instances. This should fully justify map as the best option for this task.
 *
 * Also, notice that int will be used throughout. This is because CPUs are the fastest at doing operations
 * on their native type, and this is usually an int type.
 */

//Instead of declaring this over and over just use this
//Using int to automatically set the optimal size
typedef std::unordered_map<std::string, std::array<int, 5>> mainDataStruct;

//Instead of making a switch statement for deciding which subject something is, use a map
std::map<std::string, int> const subjectIndex(
        {{"Biology",     0},
         {"Mathematics", 1},
         {"Chemistry",   2},
         {"Physics",     3},
         {"Total",       4}
        }
);

void loadInData(mainDataStruct &studentData) {
    //Each line consists of
    // studentName subect grade
    std::string studentName;
    std::string subject;
    std::string grade;
    std::ifstream ifs("bigData.txt");
    //Until end of input
    while (ifs.peek() != std::char_traits<char>::eof()) {
        ifs >> studentName >> subject >> grade;
        auto iterator = studentData.find(studentName);
        if (iterator != studentData.end()) {
            iterator->second[subjectIndex.at(subject)] = std::stoi(grade);
        } else {
            studentData[studentName] = std::array<int, 5>();
            studentData[studentName][subjectIndex.at(subject)] = std::stoi(grade);
        }
    }
    for (auto student = studentData.begin(); student != studentData.end(); student++) {
        student->second[4] = student->second[0] + student->second[1] + student->second[2] + student->second[3];
    }
}

class CompareKeys {
public:
    inline bool operator()(mainDataStruct::iterator &i, mainDataStruct::iterator &j) const {
        return i->first < j->first;
    }
};

class CompareIndex {
    // As comparators get compared highly frequently by std::sort, make both of these inline as
    // it'll give a decent speed boost
public:
    explicit CompareIndex(int index) : index(index) {}

    inline bool operator()(mainDataStruct::iterator &i, mainDataStruct::iterator &j) const {
        return i->second[index] > j->second[index];
    }

private:
    int index;
};

void printStudent(mainDataStruct::iterator student) {
    std::cout << "Name: "
              << std::left << std::setw(12) << student->first
              << std::left << std::setw(10) << " | Biology grade: " << student->second[0]
              << std::setw(10) << " | Mathematics grade: " << student->second[1]
              << std::setw(10) << " | Chemistry grade: " << student->second[2]
              << std::setw(10) << " | Physics grade: " << student->second[3]
              << std::setw(10) << " | Total grade: " << student->second[4]
              << std::endl;
}

void printData(std::vector<mainDataStruct::iterator> &toPrint) {
    for (auto i : toPrint) {
        printStudent(i);
    }
}


void printDataUntil(std::vector<mainDataStruct::iterator> &toPrint, int value, int index) {
    for (auto i : toPrint) {
        if (i->second[index] <= value) {
            break;
        }
        printStudent(i);
    }
}

class compareGrade {
public:
    explicit compareGrade(int index) : index(index) {}

    inline bool operator()(mainDataStruct::iterator &i, int const & value) const {
        return i->second[index] > value;
    }

private:
    int index;
};

int countStudentsWithGradeAbove(std::vector<mainDataStruct::iterator> &toPrint, int value, int index) {
    compareGrade comp{index};
    auto iter = std::lower_bound(toPrint.begin(), toPrint.end(), value, comp);
    return std::distance(toPrint.begin(), iter);
}


int main() {
    clock_t startTime = clock();

    mainDataStruct studentData{};
    loadInData(studentData);
    int studentCount = studentData.size();

    //Putting all the subjects first such that they match with the main data
    //Using std::vector here because we need dynamic space for adding more students, but don't need additional ways
    //to sort.
    std::array<std::vector<mainDataStruct::iterator>, 6> sortedData;
    //Prevent reallocation:
    for (auto i : sortedData) {
        i.reserve(studentCount);
    }

    //Fill everything with iterators
    for (int i = 0; i < 6; i++) {
        sortedData[i].reserve(studentData.size());
    }

    for (auto i = studentData.begin(); i != studentData.end(); i++) {
        for (int j = 0; j < 6; j++) {
            sortedData[j].emplace_back(i);
        }
    }

    for (int i = 0; i < 5; i++) {
        CompareIndex comp(i);
        std::sort(sortedData[i].begin(), sortedData[i].end(), comp);
    }
    std::sort(sortedData[5].begin(), sortedData[5].end(), CompareKeys());

    std::cout << "Time to insert and sort: " << double(clock() - startTime) / CLOCKS_PER_SEC << "s" << std::endl;

    std::string instruction;
    int instructionNum;
    while (true) {
        std::cout << ">>>>";
        std::cin >> instruction;
        for (int i = 0; i < 255 && instruction[i] != '\0'; i++) {
            instruction[i] = ::tolower(instruction[i]);
        }
        if (instruction == "biology") {
            printData(sortedData[0]);
        } else if (instruction == "maths") {
            printData(sortedData[1]);
        } else if (instruction == "chemistry") {
            printData(sortedData[2]);
        } else if (instruction == "physics") {
            printData(sortedData[3]);
        } else if (instruction == "total") {
            printData(sortedData[4]);
        } else if (instruction == "names") {
            printData(sortedData[5]);
        } else if (instruction == "biologyuntil") {
            std::cin >> instructionNum;
            printDataUntil(sortedData[0], instructionNum, 0);
        } else if (instruction == "mathsuntil") {
            std::cin >> instructionNum;
            printDataUntil(sortedData[1], instructionNum, 1);
        } else if (instruction == "chemistryuntil") {
            std::cin >> instructionNum;
            printDataUntil(sortedData[2], instructionNum, 2);
        } else if (instruction == "physicsuntil") {
            std::cin >> instructionNum;
            printDataUntil(sortedData[3], instructionNum, 3);
        } else if (instruction == "totaluntil") {
            std::cin >> instructionNum;
            printDataUntil(sortedData[4], instructionNum, 4);
        } else if (instruction == "biologycount") {
            std::cin >> instructionNum;
            std::cout << countStudentsWithGradeAbove(sortedData[0], instructionNum, 0)
                      << " students have a grade above that" << std::endl;
        } else if (instruction == "mathscount") {
            std::cin >> instructionNum;
            std::cout << countStudentsWithGradeAbove(sortedData[1], instructionNum, 1)
                      << " students have a grade above that" << std::endl;
        } else if (instruction == "chemistrycount") {
            std::cin >> instructionNum;
            std::cout << countStudentsWithGradeAbove(sortedData[2], instructionNum, 2)
                      << " students have a grade above that" << std::endl;
        } else if (instruction == "physicscount") {
            std::cin >> instructionNum;
            std::cout << countStudentsWithGradeAbove(sortedData[2], instructionNum, 3)
                      << " students have a grade above that" << std::endl;
        } else if (instruction == "totalcount") {
            std::cin >> instructionNum;
            std::cout << countStudentsWithGradeAbove(sortedData[3], instructionNum, 4)
                      << " students have a grade above that" << std::endl;
        } else if (instruction == "find") {
            std::cin >> instruction;
            auto student = studentData.find(instruction);
            if (student == studentData.end()) {
                studentData[instruction] = std::array<int, 5>();
                student = studentData.find(instruction);
            }
            printStudent(student);
        } else if (instruction == "exit") {
            break;
        } else {
            std::cout << "Sorry, that's an unrecognised command" << std::endl;
            std::cout << "you entered: " << instruction << std::endl;
        }
    }
}

#pragma clang diagnostic pop