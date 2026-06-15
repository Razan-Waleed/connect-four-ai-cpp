#include "AI.h" 
using namespace std;
/////////////////////m2

AI::AI() {

		transTable.resize(TableSize);

		// Initialize Zobrist Hashing Table (only once)
		srand(time(NULL));

		for (int r = 0; r < 6; r++) {
			for (int c = 0; c < 7; c++) {
				for (int p = 0; p < 3; p++) {
					// Better random distribution
					zobristTable[r][c][p] =
						((unsigned long long)rand() << 32) |
						((unsigned long long)rand() << 16) |
						rand();
				}
			}
		}
	
}
unsigned long long AI::hashBoard(const Board& board) {
	unsigned long long hash = 0ULL;
	for (int r = 0; r < 6; r++) {
		for (int c = 0; c < 7; c++) {
			hash ^= zobristTable[r][c][board.grid[r][c]];
		}
	}
	return hash;
}
bool AI::transposition(const Board& board, int& score, int depth) {
	unsigned long long h = hashBoard(board);
	int index = h % TableSize;

	if (transTable[index].hash == h && transTable[index].depth >= depth) {
		score = transTable[index].score;
		return true;
	}
	return false;
}
void AI::storeTransposition(const Board& board, int score, int depth) {
	unsigned long long h = hashBoard(board);
	int index = h % TableSize;

	if (depth >= transTable[index].depth) {   // Better replacement
		transTable[index].hash = h;
		transTable[index].score = score;
		transTable[index].depth = depth;
	}
}
int AI::getBestMove(Board& board, int depth) {

	int bestScore = INT_MIN;
	int bestCol = 3;  // default:center column
	vector<int> moves = orderMoves(board.getValidMoves());
	for (int col : moves) {

		// ── make move ────
		int row = board.getNextOpenRow(col);
		board.setCell(row, col, AI_VAL);
		int subtreeScore;
		if (board.checkWin() == AI_VAL) {
			board.setCell(row, col, EMPTY);
			return col;  // Winning move! Take it immediately
		}

		// ── call minimax ──────
		 subtreeScore = minimax(board, depth - 1, -100000, 100000, false);

		// ── undo ──

		board.setCell(row, col, EMPTY);

		// ── track best column ──
		if (subtreeScore > bestScore) {
			bestScore = subtreeScore;
			bestCol = col;
		}
	}
	return bestCol;
}

//  MINIMAX
int AI::minimax(Board& board, int depth, int alpha, int beta, bool isMaximizing) {
	int winner = board.checkWin();

	if (winner == AI_VAL)  return +10000;  //law el AI kesb 

	if (winner == HUMAN) return -10000;   //law el human keseb 
	if (board.isFull()) return 0;// mafeesh 7ad keseb 
	if (depth == 0) return heuristic(board); //dee lama eltakhiol bta3 el Ai Y end l numof(Depth) odam (4-6 moves) 
	//byb3at el board ll heuristic 3shan yshof a7san column


	
		int SavedScore;
	
	


	if (transposition(board, SavedScore, depth)) {
		return SavedScore;
	}
	

	 //-------**---maxi(AI TURN)------**-----
	if (isMaximizing) {  // isMaximizing =  true -> AI trun if = False -> Human turn (handled in getbestMove)

		int bestScore = INT_MIN;

		vector<int> BestMoves = orderMoves(board.getValidMoves());


		for (int col : BestMoves)
		{
			int row = board.getNextOpenRow(col);   // ya7ot el disc bat3 el ai 3ala 7aseb el gravity
			board.setCell(row, col, AI_VAL);
			if (board.checkWin() == AI_VAL) {
				storeTransposition(board, 10000, depth);
				board.setCell(row, col, EMPTY);
				
				return +10000;  
				
			}
			int subtreescore = minimax(board, depth - 1, alpha, beta, false); //hyroo7 ye7seb el future move bta3t el human to maximize its score

			board.setCell(row, col, EMPTY); //UNDO 3shan ye7seb el best 3ala board fadia(Back Tracking)

			bestScore = max(bestScore, subtreescore);
			alpha = max(alpha, bestScore); //RR - update best option for AI
			if (alpha >= beta) break; //RR

		}
		
			storeTransposition(board, bestScore, depth);

		return bestScore;
		


	}
	else {//-----**-----mini(HUMAN TURN)------**---------
		int bestScore = INT_MAX;

		vector<int> BestMoves = orderMoves(board.getValidMoves());


		for (int col : BestMoves)
		{
			int row = board.getNextOpenRow(col);   // ya7ot el disc bat3 el ai 3ala 7aseb el gravity
			board.setCell(row, col, HUMAN);

			int subtreescore = minimax(board, depth - 1, alpha, beta, true); //hyroo7 ye7seb el future move bta3t el human to maximize its score

			board.setCell(row, col, EMPTY); //UNDO 3shan ye7seb el best 3ala board fadia(Back Tracking)

			bestScore = min(bestScore, subtreescore);
			beta = min(beta, bestScore);  // RR -  update best option for HUMAN
			if (alpha >= beta) break; //RR
		}
		
			storeTransposition(board, bestScore, depth);

		

		return bestScore;
	}
}

int AI::evaluateWindow(const vector<int>& window, int aiPlayer) {
	int opponent = (aiPlayer == HUMAN) ? AI_VAL : HUMAN;

	int aiCount = 0;
	int opponentCount = 0;
	int emptyCount = 0;

	for (int cell : window) {
		if (cell == aiPlayer) aiCount++;
		else if (cell == opponent) opponentCount++;
		else                       emptyCount++;
	}

	int score = 0;
	if (aiCount == 4)                         score += 9000;  //  أقل من 10000
	else if (aiCount == 3 && emptyCount == 1) score += 100;
	else if (aiCount == 2 && emptyCount == 2) score += 10;

	if (opponentCount == 4)                         score -= 9000;  // 
	else if (opponentCount == 3 && emptyCount == 1) score -= 120;
	else if (opponentCount == 2 && emptyCount == 2) score -= 10;

	return score;
}
int AI::evaluateBoard(const Board& board, int aiPlayer) {
	int score = 0;

	// centre column bonus
	int centerCol = 3;
	for (int row = 0; row < 6; row++) {
		if (board.grid[row][centerCol] == aiPlayer)
			score += 6;
	}

	// horizontal
	for (int row = 0; row < 6; row++) {
		for (int col = 0; col <= 3; col++) {
			vector<int> window = {
				board.grid[row][col],
				board.grid[row][col + 1],
				board.grid[row][col + 2],
				board.grid[row][col + 3]
			};
			score += evaluateWindow(window, aiPlayer);
		}
	}

	// vertical
	for (int col = 0; col < 7; col++) {
		for (int row = 0; row <= 2; row++) {
			vector<int> window = {
				board.grid[row][col],
				board.grid[row + 1][col],
				board.grid[row + 2][col],
				board.grid[row + 3][col]
			};
			score += evaluateWindow(window, aiPlayer);
		}
	}

	// diagonal ↘
	for (int row = 0; row <= 2; row++) {
		for (int col = 0; col <= 3; col++) {
			vector<int> window = {
				board.grid[row][col],
				board.grid[row + 1][col + 1],
				board.grid[row + 2][col + 2],
				board.grid[row + 3][col + 3]
			};
			score += evaluateWindow(window, aiPlayer);
		}
	}

	// diagonal ↗
	for (int row = 0; row <= 2; row++) {
		for (int col = 3; col < 7; col++) {
			vector<int> window = {
				board.grid[row][col],
				board.grid[row + 1][col - 1],
				board.grid[row + 2][col - 2],
				board.grid[row + 3][col - 3]
			};
			score += evaluateWindow(window, aiPlayer);
		}
	}

	return score;
}
bool AI::contains(const vector<int>& moves, int col) {
	for (int move : moves) {
		if (move == col) return true;
	}
	return false;
}
vector<int> AI::orderMoves(const vector<int>& validMoves) {
	vector<int> preferredOrder = { 3, 2, 4, 1, 5, 0, 6 };
	vector<int> orderedMoves;
	for (int col : preferredOrder) {
		if (contains(validMoves, col)) {
			orderedMoves.push_back(col);
		}
	}
	return orderedMoves;
}
int AI::heuristic(Board& board) {
	return evaluateBoard(board, AI_VAL);
}

