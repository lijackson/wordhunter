#include "includes/WordGraph.hpp"

int WordGraph::mask(int r, int c) {
    int m = 1 << (r*4 + c);
    return m;
}

WordGraph::WordGraph(std::vector<std::vector<char> > _board) {
    for (int r = 0; r < 4; r++) {
        board.push_back(std::vector<char>());
        for (char c : _board[r]) {
            board[r].push_back(c);
        };
    }
}

std::vector<std::string> WordGraph::word_search(TrieNode* head) {
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

// I don't know what the wordhunt score is for words longer than 9
int score_word(std::string w) {
    std::unordered_map<int, int> value = {
        {3, 100},
        {4, 400},
        {5, 800},
        {6, 1400},
        {7, 1800},
        {8, 2200},
        {9, 2600},
    };
    // if (w.size() > 9)
    //     std::cout << "Extra big word found: " << w << std::endl;

    return value.count(w.size()) ? value[w.size()] : 0;
}

int calc_max_score(std::vector<std::string> words) {
    int val = 0;

    for (std::string w : words)
        val += score_word(w);
    
    return val;
}