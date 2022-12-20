#pragma once

#include <string>
#include <vector>
#include <iostream>

class BoardIO {
  public:
    std::string get_input();
    std::vector<std::vector<char> > generate_board(std::string inp);

};