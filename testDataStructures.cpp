// C++ program to demonstrate the use of std::generate
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <stack>

// Defining the generator function
struct generator {
    generator() {
        srand(1);
    }

    int operator()() {
        return rand() % 1000;
    }
};

template<typename T>
double testSort(T &data) {
    clock_t startTime = clock();
    std::generate(data.begin(), data.end(), generator());
    std::sort(data.begin(), data.end());
    return double(clock() - startTime) / CLOCKS_PER_SEC;
}

int main() {
    std::vector<int> vec(1000);
    std::cout << "VECTOR: time: " << testSort(vec) << " size in bytes: "
              << sizeof(std::vector<int>) + (vec.size() * sizeof(int)) << std::endl;

    std::array<int, 1000> arr;
    std::cout << "ARRAY: time: " << testSort(arr) << " size in bytes: "
              << sizeof(std::vector<int>) + (arr.size() * sizeof(int)) << std::endl;

    int arr2[1000]{};
    std::cout << "ARRAY: time: " << testSort(arr2) ;

    return 0;
}
