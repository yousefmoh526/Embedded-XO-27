#include <iostream>
#include <vector>
#include <algorithm>

class minimax {
private:
    short PLAYER = 0; // 0 will be the default for 'O', 1 for 'X'

    int check_for_win(const short board[][3], const short &player, const short &depth) {
        short minormax = (player != PLAYER) ? -1 : 1;

        // Check rows
        for (short i = 0; i < 3; i++) {
            if(board[i][0] == player && board[i][1] == player && board[i][2] == player) {
                return minormax * (10 - depth);
            }
        }

        // Check columns
        for (short i = 0; i < 3; i++) {
            if(board[0][i] == player && board[1][i] == player && board[2][i] == player) {
                return minormax * (10 - depth);
            }
        }

        // Check diagonals
        if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
            (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
            return minormax * (10 - depth);
        }

        // Check for draw
        bool is_full = true;
        for (short i = 0; i < 9; i++) {
            if (board[i/3][i%3] == -1) {
                is_full = false;
                break;
            }
        }
        if (is_full) return 0;

        return -999; // Game not finished
    }

    int minmax(short board[][3], short depth, const short &player, int &best_move) {
        int score = check_for_win(board, player, depth);
        if (score != -999) return score;

        int best_score = (player == PLAYER) ? -999 : 999;
        int current_move = -1;

        for (short i = 0; i < 9; i++) {
            if (board[i/3][i%3] == -1) {
                board[i/3][i%3] = player;
                int temp_score = minmax(board, depth + 1, !player, current_move);
                board[i/3][i%3] = -1; // Undo move

                if (player == PLAYER) {
                    if (temp_score > best_score) {
                        best_score = temp_score;
                        best_move = i;
                    }
                } else {
                    if (temp_score < best_score) {
                        best_score = temp_score;
                        best_move = i;
                    }
                }
            }
        }
        return best_score;
    }

public:
    minimax() = default;
    ~minimax() = default;

    short getmove(const short board[][3]) {
        int best_move = -1;
        short temp_board[3][3];

        // Create a copy of the board
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                temp_board[i][j] = board[i][j];
            }
        }

        minmax(temp_board, 0, PLAYER, best_move);
        return best_move;
    }
};

void printTicTacToeBoard(const short board[][3]) {
    std::cout << "\n";
    for (int i = 0; i < 3; i++) {
        std::cout << " ";
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 1) {
                std::cout << "X";
            } else if (board[i][j] == 0) {
                std::cout << "O";
            } else {
                std::cout << " ";
            }

            if (j < 2) {
                std::cout << " | ";
            }
        }
        std::cout << "\n";

        if (i < 2) {
            std::cout << "-----------\n";
        }
    }
    std::cout << "\n";
}

// Test function to verify AI moves
void testScenario(const char* scenarioName, short board[][3], int expectedMove) {
    std::cout << "\n=== Testing scenario: " << scenarioName << " ===\n";
    std::cout << "Initial board state:";
    printTicTacToeBoard(board);

    minimax solver;
    int move = solver.getmove(board);

    std::cout << "AI chose position: " << move << " (row: " << move/3 << ", col: " << move%3 << ")\n";
    if (move == expectedMove) {
        std::cout << "✓ Test passed! AI made the expected move.\n";
    } else {
        std::cout << "✗ Test failed! Expected move: " << expectedMove << "\n";
    }

    // Show the move on the board
    board[move/3][move%3] = 0;
    std::cout << "Board after AI move:";
    printTicTacToeBoard(board);
    std::cout << "================================\n";
}

int main() {
    // Test 1: Block opponent's winning move
    short board1[][3] = {
        {1, -1, -1},
        {1, 1, -1},
        {0, -1, -1}
    };
    testScenario("Block opponent's winning move", board1, 1);

    // Test 2: Take winning move
    short board2[][3] = {
        {0, 1, 1},
        {-1, 0, -1},
        {-1, -1, -1}
    };
    testScenario("Take winning move", board2, 0);

    // Test 3: Center control
    short board3[][3] = {
        {-1, -1, -1},
        {-1, -1, -1},
        {-1, -1, -1}
    };
    testScenario("Empty board - take center", board3, 4);

    // Test 4: Block fork
    short board4[][3] = {
        {1, -1, -1},
        {-1, 0, -1},
        {-1, -1, 1}
    };
    testScenario("Block opponent's fork", board4, 1);

    // Test 5: Force a win
    short board5[][3] = {
        {0, 1, 0},
        {-1, 0, -1},
        {1, -1, -1}
    };
    testScenario("Force a win", board5, 8);

    return 0;
}