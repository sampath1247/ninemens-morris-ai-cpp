#include "classboard.h"  // Include the header file to declare Board



// Constructor to initialize the board data
Board::Board() : data{0, 0} {}


// Encode character ('x', 'W', 'B') to 2-bit value
uint8_t Board::encode(char c) const {
    uint8_t result;
    switch (c) {
        case 'x':
            result = 0b00;
            break;
        case 'W':
            result = 0b01;
            break;
        case 'B':
            result = 0b10;
            break;
        default:
            result = 0b00;  // Default case
    }
    // std::cout << "Encoding character: " << c << " -> " << std::bitset<2>(result) << std::endl;

return result;
}


// Decode 2-bit value back to character ('x', 'W', 'B')
char Board::decode(uint8_t code) const {
        switch (code) {
            case 0b00: return 'x';
            case 0b01: return 'W';
            case 0b10: return 'B';
            default:   return 'x';
        }
    }


    // Get character at position index
char Board::getPosition(int index) const {
        assert(index >= 0 && index < 23);
        if (index < 16) {
            int shift = index * 2;
            return decode((data.part1 >> shift) & 0b11); // Mask with 0b11
        } else {
            int shift = (index - 16) * 2;
            return decode((data.part2 >> shift) & 0b11);
        }
    }



 // Set character at position `index`
void Board::setPosition(int index, char value) {
        assert(index >= 0 && index < 23);
        uint8_t code = encode(value);
    // std::cout << "Encoded code: " << std::bitset<2>(code) << std::endl;
        if (index < 16) {
            int shift = index * 2;
            data.part1 &= ~(0b11 << shift); // Clear bits using binary mask
            //std::cout<<code<<"\n";
            data.part1 |= (code << shift);  // Set new value
              //std::cout<<std::bitset<32>(part1)<<"\n";
              
        } else {
            int shift = (index - 16) * 2;
            data.part2 &= ~(0b11 << shift);
            data.part2 |= (code << shift);
     //std::cout<<part2<<"\n";

        }
            //std::cout<<toString()<<"\n";

    }


// Load entire board from a string
void Board::loadFromString(const std::string& s) {
        for (int i = 0; i < 23; i++) {
            setPosition(i, s[i]);
            //std::cout<<s[i]<<"\n";
        }
    }


// Convert board to string
std::string Board::toString() const {
        std::string s;
        for (int i = 0; i < 23; i++) {
            s += getPosition(i);
        }
            //std::cout<<s<<"\n";

        return s;
    }


 // Getter for part1 of the board
uint32_t Board::getPart1() const {
    return data.part1;
}

// Getter for part2 of the board
uint32_t Board::getPart2() const {
    return data.part2;
}




