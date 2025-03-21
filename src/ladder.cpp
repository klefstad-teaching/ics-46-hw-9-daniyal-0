#include "ladder.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cctype>

void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg 
         << " (word1: '" << word1 
         << "', word2: '" << word2 << "')" 
         << endl;
}

static vector<string> generate_neighbors(const string &word, 
                                         const set<string> &word_list)
{
    vector<string> result;
    for (int i = 0; i < (int)word.size(); ++i) {
        char original = word[i];
        for (char c = 'a'; c <= 'z'; ++c) {
            if (c == original) continue;
            string new_word = word;
            new_word[i] = c;
            if (word_list.count(new_word)) {
                result.push_back(new_word);
            }
        }
    }

    for (int i = 0; i <= (int)word.size(); ++i) {
        for (char c = 'a'; c <= 'z'; ++c) {
            string new_word = word.substr(0,i) + c + word.substr(i);
            if (word_list.count(new_word)) {
                result.push_back(new_word);
            }
        }
    }

    if (!word.empty()) {
        for (int i = 0; i < (int)word.size(); ++i) {
            string new_word = word.substr(0,i) + word.substr(i+1);
            if (word_list.count(new_word)) {
                result.push_back(new_word);
            }
        }
    }

    return result;
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d)
{
    if (std::abs((int)str1.size() - (int)str2.size()) > d)
        return false;

    int m = str1.size(), n = str2.size();
    std::vector<int> dp(n+1);

    for (int j = 0; j <= n; j++)
        dp[j] = j;

    for (int i = 1; i <= m; i++) {
        int prev = dp[0];
        dp[0] = i;
        int row_min = dp[0];

        for (int j = 1; j <= n; j++) {
            int temp = dp[j];
            int cost = (str1[i-1] == str2[j-1]) ? 0 : 1;
            dp[j] = std::min(std::min(prev + cost, dp[j] + 1), dp[j-1] + 1);

            if (dp[j] < row_min)
                row_min = dp[j];
            prev = temp;
        }

        if (row_min > d)
            return false;
    }
    return dp[n] <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word,
                                    const string& end_word,
                                    const set<string>& word_list)
{
    if (begin_word == end_word) {
        error(begin_word, end_word, "Start and end words are identical.");
        return {};
    }
    
    if (!word_list.count(end_word)) {
        error(begin_word, end_word, "End word not in dictionary.");
        return {};
    }

    queue<vector<string>> q;
    set<string> seen;
    vector<string> init;
    init.push_back(begin_word);
    q.push(init);
    seen.insert(begin_word);

    while (!q.empty()) {
        vector<string> curr = q.front();
        q.pop();
        string last = curr.back();
        vector<string> nbrs = generate_neighbors(last, word_list);
        std::sort(nbrs.begin(), nbrs.end());

        for (size_t i = 0; i < nbrs.size(); i++) {
            if (!seen.count(nbrs[i])) {
                seen.insert(nbrs[i]);
                vector<string> new_path = curr;
                new_path.push_back(nbrs[i]);
                if (nbrs[i] == end_word)
                    return new_path;
                q.push(new_path);
            }
        }
    }
    return {};
}

void load_words(set<string> & word_list, const string& file_name)
{
    ifstream in(file_name);
    if (!in.is_open()) {
        throw runtime_error("Could not open " + file_name);
    }
    string word;
    while (in >> word) {
        for (char &c : word) {
            c = static_cast<char>(tolower(c));
        }
        word_list.insert(word);
    }
    in.close();
}

void print_word_ladder(const vector<string>& ladder)
{
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    cout << "Word ladder found: ";
    for (auto &word : ladder) {
        cout << word << " ";
    }
    cout << endl;
}

static void my_assert(bool condition, const string& test_name)
{
    cout << test_name << (condition ? " passed" : " failed") << endl;
}

void verify_word_ladder()
{
    set<string> word_list;
    load_words(word_list, "src/words.txt");

    {
        vector<string> result = generate_word_ladder("cat", "dog", word_list);
        my_assert(result.size() == 4, "generate_word_ladder(\"cat\", \"dog\")");
    }
    {
        vector<string> result = generate_word_ladder("marty", "curls", word_list);
        my_assert(result.size() == 6, "generate_word_ladder(\"marty\", \"curls\")");
    }
    {
        vector<string> result = generate_word_ladder("code", "data", word_list);
        my_assert(result.size() == 6, "generate_word_ladder(\"code\", \"data\")");
    }
    {
        vector<string> result = generate_word_ladder("work", "play", word_list);
        my_assert(result.size() == 6, "generate_word_ladder(\"work\", \"play\")");
    }
    {
        vector<string> result = generate_word_ladder("sleep", "awake", word_list);
        my_assert(result.size() == 8, "generate_word_ladder(\"sleep\", \"awake\")");
    }
    {
        vector<string> result = generate_word_ladder("car", "cheat", word_list);
        my_assert(result.size() == 4, "generate_word_ladder(\"car\", \"cheat\")");
    }

    cout << "\nverify_word_ladder() tests completed.\n";
}