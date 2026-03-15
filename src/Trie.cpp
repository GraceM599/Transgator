//
//

#include "Trie.h"


std::vector<std::string> getValues(std::string line) {
    line  = line + ",";
    std::vector<std::string> r = {};
    std::string curr = "";
    for (int i = 0; i<line.size(); ++i) {
        if (line[i] == ',') {
            r.push_back(curr);
            curr = "";
        }
        else {
            curr += line[i];
        }
    }
    return r;
}
bool Trie::loadData() {
    std::fstream in;
    bool success = true;
    in.open("../../data/dictionary.csv");
    if (!in) {
        std::cout << "Issue openning dictionary.csv in Trie.cpp" << std::endl;
        return 0;
    }
    std::string line = "";
    while (std::getline(in, line)) {
        std::vector<std::string> values = getValues(line);

        try {
            unsigned long long freq = std::stoull(values[2]);
            success = insert(values[0], values[1], freq);
        }
        catch (const std::invalid_argument& e) {
            //std::cout << "Invalid number: " << values[2] << std::endl;

            continue;
        }

        if (!success) {
            //std::cout << "Error with line: " << line << std::endl;
            continue;
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
    currentNode->word = key;
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
//helper comparator function is based on info found here: https://www.geeksforgeeks.org/cpp/sorting-vector-tuple-c-descending-order/
bool sortbyth(const std::tuple<std::string, std::string, int>& a, const std::tuple<std::string, std::string, int>& b){
    return (std::get<2>(a) > std::get<2>(b));
}
std::vector<std::tuple<std::string, std::string>> Trie::prefixSearch(std::string key) {
    std::vector<std::tuple<std::string, std::string, unsigned long long>> result;
    std::vector<std::tuple<std::string, std::string>> smallerResult;
    int index = 0;
    if (key.size() == 0) {
        return smallerResult;
    }
    //this next block of code is getting us to the point that the user typed in
    TrieNode* currentNode = root;
    for (char c : key) {
        char c2 = std::tolower(c);
        index = c2 - 'a';
        if (!currentNode->children[index]) {
            return smallerResult;
        }
        currentNode = currentNode->children[index];
    }
    //current node is now pointing to the aray of elements off the last leter the user typed in
    //here we want to go into a recursive depth first search - 3 letters deep
    prefixSearchHelper(currentNode, result, 0);
    // std::sort(result.begin(),result.end(),sortbyth);
    for (int i = 0; i < 10; i++) { //this may change to be larger or smaller than ten after talking to grace
        smallerResult.push_back(std::make_tuple(std::get<0>(result[i]), std::get<1>(result[i])));
    }
    return smallerResult;
}
void Trie::prefixSearchHelper(Trie::TrieNode* start, std::vector<std::tuple<std::string, std::string, unsigned long long>>& result, int count) {
    //this is another base case - if going 3 down is a dead end then just return
    if (!start) {
        return;
    }
    if (count == 3) { //this is the base case - we went three down are checking if its a word- if so we add it and return if not just return
        if (start->isEnd) {
            result.push_back(std::make_tuple(start->word, start->conversion, start->frequency));
        }
        return;
    }
    for (int i = 0; i < 26; i++) {
        //this means for each letter we check
        if (start->children[i]) {
            prefixSearchHelper(start->children[i], result, count + 1);
        }
    }
}