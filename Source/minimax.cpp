#include <iostream>
#include <vector>
#include <algorithm>

class minimax
{
private:

    short PLAYER = 0; // 0 will be the default for 'O', 1 for 'X'
    short current_move = -1, current_min = 999, current_max = -999;


    /// @brief Checks for a win in the board.
    /// @param board The board to check. 3x3
    /// @param player Player(bot) symbol, either 0 or 1
    /// @return +10 for a win, -10 for a loss, 0 for a draw
    int check_for_win(const short board[][3], const short &player, const short &depth) 
    {
        short minormax;
        
        if (player != PLAYER)
        {
            minormax = -1;
        }
        else
        {
            minormax = 1;           // By default, this function is used to get max value, if we are using it for opponent, it calculates min.
        }


        for (short i = 0; i < 3; i++)
        {
            if(board[i][0] == player && board[i][1] == player && board[i][2] == player) // Row Check
            {
                return minormax*10 + -1*minormax*depth; // win 
            }

            if(board[0][i] == player && board[1][i] == player && board[2][i] == player) // Column Check
            {
                return minormax*10 + -1*minormax*depth; // win   
            }
        }

        if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||        // Diagonal Check            
        (board[0][2] == player && board[1][1] == player && board[2][0] == player))         
        {
            return minormax*10 + -1*minormax*depth;
        }
        
        for (short i = 0; i < 9; i++)
        {
            if (board[i/3][i%3] == -1)
            {
                return minormax*999;
            }
        }

        return 0;
    }


    /// @brief Minimax function calculates best move
    /// @param board The board.
    /// @param depth How many previous moves have been made.
    /// @param player player symbol.
    /// @return the index of the best move.
    int minmax(const short board[][3], short depth, const short &player)
    {
        const int win = check_for_win(board, player, depth); // check for a win
        if ((win <= 10 && win > 0) || (win < 0 && win >= -10)) return win;

        short temp_score = 0;
        short which_move = depth % 2; // 0 for player, 1 for opponent.
        short next_player = -1;
        if (which_move) next_player = 0;            // so we can switch between min and max
        else next_player = 1;
        short new_board[3][3];
        std::copy(&board[0][0], &board[0][0] + 3*3, &new_board[0][0]);

        for (short i = 0; i < 9; i++)
        {
            if (board[i/3][i%3] == -1)
            {
                new_board[i/3][i%3] = player;
                temp_score = minmax(new_board, depth + 1, next_player);
                if (temp_score > current_max && which_move)                 
                {
                    current_move = i;
                    current_max = temp_score;
                }
                else if (temp_score < current_min && !which_move)
                {
                    current_move = i;
                    current_min = temp_score;
                }
                
            }
        }
        
        return current_move;
    }


public:
    minimax();
    ~minimax();

    short getmove(const short board[][3])
    {
        return minmax(board, 0, PLAYER);
    }



};

minimax::minimax(/* args */)
{
}


minimax::~minimax()
{
}

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
                std::cout << " "; // Assuming -1 or any other value is empty
            }
            
            // Print vertical dividers between columns
            if (j < 2) {
                std::cout << " | ";
            }
        }
        std::cout << "\n";
        
        // Print horizontal dividers between rows
        if (i < 2) {
            std::cout << "-----------\n";
        }
    }
    std::cout << "\n";
}


int main()
{
    short board[][3] = {{1, -1, -1}, {1, 1, -1}, {0, -1, -1}};
    minimax solver;
    board[solver.getmove(board)/3][solver.getmove(board)%3] = 0;
    printTicTacToeBoard(board);
}
