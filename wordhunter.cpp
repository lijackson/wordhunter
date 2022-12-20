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
#include "includes/BoardIO.hpp"

int main() {
    // Load dictionary
    auto start = std::chrono::high_resolution_clock::now();
    TrieNode head = TrieNode::load_trie_dict("resources/scrabble_dict.txt");
    auto dict_end = std::chrono::high_resolution_clock::now();

    // Load board from input
    std::cout << "Input board state below" << std::endl;
    std::cout << "[four four-letter lines, not space-separated]" << std::endl;
    
    BoardIO bio;
    std::string board_str = bio.get_input();
    std::vector<std::vector<char> > board = bio.generate_board(board_str);

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