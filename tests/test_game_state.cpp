#include <gtest/gtest.h>
#include "minimax.h" // Includes PLAYER, switch_player(), global_check_for_win()
#include <cstdlib> // For srand, rand
#include <ctime>   // For time

// Helper to fill the board easily
void fill_board(short board[3][3], const short vals[9]) {
    for (int i = 0; i < 9; ++i) {
        board[i/3][i%3] = vals[i];
    }
}

// Test Suite: GameStateTurnTest
// Note: Each test case will independently set up PLAYER for clarity
TEST(GameStateTurnTest, InitialPlayerTurn_DefaultIsO) {
    // Reset PLAYER to its initial state before testing
    // Assuming PLAYER is 0 ('O') at the start by default
    PLAYER = 0; // Explicitly set to ensure known state
    EXPECT_EQ(PLAYER, 0); // Verify initial player is 'O' (0)
}

TEST(GameStateTurnTest, TurnAlternation_FromOtoX) {
    PLAYER = 0; // Start with 'O'
    switch_player();
    EXPECT_EQ(PLAYER, 1); // Should switch to 'X'
}

TEST(GameStateTurnTest, TurnAlternation_FromXtoO) {
    PLAYER = 1; // Start with 'X'
    switch_player();
    EXPECT_EQ(PLAYER, 0); // Should switch back to 'O'
}

TEST(GameStateTurnTest, TurnAlternation_MultipleSwitches) {
    PLAYER = 0; // Start with 'O'
    switch_player(); // X
    EXPECT_EQ(PLAYER, 1);
    switch_player(); // O
    EXPECT_EQ(PLAYER, 0);
    switch_player(); // X
    EXPECT_EQ(PLAYER, 1);
}

// Case: Global PLAYER is X, and X wins (global_check_for_win with PLAYER should return 1)
TEST(GameStateTurnTest, GameOverOnWin_CurrentPlayerWinsAsX) {
    short board[3][3];
    short x_wins_config[] = {
        1, 1, 1,
        0, 0, -1,
        -1, -1, -1
    };
    fill_board(board, x_wins_config);

    PLAYER = 1; // Set global PLAYER to X
    int result = global_check_for_win(board, PLAYER); // Check for win where 'player' arg is global PLAYER
    EXPECT_EQ(result, 1); // Should be a win (1, as PLAYER == 'X')
}

// Case: Global PLAYER is O, and O wins (global_check_for_win with PLAYER should return 1)
TEST(GameStateTurnTest, GameOverOnWin_CurrentPlayerWinsAsO) {
    short board[3][3];
    short o_wins_config[] = {
        0, 0, 0,
        1, 1, -1,
        -1, -1, -1
    };
    fill_board(board, o_wins_config);

    PLAYER = 0; // Set global PLAYER to O
    int result = global_check_for_win(board, PLAYER); // Check for win where 'player' arg is global PLAYER
    EXPECT_EQ(result, 1); // Should be a win (1, as PLAYER == 'O')
}

// Case: Global PLAYER is X, but O wins (global_check_for_win with PLAYER should return -1)
TEST(GameStateTurnTest, GameOverOnWin_OppositePlayerWinsAsO) {
    short board[3][3];
    short o_wins_config[] = {
        0, 0, 0,
        1, 1, -1,
        -1, -1, -1
    };
    fill_board(board, o_wins_config);

    PLAYER = 1; // Set global PLAYER to X (opposite of actual winner O)
    int result = global_check_for_win(board, PLAYER); // Check for win where 'player' arg is global PLAYER
    EXPECT_EQ(result, 999); // Should be a loss for current PLAYER (-1)
}

// Case: Global PLAYER is O, but X wins (global_check_for_win with PLAYER should return -1)
TEST(GameStateTurnTest, GameOverOnWin_OppositePlayerWinsAsX) {
    short board[3][3];
    short x_wins_config[] = {
        1, 1, 1,
        0, 0, -1,
        -1, -1, -1
    };
    fill_board(board, x_wins_config);

    PLAYER = 0; // Set global PLAYER to O (opposite of actual winner X)
    int result = global_check_for_win(board, PLAYER); // Check for win where 'player' arg is global PLAYER
    EXPECT_EQ(result, 999); // Should be a loss for current PLAYER (-1)
}


TEST(GameStateTurnTest, GameOverOnDraw) {
    short board[3][3];
    short draw_config[] = {
        1, 0, 1,
        0, 1, 0,
        0, 1, 0
    };
    fill_board(board, draw_config);

    PLAYER = 1; // Player doesn't matter for draw, but set for consistency to X
    int result = global_check_for_win(board, PLAYER);
    EXPECT_EQ(result, 0); // Should be a draw (0)
}

TEST(GameStateTurnTest, GameInProgress_FromCurrentPlayerPerspective) {
    short board[3][3];
    short in_progress_config[] = {
        1, -1, 0,
        -1, -1, -1,
        -1, -1, -1
    };
    fill_board(board, in_progress_config);

    PLAYER = 1; // Global PLAYER is X
    int result_x = global_check_for_win(board, PLAYER); // Check win for X
    EXPECT_EQ(result_x, 999); // Should be "game not finished" for current player perspective
}

TEST(GameStateTurnTest, GameInProgress_FromOppositePlayerPerspective) {
    short board[3][3];
    short in_progress_config[] = {
        1, -1, 0,
        -1, -1, -1,
        -1, -1, -1
    };
    fill_board(board, in_progress_config);

    PLAYER = 0; // Global PLAYER is O
    int result_o = global_check_for_win(board, PLAYER); // Check win for O
    EXPECT_EQ(result_o, 999); // Should be "game not finished" for current player perspective
}


int main(int argc, char **argv) {
    srand(time(NULL)); // Seed random number generator ONCE
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}