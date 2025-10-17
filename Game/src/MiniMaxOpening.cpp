#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <climits>
#include "classboard.h"
#include <unordered_map>
#include <algorithm>




using namespace std;
unordered_map<string, int> memo;


// Global counter for positions evaluated by static estimation
static int positionsEvaluated = 0;

// Forward declarations
bool closeMill(int idx, const Board &board);
void GenerateRemove(const Board &board, vector<Board> &L, char opponent);
vector<Board> GenerateAdd(const Board &board, char color);
vector<Board> generateMovesOpening(const Board &board, char color);
int evaluate(const Board &board);
int minimax(const Board &board, int depth, bool isWhiteTurn);
Board findBestMove(const Board &board, int depth);

// Static evaluation function: White pieces - Black pieces (for opening phase)
int evaluate(const Board &board) {
    int score = 0;
    for (int i = 0; i < 23; ++i) {
        char p = board.getPosition(i);
        if (p == 'W') score += 1;
        else if (p == 'B') score -= 1;
    }
    positionsEvaluated++;
    return score;   
}






int minimax(const Board &board, int depth, bool isWhiteTurn) {
    // Step 1: Check if the board has already been evaluated
    string boardState = board.toString();
    if (memo.find(boardState) != memo.end()) {
        return memo[boardState];  // Return the cached evaluation score
    }

    // Step 2: Base case for recursion (leaf node)
    if (depth == 0) {
        int eval = evaluate(board);  // Static evaluation of the board
        memo[boardState] = eval;     // Cache the result
        return eval;
    }

    // Step 3: Generate possible moves
    char color = isWhiteTurn ? 'W' : 'B';
    vector<Board> moves = generateMovesOpening(board, color);

    if (moves.empty()) {
        int eval = evaluate(board);  // No moves available, return evaluation
        memo[boardState] = eval;     // Cache the result
        return eval;
    }

    // Step 4: Recursively evaluate moves
    if (isWhiteTurn) {
        int maxEval = INT_MIN;
        for (const auto &m : moves) {
            int eval = minimax(m, depth - 1, false);
            maxEval = max(maxEval, eval);
        }
        memo[boardState] = maxEval;  // Cache the result
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (const auto &m : moves) {
            int eval = minimax(m, depth - 1, true);
            minEval = min(minEval, eval);
        }
        memo[boardState] = minEval;  // Cache the result
        return minEval;
    }
}


// Find the best move for White in the opening phase
Board findBestMove(const Board &board, int depth) {
    int bestScore = INT_MIN;
    Board bestMove = board;
    vector<Board> moves = generateMovesOpening(board, 'W'); // Moves for White

    if (moves.empty()) {
        cerr << "No moves available for White!" << endl;
        return board;
    }

    for (const auto &m : moves) {
        int score = minimax(m, depth - 1, false); // Black's turn next
        if (score > bestScore) {
            bestScore = score;
            bestMove = m;
        }
    }

    cout << "Positions evaluated by static estimation: " << positionsEvaluated << endl;
    cout << "MINIMAX estimate: " << bestScore << endl;
    return bestMove;
}

// Read board state from input file
Board readBoardFromFile(const string &fname) {
    ifstream in(fname);
    if (!in) {
        cerr << "Error: Unable to open input file " << fname << endl;
        exit(1);
    }
    string s;
    in >> s;
    if (s.length() != 23) {
        cerr << "Error: Input string must be 23 characters long!" << endl;
        exit(1);
    }
    Board b;
    b.loadFromString(s);
    return b;
}

// Write board state to output file
void writeBoardToFile(const string &fname, const Board &b) {
    ofstream out(fname);
    if (!out) {
        cerr << "Error: Unable to open output file " << fname << endl;
        exit(1);
    }
    out << b.toString();
}

// Generate moves by placing a piece of the specified color
vector<Board> GenerateAdd(const Board &board, char color) {
    vector<Board> L;
    char opponent = (color == 'W') ? 'B' : 'W';

    for (int i = 0; i < 23; ++i) {
        if (board.getPosition(i) == 'x') {
            Board b = board;
            b.setPosition(i, color);
            if (closeMill(i, b)) {
                GenerateRemove(b, L, opponent); // Remove opponent's piece if mill formed
            } else {
                L.push_back(b); // Add board without removal
            }
        }
    }
    return L;
}

// Wrapper for generating opening moves
vector<Board> generateMovesOpening(const Board &board, char color) {
    vector<Board> moves = GenerateAdd(board, color);
        // Sort moves based on heuristic (e.g., those that form a mill are prioritized)


    return moves;


    // return GenerateAdd(board, color);
}

// Generate boards by removing one opponent piece after a mill is formed
void GenerateRemove(const Board &board, vector<Board> &L, char opponent) {
    bool removed = false;
    // First, try to remove pieces not in a mill
    for (int i = 0; i < 23; ++i) {
        if (board.getPosition(i) == opponent && !closeMill(i, board)) {
            Board b2 = board;
            b2.setPosition(i, 'x');
            L.push_back(b2);
            removed = true;
        }
    }
    // If all opponent pieces are in mills, remove any opponent piece
    if (!removed) {
        for (int i = 0; i < 23; ++i) {
            if (board.getPosition(i) == opponent) {
                Board b2 = board;
                b2.setPosition(i, 'x');
                L.push_back(b2);
            }
        }
    }
}

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

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: MiniMaxOpening <infile> <outfile> <depth>" << endl;
        return 1;
    }

    string inF = argv[1];
    string outF = argv[2];
    int depth = stoi(argv[3]);

    if (depth < 1) {
        cerr << "Error: Depth must be a positive integer!" << endl;
        return 1;
    }

    positionsEvaluated = 0; // Reset counter
    Board board = readBoardFromFile(inF);
    Board best = findBestMove(board, depth);
    writeBoardToFile(outF, best);

    cout << "Input position:  " << board.toString() << endl;
    cout << "Output position: " << best.toString() << endl;
    return 0;
}