#include "Game.h"




void Game::setup() {
    cout << "Enter your name: ";
    getline(cin, playerName);
}
int Game::getHumanMove() {
    int col;
    while (true) {
        cout << playerName << ", pick a column (0-6): ";
        cin >> col;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Please enter a valid number\n";
            continue;
        }
        if (!board.isValidMove(col)) {
            cout << "Column " << col << " is not valid, try again\n";
            continue;
        }
        return col;
    }
}
void Game::switchTurn() {
    if (turn == HUMAN)
        turn = AI_PLAYER;
    else
        turn = HUMAN;
}


void Game::playTurn() {
    int col;
    // HUMAN TURN
    if (turn == HUMAN) {
        col = getHumanMove();
    }
    // AI TURN
    else {
        cout << "AI is thinking...\n";
        col = ai.getBestMove(board, 6);  // use ai object
        cout << "AI picked column " << col << "\n";
    }
    // place the disc on board
    board.dropCoin(col, turn);
    // display updated board
    board.print();
    // check if someone won
    int res = board.checkWin();  // use board object
    if (res != EMPTY) {
        over = true;
        winner = res;
        if (winner == HUMAN)
            cout << playerName << " won!\n";
        else
            cout << "AI won!\n";
        return;
    }
    // check draw condition
    if (board.isFull()) {
        over = true;
        cout << "It's a draw\n";
        return;
    }
    // switch player turn
    switchTurn();
}
void Game::resetGame() {
    board.reset();
    turn = HUMAN;
    over = false;
    winner = EMPTY;
}
void Game::run() {
    setup();
    bool playAgain = true;
    while (playAgain) {
        resetGame();
        cout << "\n  CONNECT FOUR\n";
        cout << "  " << playerName << " = X  |  AI = O\n\n";
        board.print();
        while (!over) {
            playTurn();
        }
        cout << "Play again? (y/n): ";
        char again;
        cin >> again;
        playAgain = (again == 'y' || again == 'Y');
        cin.ignore();
    }
}