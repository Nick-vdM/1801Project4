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
#include <limits>
#include <fstream>
#include <algorithm>
#include <iomanip>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

/*
 * ORDER OF NOTATION SOURCE:
 *  https://github.com/gibsjose/cpp-cheat-sheet/blob/master/Data%20Structures%20and%20Algorithms.md#15-map-stdmap-and-stdunordered_map
 * All notations for structures of structure complexities were taken from there
 *
 * =============================Structures & Justifications====================================
 * The task can be broken down into three main steps:
 *      1. Loading in the data
 *      2. Sorting the data
 *      3. Printing sorted data
 * Notice that 1 & 2 are seperate - sorting the data as its inserted is typically slower than sorting it with a
 * quick sort at the end.
 *      Containers decisions should be based around these tasks.
 * ======LOADING IN DATA========
 * While data is being initially loaded in, it does not have to be in any specific order. However, multiple
 * lines contain the same student, which means a student must be tested whether they already exist or not, and
 * if they do the grade can just get appended. As such, using a hash on the name would be the fastest way to
 * access this, as finding a student name is O(c) with that - meaning a type of map should be used.
 *      The other two properties that can be added is making it unordered and/or a multimap. First of all, this data
 * does not need to be unordered, meaning grabbing the O(1) time from unordered_map is worthwhile. However, having
 * multiple elements with the same key is not useful even if it could speed up access times as we don't want redundant
 *  Getting around that would require extra implementations that is not worthwhile.
 *      An arguable downside of an unordered map is that the data gets scrambled after something is inserted. However,
 * this does not actually impact what the iterators point to and therefore it should be fine. Another negative thing is
 * that it uses up more memory than a map to store things, however, the insert and access time being O(1) is more
 * valuable.
 * ----------MAIN STORAGE CONTAINER: unordered_map : INSERT: O(1) ; O(1) ; O(1)
 * =====SORTING DATA============
 * Before deciding on the container, it is probably a better decision to pick the container based on
 * the algorithm begin used. Firstly, a few properties should be chosen:
 *      - The data size is not that big ( 200 elements per container at most ; 8 bytes per iterator
 *          = 8*200 = 1600 ~= 1.56 kilobytes per program)
 *      - The data will be completely scrambled - no sections will be partially sorted
 *      - Average time is the most important (because the sort will be ran six times)
 * The four main sorting algorithms that will be inspected are:
 *      ALGORITHM   |   AVERAGE TIME    |   WORST TIME  |   MEMORY      |   PREFERRED CONTAINER
 *      quick       |   n*log(n)        |   n^2         |   O(log(n))   |   Array
 *      Merge       |   n*log(n)        |   n*log(n)    |   O(n)        |   List
 *      Heap        |   n*log(n)        |   n*log(n)    |   O(1)        |   Heap
 *      Introsort   |   n*log(n)        |   n*log(n)    |   O(log(n))   |   Heap
 * While heap and introsort are both arguably better options than quick sort, using a heap structure will
 * add complexity due to the iterator requiring a more complicated access time. Additionally, O(n) storage for the
 * merge sort is an excessive amount - it'd likely be better to create a heap sort structure and tank the print speed.
 *      Quick sort is the most attractive option. It simplifies future printing steps, its average time matches everything
 * and its memory usage is fairly light (log(1600) ~= 10.6 bytes). The main downside being faced is the worst time,
 * however, the focus here is the average time as it will have to be sorted several times.
 * --------Algorithm: Quick sort
 * Note however, if the size is being based off of the size of the data, it requires a constexpr to initialise the
 * size of an array. As such, a vector will have to be used that gets forced into a specific size.
 * --------Data structures: Vectors
 */

//Instead of declaring this over and over just use this
//Using size_t to automatically set the optimal size
typedef std::unordered_map<std::string, std::array<size_t, 5>> mainDataStruct;

//Instead of making a switch statement for deciding which subject something is, use a map
std::map<std::string, size_t> const subjectIndex(
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
    std::ifstream ifs("data.txt");
    //Until end of input
    while (ifs.peek() != std::char_traits<char>::eof()) {
        ifs >> studentName >> subject >> grade;
        auto iterator = studentData.find(studentName);
        if (iterator != studentData.end()) {
            iterator->second[subjectIndex.at(subject)] = std::stoi(grade);
        } else {
            studentData[studentName] = std::array<size_t, 5>();
            studentData[studentName][subjectIndex.at(subject)] = std::stoi(grade);
        }
    }
    for (auto student = studentData.begin(); student != studentData.end(); student++) {
        student->second[4] = student->first[0] + student->first[1] + student->first[2] + student->first[3];
    }
}

class CompareKeys {
public:
    bool operator()(mainDataStruct::iterator &i, mainDataStruct::iterator &j) const {
        return i->first < j->first;
    }
};

class CompareIndex {
public:
    explicit CompareIndex(size_t index) : index(index) {}

    bool operator()(mainDataStruct::iterator &i, mainDataStruct::iterator &j) const {
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


void printDataUntil(std::vector<mainDataStruct::iterator> &toPrint, size_t value, size_t index) {
    for (auto i : toPrint) {
        if (i->second[index] < value) {
            break;
        }
        printStudent(i);
    }
}

int main() {
    //For debugging purposes use this:

    mainDataStruct studentData{};
    loadInData(studentData);
    size_t studentCount = studentData.size();

    //Structure order will be:
    // Biology : 0
    // Mathematics : 1
    // Chemistry: 2
    // Physics : 3
    // Total : 4
    // Names : 5
    //Putting all the subjects first such that they match with the main data
    std::array<std::vector<mainDataStruct::iterator>, 6> sortedData;
    //Prevent reallocation:
    for (auto i : sortedData) {
        i.reserve(studentCount);
    }

    //Fill everything with iterators
    for (auto i = studentData.begin(); i != studentData.end(); i++) {
        for (int j = 0; j < 6; j++) {
            sortedData[j].push_back(i);
        }
    }

    for (int i = 0; i < 5; i++) {
        CompareIndex comp(i);
        std::sort(sortedData[i].begin(), sortedData[i].end(), comp);
    }
    std::sort(sortedData[5].begin(), sortedData[5].end(), CompareKeys());

    std::string instruction;
    size_t instructionNum;
    while (true) {
        std::cout  << ">>>>";
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
        } else if (instruction == "find") {
            std::cin >> instruction;
            auto student = studentData.find(instruction);
            printStudent(student);
        } else if (instruction == "time") {

        } else if (instruction == "memory") {

        } else {
            std::cout << "Sorry, that's an unrecognised command" << std::endl;
            std::cout << "you entered: " << instruction << std::endl;
        }
    }
}

#pragma clang diagnostic pop