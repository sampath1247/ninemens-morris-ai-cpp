#include <iostream>
#include <string>
#include <cassert>
#include <cstdint>
#include "classboard.h"  // Include the header file for Board class



void testStringConversion() {
    Board board;
    std::string input = "xxxxxxxxxWxxxxxxBxxxxxx";
    board.loadFromString(input);

    // Verify part1 and part2 in binary
    uint32_t expected_part1 = 
        (0b01 << 18);  // 'B' at position 15
        
          //std::cout<<board.getPart2();

    uint32_t expected_part2 = (0b10 << 0); // Positions 16-22 are 'x'

    assert(board.getPart1() == expected_part1);
    assert(board.getPart2() == expected_part2);
    //std::cout<<board.toString();
     assert(board.toString() == input);
    std::cout << "Test 1 Passed: String conversion\n";
}


void testPositions() {
    Board board;
    board.setPosition(0, 'W');   // Part1, position 0
        //std::cout<<board.toString();

    board.setPosition(15, 'B');  // Part1, position 15
        //std::cout<<board.toString();

    board.setPosition(16, 'W');  // Part2, position 16
    board.setPosition(22, 'B');  // Part2, position 22

    assert(board.getPart1() == (0b01 << 0 | 0b10 << 30));
    assert(board.getPart2() == (0b01 << 0 | 0b10 << 12));
    assert(board.toString() == "WxxxxxxxxxxxxxxBWxxxxxB");
    std::cout << "Test 2 Passed:positions\n";
}

int main() {
     testStringConversion();
    testPositions();
    Board board;

    // Load board state from a string
    std::string input = "xxxxxxxxxWxxxxxxBxxxxxx";
    board.loadFromString(input);
    //std::cout<<board.getPart1()<<"\n";
    
    //assert(board.getPart1() == (0b01 << 18 | 0b10 << 30));

    std::cout << "All tests passed!\n";
    return 0;
}