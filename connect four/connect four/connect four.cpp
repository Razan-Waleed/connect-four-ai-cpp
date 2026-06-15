#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Board.h"
#include "AI.h"
#include <string>
using namespace std;

// constants
const int CELL_SIZE = 100;
const int ROWS = 6;
const int COLS = 7;
const int WIDTH = COLS * CELL_SIZE;
const int HEIGHT = (ROWS + 1) * CELL_SIZE;

// player constants
const int EMPTY = 0;
const int HUMAN = 1;
const int AI_PLAYER = 2;

// colors
const sf::Color BG_COLOR(20, 20, 40);
const sf::Color BOARD_COLOR(30, 100, 200);
const sf::Color EMPTY_COLOR(20, 20, 40);
const sf::Color HUMAN_COLOR(220, 50, 50);
const sf::Color AI_COLOR(220, 220, 50);

void drawText(sf::RenderWindow& window, sf::Font& font,
    const string& msg, int size, sf::Color color,
    float x, float y, bool center = false) {
    sf::Text text;
    text.setFont(font);
    text.setString(msg);
    text.setCharacterSize(size);
    text.setFillColor(color);
    if (center) {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    }
    text.setPosition(x, y);
    window.draw(text);
}

void drawBoard(sf::RenderWindow& window, Board& board,
    sf::Font& font, int hoverCol) {
    window.clear(BG_COLOR);

    // blue board
    sf::RectangleShape bg(sf::Vector2f(WIDTH, ROWS * CELL_SIZE));
    bg.setFillColor(BOARD_COLOR);
    bg.setPosition(0, CELL_SIZE);
    window.draw(bg);

    // hover
    if (hoverCol >= 0 && hoverCol < COLS && board.isValidMove(hoverCol)) {
        sf::RectangleShape colHighlight(sf::Vector2f(CELL_SIZE, ROWS * CELL_SIZE));
        colHighlight.setFillColor(sf::Color(255, 255, 255, 30));
        colHighlight.setPosition(hoverCol * CELL_SIZE, CELL_SIZE);
        window.draw(colHighlight);

        sf::CircleShape preview(40.f);
        preview.setOrigin(40.f, 40.f);
        preview.setFillColor(sf::Color(220, 50, 50, 180));
        preview.setPosition(
            hoverCol * CELL_SIZE + CELL_SIZE / 2.f,
            CELL_SIZE / 2.f
        );
        window.draw(preview);

        sf::ConvexShape arrow;
        arrow.setPointCount(3);
        arrow.setPoint(0, sf::Vector2f(0.f, 0.f));
        arrow.setPoint(1, sf::Vector2f(20.f, 0.f));
        arrow.setPoint(2, sf::Vector2f(10.f, 15.f));
        arrow.setFillColor(sf::Color::White);
        arrow.setPosition(
            hoverCol * CELL_SIZE + CELL_SIZE / 2.f - 10.f,
            CELL_SIZE - 20.f
        );
        window.draw(arrow);
    }

    // cells
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            sf::CircleShape circle(40.f);
            circle.setOrigin(40.f, 40.f);
            circle.setPosition(
                c * CELL_SIZE + CELL_SIZE / 2.f,
                r * CELL_SIZE + CELL_SIZE / 2.f + CELL_SIZE
            );
            int cell = board.getCell(r, c);
            if (cell == HUMAN)     circle.setFillColor(HUMAN_COLOR);
            else if (cell == AI_PLAYER) circle.setFillColor(AI_COLOR);
            else                        circle.setFillColor(EMPTY_COLOR);
            window.draw(circle);
        }
    }
}

// name input screen
string getPlayerName(sf::RenderWindow& window, sf::Font& font) {
    string name = "";
    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(32);
    inputText.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') {
                    // backspace
                    if (!name.empty())
                        name.pop_back();
                }
                else if (event.text.unicode == '\r' ||
                    event.text.unicode == '\n') {
                    // enter — confirm name
                    if (!name.empty())
                        return name;
                }
                else if (event.text.unicode < 128) {
                    name += (char)event.text.unicode;
                }
            }
        }

        window.clear(BG_COLOR);

        drawText(window, font, "Enter your name:", 36,
            sf::Color::White, WIDTH / 2.f, HEIGHT / 2.f - 60.f, true);

        // input box
        sf::RectangleShape box(sf::Vector2f(400.f, 50.f));
        box.setFillColor(sf::Color(50, 50, 80));
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(2.f);
        box.setOrigin(200.f, 25.f);
        box.setPosition(WIDTH / 2.f, HEIGHT / 2.f);
        window.draw(box);

        inputText.setString(name + "|");
        sf::FloatRect bounds = inputText.getLocalBounds();
        inputText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        inputText.setPosition(WIDTH / 2.f, HEIGHT / 2.f);
        window.draw(inputText);

        drawText(window, font, "Press Enter to start", 24,
            sf::Color(150, 150, 150), WIDTH / 2.f,
            HEIGHT / 2.f + 60.f, true);

        window.display();
    }
    return "Player";
}

int main() {
    sf::RenderWindow window(
        sf::VideoMode(WIDTH, HEIGHT),
        "Connect Four"
    );
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        // no font
    }
    // load sounds
   // load sounds
    sf::SoundBuffer dropBuffer, winBuffer;
    dropBuffer.loadFromFile("652641__krokulator__click4.wav");
    winBuffer.loadFromFile("752709__nox_sound__voice_crowd_small_expression_applause_cheer_stereo.wav");
    sf::Sound dropSound, winSound;
    dropSound.setBuffer(dropBuffer);
    winSound.setBuffer(winBuffer);
    // get player name from SFML screen
    string playerName = getPlayerName(window, font);

    Board board;
    AI    ai;

    int  turn = HUMAN;
    bool over = false;
    int  winner = EMPTY;
    int  hoverCol = -1;
    bool aiThink = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseMoved)
                hoverCol = event.mouseMove.x / CELL_SIZE;

            if (!over && turn == HUMAN &&
                event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                int col = event.mouseButton.x / CELL_SIZE;
                if (board.isValidMove(col)) {
                   
                    board.dropCoin(col, HUMAN);
                    dropSound.play();  // add this
                    int res = board.checkWin();
                    if (res == HUMAN) {
                        over = true; winner = HUMAN;
                    }
                    else if (res == AI_PLAYER) {
                        over = true; winner = AI_PLAYER;
                    }
                    else if (board.isFull()) {
                        over = true; winner = EMPTY;
                    }
                    else {
                        turn = AI_PLAYER;
                        aiThink = true;
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::R) {
                board.reset();
                turn = HUMAN;
                over = false;
                winner = EMPTY;
                aiThink = false;
            }
        }

        // AI turn
        if (!over && turn == AI_PLAYER && aiThink) {
            drawBoard(window, board, font, -1);
            drawText(window, font, "AI thinking...", 32,
                sf::Color::Yellow, WIDTH / 2.f, 35.f, true);
            window.display();
            sf::sleep(sf::milliseconds(500));

            int col = ai.getBestMove(board, 6);
            board.dropCoin(col, AI_PLAYER);

            dropSound.play();  // add this
            int res = board.checkWin();
            if (res == AI_PLAYER) {
                over = true; winner = AI_PLAYER;
                winSound.play();
            }
            else if (res == HUMAN) {
                over = true; winner = HUMAN;
                winSound.play();
            }
            else if (board.isFull()) {
                over = true; winner = EMPTY;
            }
            else {
                turn = HUMAN;
            }
            aiThink = false;
        }

        // draw
        drawBoard(window, board, font,
            (!over && turn == HUMAN) ? hoverCol : -1);

        // status text
        if (over) {
            string msg;
            if (winner == HUMAN)     msg = playerName + " won! Press R to restart";
            else if (winner == AI_PLAYER) msg = "AI won! Press R to restart";
            else                          msg = "Draw! Press R to restart";
            drawText(window, font, msg, 32,
                sf::Color::White, WIDTH / 2.f, 35.f, true);
        }
        else if (turn == HUMAN) {
            drawText(window, font, playerName + "'s turn", 32,
                sf::Color::Red, WIDTH / 2.f, 35.f, true);
        }
        else {
            drawText(window, font, "AI thinking...", 32,
                sf::Color::Yellow, WIDTH / 2.f, 35.f, true);
        }

        window.display();
    }
    return 0;
}