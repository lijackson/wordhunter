#pragma once

#include <array>
#include <random>
#include <string>
#include <vector>

class Boggler {
public:
    /**
     * Generates a random 4x4 Boggle board.
     * Uses standard Boggle dice distribution with shuffled positions.
     *
     * @return A 4x4 vector of characters representing the Boggle board
     */
    std::vector<std::vector<char>> generate_board();

    /**
     * Analyzes word frequency across 1 million random Boggle boards.
     * Generates boards, finds all valid words, and calculates the percentage
     * of boards each word appears in. Displays top 100 words for each length.
     */
    void analysis();

private:
    /**
     * Gets a random letter from the standard Boggle die at the specified index.
     * Boggle has 16 dice, each with 6 faces.
     *
     * @param die_index The index of the die (0-15)
     * @return A random character from that die
     */
    char get_random_letter(int die_index);

    // Random number generator for better performance and quality
    std::mt19937 rng_{std::random_device{}()};
};

