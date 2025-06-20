#ifndef MINIMAX_H
#define MINIMAX_H

// Global variable declaration
extern short PLAYER; // 0 for 'O', 1 for 'X'

// Function declarations
void printTicTacToeBoard(const short board[][3]);

int global_check_for_win(const short board[][3], const short &player);

// Minimax class declaration
class minimax {
private:
    int check_for_win(const short board[][3], const short &player, const short &depth);
    int minmax(short board[][3], short depth, const short &player, int &best_move, int alpha, int beta);

public:
    minimax() = default;
    ~minimax() = default;
    
    short getmove(const short board[][3]);
};

// Global minimax instance declaration
extern minimax hard_moves;

// AI move function declarations
int easy_move(short board[][3]);
int medium_move(short board[][3]);
int hard_move(short board[][3]);

// Utility function declarations
void switch_player();

// Test function declaration
void testScenario(const char* scenarioName, short board[][3], int expectedMove);

#endif // MINIMAX_H
