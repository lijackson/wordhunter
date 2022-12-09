#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <stack>
#include <bits/stdc++.h>
#include <chrono>

class TrieNode {
  public:
    std::unordered_map<char, TrieNode*> letters;
    TrieNode* parent;
    char val;
    bool end;

    static void add_word(TrieNode* head, std::string word) {
        for (char c : word) {
            if (!head->letters.count(c))
                head->letters[c] = new TrieNode(head, c);
            head = head->letters[c];
        }
        head->end = true;
    }

    static std::string recreate(TrieNode* t) {
        std::string ans;
        while (t->parent != NULL) {
            ans += t->val;
            t = t->parent;
        }
        std::reverse(ans.begin(), ans.end());
        return ans;
    }

    TrieNode* get(char c) {
        if (!letters.count(c))
            return NULL;
        return letters[c];
    }

    TrieNode(TrieNode* p, char c) {
        end = false;
        parent = p;
        val = c;
    }

    TrieNode() {
        end = false;
        parent = NULL;
        val = '~';
    }

    ~TrieNode() {
        for (const auto &item : letters)
            delete letters[item.first];
    }
};

class Graph {
  private:
    int mask(int r, int c) {
        int m = 1 << (r*4 + c);
        return m;
    }

  public:

    std::vector<std::vector<char> > board;

    Graph(std::vector<std::vector<char> > _board) {
        for (int r = 0; r < 4; r++) {
            board.push_back(std::vector<char>());
            for (char c : _board[r]) {
                board[r].push_back(c);
            };
        }
    }

    std::vector<std::string> word_search(TrieNode* head) {
        std::unordered_set<std::string> unique;
        std::vector<std::string> answers;
        std::stack<std::pair<int, TrieNode*> > search[4][4];

        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                search[r][c].push({mask(r,c), head->get(board[r][c])});
            }
        }

        bool empt = false;

        while (!empt) {
            empt = true;
            for (int r = 0; r < 4; r++) for (int c = 0; c < 4; c++) {
                while (!search[r][c].empty()) {
                
                    std::pair<int, TrieNode*> curr = search[r][c].top();
                    search[r][c].pop();

                    if (curr.second == NULL)
                        continue;

                    if (curr.second->end) {
                        std::string ans = TrieNode::recreate(curr.second);
                        if (!unique.count(ans)) {
                            unique.insert(ans);
                            answers.push_back(ans);
                        }
                    }

                    for (int dx : {-1, 0, 1}) for (int dy : {-1, 0, 1}) {
                        int nr = r+dy;
                        int nc = c+dx;

                        if (nr < 0 || nr >= 4 || nc < 0 || nc >= 4 || (mask(nr,nc) & curr.first))
                            continue;

                        search[nr][nc].push({curr.first | mask(nr,nc), curr.second->get(board[nr][nc])});
                        empt = false;
                    }
                }
            }
        }

        return answers;
    }
};

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

TrieNode load_trie_dict(std::string file) {
    
    TrieNode head = TrieNode();

    std::fstream myfile("scrabble_dict.txt");
 
    if (!myfile.is_open())
        exit(1);
    
    std::string mystr = "";

    while (myfile.good()) {
        myfile >> mystr;
        TrieNode::add_word(&head, mystr);
        mystr = "";
    }

    return head;
}

int main() {
    // Load dictionary
    auto start = std::chrono::high_resolution_clock::now();
    TrieNode head = load_trie_dict("scrabble_dict.txt");
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
    Graph g(board);

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