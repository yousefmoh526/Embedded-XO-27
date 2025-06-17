#include "minimax.h"

#include <iostream>
#include <algorithm>

short PLAYER = 0; // 0 will be the default for 'O', 1 for 'X'

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

/// @brief This function checks if any of the two sides has won the game or has lost it or a draw has taken place
/// @param board This is the board
/// @param player The symbol for the current player or the one you want to check for. 1 is X 0 is 0 and -1 is NOONE
/// @param depth Extra var to calculate the closer win or the further loss.
/// @return returns the score for the win or loss condition
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


    /// @brief The main function to find the optimal move, is called numerous times each with time with a depth. has alpha-beta pruning
    /// @param board the board
    /// @param depth recrusion depth, this is used to give each win or loss a value to make them more desirable
    /// @param player the symbol of the current turns player
    /// @param best_move last best move found, this is a holds it to make it available for comparisons
    /// @param alpha alpha in alpha-beta pruning
    /// @param beta beta in alpha-beta pruning
    /// @return the best/optimal move in the current board.

// Recursive Minimax function with Alpha-Beta Pruning
int minimax::minmax(short board[][3], short depth, const short &player, int &best_move, int alpha, int beta) {
    int score = check_for_win(board, !player, depth);
    if (score != -999) return score; // Game is decided (win/loss/draw)

    int best_score = (player == PLAYER) ? -999 : 999;
    int current_move = -1;

    // Iterate over all possible moves
    for (short i = 0; i < 9; i++) {
        if (board[i / 3][i % 3] == -1) { // Empty slot
            board[i / 3][i % 3] = player; // Make move
            int temp_score = minmax(board, depth + 1, !player, current_move, alpha, beta);
            board[i / 3][i % 3] = -1; // Undo move

            if (player == PLAYER) { // Maximizing player
                if (temp_score > best_score) {
                    best_score = temp_score;
                    best_move = i;
                    alpha = std::max(alpha, best_score);
                }
            } else { // Minimizing opponent
                if (temp_score < best_score) {
                    best_score = temp_score;
                    best_move = i;
                    beta = std::min(beta, best_score);
                }
            }

            if (alpha >= beta) break; // Alpha-Beta pruning (cut unnecessary branches)
        }
    }
    return best_score;
}

    // Constructor
    minimax::minimax() = default;

    // Destructor
    minimax::~minimax() = default;

    // Function implementation
    short minimax::getmove(const short board[][3]) {
        int best_move = -1;
        short temp_board[3][3];

        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
                temp_board[i][j] = board[i][j];

        minmax(temp_board, 0, PLAYER, best_move, -9999, 9999);
        return best_move;
    }




minimax hard_moves;                 // used for medium 

/// @brief Generates easy moves for easy mode; just random moves.
/// @param board The board.
/// @return A random move.
int easy_move(short board[][3])
{
    
    unsigned short move = rand() % 10;
    for (; 1;)
    {
        if (board[move / 3][move % 3] != -1)
        {
            move = rand() % 10;       
        }
        else
        {
            return move;
        }

    }

}

/// @brief Generates a medium move; half of the time a random move and the other half the optimal.
/// @param board The board.
/// @return Medium move.
int medium_move(short board[][3])
{
    unsigned short mode = rand() % 10;
    if(mode >= 5)
    {
        return hard_moves.getmove(board);
    }
    else
    {
        return easy_move(board);
    }
    
}

/// @brief Finds the optimal move.
/// @param board again?
/// @return The optimal move.
int hard_move(short board[][3])
{
    return hard_moves.getmove(board);
}

/// @brief if needed by Eng/Yaya
void switch_player()
{
    PLAYER = !PLAYER;
}

/// @brief if needed by Eng/Yaya
void switch_player_to_x()
{
    PLAYER = 1;
}

/// @brief if needed by Eng/Yaya
void switch_player_to_o()
{
    PLAYER = 0;
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


