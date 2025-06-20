#include <gtest/gtest.h>
#include "easy_move.h"
#include "medium_move.h"
#include "hard_move.h"
#include <cstdlib> // For srand, rand
#include <ctime>   // For time
#include <iostream> // For std::cout

// Helper for move validation - DEFINED ONLY ONCE
bool is_valid_move(const short board[3][3], int move) {
    if (move < 0 || move > 8) return false;
    return board[move/3][move%3] == -1;
}

TEST(MoveValidationTest, OnlyEmptyCellsAreValid) {
    short board[3][3] = {
        {1, 0, 1},
        {1, 0, 0},
        {0, 1, -1}
    };
    for (int i = 0; i < 8; ++i) {
        EXPECT_FALSE(is_valid_move(board, i));
    }
    EXPECT_TRUE(is_valid_move(board, 8));
}

TEST(MoveValidationTest, OutOfBoundsAreInvalid) {
    short board[3][3] = {
        {-1, -1, -1},
        {-1, -1, -1},
        {-1, -1, -1}
    };
    EXPECT_FALSE(is_valid_move(board, -1));
    EXPECT_FALSE(is_valid_move(board, 9));
}

TEST(MoveValidationTest, AllFilledCellsAreInvalid) {
    short board[3][3] = {
        {1, 0, 1},
        {1, 0, 0},
        {0, 1, 1}
    };
    for (int i = 0; i < 9; ++i) {
        EXPECT_FALSE(is_valid_move(board, i));
    }
}

TEST(MoveValidationTest, EasyMoveNeverInvalid) {
    short board[3][3] = {
        {1, 0, 1},
        {1, 0, 0},
        {0, 1, -1}
    };
    int move = easy_move(board);
    EXPECT_TRUE(is_valid_move(board, move));
}

TEST(MoveValidationTest, MediumMoveNeverInvalid) {
    short board[3][3] = {
        {1, 0, 1},
        {1, 0, 0},
        {0, 1, -1}
    };
    int move = medium_move(board);
    EXPECT_TRUE(is_valid_move(board, move));
}

TEST(MoveValidationTest, HardMoveNeverInvalid) {
    short board[3][3] = {
        {1, 0, 1},
        {1, 0, 0},
        {0, 1, -1}
    };
    int move = hard_move(board);
    EXPECT_TRUE(is_valid_move(board, move));
}

TEST(MoveValidationTest, FirstMoveOnEmptyBoardIsValid) {
    short board[3][3] = {
        {-1, -1, -1},
        {-1, -1, -1},
        {-1, -1, -1}
    };
    int move = easy_move(board);
    EXPECT_TRUE(is_valid_move(board, move));
}

TEST(MoveValidationTest, LastMoveOnNearlyFullBoardIsValid) {
    short board[3][3] = {
        {1, 0, 1},
        {1, 0, 0},
        {0, 1, -1}
    };
    int move = easy_move(board);
    EXPECT_EQ(move, 8);
    EXPECT_TRUE(is_valid_move(board, move));
}

TEST(MoveValidationTest, OnlyOneEmptyCell) {
    short board[3][3] = {
        {1, 0, 1},
        {1, 0, 0},
        {0, 1, -1}
    };
    for (int i = 0; i < 8; ++i) {
        EXPECT_FALSE(is_valid_move(board, i));
    }
    EXPECT_TRUE(is_valid_move(board, 8));
}

TEST(MoveValidationTest, NoEmptyCells_ReturnsInvalidMove) {
    std::cout << "DEBUG: Entering NoEmptyCells_ReturnsInvalidMove test" << std::endl;
    short board[3][3] = {
        {1, 0, 1},
        {1, 0, 0},
        {0, 1, 1}
    };
    std::cout << "DEBUG: Calling easy_move for NoEmptyCells_ReturnsInvalidMove" << std::endl;
    int move = easy_move(board);
    std::cout << "DEBUG: easy_move returned: " << move << std::endl;
    EXPECT_EQ(move, -1);
    EXPECT_FALSE(is_valid_move(board, move));
    std::cout << "DEBUG: Exiting NoEmptyCells_ReturnsInvalidMove test" << std::endl;
}

int main(int argc, char **argv) {
    srand(time(NULL));
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
