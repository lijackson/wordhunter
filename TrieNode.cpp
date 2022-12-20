#include "includes/TrieNode.hpp"

int TrieNode::trie_size(TrieNode* n) {
    if (!n)
        return 0;
    int sze = sizeof(*n);
    for (auto const &l : n->letters)
        sze += TrieNode::trie_size(l.second);
    return sze;
}

TrieNode TrieNode::load_trie_dict(std::string file) {
    
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