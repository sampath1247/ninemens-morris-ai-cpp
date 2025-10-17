#ifndef CLASSBOARD_H
#define CLASSBOARD_H
#include <iostream>
#include <string>
#include <cassert>
#include <cstdint>


// Define BoardData structure
struct BoardData {
    uint32_t part1; // Positions 0-15 (32 bits)
    uint32_t part2; // Positions 16-22 (14 bits)
};

class Board {
private:
    BoardData data;  

    uint8_t encode(char c) const;
    char decode(uint8_t code) const;

public:
    Board();  // Constructor
    char getPosition(int index) const;
    void setPosition(int index, char value);
    void loadFromString(const std::string& s);
    std::string toString() const;

    // Getters for unit tests
    uint32_t getPart1() const;
    uint32_t getPart2() const;
};

#endif
