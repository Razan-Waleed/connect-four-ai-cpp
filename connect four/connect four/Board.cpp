
#include "Board.h"
#include <iostream>
#include <vector>

using namespace std;

Board::Board() {
    grid.resize(ROWS, vector<int>(COLS, EMPTY));
}

// Drop a coin into a column
bool Board::dropCoin(int col, int player) {
    if (col < 0 || col >= COLS) return false;

    int row = getNextOpenRow(col);
    if (row == -1) return false;

    grid[row][col] = player;   // This should put it at the lowest empty row
    return true;
}
int Board::getNextOpenRow(int col) const {
    // Start from the BOTTOM and go up until we find an empty spot
    for (int row = ROWS - 1; row >= 0; row--) {
        if (grid[row][col] == EMPTY) {
            return row;        // This is the correct row where the piece should land
        }
    }
    return -1;  // column is full
}
// Check if a column is valid for playing
bool Board::isValidMove(int col) const {
    if (col < 0 || col >= COLS) {
        return false;
    }
    return getNextOpenRow(col) != -1;
}

// Get all valid moves with center-first ordering
vector<int> Board::getValidMoves() const {
    vector<int> moves;
    for (int col = 0; col < COLS; col++) {
        if (isValidMove(col)) {
            moves.push_back(col);
        }
    }
    return moves;
}

// Check if board is full
bool Board::isFull() const {
    for (int col = 0; col < COLS; col++) {
        if (isValidMove(col)) {
            return false;  // found a valid move = not full
        }
    }
    return true;  // no valid moves = full
}

// Get cell value
int Board::getCell(int row, int col) const {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) return -1;
    return grid[row][col];
}

void Board::setCell(int row, int col, int value) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) return;
    grid[row][col] = value;
}

void Board::print() const {
    cout << "\n";
    for (int row = 0; row < ROWS; row++) {        // Changed: print from TOP to BOTTOM
        cout << " |";
        for (int col = 0; col < COLS; col++) {
            char symbol = '.';
            if (grid[row][col] == HUMAN) symbol = 'X';
            else if (grid[row][col] == AI) symbol = 'O';
            cout << " " << symbol << " |";
        }
        cout << "\n";
    }
    cout << " +";
    for (int col = 0; col < COLS; col++) cout << "---+";
    cout << "\n";
    cout << "   0   1   2   3   4   5   6\n\n";
}

// helper — checks if 4 cells are all the same non-empty value
int Board::checkWindow(int a, int b, int c, int d) const {
    if (a == b && b == c && c == d && a != EMPTY)
        return a;
    return EMPTY;
}
// must match exactly what team 1 declared
int Board::checkWin() const {
    int result = EMPTY;
    // --- Horizontal ---
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c <= COLS - 4; c++) {
            result = checkWindow(
                getCell(r, c),
                getCell(r, c + 1),
                getCell(r, c + 2),
                getCell(r, c + 3)
            );
            if (result != EMPTY) return result;
        }
    // --- Vertical ---
    for (int c = 0; c < COLS; c++)
        for (int r = 0; r <= ROWS - 4; r++) {
            result = checkWindow(
                getCell(r, c),
                getCell(r + 1, c),
                getCell(r + 2, c),
                getCell(r + 3, c)
            );
            if (result != EMPTY) return result;
        }
    // --- Diagonal ↘ ---
    for (int r = 0; r <= ROWS - 4; r++)
        for (int c = 0; c <= COLS - 4; c++) {
            result = checkWindow(
                getCell(r, c),
                getCell(r + 1, c + 1),
                getCell(r + 2, c + 2),
                getCell(r + 3, c + 3)
            );
            if (result != EMPTY) return result;
        }
    // --- Diagonal ↗ ---
    for (int r = 3; r < ROWS; r++)
        for (int c = 0; c <= COLS - 4; c++) {
            result = checkWindow(
                getCell(r, c),
                getCell(r - 1, c + 1),
                getCell(r - 2, c + 2),
                getCell(r - 3, c + 3)
            );
            if (result != EMPTY) return result;
        }
    return EMPTY;
}
void Board::reset() {
    for (int r = 0; r < 6; r++)
        for (int c = 0; c < 7; c++)
            grid[r][c] = EMPTY;
}

