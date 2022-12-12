#pragma once

#include <unordered_map>
#include <string>
#include <algorithm>

class TrieNode {
  public:
    std::unordered_map<char, TrieNode*> letters;
    TrieNode* parent;
    char val;
    bool end;

    static void add_word(TrieNode* head, std::string word);
    static std::string recreate(TrieNode* t);
    
    TrieNode* get(char c);

    TrieNode(TrieNode* p, char c);
    TrieNode();
    ~TrieNode();
};