//
// Created by nick on 25/9/19.
//

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <cstring>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

/*
 * =============================Structures & Justifications====================================
 *      The overall data must be fast to access and light on storage. Compressing students such
 * that their subjects are all together is thus worthwhile as it simplifies future sorts;
 * it doesn't have to find the same student up to four times.
 * Secondly, indexing must be easy if there will be multiple ways of sorting the data. As a
 * school would logically gain students over time, making this structure dynamic could be
 * useful as well.
 *      Another valid decision could be using map, as 'names' have to be eventually sorted as well. However,
 * access by key in a map is O(log(n)). As the main purpose of this is to store the data efficiently and
 * be able to access it easily - its probably better to grab the faster access time and sort names on its own.
 *
 * While using the names as keys could be neat, the amount of data from storing their names
 * in all the sorted lists seems like an excessive amount of data. Instead, making a new
 * number and storing the names separately for faster access times and lighter storage overall. Note that
 * instead of using say an integer for the key type, using an auto type will lighten the data storage in small cases too
 *      Note as well, the grades will be stored in order of 'Physics, Mathematics, Chemistry, Biology and Total,' but this
 * doesn't actually affect anything.
 *
 * =================================MAIN STRUCTURE===================================
 * std::vector<std::pair<std::string, std::array<short, 5>>>;
 * ==================================================================================
 * INSERT: O(1)     -       ACCESS: O(1)        -       REMOVE: O(1)
 *
 * Given that it is a requirement for the data to be sorted in multiple ways, a light weight and fast to insert list is
 * preferred - leaving it between map and set, and since map uses key values, iterating could be slow if strings are used
 * and indexes would be redundant. Additionally, using a single set type makes inserts more complicated than necessary
 * as
 *
 * Note another contender would be a forward list as the data is always read from top to bottom, but there seems to
 * be no way to turn it into an associative container in STL's documentation.
 *
 * ===========================SORTED CATEGORIES======================================
 * std::multimap<std::string, short> names;
 * std::multimap<short, short> physicsGrades;
 * std::multimap<short, short> mathsGrades;
 * std::multimap<short, short> chemistryGrades;
 * std::multimap<short, short> biologyGrades;
 * std::multimap<short, short> totalGrades;
 * ==================================================================================
 * INSERT: O(log(n))     -       ACCESS O(log(n))     -       DELETION: O(log(n))
 */

class studentData {
public:
    void loadData() {
        //Each line consists of
        // studentName subect grade
        std::string studentName;
        std::string subject;
        std::string grade;
        //Until an end of file
        while (std::cin.peek() != std::char_traits<char>::eof()) {
            std::cin >> studentName >> subject >> grade;

            //Search for whether its already in vector
            short index = 0;
            index = findStudentByName(studentName);
            if (index != -1) {
                std::pair<std::string, std::array<short, 5>> newData;
                newData.first = studentName;
                // stos is undefined so use stoi and convert to short
                // short cast should be done automatically
                newData.second[findSubjectIndex(subject)] = static_cast<short>(std::stoi(grade));
                studentData.push_back(newData);
            } else {
                studentData[index].second[findSubjectIndex(subject)] = std::stoi(grade);
            }
        }

        //Do all the maths for the total grade
        for (auto row = studentData.begin(); row != studentData.end(); row++) {
            row->second[4] = row->second[0] + row->second[1] + row->second[2] + row->second[3];
        }
    }

    const std::multimap<std::string, short> &getSortedNames() const {
        return sortedNames;
    }

    const std::multimap<short, short> &getSortedPhysicsGrades() const {
        return sortedPhysicsGrades;
    }

    const std::multimap<short, short> &getSortedMathsGrades() const {
        return sortedMathsGrades;
    }

    const std::multimap<short, short> &getSortedChemistryGrades() const {
        return sortedChemistryGrades;
    }

    const std::multimap<short, short> &getSortedBiologyGrades() const {
        return sortedBiologyGrades;
    }

    const std::multimap<short, short> &getSortedTotalGrades() const {
        return sortedTotalGrades;
    }


    void printSortedNames() {
        printDataFromMap(sortedNames);
    }

    void printSortedPhysicsGrades() {
        printDataFromMap(sortedPhysicsGrades);
    }

    void printSortedPhysicsGradesUntil(short until) {
        printGradeDataUntil(sortedPhysicsGrades, until);
    }

    void printSortedMathsGrades() {
        printDataFromMap(sortedMathsGrades);
    }

    void printSortedMathsGradesUntil(short until) {
        printGradeDataUntil(sortedMathsGrades, until);
    }


    void printSortedChemistryGrades() {
        printDataFromMap(sortedChemistryGrades);
    }

    void printSortedChemistryGradesUntil(short until) {
        printGradeDataUntil(sortedChemistryGrades, until);
    }

    void printSortedBiologyGrades() {
        printDataFromMap(sortedBiologyGrades);
    }

    void printSortedTotalGradesUntil(short until) {
        printGradeDataUntil(sortedTotalGrades, until);
    }

    void printSortedTotalGrades() {
        printDataFromMap(sortedTotalGrades);
    }

    void printSortedBiologyGradesUntil(short until) {
        printGradeDataUntil(sortedBiologyGrades, until);
    }

private:
    short findStudentByName(std::string name) {
        //Returns the index if found, returns -1 if not found
        for (short i = 0; i < studentData.size(); i++) {
            if (studentData[i].first == name) {
                return i;
            }
        }
        return -1;
    }

    short findSubjectIndex(std::string subjectName) {
        if (subjectName == "Physics") {

        } else if (subjectName == "Mathematics") {

        } else if (subjectName == "Chemistry") {

        } else if (subjectName == "Biology") {

        } else {
            throw std::string("ERROR: Attempted to identify an unknown subject");
        }
    }

    // Not reusing these structures as it could use a fair bit of processing
    // since it would add one comparison each loop to printing everything
    void printGradeDataUntil(std::multimap<short, short> &data, short &until) const {
        for (auto item = data.begin(); item != data.end(); item++) {
            if (item->first < until) {
                break;
            }
            auto rowOfData = studentData[item->second];
            std::cout << "Name: " << rowOfData.first
                      << " Physics grade: " << rowOfData.second[0]
                      << " Mathematics grade: " << rowOfData.second[1]
                      << " Chemistry grade: " << rowOfData.second[2]
                      << " Biology grade: " << rowOfData.second[3]
                      << " Total grade: " << rowOfData.second[4]
                      << std::endl;
        }
    }

    template<typename T>
    void printDataFromMap(std::multimap<T, short> &data) const {
        for (auto item = data.begin(); item != data.end(); item++) {
            auto rowOfData = studentData[item->second];
            std::cout << "Name: " << rowOfData.first
                      << " Physics grade: " << rowOfData.second[0]
                      << " Mathematics grade: " << rowOfData.second[1]
                      << " Chemistry grade: " << rowOfData.second[2]
                      << " Biology grade: " << rowOfData.second[3]
                      << " Total grade: " << rowOfData.second[4]
                      << std::endl;
        }
    }

    std::vector<std::pair<std::string, std::array<short, 5>>> studentData;

    std::multimap<std::string, short> sortedNames;
    std::multimap<short, short> sortedPhysicsGrades;
    std::multimap<short, short> sortedMathsGrades;
    std::multimap<short, short> sortedChemistryGrades;
    std::multimap<short, short> sortedBiologyGrades;
    std::multimap<short, short> sortedTotalGrades;

};

int main() {
    studentData data{};

    data.loadData();
    std::string instruction;
    int until;
    while (true) {
        std::cin >> instruction;
        if (instruction == "names") {
            data.printSortedNames();
        } else if (instruction == "physics") {
            data.printSortedPhysicsGrades();
        } else if (instruction == "physicsUntil") {
            std::cin >> until;
            data.printSortedPhysicsGradesUntil(until);
        } else if (instruction == "maths") {
            data.printSortedMathsGrades();
        } else if (instruction == "mathsUntil") {
           std::cin >> until;
            data.printSortedMathsGradesUntil(until);
        } else if (instruction == "chemistry") {
            data.printSortedChemistryGrades();
        } else if (instruction == "chemistryUntil") {
            std::cin >> until;
            data.printSortedChemistryGradesUntil(until);
        } else if (instruction == "biology") {
            data.printSortedBiologyGrades();
        } else if (instruction == "biologyUntil") {
            std::cin >> until;
            data.printSortedPhysicsGradesUntil(until);
        } else if (instruction == "total") {
            data.printSortedTotalGrades();
        } else if (instruction == "totalUntil") {
            std::cin >> until;
            data.printSortedTotalGradesUntil(until);
        }
    }
}

#pragma clang diagnostic pop