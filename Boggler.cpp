#include "includes/Boggler.hpp"
#include "includes/WordGraph.hpp"
#include "includes/TrieNode.hpp"
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <algorithm>
#include <map>
#include <iomanip>

// Standard Boggle dice
// Each inner vector represents the 6 faces of a die
static const char BOGGLE_DICE[16][6] = {
    {'A', 'A', 'E', 'E', 'G', 'N'},
    {'A', 'B', 'B', 'J', 'O', 'O'},
    {'A', 'C', 'H', 'O', 'P', 'S'},
    {'A', 'F', 'F', 'K', 'P', 'S'},
    {'A', 'O', 'O', 'T', 'T', 'W'},
    {'C', 'I', 'M', 'O', 'T', 'U'},
    {'D', 'E', 'I', 'L', 'R', 'X'},
    {'D', 'E', 'L', 'R', 'V', 'Y'},
    {'D', 'I', 'S', 'T', 'T', 'Y'},
    {'E', 'E', 'G', 'H', 'N', 'W'},
    {'E', 'E', 'I', 'N', 'S', 'U'},
    {'E', 'H', 'R', 'T', 'V', 'W'},
    {'E', 'I', 'O', 'S', 'S', 'T'},
    {'E', 'L', 'R', 'T', 'T', 'Y'},
    {'H', 'I', 'M', 'N', 'U', 'Q'},
    {'H', 'L', 'N', 'N', 'R', 'Z'}
};

std::vector<std::vector<char> > Boggler::generate_board() {
    std::vector<std::vector<char> > board(4, std::vector<char>(4));

    // Create a shuffled order of dice indices
    std::vector<int> dice_order;
    for (int i = 0; i < 16; i++) {
        dice_order.push_back(i);
    }
    std::random_shuffle(dice_order.begin(), dice_order.end());

    // Place dice in shuffled order on the board
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int position = i * 4 + j;
            int die_index = dice_order[position];
            board[i][j] = get_random_letter(die_index);
        }
    }

    return board;
}

char Boggler::get_random_letter(int die_index) {
    if (die_index < 0 || die_index >= 16) {
        return 'A';
    }

    int random_face = rand() % 6;
    return BOGGLE_DICE[die_index][random_face];
}

void Boggler::analysis() {
    const int NUM_BOARDS = 1000000;
    
    // Load dictionary
    TrieNode head = TrieNode::load_trie_dict("resources/scrabble_dict.txt");
    
    // Map to store word frequencies: word -> count
    std::map<std::string, int> word_counts;
    
    // Generate boards and find words
    for (int b = 0; b < NUM_BOARDS; b++) {
        std::vector<std::vector<char> > board = generate_board();
        
        // Create WordGraph and search for words
        WordGraph wg(board, false);
        std::vector<std::string> words = wg.word_search(&head);
        
        // Track each word found
        for (const std::string& word : words) {
            word_counts[word]++;
        }
        
        // Progress indicator
        if ((b + 1) % 100000 == 0) {
            std::cout << "Processed " << (b + 1) << " boards..." << std::endl;
        }
    }
    
    // Group words by length
    std::map<int, std::vector<std::pair<std::string, double> > > words_by_length;
    
    for (const auto& entry : word_counts) {
        const std::string& word = entry.first;
        int count = entry.second;
        double percentage = (count * 100.0) / NUM_BOARDS;
        int length = word.length();
        words_by_length[length].push_back(std::make_pair(word, percentage));
    }
    
    // Sort each length group by percentage (descending) and display top 100
    for (auto& length_group : words_by_length) {
        int length = length_group.first;
        std::vector<std::pair<std::string, double> >& words = length_group.second;
        
        // Sort by percentage descending
        std::sort(words.begin(), words.end(), 
            [](const std::pair<std::string, double>& a, 
               const std::pair<std::string, double>& b) {
                return a.second > b.second;
            });
        
        std::cout << "\n=== Words of Length " << length << " ===" << std::endl;
        std::cout << std::left << std::setw(20) << "Word" << std::right << std::setw(10) << "Percentage" << std::endl;
        std::cout << std::string(30, '-') << std::endl;
        
        int count = 0;
        for (const auto& word_pair : words) {
            if (count >= 100) break;
            std::cout << std::left << std::setw(20) << word_pair.first 
                      << std::right << std::setw(9) << std::fixed << std::setprecision(4) 
                      << word_pair.second << "%" << std::endl;
            count++;
        }
    }
}
