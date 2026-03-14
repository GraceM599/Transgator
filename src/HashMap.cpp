//
//

#include "HashMap.h"


    unsigned long long HashMap::hash(const std::string& en)
    {
        unsigned long hash = 5381;

        for (const char c : en) {
            hash = ((hash << 5) + hash) + c;
        }

        return hash;
    }
    HashMap::HashMap() {
        loadData();
    }
    HashMap::~HashMap(){}
    std::vector<std::string> getValues(std::string line) {
        std::vector<std::string> r = {};
        std::string curr = "";
        line += ",";
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
    bool HashMap::loadData() {
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

    bool HashMap::insert(std::string en, std::string trans, unsigned long long freq)  {
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
    std::string HashMap::search(std::string en)  {
        int loc = hash(en) % table.size(); // ensure within bounds

        for (int i = 0; i < table.size(); ++i) {
            int index = (loc + i) % table.size();

            if (table[index].en == en) {
                //key matches
                return table[index].translation;
            }

        }
        return "Not found!";
    }
    Slot HashMap::search_Slot(std::string en) {
        int loc = hash(en) % table.size(); // ensure within bounds

        for (int i = 0; i < table.size(); ++i) {
            int index = (loc + i) % table.size();

            if (table[index].en == en) {
                //key matches
                return table[index];
            }

        }
        return Slot();
    }

    std::vector<std::tuple<std::string, std::string>> HashMap::prefixSearch(std::string en) {
        //take key and add 1-3 characters to the end to get all valid words up to 3 letters away from prefix
        //std::string search and add to final vector<tuple> if return != null
        std::vector<std::string> chars = {
            "", "a","b","c","d","e","f","g","h","i","j","k","l","m",
            "n","o","p","q","r","s","t","u","v","w","x","y","z"
        };
        std::string word;
        Slot t;
        auto f = [](const Slot& a, const Slot& b) {
            return a.freq < b.freq;
        };
        std::priority_queue<Slot, std::vector<Slot>, decltype(f)> total(f);
        std::unordered_set<std::string> seen;

        for (int i=0; i<chars.size(); ++i) {
            for (int j = 0; j<chars.size(); ++j) {
                for (int k = 0; k<chars.size(); ++k) {
                    word = en + chars[i] + chars[j] + chars[k];
                    if (seen.count(word)) {
                        continue;
                    }
                    seen.insert(word);
                    t = search_Slot(word);
                    if (t.freq != 0) {
                        total.push(t);
                    }
                }
            }
        }
        std::vector<std::tuple<std::string, std::string>> result;
        int limit = std::min(10, (int)total.size());
        for (int i=0; i<limit; ++i) {
            auto curr = total.top();
            total.pop();
            result.push_back({curr.en, curr.translation});
        }
        return result;
    }
    double HashMap::getFunctionTime(std::string func, std::string word) {
        auto start = std::chrono::high_resolution_clock::now();
        if (func == "constructor") {
            start = std::chrono::high_resolution_clock::now();
            HashMap();
        }
        if (func == "word search") {
            start = std::chrono::high_resolution_clock::now();
            search(word);

        }
        if (func == "prefix search") {
            start = std::chrono::high_resolution_clock::now();
            prefixSearch(word);
        }

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> diff = end - start;


        return diff.count();

    }


