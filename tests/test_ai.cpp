#include <gtest/gtest.h>
#include "minimax.h"

// Helper to fill the board
void fillBoard(short board[3][3], const short vals[9]) {
    for (int i = 0; i < 9; ++i)
        board[i/3][i%3] = vals[i];
}

// Test: AI should pick the only available move
TEST(MinimaxAITest, PicksLastAvailableMove) {
    short board[3][3] = {
        {1, 0, 1},
        {1, 0, 0},
        {0, 1, -1}
    };
    PLAYER = 1; // X's turn
    int move = hard_move(board);
    EXPECT_EQ(move, 8); // Only index 8 is left
}

// Test: AI blocks opponent's win
TEST(MinimaxAITest, BlocksOpponentWin) {
    short board[3][3] = {
        {1, 0, -1},
        {0, 1, -1},
        {-1, -1, -1}
    };
    PLAYER = 0; // O's turn
    int move = hard_move(board);
    // The best move is to block X at (2,0) or (2,2)
    EXPECT_TRUE(move == 6 || move == 8);
}

// Test: AI takes winning move
TEST(MinimaxAITest, TakesWinningMove) {
    short board[3][3] = {
        {1, 1, -1},
        {0, 0, -1},
        {-1, -1, -1}
    };
    PLAYER = 1; // X's turn
    int move = hard_move(board);
    EXPECT_EQ(move, 2); // X can win at (0,2)
}

// Test: AI plays optimally for a draw
TEST(MinimaxAITest, PlaysForDraw) {
    short board[3][3] = {
        {1, 0, 1},
        {1, 0, 0},
        {0, 1, -1}
    };
    PLAYER = 0; // O's turn
    int move = hard_move(board);
    EXPECT_EQ(move, 8); // Only move left, results in a draw
}

// Test: AI does not play in an already filled cell
TEST(MinimaxAITest, DoesNotPlayInFilledCell) {
    short board[3][3] = {
        {1, 0, 1},
        {1, 0, 0},
        {0, 1, 1}
    };
    PLAYER = 0; // O's turn
    int move = hard_move(board);
    EXPECT_TRUE(move < 0 || move > 8); // No valid moves left
}

// Test: AI plays first move (empty board)
TEST(MinimaxAITest, PlaysFirstMove) {
    short board[3][3] = {
        {-1, -1, -1},
        {-1, -1, -1},
        {-1, -1, -1}
    };
    PLAYER = 1; // X's turn
    int move = hard_move(board);
    EXPECT_GE(move, 0);
    EXPECT_LE(move, 8);
}

// Test: AI plays when only one move left (last cell)
TEST(MinimaxAITest, PlaysLastCell) {
    short board[3][3] = {
        {1, 0, 1},
        {1, 0, 0},
        {0, 1, -1}
    };
    PLAYER = 0; // O's turn
    int move = hard_move(board);
    EXPECT_EQ(move, 8);
}

// Test: AI plays as both X and O
TEST(MinimaxAITest, PlaysAsXandO) {
    short board[3][3] = {
        {1, 0, 1},
        {1, 0, 0},
        {0, 1, -1}
    };
    PLAYER = 1; // X's turn
    int moveX = hard_move(board);
    PLAYER = 0; // O's turn
    int moveO = hard_move(board);
    EXPECT_EQ(moveX, 8);
    EXPECT_EQ(moveO, 8);
}

// Test: AI does not crash on invalid boards (all filled)
TEST(MinimaxAITest, HandlesFullBoard) {
    short board[3][3] = {
        {1, 0, 1},
        {1, 0, 0},
        {0, 1, 1}
    };
    PLAYER = 1; // X's turn
    int move = hard_move(board);
    EXPECT_TRUE(move < 0 || move > 8); // No valid moves
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}