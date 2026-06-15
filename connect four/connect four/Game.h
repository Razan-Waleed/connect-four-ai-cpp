#pragma once
#include "Board.h"
#include "AI.h"
#include <iostream>
#include <string>
using namespace std;

class Game {
private:
     int EMPTY = 0;
     int HUMAN = 1;
     int AI_PLAYER = 2;

    Board  board;
    AI     ai;
    string playerName = "";
    int    turn = HUMAN;
    bool   over = false;
    int    winner = EMPTY;
    void resetGame();
    int  getHumanMove();
    void switchTurn();
    void playTurn();
    void setup();

public:
    void run();
};