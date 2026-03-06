//
// Created by mihir on 3/5/2026.
//
#include <vector>

#include "dictionary.h"
struct Terminate {

};
struct Node {
    char val = 0;
    Node* next = nullptr;
    //Terminate terminate;
    bool term = 0;
    std::string translation = "";
};
class Trie : dictionary{

    std::vector<Node*> root;
public:
    Trie();
    ~Trie();
    bool insert(std::string en, std::string trans) override;
    std::string search(std::string en) override;
    //bool delete();
};


