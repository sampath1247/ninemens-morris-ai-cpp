#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <climits>
#include "classboard.h"

using namespace std;

static int positionsEvaluated = 0;

static const vector<vector<int>> neighbors = {
    {1, 3, 8},    // 0: a0 -> d0, b1, a3
    {0, 2, 4},    // 1: d0 -> a0, g0, d1
    {1, 5, 13},   // 2: g0 -> d0, f1, g3
    {0, 6, 9, 4}, // 3: b1 -> a0, c2, b3, d1
    {1, 3, 5},    // 4: d1 -> d0, b1, f1
    {2, 7, 12, 4},// 5: f1 -> g0, e2, f3, d1
    {3, 7, 10},   // 6: c2 -> b1, e2, c3
    {5, 6, 11},// 7: e2 -> d1, f1, c2, d4
    {0, 9, 20},   // 8: a3 -> a0, b3, a6
    {3, 8, 10,17},   // 9: b3 -> b1, a3, c3
    {6,9, 14 },   // 10: c3 -> d1, b3, e3
    {7, 12, 16}, // 11: e3 -> c3, f3, e4
    {5, 11,13, 19},  // 12: f3 -> f1, e3, f5
    {2, 12, 22},  // 13: g3 -> g0, f3, g6
    {10, 15, 17},  // 14: c4 -> c2, d4, b5
    {14, 16,18},  // 15: d4 -> e2, c4, e4
    {11, 15, 19}, // 16: e4 -> e3, d4, d5
    {14, 18,20},     // 17: b5 -> c4, d5
    {15, 17, 19,21}, // 18: d5 -> e4, b5, f5
    {12, 16,22},     // 19: f5 -> f3, d5
    {8,17, 21},      // 20: a6 -> a3, d6
    {18,20, 22},     // 21: d6 -> a6, g6
    {13, 19,21}      // 22: g6 -> g3, d6
};


bool closeMill(int j, const Board &b) {
    char C = b.getPosition(j);
    if (C == 'x') return false;
    switch (j) {
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

int countPieces(const Board &board, char color) {
    int count = 0;
    for (int i = 0; i < 23; ++i) {
        if (board.getPosition(i) == color) count++;
    }
    return count;
}

// Board swapColors(const Board &board) {
//     Board swapped;
//     for (int i = 0; i < 23; ++i) {
//         char p = board.getPosition(i);
//         if (p == 'W') swapped.setPosition(i, 'B');
//         else if (p == 'B') swapped.setPosition(i, 'W');
//         else swapped.setPosition(i, 'x');
//     }
//     return swapped;
// }

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

vector<Board> GenerateMove(const Board &board, char color) {
    vector<Board> L;
    char opponent = (color == 'W') ? 'B' : 'W';
    for (int i = 0; i < 23; ++i) {
        if (board.getPosition(i) == color) {
            for (int neighbor : neighbors[i]) {
                if (board.getPosition(neighbor) == 'x') {
                    Board b = board;
                    b.setPosition(i, 'x');
                    b.setPosition(neighbor, color);
                    if (closeMill(neighbor, b)) {
                        GenerateRemove(b, L, opponent);
                    } else {
                        L.push_back(b);
                    }
                }
            }
        }
    }
    return L;
}

vector<Board> GenerateHopping(const Board &board, char color) {
    vector<Board> L;
    char opponent = (color == 'W') ? 'B' : 'W';
    for (int i = 0; i < 23; ++i) {
        if (board.getPosition(i) == color) {
            for (int j = 0; j < 23; ++j) {
                if (board.getPosition(j) == 'x') {
                    Board b = board;
                    b.setPosition(i, 'x');
                    b.setPosition(j, color);
                    if (closeMill(j, b)) {
                        GenerateRemove(b, L, opponent);
                    } else {
                        L.push_back(b);
                    }
                }
            }
        }
    }
    return L;
}

vector<Board> GenerateMovesMidgameEndgame(const Board &board, char color) {
    int pieces = countPieces(board, color);
    if (pieces == 3) {
        return GenerateHopping(board, color);
    } else {
        return GenerateMove(board, color);
    }
}

int evaluateMidgameEndgame(const Board &board) {
    positionsEvaluated++;
    int numWhitePieces = countPieces(board, 'W');
    int numBlackPieces = countPieces(board, 'B');
    if (numBlackPieces <= 2) return 10000;  // White wins
    if (numWhitePieces <= 2) return -10000; // Black wins
    vector<Board> blackMoves = GenerateMovesMidgameEndgame(board, 'B');
    if (blackMoves.empty()) return 10000;   // White wins (Black can't move)
    vector<Board> whiteMoves = GenerateMovesMidgameEndgame(board, 'W');
    if (whiteMoves.empty()) return -10000;  // Black wins (White can't move)
    //cout<< blackMoves.size()<<"\n";

    return 1000*(numWhitePieces - numBlackPieces)-blackMoves.size(); // Simple evaluation
}

int minimaxMidgameEndgame(const Board &board, int depth, bool isWhiteTurn) {
    if (depth == 0) {
        return evaluateMidgameEndgame(board);
    }
    char color = isWhiteTurn ? 'W' : 'B';
    vector<Board> moves = GenerateMovesMidgameEndgame(board, color);
    if (moves.empty()) {
        return evaluateMidgameEndgame(board);
    }
    if (isWhiteTurn) {
        int maxEval = INT_MIN;
        for (const auto &m : moves) {
            int eval = minimaxMidgameEndgame(m, depth - 1, false);
            maxEval = max(maxEval, eval);
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (const auto &m : moves) {
            int eval = minimaxMidgameEndgame(m, depth - 1, true);
            minEval = min(minEval, eval);
        }
        return minEval;
    }
}

Board findBestMove(const Board &board, int depth) {
    int bestScore = INT_MIN;
    Board bestMove = board;
    vector<Board> moves = GenerateMovesMidgameEndgame(board, 'W');
    if (moves.empty()) {
        cerr << "No moves available for White!" << endl;
        return board;
    }
    for (const auto &m : moves) {
        int score = minimaxMidgameEndgame(m, depth - 1, false);
        if (score > bestScore) {
            bestScore = score;
            bestMove = m;
        }
    }
    cout << "Positions evaluated by static estimation: " << positionsEvaluated << endl;
    cout << "MINIMAX estimate: " << bestScore << endl;
    return bestMove;
}

Board readBoardFromFile(const string &fname) {
    ifstream in(fname);
    string s; in >> s;
    Board b;
    b.loadFromString(s);
    return b;
}

void writeBoardToFile(const string &fname, const Board &b) {
    ofstream out(fname);
    out << b.toString();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: MiniMaxGame <infile> <outfile> <depth>\n";
        return 1;
    }
    string inF = argv[1];
    string outF = argv[2];
    int depth = stoi(argv[3]);
    positionsEvaluated = 0;
    Board board = readBoardFromFile(inF);
    Board best = findBestMove(board, depth);
    writeBoardToFile(outF, best);

    cout << "Input position:  " << board.toString() << endl;
    cout << "Output position: " << best.toString() << endl;
    return 0;
}