#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <vector>
using namespace std;

class Board
{
private:



    const int EMPTY = 0;
    const int HUMAN = 1;
    const int AI = 2;


public:
    static constexpr int ROWS = 6;
    static constexpr int COLS = 7;


    vector<vector<int>> grid;

    Board();
    void reset();
    int checkWindow(int a, int b, int c, int d) const;
    bool dropCoin(int col, int player);
    int getNextOpenRow(int col) const;
    bool isValidMove(int col) const;
    vector<int> getValidMoves() const;
    bool isFull() const;
    int getCell(int r, int c) const;
    void setCell(int r, int c, int val);
    void print() const;
    int checkWin() const;



};


