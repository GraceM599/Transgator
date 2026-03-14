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
    //takes string and returns the hash as a large number
    unsigned long long hash(const std::string& en);

public:
    //creates object and loads data into the map from dictionary.csv
    HashMap();
    ~HashMap();
    //splits a csv line into english word, spanish word, and frequency then returns in a vector
    std::vector<std::string> getValues(std::string line);
    //loads each line of csv into the object by inserting the word, translation, and freq
    bool loadData();

    //inserts english, translation, and freq as an entry in the hash table
    bool insert(std::string en, std::string trans, unsigned long long freq) override;
    //returns translation of english word through hash table lookup
    //returns "Not found!" if english word is not in dictionary
    std::string search(std::string en) override;
    //returns the Slot that the key en is stored at.
    Slot search_Slot(std::string en);

    //returns a vector of <en, translation> pairs with a maximum size of 10 sorted by the frequency the english word is used
    std::vector<std::tuple<std::string, std::string>> prefixSearch(std::string en);

    //returns the elapsed time of a specified function
    // "constructor" -> hash map creation time
    // "word search" -> single word translation time
    // "prefix search" -> prefix search translation time
    double getFunctionTime(std::string func, std::string word = "");

};


