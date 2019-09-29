//
// Created by nick on 29/9/19.
//

#include <iostream>
#include <vector>
#include <forward_list>
#include <algorithm>
#include <random>

std::string subjects[4]{"Physics", "Biology", "Mathematics", "Chemistry"};


int main(int argc, char **argv) {
    std::forward_list<std::string> stuff;
    int max = std::stoi(argv[1]);
    for (int i = 0; i < max; i++) {
        for (int sub = 0; sub < 4; sub++) {
            std::string toAdd = std::to_string(i) + " " + subjects[sub] + " " + std::to_string(rand() % 100);
            stuff.push_front(toAdd);
        }
    }

    std::vector<std::forward_list<std::string>::iterator> toScramble;
    for (auto iter = stuff.begin(); iter != stuff.end();iter++){
       toScramble.push_back(iter);
    }
    auto rng = std::default_random_engine {}; 
    std::shuffle(toScramble.begin(), toScramble.end(), rng);

    for(auto i : toScramble){
        std::cout << *i << std::endl;
    }
    return 0;

}
