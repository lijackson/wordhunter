#pragma once

#include <vector>
#include <unordered_set>
#include <stack>
#include <iostream>

#include "TrieNode.hpp"

class WordGraph {
  private:
    int mask(int r, int c);

  public:

    std::vector<std::vector<char> > board;

    WordGraph(std::vector<std::vector<char> > _board);

    std::vector<std::string> word_search(TrieNode* head);
};

// I don't know what the wordhunt score is for words longer than 9
int score_word(std::string w);
int calc_max_score(std::vector<std::string> words);