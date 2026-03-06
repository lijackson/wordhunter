#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <unordered_map>
#include <vector>
#include "includes/Boggler.hpp"
#include "includes/WordGraph.hpp"
#include "includes/TrieNode.hpp"

// Standard Boggle dice configuration
// Each die has 6 faces with letters
static constexpr std::array<std::array<char, 6>, 16> BOGGLE_DICE = {{
    {{'A', 'A', 'E', 'E', 'G', 'N'}},
    {{'A', 'B', 'B', 'J', 'O', 'O'}},
    {{'A', 'C', 'H', 'O', 'P', 'S'}},
    {{'A', 'F', 'F', 'K', 'P', 'S'}},
    {{'A', 'O', 'O', 'T', 'T', 'W'}},
    {{'C', 'I', 'M', 'O', 'T', 'U'}},
    {{'D', 'E', 'I', 'L', 'R', 'X'}},
    {{'D', 'E', 'L', 'R', 'V', 'Y'}},
    {{'D', 'I', 'S', 'T', 'T', 'Y'}},
    {{'E', 'E', 'G', 'H', 'N', 'W'}},
    {{'E', 'E', 'I', 'N', 'S', 'U'}},
    {{'E', 'H', 'R', 'T', 'V', 'W'}},
    {{'E', 'I', 'O', 'S', 'S', 'T'}},
    {{'E', 'L', 'R', 'T', 'T', 'Y'}},
    {{'H', 'I', 'M', 'N', 'U', 'Q'}},
    {{'H', 'L', 'N', 'N', 'R', 'Z'}}
}};

std::vector<std::vector<char>> Boggler::generate_board() {
    constexpr size_t BOARD_SIZE = 4;
    constexpr size_t TOTAL_DICE = 16;

    std::vector<std::vector<char>> board(BOARD_SIZE, std::vector<char>(BOARD_SIZE));

    // Create and shuffle dice order for randomization
    std::vector<size_t> dice_order(TOTAL_DICE);
    for (size_t i = 0; i < TOTAL_DICE; ++i) {
        dice_order[i] = i;
    }
    std::shuffle(dice_order.begin(), dice_order.end(), rng_);

    // Place dice in shuffled order on the board
    for (size_t row = 0; row < BOARD_SIZE; ++row) {
        for (size_t col = 0; col < BOARD_SIZE; ++col) {
            size_t board_index = row * BOARD_SIZE + col;
            size_t die_index = dice_order[board_index];
            board[row][col] = get_random_letter(die_index);
        }
    }

    return board;
}

char Boggler::get_random_letter(int die_index) {
    if (die_index < 0 || static_cast<size_t>(die_index) >= BOGGLE_DICE.size()) {
        return 'A';  // Default fallback
    }

    // Use uniform distribution for better randomness
    std::uniform_int_distribution<size_t> dist(0, 5);
    size_t random_face = dist(rng_);
    return BOGGLE_DICE[die_index][random_face];
}

void Boggler::analysis() {
    constexpr int NUM_BOARDS = 1000000;
    constexpr int PROGRESS_INTERVAL = 100000;

    std::cout << "Starting Boggle Board Analysis..." << std::endl;
    std::cout << "Generating " << NUM_BOARDS << " random boards and analyzing word frequencies." << std::endl;
    std::cout << std::endl;

    // Load dictionary once
    TrieNode head = TrieNode::load_trie_dict("resources/scrabble_dict.txt");

    // Use unordered_map for O(1) average lookup time instead of map's O(log n)
    // This significantly speeds up word counting with many unique words
    std::unordered_map<std::string, int> word_counts;

    // Generate boards and find words
    for (int board_num = 0; board_num < NUM_BOARDS; ++board_num) {
        auto board = generate_board();

        // Create WordGraph and search for words
        WordGraph wg(board, false);
        std::vector<std::string> words = wg.word_search(&head);

        // Track each word found - use range-based for loop for clarity
        for (const auto& word : words) {
            ++word_counts[word];
        }

        // Progress indicator
        if ((board_num + 1) % PROGRESS_INTERVAL == 0) {
            std::cout << "Processed " << (board_num + 1) << " boards..." << std::endl;
        }
    }

    // Group words by length using a map (ordered by length)
    std::map<size_t, std::vector<std::pair<std::string, double> > > words_by_length;

    // Calculate percentages
    for (const auto& entry : word_counts) {
        const std::string& word = entry.first;
        int count = entry.second;
        double percentage = (static_cast<double>(count) * 100.0) / NUM_BOARDS;
        size_t length = word.length();
        words_by_length[length].emplace_back(word, percentage);
    }

    // Sort each length group by percentage (descending) and display top 100
    for (auto& length_group : words_by_length) {
        size_t length = length_group.first;
        auto& words = length_group.second;

        // Sort by percentage descending using lambda
        std::sort(words.begin(), words.end(),
            [](const auto& a, const auto& b) {
                return a.second > b.second;
            });

        std::cout << "\n=== Words of Length " << length << " ===" << std::endl;
        std::cout << std::left << std::setw(20) << "Word"
                  << std::right << std::setw(10) << "Percentage" << std::endl;
        std::cout << std::string(30, '-') << std::endl;

        size_t count = 0;
        for (const auto& word_pair : words) {
            if (count >= 100) break;
            std::cout << std::left << std::setw(20) << word_pair.first
                      << std::right << std::setw(9) << std::fixed << std::setprecision(4)
                      << word_pair.second << "%" << std::endl;
            ++count;
        }
    }

    std::cout << std::endl << "Analysis complete!" << std::endl;
}
