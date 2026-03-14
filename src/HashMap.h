#pragma once
#include <array>

#include "dictionary.h"
#include <fstream>
#include <iostream>
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

    unsigned long long hash(const std::string& en)
    {
        unsigned long hash = 5381;

        for (const char c : en) {
            hash = ((hash << 5) + hash) + c;
        }

        return hash;
    }
public:
    HashMap() {
        loadData();
    }
    ~HashMap();
    std::vector<std::string> getValues(std::string line) {
        std::vector<std::string> r = {};
        std::string curr = "";
        line += ",";
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
            success = insert(values[0], values[1], std::stoull(values[2]));
            if (!success) {
                std::cout << "Error with line: " << line << std::endl;
            }
        }
        return 1;

    }

    bool insert(std::string en, std::string trans, unsigned long long freq) override {
        //modulus by M **
        int loc = hash(en) % table.size(); // ensure within bounds

        for (int i = 0; i < table.size(); ++i) {
            int index = (loc + i) % table.size();

            if (table[index].en == "") {
                //Empty slot
                table[index].en = en;
                table[index].translation = trans;
                table[index].freq = freq;
                return true;
            }

            if (table[index].en == en) {
                //key matchhes
                table[index].translation = trans;
                return true;
            }
            //else move to next spot

        }
    }
    std::string search(std::string en) override {
        int loc = hash(en) % table.size(); // ensure within bounds

        for (int i = 0; i < table.size(); ++i) {
            int index = (loc + i) % table.size();

            if (table[index].en == en) {
                //key matches
                return table[index].translation;
            }

        }
        return "Not found in table";
    }
    //std::prefix_search

};


