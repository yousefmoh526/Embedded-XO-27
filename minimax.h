#ifndef MINIMAX_H
#define MINIMAX_H
#include <iostream>
#include <algorithm>


extern short PLAYER; // Global player symbol (0 for 'O', 1 for 'X')

void printTicTacToeBoard(const short board[][3]);
int check_for_win(const short board[][3], const short &player, const short &depth);

class minimax {
private:
    int minmax(short board[][3], short depth, const short &player, int &best_move, int alpha, int beta);

public:
    minimax();
    ~minimax();
    short getmove(const short board[][3]);
};

extern minimax hard_moves;

int easy_move(short board[][3]);
int medium_move(short board[][3]);
int hard_move(short board[][3]);

void switch_player();
void switch_player_to_x();
void switch_player_to_o();

void testScenario(const char* scenarioName, short board[][3], int expectedMove);

#endif // MINIMAX_H
