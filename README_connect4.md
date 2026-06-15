# Connect Four AI 🔴🟡

A Connect Four game where a Human plays against an AI opponent, built in C++ with both a console mode and a graphical SFML interface. The AI combines five algorithm paradigms to make strong, fast decisions.

---

## Features

- 🎮 Two game modes — Console and SFML graphical window
- 🤖 AI opponent using Minimax with depth-6 search
- 🔊 Audio and graphics via SFML
- ⚡ Evaluates ~343 nodes per turn (vs 117,649 without pruning)
- 🏆 Returns best move in under one second

---

## AI Algorithm Paradigms

| Paradigm | Component | Purpose |
|---|---|---|
| Divide & Conquer | Minimax | Recursively splits the game tree into sub-problems |
| Backtracking | Move Undo | Undoes hypothetical moves to reuse a single board object |
| Dynamic Programming | Transposition Table | Caches board positions using Zobrist hashing to avoid recomputation |
| Greedy | Heuristic Evaluation | Scores board positions at depth limit using local window patterns |
| Greedy | Move Ordering | Visits centre columns first to maximize Alpha-Beta cutoffs |
| Branch & Bound | Alpha-Beta Pruning | Cuts branches that cannot improve the current best result |

---

## How to Run

### Requirements
- C++ compiler
- [SFML library](https://www.sfml-dev.org/)

### Compile & Run
```bash
g++ -o connect_four connect_four.cpp Board.cpp AI.cpp Game.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
./connect_four
```

---

## Project Structure

```
├── connect_four.cpp   ← SFML graphical entry point
├── Game.h / Game.cpp  ← Console game mode
├── Board.h / Board.cpp ← Board logic (6×7 grid)
├── AI.h / AI.cpp      ← Minimax AI engine
```

---

## Board

- **6 rows × 7 columns**
- Win condition: 4 consecutive discs in any direction (horizontal, vertical, diagonal)

---

## Built With

- C++
- SFML (Graphics & Audio)
- Algorithm paradigms: Minimax, Alpha-Beta Pruning, Transposition Table, Backtracking, Greedy Heuristics

---

*Developed as a group project — Year 2, Algorithms Design & Analysis Course.*
