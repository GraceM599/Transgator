//
// Created by mihir on 3/5/2026.
//

#include "Trie.h"


std::vector<std::string> getValues(std::string line) {
    line  = line + ",";
    std::vector<std::string> r = {};
    std::string curr = "";
    for (int i = 0; i<line.size(); ++i) {
        if (line[i] == ',') {
            r.push_back(curr);
        }
    }
    return r;
}
bool Trie::loadData() {
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

bool Trie::insert(std::string key, std::string value, unsigned long long usage){
    int index = 0;
    if (key.size() == 0) {
        return false;
    }
    if (!root) {
        root = new TrieNode();
    }
    TrieNode* currentNode = root;
    for (char c : key) {
        char c2 = std::tolower(c);
        index = c2 - 'a';
        if (!currentNode->children[index]) {
            currentNode->children[index] = new TrieNode();
            currentNode = currentNode->children[index];
        }
        else {
            currentNode = currentNode->children[index];
        }
    }
    currentNode->isEnd = true;
    currentNode->conversion = value;
    currentNode->frequency = usage;
    return true;

}

std::string Trie::search(std::string key) {
    TrieNode* currentNode = root;
    for (char c : key) {
        char c2 = std::tolower(c);
        int index = c2 - 'a';
        if (!currentNode->children[index]) {
            return "not found";
        }
        else{
            currentNode = currentNode->children[index];
        }
    }
    if (currentNode->isEnd) {
        return currentNode->conversion;
    }
    else {
        return "not found";
    }
}

Trie::~Trie() {
    delete root;
}

std::vector<std::tuple<std::string, std::string, unsigned long long>> Trie::prefixSearch(std::string key) {
    std::vector<std::tuple<std::string, std::string, unsigned long long>> result;
    int index = 0;
    if (key.size() == 0) {
        return result;
    }
    TrieNode* currentNode = root;
    for (char c : key) {
        char c2 = std::tolower(c);
        index = c2 - 'a';
        if (!currentNode->children[index]) {
            currentNode->children[index] = new TrieNode();
            currentNode = currentNode->children[index];
        }
        else {
            currentNode = currentNode->children[index];
        }
    }
    return result;
}