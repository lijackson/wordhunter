#include "includes/TrieNode.hpp"

void TrieNode::add_word(TrieNode* head, std::string word) {
    for (char c : word) {
        if (!head->letters.count(c))
            head->letters[c] = new TrieNode(head, c);
        head = head->letters[c];
    }
    head->end = true;
}

std::string TrieNode::recreate(TrieNode* t) {
    std::string ans;
    while (t->parent != NULL) {
        ans += t->val;
        t = t->parent;
    }
    std::reverse(ans.begin(), ans.end());
    return ans;
}

TrieNode* TrieNode::get(char c) {
    if (!letters.count(c))
        return NULL;
    return letters[c];
}

TrieNode::TrieNode(TrieNode* p, char c) {
    end = false;
    parent = p;
    val = c;
}

TrieNode::TrieNode() {
    end = false;
    parent = NULL;
    val = '~';
}

TrieNode::~TrieNode() {
    for (const auto &item : letters)
        delete letters[item.first];
}