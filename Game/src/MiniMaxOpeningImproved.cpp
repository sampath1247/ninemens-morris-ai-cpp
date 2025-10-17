#include <iostream>
#include <limits>
#include <vector>
#include <fstream>
#include <string>
#include "classboard.h" // Assumed header for Board class

using namespace std;

int positionsEvaluated = 0;



// Check if placing a piece at position 'idx' forms a mill
bool closeMill(int idx, const Board &b) {
    char C = b.getPosition(idx);
    if (C == 'x') return false;

    switch (idx) {
        case 0: return (b.getPosition(1) == C && b.getPosition(2) == C) ||   // a0-d0-g0
                       (b.getPosition(8) == C && b.getPosition(20) == C);    // a0-a3-a6
        case 1: return (b.getPosition(0) == C && b.getPosition(2) == C);     // d0-a0-g0
        case 2: return (b.getPosition(0) == C && b.getPosition(1) == C) ||   // g0-a0-d0
                       (b.getPosition(13) == C && b.getPosition(22) == C);   // g0-g3-g6
        case 3: return (b.getPosition(4) == C && b.getPosition(5) == C) ||   // b1-d1-f1
                       (b.getPosition(9) == C && b.getPosition(17) == C);    // b1-b3-b5
        case 4: return (b.getPosition(3) == C && b.getPosition(5) == C);     // d1-b1-f1
        case 5: return (b.getPosition(3) == C && b.getPosition(4) == C) ||   // f1-b1-d1
                       (b.getPosition(12) == C && b.getPosition(19) == C);   // f1-f3-f5
        case 6: return (b.getPosition(10) == C && b.getPosition(14) == C);   // c2-c3-c4
        case 7: return (b.getPosition(11) == C && b.getPosition(16) == C);   // e2-e3-e4
        case 8: return (b.getPosition(9) == C && b.getPosition(10) == C) ||  // a3-b3-c3
                       (b.getPosition(0) == C && b.getPosition(20) == C);    // a3-a0-a6
        case 9: return (b.getPosition(8) == C && b.getPosition(10) == C) ||  // b3-a3-c3
                       (b.getPosition(3) == C && b.getPosition(17) == C);    // b3-b1-b5
        case 10: return (b.getPosition(8) == C && b.getPosition(9) == C) ||  // c3-a3-b3
                        (b.getPosition(6) == C && b.getPosition(14) == C);   // c3-c2-c4
        case 11: return (b.getPosition(7) == C && b.getPosition(16) == C) || // e3-e2-e4
                        (b.getPosition(12) == C && b.getPosition(13) == C);  // e3-f3-g3
        case 12: return (b.getPosition(11) == C && b.getPosition(13) == C) ||// f3-e3-g3
                        (b.getPosition(5) == C && b.getPosition(19) == C);   // f3-f1-f5
        case 13: return (b.getPosition(11) == C && b.getPosition(12) == C) ||// g3-e3-f3
                        (b.getPosition(2) == C && b.getPosition(22) == C);   // g3-g0-g6
        case 14: return (b.getPosition(6) == C && b.getPosition(10) == C) || // c4-c2-c3
                        (b.getPosition(15) == C && b.getPosition(16) == C);  // c4-d4-e4
        case 15: return (b.getPosition(14) == C && b.getPosition(16) == C) ||// d4-c4-e4
                        (b.getPosition(18) == C && b.getPosition(21) == C);  // d4-d5-d6
        case 16: return (b.getPosition(14) == C && b.getPosition(15) == C) ||// e4-c4-d4
                        (b.getPosition(7) == C && b.getPosition(11) == C);   // e4-e2-e3
        case 17: return (b.getPosition(3) == C && b.getPosition(9) == C) ||  // b5-b1-b3
                        (b.getPosition(18) == C && b.getPosition(19) == C);  // b5-d5-f5
        case 18: return (b.getPosition(17) == C && b.getPosition(19) == C) ||// d5-b5-f5
                        (b.getPosition(15) == C && b.getPosition(21) == C);  // d5-d4-d6
        case 19: return (b.getPosition(5) == C && b.getPosition(12) == C) || // f5-f1-f3
                        (b.getPosition(17) == C && b.getPosition(18) == C);  // f5-b5-d5
        case 20: return (b.getPosition(0) == C && b.getPosition(8) == C) ||  // a6-a0-a3
                        (b.getPosition(21) == C && b.getPosition(22) == C);  // a6-d6-g6
        case 21: return (b.getPosition(20) == C && b.getPosition(22) == C) ||// d6-a6-g6
                        (b.getPosition(15) == C && b.getPosition(18) == C);  // d6-d4-d5
        case 22: return (b.getPosition(2) == C && b.getPosition(13) == C) || // g6-g0-g3
                        (b.getPosition(20) == C && b.getPosition(21) == C);  // g6-a6-d6
        default: return false;
    }
}

// Count mills for a given color
int countMills(const Board &b, char color) {
    int mills = 0;
    for (int i = 0; i < 23; ++i) {
        if (b.getPosition(i) == color && closeMill(i, b)) {
            mills++;
        }
    }
    return mills / 3; // Each mill is counted thrice (once per position), so divide by 3
}

// Generate boards by removing an opponent's piece after a mill
void GenerateRemove(const Board &board, vector<Board> &L, char opponent) {
    bool removed = false;
    for (int i = 0; i < 23; ++i) {
        if (board.getPosition(i) == opponent && !closeMill(i, board)) {
            Board b2 = board;
            b2.setPosition(i, 'x');
            L.push_back(b2);
            removed = true;
        }
    }
    if (!removed) { // If no non-mill pieces, remove any opponent piece
        for (int i = 0; i < 23; ++i) {
            if (board.getPosition(i) == opponent) {
                Board b2 = board;
                b2.setPosition(i, 'x');
                L.push_back(b2);
            }
        }
    }
}

// Generate possible moves by placing a piece
vector<Board> GenerateAdd(const Board &board, char color) {
    vector<Board> L;
    char opponent = (color == 'W') ? 'B' : 'W';
    for (int i = 0; i < 23; ++i) {
        if (board.getPosition(i) == 'x') {
            Board b = board;
            b.setPosition(i, color);
            if (closeMill(i, b)) {
                GenerateRemove(b, L, opponent);
            } else {
                L.push_back(b);
            }
        }
    }
    return L;
}

// Improved static estimation function
int staticEstimation(const Board &board) {
    int whitePieces = 0, blackPieces = 0;
    for (int i = 0; i < 23; ++i) {
        char p = board.getPosition(i);
        if (p == 'W') whitePieces++;
        else if (p == 'B') blackPieces++;
    }
    int whiteMills = countMills(board, 'W');
    int blackMills = countMills(board, 'B');
    int whiteMobility = GenerateAdd(board, 'W').size();
    int blackMobility = GenerateAdd(board, 'B').size();

    positionsEvaluated++;
    return (10 * (whitePieces - blackPieces)) + 
           (5 * (whiteMobility - blackMobility)) + 
           (15 * whiteMills - 10 * blackMills);
}

// Minimax algorithm (no Alpha-Beta pruning)
int minimax(const Board &board, int depth, bool isWhiteTurn) {
    if (depth == 0) {
        return staticEstimation(board);
    }
    char color = isWhiteTurn ? 'W' : 'B';
    vector<Board> moves = GenerateAdd(board, color);
    if (moves.empty()) {
        return staticEstimation(board);
    }
    if (isWhiteTurn) { // White maximizes
        int maxEval = numeric_limits<int>::min();
        for (const Board &move : moves) {
            int eval = minimax(move, depth - 1, false);
            maxEval = max(maxEval, eval);
        }
        return maxEval;
    } else { // Black minimizes
        int minEval = numeric_limits<int>::max();
        for (const Board &move : moves) {
            int eval = minimax(move, depth - 1, true);
            minEval = min(minEval, eval);
        }
        return minEval;
    }
}

// Find the best move for White
Board findBestMove(const Board &board, int depth) {
    int bestScore = numeric_limits<int>::min();
    Board bestMove = board;
    vector<Board> moves = GenerateAdd(board, 'W');
    if (moves.empty()) {
        cerr << "No valid moves available!" << endl;
        return board;
    }
    for (const Board &move : moves) {
        int score = minimax(move, depth - 1, false);
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    cout << "Positions evaluated by static estimation: " << positionsEvaluated << endl;
    cout << "MINIMAX estimate: " << bestScore << endl;
    return bestMove;
}

// File I/O functions
Board readBoardFromFile(const string &filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        cerr << "Error: Cannot open input file " << filename << endl;
        exit(1);
    }
    string s;
    if (!(in >> s) || s.length() != 23) {
        cerr << "Error: Invalid board string in " << filename << endl;
        exit(1);
    }
    Board b;
    b.loadFromString(s);
    in.close();
    return b;
}

void writeBoardToFile(const string &filename, const Board &b) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Error: Cannot open output file " << filename << endl;
        exit(1);
    }
    out << b.toString();
    out.close();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: MiniMaxOpeningImproved <infile> <outfile> <depth>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];
    int depth;
    try {
        depth = stoi(argv[3]);
        if (depth <= 0) throw invalid_argument("Depth must be positive");
    } catch (const exception &e) {
        cerr << "Error: Invalid depth value '" << argv[3] << "'" << endl;
        return 1;
    }
    Board board = readBoardFromFile(inputFile);
    positionsEvaluated = 0;
    Board best = findBestMove(board, depth);
    writeBoardToFile(outputFile, best);
    cout << "Input position:  " << board.toString() << endl;
    cout << "Output position: " << best.toString() << endl;
    return 0;
}