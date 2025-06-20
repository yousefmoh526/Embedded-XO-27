#include <gtest/gtest.h>
#include "minimax.h" // Includes global_check_for_win and PLAYER

// Helper to fill the board easily
void fill_board(short board[3][3], const short vals[9]) {
    for (int i = 0; i < 9; ++i) {
        board[i/3][i%3] = vals[i];
    }
}

// Test Suite: WinDetectionTest
TEST(WinDetectionTest, HorizontalWin_PlayerX) {
    short board[3][3];
    short config[] = {
        1, 1, 1, // X wins
        0, 0, -1,
        -1, -1, -1
    };
    fill_board(board, config);
    PLAYER = 1; // Assume X is PLAYER
    EXPECT_EQ(global_check_for_win(board, PLAYER), 1);
}

TEST(WinDetectionTest, HorizontalWin_PlayerO) {
    short board[3][3];
    short config[] = {
        0, 0, 0, // O wins
        1, 1, -1,
        -1, -1, -1
    };
    fill_board(board, config);
    PLAYER = 0; // Assume O is PLAYER
    EXPECT_EQ(global_check_for_win(board, PLAYER), 1);
}

TEST(WinDetectionTest, VerticalWin_PlayerX) {
    short board[3][3];
    short config[] = {
        1, 0, -1,
        1, 0, -1,
        1, -1, -1 // X wins
    };
    fill_board(board, config);
    PLAYER = 1; // Assume X is PLAYER
    EXPECT_EQ(global_check_for_win(board, PLAYER), 1);
}

TEST(WinDetectionTest, VerticalWin_PlayerO) {
    short board[3][3];
    short config[] = {
        0, 1, -1,
        0, 1, -1,
        0, -1, -1 // O wins
    };
    fill_board(board, config);
    PLAYER = 0; // Assume O is PLAYER
    EXPECT_EQ(global_check_for_win(board, PLAYER), 1);
}

TEST(WinDetectionTest, DiagonalWin_Main_PlayerX) {
    short board[3][3];
    short config[] = {
        1, 0, -1,
        -1, 1, -1,
        -1, 0, 1 // X wins
    };
    fill_board(board, config);
    PLAYER = 1; // Assume X is PLAYER
    EXPECT_EQ(global_check_for_win(board, PLAYER), 1);
}

TEST(WinDetectionTest, DiagonalWin_Main_PlayerO) {
    short board[3][3];
    short config[] = {
        0, 1, -1,
        -1, 0, -1,
        -1, 1, 0 // O wins
    };
    fill_board(board, config);
    PLAYER = 0; // Assume O is PLAYER
    EXPECT_EQ(global_check_for_win(board, PLAYER), 1);
}

TEST(WinDetectionTest, DiagonalWin_Anti_PlayerX) {
    short board[3][3];
    short config[] = {
        -1, 0, 1,
        -1, 1, -1,
        1, 0, -1 // X wins
    };
    fill_board(board, config);
    PLAYER = 1; // Assume X is PLAYER
    EXPECT_EQ(global_check_for_win(board, PLAYER), 1);
}

TEST(WinDetectionTest, DiagonalWin_Anti_PlayerO) {
    short board[3][3];
    short config[] = {
        -1, 1, 0,
        -1, 0, -1,
        0, 1, -1 // O wins
    };
    fill_board(board, config);
    PLAYER = 0; // Assume O is PLAYER
    EXPECT_EQ(global_check_for_win(board, PLAYER), 1);
}

TEST(WinDetectionTest, DrawCondition) {
    short board[3][3];
    short config[] = {
        1, 0, 1,
        0, 1, 0,
        0, 1, 0 // Draw
    };
    fill_board(board, config);
    PLAYER = 1; // Doesn't matter for draw, but set for consistency
    EXPECT_EQ(global_check_for_win(board, PLAYER), 0);
}

TEST(WinDetectionTest, GameInProgress_NoWin) {
    short board[3][3];
    short config[] = {
        1, -1, 0,
        -1, 1, -1,
        0, -1, -1 // Game not finished, no win
    };
    fill_board(board, config);
    PLAYER = 1; // Test for X's perspective
    EXPECT_NE(global_check_for_win(board, PLAYER), 1);
    EXPECT_NE(global_check_for_win(board, PLAYER), 0);

    PLAYER = 0; // Test for O's perspective
    EXPECT_NE(global_check_for_win(board, PLAYER), 1);
    EXPECT_NE(global_check_for_win(board, PLAYER), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}