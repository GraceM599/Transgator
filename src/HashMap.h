#pragma once
#include <array>
#include <chrono>

#include "dictionary.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <utility>
#include <vector>


struct Slot {
    std::string en;
    std::string translation;
    unsigned long long freq;
    Slot(std::string e, std::string t, unsigned long long f) {
        this->en = std::move(e);
        this->translation = std::move(t);
        this->freq = f;
    }
    Slot() {
        en = "";
        translation = "";
        freq = 0;
    }
};

//Used hash function djb2 from http://www.cse.yorku.ca/%7Eoz/hash.html
//Known to be good for strings
class HashMap : dictionary{
    std::array<Slot, 500000> table;

    unsigned long long hash(const std::string& en);

public:
    HashMap();
    ~HashMap();
    std::vector<std::string> getValues(std::string line);
    bool loadData();

    bool insert(std::string en, std::string trans, unsigned long long freq) override;
    std::string search(std::string en) override;
    Slot search_Slot(std::string en);

    std::vector<std::tuple<std::string, std::string>> prefixSearch(std::string en);

    double getFunctionTime(std::string func, std::string word = "");

};


