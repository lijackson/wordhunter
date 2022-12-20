#include "includes/BoardIO.hpp"

std::string BoardIO::get_input() {
    std::string b;
    while (b.size() < 16) {
        std::string inp;
        std::cin >> inp;
        b += inp;
    }

    if (b.size() > 16)
        exit(1);

    return b;
}

std::vector<std::vector<char> > BoardIO::generate_board(std::string inp) {
    if (inp.size() != 16)
        exit(1);

    std::vector<std::vector<char> > board = {
        {'D', 'E', 'A', 'D'},
        {'B', 'E', 'E', 'F'},
        {'C', 'A', 'F', 'E'},
        {'B', 'A', 'B', 'E'}
    };

    for (int i = 0; i < 4; i++) 
        for (int j = 0; j < 4; j++)
            board[i][j] = toupper(inp[i*4 + j]);

    return board;
}