#pragma once

#include <vector>
#include <unordered_set>
#include <stack>

#include "TrieNode.hpp"

class WordGraph {
  private:
    int mask(int r, int c);

  public:

    std::vector<std::vector<char> > board;

    WordGraph(std::vector<std::vector<char> > _board);

    std::vector<std::string> word_search(TrieNode* head);
};
