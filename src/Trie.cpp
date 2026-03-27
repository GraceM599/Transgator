//
//

#include "Trie.h"
#include <string>
#include <algorithm>
#include <chrono>


std::vector<std::string> getValues(std::string line) { //helper function to get data from the csv that is seperated by commas
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
//load data is a function that is called in the constructor that parses through every row in the csv and inserts it into the tree
bool Trie::loadData() {
    std::fstream in;
    bool success = true;
    in.open("../../data/dictionary.csv");
    if (!in) {
        std::cout << "Issue opening dictionary.csv in Trie.cpp" << std::endl;
        return 0;
    }
    std::string line = "";
    while (std::getline(in, line)) {
        std::vector<std::string> values = getValues(line);

        try {
            unsigned long long freq = std::stoull(values[2]);
            success = insert(values[0], values[1], freq); //here is where it is added into the tree
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

bool Trie::insert(std::string key, std::string value, unsigned long long usage){ //insert appends a new word to the tree
                                                                                // only called in the constructor when the tree is made
    int index = 0;
    if (key.size() == 0) {
        return false;
    }
    if (!root) {
        root = new TrieNode(); //if this is the first letter create a new node
    }
    TrieNode* currentNode = root;
    for (char c : key) {
        char c2 = std::tolower(c);
        index = c2 - 'a'; //each char is converted to an index in the array that the node pointer holds through this math
        if (!currentNode->children[index]) {
            currentNode->children[index] = new TrieNode(); //if you reach a new letter where the value in the array at that index is nullptr activate the index by making it point to a new array of 26
            currentNode = currentNode->children[index];
        }
        else {
            currentNode = currentNode->children[index]; //if not keep traversing down the tree
        }
    }
    //once you reach the spot in the tree add all the information
    currentNode->isEnd = true;
    currentNode->word = key;
    currentNode->conversion = value;
    currentNode->frequency = usage;
    return true;

}

std::string Trie::search(std::string key) {
    TrieNode* currentNode = root;
    for (char c : key) { //loop through every char in the word
        char c2 = std::tolower(c);
        int index = c2 - 'a';
        if (!currentNode->children[index]) { //if the char doesn't exist in the order of the word we don't have the word in our dictionary
            return "not found";
        }
        else{
            currentNode = currentNode->children[index]; //keep traversing down the tree
        }
    }
    if (currentNode->isEnd) { //check that we are actually at the end of each word if we are return the conversion stored there
        currentNode->count++;
        return currentNode->conversion;
    }
    return "not found";
}

Trie::~Trie() {
    for (int i = 0; i < 26; i++) {
        if (root->children[i] != nullptr) {
            delete root->children[i]; //this line will call the child's destructor if the child is not nullptr
        }
    }
    delete root;
}

//helper comparator function is based on info found here: https://www.geeksforgeeks.org/cpp/sorting-vector-tuple-c-descending-order/
bool sortbyth(const std::tuple<std::string, std::string, unsigned long long>& a,const std::tuple<std::string, std::string, unsigned long long>& b) {
    return std::get<2>(a) > std::get<2>(b);
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
    //current node is now pointing to the aray of elements off the last letter the user typed in
    //here we want to go into a recursive depth first search - 3 letters deep
    prefixSearchHelper(currentNode, result, 0);
    std::sort(result.begin(),result.end(),sortbyth);

    if (result.size() > 5) {
        for (int i = 0; i < 5; i++) { //this may change to be larger or smaller than ten after talking to grace
            smallerResult.push_back(std::make_tuple(std::get<0>(result[i]), std::get<1>(result[i])));
        }
    }
    else {
        for (int i = 0; i < result.size(); i++) {
            smallerResult.push_back(std::make_tuple(std::get<0>(result[i]), std::get<1>(result[i])));
        }
    }

    return smallerResult;
}

unsigned long long adaptiveFormula(unsigned long long key, int count) {
    double a = 1.0; //this is what we will be multiplying frequency by
    double b = 5.0; //this is what we will be multiplying count by
    double normalizedCount = std::log(1+count); //I am choosing to normalize the count so it doesn't dominate as soon as one thing is disproportionatly searched for
    unsigned long long result = (a * key) + (b * normalizedCount);
    return result;
}
void Trie::prefixSearchHelper(Trie::TrieNode* start, std::vector<std::tuple<std::string, std::string, unsigned long long>>& result, int count) {
    //this is another base case - if going 3 down is a dead end then just return
    if (!start) {
        return;
    }

    if (start->isEnd) { //check if where we are at is a word if it is add it because we want any words with 0-3 chars added on
        //here we will go into a calculation on what frequency we want
        unsigned long long newFrequency = adaptiveFormula(start->frequency, start->count);
        result.push_back(std::make_tuple(start->word, start->conversion, newFrequency));
    }

    if (count == 3) { //we want to only go three down so break out of the recursive call here
        return;
    }
    for (int i = 0; i < 26; i++) {
        //this means for each letter we check
        if (start->children[i]) {
            prefixSearchHelper(start->children[i], result, count + 1);
        }
    }
}

std::string Trie::getFunctionTime(std::string function, std::string word) { //function to compare the time of the two data structures
    auto clock = std::chrono::high_resolution_clock::now();
    //check which of the 3 commands we are doing
    if (function == "constructor") {
        clock = std::chrono::high_resolution_clock::now();
        Trie t;
    }
    else if (function == "word search") {
        clock = std::chrono::high_resolution_clock::now();
        search(word);
    }
    else if (function == "prefix search") {
        clock = std::chrono::high_resolution_clock::now();
        prefixSearch(word);
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - clock;
    // convert the time to a string so that it is printable
    return std::to_string(diff.count());
    //comment here
}