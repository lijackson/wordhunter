#include "includes/Boggler.hpp"
#include <iostream>

int main() {
    std::cout << "Starting Boggle Board Analysis..." << std::endl;
    std::cout << "Generating 1,000,000 random boards and analyzing word frequencies." << std::endl;
    std::cout << std::endl;
    
    Boggler boggler;
    boggler.analysis();
    
    std::cout << std::endl << "Analysis complete!" << std::endl;
    
    return 0;
}
