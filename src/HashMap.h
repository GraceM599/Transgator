#pragma once
#include "dictionary.h"
#include <fstream>
#include <iostream>
#include <vector>

class HashMap : dictionary{

    int hash(std::string en);
public:
    HashMap() {
        loadData();
    }
    ~HashMap();
    std::vector<std::string> getValues(std::string line) {
        std::vector<std::string> r = {};
        std::string curr = "";
        for (int i = 0; i<line.size(); ++i) {
            if (line[i] == ',') {
                r.push_back(curr);
            }
        }
    }
    bool loadData() {
        std::fstream in;
        bool success = true;
        in.open("data/dictionary.csv");
        if (!in) {
            std::cout << "Issue openning dictionary.csv in HashMap.h" << std::endl;
            return 0;
        }
        std::string line = "";
        while (std::getline(in, line)) {
            std::vector<std::string> values = getValues(line);
            success = insert(values[0], values[1]);
            if (!success) {
                std::cout << "Error with line: " << line << std::endl;
            }
        }
        return 1;

    }
    bool insert(std::string en, std::string trans) override;
    std::string search(std::string en) override;
};


