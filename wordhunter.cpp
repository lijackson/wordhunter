#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <chrono>

#include "includes/WordGraph.hpp"
#include "includes/TrieNode.hpp"

// I don't know what the wordhunt score is for words longer than 9
int calc_max_score(std::vector<std::string> words) {
    int val = 0;
    std::unordered_map<int, int> value = {
        {3, 100},
        {4, 400},
        {5, 800},
        {6, 1400},
        {7, 1800},
        {8, 2200},
        {9, 2600},
    };

    for (std::string w : words)
        if (value.count(w.size()))
            val += value[w.size()];
        else if (w.size() > 9)
            std::cout << "Extra big word found: " << w << std::endl;
    
    return val;
}

int trie_size(TrieNode* n) {
    if (!n)
        return 0;
    int sze = sizeof(*n);
    for (auto const &l : n->letters)
        sze += trie_size(l.second);
    return sze;
}

TrieNode load_trie_dict(std::string file) {
    
    TrieNode head = TrieNode();

    std::fstream myfile(file);
 
    if (!myfile.is_open())
        exit(1);
    
    std::string mystr = "";

    while (myfile.good()) {
        myfile >> mystr;
        TrieNode::add_word(&head, mystr);
        mystr = "";
    }
    // std::cout << "Size of trie: " << trie_size(&head) << " bytes" << std::endl;

    return head;
}

int main() {
    // Load dictionary
    auto start = std::chrono::high_resolution_clock::now();
    TrieNode head = load_trie_dict("resources/scrabble_dict.txt");
    auto dict_end = std::chrono::high_resolution_clock::now();

    // Set up board
    std::vector<std::vector<char> > board = {
        {'S', 'T', 'R', 'L'},
        {'H', 'E', 'E', 'I'},
        {'S', 'A', 'T', 'H'},
        {'E', 'L', 'S', 'O'}
    };

    // Load board from input
    std::cout << "Input board state below" << std::endl;
    std::cout << "[four four-letter lines, not space-separated]" << std::endl;
    for (int i = 0; i < 4; i++) {
        std::string inp;
        std::cin >> inp;
        if (inp.size() != 4)
            exit(1);
        for (int j = 0; j < 4; j++)
            board[i][j] = toupper(inp[j]);
    }
    std::cout << std::endl;

    // Start calculation timer
    auto calc_start = std::chrono::high_resolution_clock::now();

    // Set up graph representation of board
    WordGraph g(board);

    // Calculate answers and sort them by length
    std::vector<std::string> answers = g.word_search(&head);
    std::sort(answers.begin(), answers.end(), [] (const std::string& a, const std::string& b) {
        return a.size() < b.size();
    });

    auto calc_end = std::chrono::high_resolution_clock::now();

    for (std::string s : answers)
        if (s.size() >= 3)
            std::cout << s << std::endl;
    
    int max_score = calc_max_score(answers);
    std::cout << "Max possible score: " << max_score << std::endl;

    double load_time = std::chrono::duration_cast<std::chrono::microseconds>(dict_end - start).count() / 1000.0;
    double find_time = std::chrono::duration_cast<std::chrono::microseconds>(calc_end - calc_start).count() / 1000.0;
    std::cout << "Time to load word dict: " << load_time << " ms" << std::endl;
    std::cout << "Time to find answers: " << find_time << " ms" << std::endl;
    
    return 0;
}