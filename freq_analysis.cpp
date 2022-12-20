#include "includes/freq_analysis.hpp"

int main() {
    TrieNode head = TrieNode::load_trie_dict("resources/scrabble_dict.txt");

    std::fstream myfile("resources/boards.txt");
 
    if (!myfile.is_open())
        exit(1);
    
    std::vector<std::string> boards;
    while (myfile.good()) {
        std::string mystr = "";
        myfile >> mystr;
        boards.push_back(mystr);
    }

    std::vector<std::string> all_answers;

    BoardIO bio;
    for (std::string b : boards) {
        std::vector<std::vector<char> > board = bio.generate_board(b);
        WordGraph g(board);
        std::vector<std::string> answers = g.word_search(&head);

        for (std::string w : answers)
            all_answers.push_back(w);

        std::cout << b << " | best score : " << calc_max_score(answers) << std::endl;
    }

    int longest_pattern = 3;
    std::unordered_map<std::string, int> freq_dict;
    for (std::string w : all_answers) {
        int word_score = score_word(w);
        for (int length = 1; length < longest_pattern; length++) {
            for (int i = 0; i <= w.size() - length; i++) {
                std::string pattern;
                for (int o = 0; o < length; o++) {
                    pattern += w[i+o];
                }

                if (!freq_dict.count(pattern))
                    freq_dict[pattern] = 0;
                freq_dict[pattern] += word_score;
            }
        }
    }

    std::vector<std::string> pattern_list;

    for (const auto &b : freq_dict) {
        pattern_list.push_back(b.first);
    }

    std::sort(pattern_list.begin(), pattern_list.end(), [&freq_dict] (const std::string& a, const std::string& b) {
        return freq_dict[a] < freq_dict[b];
    });
    
    for (std::string w : pattern_list)
        std::cout << w << ": " << freq_dict[w] << std::endl;
}