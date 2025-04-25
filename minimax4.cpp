#include <iostream>
#include <vector>
#include <algorithm>

void printBoard(const short board[][4], int size) {
    std::cout << "Current 4x4 Board:" << std::endl;
    std::cout << "+---+---+---+---+" << std::endl;
    
    for (int i = 0; i < size; i++) {
        std::cout << "|";
        for (int j = 0; j < size; j++) {
            char symbol;
            if (board[i][j] == 1) {
                symbol = 'X';
            } else if (board[i][j] == 0) {
                symbol = 'O';
            } else {
                symbol = ' ';
            }
            std::cout << " " << symbol << " |";
        }
        
        // Print row number indicators on the right
        std::cout << " " << i << std::endl;
        
        // Print separator between rows
        std::cout << "+---+---+---+---+" << std::endl;
    }
    
    // Print column numbers at the bottom
    std::cout << "  0   1   2   3  " << std::endl;
}


class minimax {
private:
    short PLAYER = 0; // 0 will be the default for 'O', 1 for 'X'

    int check_for_win(const short board[][4], const short &player, const short &depth) {
        short minormax = (player != PLAYER) ? -1 : 1;



        // Check rows
        for (short i = 0; i < 4; i++) {
            if(board[i][0] == player && board[i][1] == player && board[i][2] == player && board[i][3] == player) {
                return minormax * (10 - depth);
            }
        }

        // Check columns
        for (short i = 0; i < 4; i++) {
            if(board[0][i] == player && board[1][i] == player && board[2][i] == player && board[3][i] == player) {
                return minormax * (10 - depth);
            }
        }

        // Check diagonals
        if ((board[0][0] == player && board[1][1] == player && board[2][2] == player && board[3][3] == player) ||
            (board[0][3] == player && board[1][2] == player && board[2][1] == player && board[3][0] == player)) {
            return minormax * (10 - depth);
        }

        // Check for draw
        bool is_full = true;
        for (short i = 0; i < 16; i++) {
            if (board[i/4][i%4] == -1) {
                is_full = false;
                break;
            }
        }
        if (is_full) return 0;

        return -999; // Game not finished
    }

    int minmax(short board[][4], short depth, const short &player, int &best_move, int alpha, int beta) {
        int score = check_for_win(board, !player, depth);
        if (score != -999) return score;

        int best_score = (player == PLAYER) ? -999 : 999;
        int current_move = -1;
        printBoard(board, 4);

        for (short i = 0; i < 16; i++) {
            if (board[i/4][i%4] == -1) {
                board[i/4][i%4] = player;
                int temp_score = minmax(board, depth + 1, !player, current_move, alpha, beta);
                board[i/4][i%4] = -1; // Undo move

                if (player == PLAYER) {
                    if (temp_score > best_score) {
                        best_score = temp_score;
                        best_move = i;
                        alpha = std::max(alpha, best_score);
                    }
                } else {
                    if (temp_score < best_score) {
                        best_score = temp_score;
                        best_move = i;
                        beta = std::min(beta, best_score);
                    }
                }
                
                if (alpha >= beta) break;
            }
        }
        return best_score;
    }

public:
    minimax() = default;
    ~minimax() = default;

    short getmove(const short board[][4]) {
        int best_move = -1;
        short temp_board[4][4];

        // Create a copy of the board
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                temp_board[i][j] = board[i][j];
            }
        }

        minmax(temp_board, 0, PLAYER, best_move, -9999, 9999);
        return best_move;
    }
};


// Test function to verify AI moves
void testScenario(const char* scenarioName, short board[][4], int expectedMove) {
    std::cout << "\n=== Testing scenario: " << scenarioName << " ===\n";
    std::cout << "Initial board state:";
    printBoard(board, 4);

    minimax solver;
    int move = solver.getmove(board);

    std::cout << "AI chose position: " << move << " (row: " << move/3 << ", col: " << move%3 << ")\n";
    if (move == expectedMove) {
        std::cout << "✓ Test passed! AI made the expected move.\n";
    } else {
        std::cout << "✗ Test failed! Expected move: " << expectedMove << "\n";
    }

    // Show the move on the board
    board[move/4][move%4] = 0;
    std::cout << "Board after AI move:";
    printBoard(board, 4);
    std::cout << "================================\n";
}

int main() {


// Test 8: Force a draw
short board10[][4] = {
    {0, 1, 0, 1},
    {1, 0, 1, 0},
    {0, -1, 0, 1},
    {1, 0, -1, -1}
};
testScenario("Force a draw", board10, 9);
}