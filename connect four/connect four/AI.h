#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <vector>

#include <climits>
#include <algorithm>

#include "Board.h"
using namespace std;



class AI {
public:
    AI();
    int getBestMove(Board& board, int depth);
    unsigned long long zobristTable[6][7][3];
private:
    const  int EMPTY = 0;
    const  int HUMAN = 1;
    const int AI_VAL = 2;
    struct Hashing {
        unsigned long long hash = 0;
        int score = 0;
        int depth = -1;
    };
    const int TableSize = 1048576;
    vector<Hashing> transTable;
    int evaluateWindow(const vector<int>& window, int aiPlayer);
    int evaluateBoard(const Board& board, int aiPlayer);
    bool contains(const vector<int>& moves, int col);
    vector<int> orderMoves(const vector<int>& validMoves);
    int minimax(Board& board, int depth, int alpha, int beta, bool isMaximizing);
    int heuristic(Board& board);


    unsigned long long hashBoard(const Board& board);
    bool transposition(const Board& board, int& score, int depth);
    void storeTransposition(const Board& board, int score, int depth);
};