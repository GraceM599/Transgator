//
// Created by mihir on 3/5/2026.
//
#include <fstream>
#include <iostream>
#include <vector>

#include "dictionary.h"

class Trie : dictionary {
    struct TrieNode {
        bool isEnd;
        TrieNode* children[26];
        std::string conversion;
        std::string word;
        unsigned long long frequency;
        TrieNode() : isEnd(false), conversion("") {
            for (int i = 0; i < 26; i++) {
                children[i] = nullptr;
            }
        }
    };
public:
    TrieNode* root = nullptr;
    Trie(){
        loadData();
    };
    ~Trie();
    bool insert(std::string key, std::string value, unsigned long long usage);
    std::string search(std::string key);
    std::vector<std::tuple<std::string, std::string>> prefixSearch(std::string key);
    bool loadData();
    void prefixSearchHelper(TrieNode* start, std::vector<std::tuple<std::string, std::string, unsigned long long>>& result, int count);


};


