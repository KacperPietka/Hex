#include <iostream>
#include <fstream>
#include "Stack.h"

#define MAX_WIDTH 67
#define MAX_HEIGHT 23
#define BLUE_PAWN 'b'
#define RED_PAWN 'r'
#define END_LINE '\n'
#define DASH '-'
#define NUMBER_OF_DASHES_START_END 3
#define SPACE ' '
#define MAX_BOARD_SIZE 11

class Questions
{
public:
    const char* board_size_string = "BOARD_SIZE";
    const char* pawns_number_string = "PAWNS_NUMBER";
    const char* is_board_correct_string = "IS_BOARD_CORRECT";
    const char* is_board_possible_string = "IS_BOARD_POSSIBLE";
    const char* is_game_over_string = "IS_GAME_OVER";
    const char* can_red_win_in_1_move_with_naive_oppponent = "CAN_RED_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT";
    const char* can_blue_win_in_1_move_with_naive_oppponent = "CAN_BLUE_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT";
    const char* can_red_win_in_2_move_with_naive_oppponent = "CAN_RED_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT";
    const char* can_blue_win_in_2_move_with_naive_oppponent = "CAN_BLUE_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT";
};

class Board
{
public:
    char** temp_board = nullptr;
    char** square_board = nullptr;
    char board[MAX_HEIGHT][MAX_WIDTH]{};
    int Board_Size = 0;
    bool end_board = false;
    bool start_board = false;
    bool correct_board = true;
    bool Game_over_answered = false;
    bool game_over = false;
    bool can_blue_win_in_1_move = false;
    bool can_red_win_in_1_move = false;
    bool can_blue_win_in_2_move = false;
    bool can_red_win_in_2_move = false;
    int start_board_int = 0;
    int pawns_number = 0, red_pawns = 0, blue_pawns = 0;
    int row = 0;
    bool red_won = false, blue_won = false;
    char turn;
    int number_of_empty_places = 0;
    bool board_possible = false;
    void ResetBoardVariables()

    {
        pawns_number = 0;
        Board_Size = 0;
        red_pawns = 0;
        blue_pawns = 0;
        red_won = false;
        blue_won = false;
        start_board = true;
        end_board = false;
        Game_over_answered = false;
        start_board_int = 0;
        can_blue_win_in_1_move = false;
        can_red_win_in_1_move = false;
        can_blue_win_in_2_move = false;
        can_red_win_in_2_move = false;
        number_of_empty_places = 0;
        board_possible = false;

    }
};

int count_size_of_a_board(const char* line)
{
    int board_size = 0;
    for (int i = 0; i < MAX_WIDTH; i++)
    {
        char c = line[i];
        if (c == '<') { board_size++; }
        if (c == '\n') { break; }
    }
    return board_size;
}

void create_temporary_board(Board& board)
{
    board.temp_board = new char* [board.Board_Size * 2 - 1];
    for (int i = 0; i < board.Board_Size * 2 - 1; i++) {
        board.temp_board[i] = new char[board.Board_Size * 2 - 1];
    }
    for (int i = 0; i < board.Board_Size * 2 - 1; ++i) {
        for (int j = 0; j < board.Board_Size; ++j) {
            board.temp_board[i][j] = ' ';
        }
    }
    int x = 0, y = 0;
    for (int i = 0; i < board.Board_Size * 2; i++)
    {
        for (int j = 0; j < MAX_WIDTH; j++)
        {
            if (board.board[i][j + 1] == '\n')
            {
                break;
            }
            if (board.board[i][j] == '<')
            {
                if (board.board[i][j + 2] == SPACE) {
                    board.temp_board[x][y] = '#'; board.number_of_empty_places++;
                }
                else { board.temp_board[x][y] = board.board[i][j + 2]; }
                y++;
            }
        }
        if (y > 0) { x++; }
        y = 0;
    }
}

void create_square_board(Board& board)
{
    board.square_board = new char* [board.Board_Size];
    for (int i = 0; i < board.Board_Size; i++) {
        board.square_board[i] = new char[board.Board_Size];
    }
    for (int i = 0; i < board.Board_Size; ++i) {
        for (int j = 0; j < board.Board_Size; ++j) {
            board.square_board[i][j] = ' ';
        }
    }
    int y;
    int temp_size = board.Board_Size;
    for (int i = 0; i < temp_size; i++) {
        y = board.Board_Size - i - 1;
        for (int j = 0; j < temp_size; j++) {
            if (j < y)
            {
                board.square_board[i][j] = board.temp_board[i + j][j];
            }
            else
            {
                board.square_board[i][j] = board.temp_board[i + j][y];
            }
        }
    }
}

void clear_temp_board(Board& board) {
    for (int i = 0; i < board.Board_Size * 2 - 1; ++i) {
        delete[] board.temp_board[i];
    }
    board.temp_board = nullptr;
}

void clear_square_board(Board& board) {
    for (int i = 0; i < board.Board_Size; ++i) {
        delete[] board.square_board[i];
    }
    board.square_board = nullptr;
}

void Check_Board_Size(Board& board, Questions& questions)
{
    for (int i = 0; i < 10; i++)
    {
        if (board.board[board.row][i] != questions.board_size_string[i])
        {
            break;
        }
        if (i == 9) { std::cout << board.Board_Size << std::endl; }
    }
}

void Check_Pawns_Number(Board& board, Questions& questions)
{
    for (int i = 0; i < 12; i++)
    {
        if (board.board[board.row][i] != questions.pawns_number_string[i])
        {
            break;
        }
        if (i == 11) {
            std::cout << board.pawns_number << std::endl;
        }
    }
}

bool DFS_BLUE(Board& board, int Start_row, int Start_column, bool** visited)
{
    int Size = board.Board_Size;

    Stack dfs_stack;

    dfs_stack.push(Start_row);
    dfs_stack.push(Start_column);


    while (!dfs_stack.isEmpty())
    {
        int column = dfs_stack.top_element();
        dfs_stack.pop();
        int row = dfs_stack.top_element();
        dfs_stack.pop();


        visited[row][column] = true;

        if (row == Size - 1)
        {
            return true;
        }

        int possible_row_moves[] = { 1, 1,0, 0, -1, -1 };
        int possible_column_moves[] = { 0, 1,1, -1, 0, -1 };

        for (int i = 0; i < 6; ++i)
        {

            int newRow = row + possible_row_moves[i];
            int newCol = column + possible_column_moves[i];
            if (newRow >= 0 && newRow < Size && newCol >= 0 && newCol < Size && board.square_board[newRow][newCol] == BLUE_PAWN && !visited[newRow][newCol]) {
                dfs_stack.push(newRow);
                dfs_stack.push(newCol);
            }
        }

    }

    return false;


}

bool DFS_RED(Board& board, int Start_row, int Start_column, bool** visited)
{
    int Size = board.Board_Size;
    Stack dfs_stack;

    dfs_stack.push(Start_row);
    dfs_stack.push(Start_column);


    while (!dfs_stack.isEmpty())
    {
        int column = dfs_stack.top_element();
        dfs_stack.pop();
        int row = dfs_stack.top_element();
        dfs_stack.pop();


        visited[row][column] = true;

        if (column == Size - 1)
        {
            return true;
        }

        int possible_row_moves[] = { 1, 1,0, 0, -1, -1 };
        int possible_column_moves[] = { 0, 1,1, -1, 0, -1 };

        for (int i = 0; i < 6; ++i)
        {
            int newRow = row + possible_row_moves[i];
            int newCol = column + possible_column_moves[i];
            if (newRow >= 0 && newRow < Size && newCol >= 0 && newCol < Size && board.square_board[newRow][newCol] == RED_PAWN && !visited[newRow][newCol]) {
                dfs_stack.push(newRow);
                dfs_stack.push(newCol);
            }
        }

    }

    return false;


}

bool isRConnected(Board& board) {
    int Size = board.Board_Size;
    bool** visited = new bool* [Size];
    for (int i = 0; i < Size; ++i) {
        visited[i] = new bool[Size] {false};
    }
    for (int i = 0; i < Size; ++i) {
        if (board.square_board[i][0] == RED_PAWN && !visited[i][0]) {
            if (DFS_RED(board, i, 0, visited)) {
                return true;

            }
        }
    }
    for (int i = 0; i < Size; ++i) {
        delete[] visited[i];
    }
    delete[] visited;
    return false;
}

bool isBConnected(Board& board) {
    int Size = board.Board_Size;
    bool** visited = new bool* [Size];
    for (int i = 0; i < Size; ++i) {
        visited[i] = new bool[Size] {false};
    }


    for (int i = 0; i < Size; ++i) {
        if (board.square_board[0][i] == BLUE_PAWN && !visited[0][i]) {
            if (DFS_BLUE(board, 0, i, visited)) {
                return true;
            }
        }
    }
    for (int i = 0; i < Size; ++i) {
        delete[] visited[i];
    }
    delete[] visited;
    return false;
}

void Check_Is_Board_Correct(Board& board, Questions& questions)
{
    if (board.red_pawns == board.blue_pawns + 1 || board.red_pawns == board.blue_pawns)
    {
        board.correct_board = true;
    }
    else
    {
        board.correct_board = false;
    }
    for (int i = 0; i < 16; i++)
    {
        if (board.board[board.row][i] != questions.is_board_correct_string[i])
        {
            break;
        }
        if (i == 15) {
            if (board.red_pawns == board.blue_pawns + 1 || board.red_pawns == board.blue_pawns)
            {
                std::cout << "YES" << std::endl;
            }
            else
            {
                board.correct_board = false;
                std::cout << "NO" << std::endl;
            }
        }
    }
}

void Answer_Is_Game_Over(Board& board, Questions& questions)
{
    for (int i = 0; i < 12; i++)
    {
        if (board.board[board.row][i] != questions.is_game_over_string[i])
        {
            break;
        }
        if (i == 11) {
            if (!board.correct_board)
            {
                std::cout << "NO" << std::endl;
            }
            else if (board.red_won)
            {
                std::cout << "YES RED" << std::endl;

            }
            else if (board.blue_won)
            {
                std::cout << "YES BLUE" << std::endl;
            }
            else
            {
                std::cout << "NO" << std::endl;
            }

        }
    }
}

void Check_Is_Game_Over(Board& board)
{

    if (board.end_board && !board.Game_over_answered) {
        board.red_won = isRConnected(board);
        board.blue_won = isBConnected(board);
        board.Game_over_answered = true;

    }

}

void Check_Is_Board_Possible(Board& board, Questions& questions)
{

    for (int i = 0; i < 17; i++)
    {
        if (board.board[board.row][i] != questions.is_board_possible_string[i])
        {
            break;
        }
        if (i == 16) {
            if (board.blue_won) {
                for (int x = 0; x < board.Board_Size; x++)
                {
                    for (int y = 0; y < board.Board_Size; y++)
                    {
                        if (board.square_board[x][y] == BLUE_PAWN)
                        {
                            board.square_board[x][y] = '#';
                            if (!isBConnected(board)) {
                                board.board_possible = true;
                            }
                            board.square_board[x][y] = BLUE_PAWN;
                        }
                    }
                }
            }
            else if (board.red_won){
                for (int x = 0; x < board.Board_Size; x++)
                {
                    for (int y = 0; y < board.Board_Size; y++)
                    {
                        if (board.square_board[x][y] == RED_PAWN)
                        {
                            board.square_board[x][y] = '#';
                            if (!isRConnected(board)) {
                                board.board_possible = true;
                            }
                            board.square_board[x][y] = RED_PAWN;
                        }
                    }
                }
            }
            else {
                if (board.correct_board) { board.board_possible = true; }
            }
            if (!board.board_possible || !board.correct_board || (board.red_won && board.blue_pawns == board.red_pawns) || (board.blue_won && board.red_pawns > board.blue_pawns))
            {
                std::cout << "NO" << std::endl;
            }
            else
            {
                std::cout << "YES" << std::endl;
            }
        }
    }
}

void Check_can_red_win_in_1_move_with_naive_oppponent(Board& board, Questions& questions)
{
    int temp_size = board.Board_Size;
    for (int i = 0; i < 41; i++)
    {
        if (board.board[board.row][i] != questions.can_red_win_in_1_move_with_naive_oppponent[i])
        {
            break;
        }
        if (i == 40)
        {
            if (!board.blue_won && !board.red_won && board.end_board && board.correct_board)
            {
                if (board.blue_pawns == board.red_pawns)
                {
                    board.turn = RED_PAWN;
                }
                else {
                    board.turn = BLUE_PAWN;
                }

                if (board.turn == RED_PAWN)
                {
                    if (board.number_of_empty_places > 0 && board.red_pawns >= temp_size - 1)
                    {
                        for (int x = 0; x < temp_size; x++)
                        {
                            for (int y = 0; y < temp_size; y++)
                            {
                                if (board.square_board[x][y] == '#')
                                {
                                    board.square_board[x][y] = RED_PAWN;
                                    board.can_red_win_in_1_move = isRConnected(board);
                                    board.square_board[x][y] = '#';
                                }
                                if (board.can_red_win_in_1_move)
                                {
                                    break;
                                }
                            }
                            if (board.can_red_win_in_1_move)
                            {
                                break;
                            }
                        }
                    }
                }
                else
                {
                    if (board.number_of_empty_places > 1 && board.red_pawns >= temp_size - 1)
                    {
                        for (int x = 0; x < temp_size; x++)
                        {
                            for (int y = 0; y < temp_size; y++)
                            {
                                if (board.square_board[x][y] == '#')
                                {
                                    board.square_board[x][y] = RED_PAWN;
                                    board.can_red_win_in_1_move = isRConnected(board);
                                    board.square_board[x][y] = '#';
                                }
                                if (board.can_red_win_in_1_move)
                                {
                                    break;
                                }
                            }
                            if (board.can_red_win_in_1_move)
                            {
                                break;
                            }
                        }
                    }
                }

            }
            if (board.can_red_win_in_1_move)
            {
                std::cout << "YES" << std::endl;

                //outputFile << "YES" << std::endl;
            }
            else
            {
                std::cout << "NO" << std::endl;
                //outputFile << "NO" << std::endl;

            }

        }
    }





}

void Check_can_blue_win_in_1_move_with_naive_oppponent(Board& board, Questions& questions)
{
    int temp_size = board.Board_Size;
    for (int i = 0; i < 42; i++)
    {
        if (board.board[board.row][i] != questions.can_blue_win_in_1_move_with_naive_oppponent[i])
        {
            break;
        }
        if (i == 41)
        {
            if (!board.blue_won && !board.red_won && board.end_board && board.correct_board)
            {
                if (board.blue_pawns == board.red_pawns)
                {
                    board.turn = RED_PAWN;
                }
                else {
                    board.turn = BLUE_PAWN;
                }

                if (board.turn == BLUE_PAWN)
                {
                    if (board.number_of_empty_places > 0 && board.blue_pawns >= temp_size - 1)
                    {
                        for (int x = 0; x < temp_size; x++)
                        {
                            for (int y = 0; y < temp_size; y++)
                            {
                                if (board.square_board[x][y] == '#')
                                {
                                    board.square_board[x][y] = BLUE_PAWN;
                                    board.can_blue_win_in_1_move = isBConnected(board);
                                    board.square_board[x][y] = '#';
                                }
                                if (board.can_blue_win_in_1_move)
                                {
                                    break;
                                }
                            }
                            if (board.can_blue_win_in_1_move)
                            {
                                break;
                            }
                        }
                    }
                }
                else
                {
                    if (board.number_of_empty_places > 1 && board.blue_pawns >= temp_size - 1)
                    {
                        for (int x = 0; x < temp_size; x++)
                        {
                            for (int y = 0; y < temp_size; y++)
                            {
                                if (board.square_board[x][y] == '#')
                                {
                                    board.square_board[x][y] = BLUE_PAWN;
                                    board.can_blue_win_in_1_move = isBConnected(board);
                                    board.square_board[x][y] = '#';
                                }
                                if (board.can_blue_win_in_1_move)
                                {
                                    break;
                                }
                            }
                            if (board.can_blue_win_in_1_move)
                            {
                                break;
                            }
                        }
                    }
                }

            }
            if (board.can_blue_win_in_1_move)
            {
                std::cout << "YES" << std::endl;
                //outputFile << "YES" << std::endl;
            }
            else
            {
                std::cout << "NO" << std::endl;
                //outputFile << "NO" << std::endl;

            }

        }
    }

}

void Check_can_blue_win_in_2_move_with_naive_oppponent(Board& board, Questions& questions)
{
    int temp_size = board.Board_Size;
    bool blue_in_1_move = false;
    int blue = 0;
    for (int i = 0; i < 42; i++)
    {
        if (board.board[board.row][i] != questions.can_blue_win_in_2_move_with_naive_oppponent[i])
        {
            break;
        }
        if (i == 41)
        {
            if (board.correct_board && !board.blue_won && !board.red_won && board.end_board)
            {
                if (board.blue_pawns == board.red_pawns)
                {
                    board.turn = RED_PAWN;
                }
                else {
                    board.turn = BLUE_PAWN;
                }


                if (board.turn == BLUE_PAWN)
                {
                    if (board.number_of_empty_places > 2 && board.blue_pawns >= temp_size - 2)
                    {
                        for (int x = 0; x < temp_size; x++)
                        {
                            for (int y = 0; y < temp_size; y++)
                            {
                                if (board.square_board[x][y] == '#')
                                {
                                    board.square_board[x][y] = BLUE_PAWN;
                                    if (isBConnected(board))
                                    {
                                        blue++;
                                        board.square_board[x][y] = '#';
                                        continue;
                                    }
                                    for (int s = 0; s < temp_size; s++)
                                    {
                                        for (int z = 0; z < temp_size; z++)
                                        {
                                            if (board.square_board[s][z] == '#')
                                            {
                                                board.square_board[s][z] = BLUE_PAWN;
                                                board.can_blue_win_in_2_move = isBConnected(board);
                                                board.square_board[s][z] = '#';
                                            }
                                            if (board.can_blue_win_in_2_move)
                                            {
                                                break;

                                            }
                                        }
                                        if (board.can_blue_win_in_2_move)
                                        {
                                            break;

                                        }
                                    }
                                    board.square_board[x][y] = '#';
                                }

                                if (board.can_blue_win_in_2_move)
                                {
                                    break;

                                }
                            }
                            if (board.can_blue_win_in_2_move)
                            {
                                break;

                            }
                        }
                    }
                }
                else
                {
                    if (board.number_of_empty_places > 3 && board.blue_pawns >= temp_size - 2)
                    {
                        for (int x = 0; x < temp_size; x++)
                        {
                            for (int y = 0; y < temp_size; y++)
                            {
                                if (board.square_board[x][y] == '#')
                                {
                                    board.square_board[x][y] = BLUE_PAWN;
                                    if (isBConnected(board))
                                    {
                                        blue++;
                                        board.square_board[x][y] = '#';
                                        continue;
                                    }
                                    for (int s = 0; s < temp_size; s++)
                                    {
                                        for (int z = 0; z < temp_size; z++)
                                        {
                                            if (board.square_board[s][z] == '#')
                                            {
                                                board.square_board[s][z] = BLUE_PAWN;
                                                board.can_blue_win_in_2_move = isBConnected(board);
                                                board.square_board[s][z] = '#';
                                            }
                                            if (board.can_blue_win_in_2_move)
                                            {
                                                break;

                                            }
                                        }
                                        if (board.can_blue_win_in_2_move)
                                        {
                                            break;

                                        }
                                    }
                                    board.square_board[x][y] = '#';
                                }

                                if (board.can_blue_win_in_2_move)
                                {
                                    break;

                                }
                            }
                            if (board.can_blue_win_in_2_move)
                            {
                                break;

                            }
                        }
                    }
                }
            }
            if (board.can_blue_win_in_2_move && blue < board.number_of_empty_places)
            {
                std::cout << "YES" << std::endl;
            }
            else
            {
                std::cout << "NO" << std::endl;

            }

        }
    }

}

void Check_can_red_win_in_2_move_with_naive_oppponent(Board& board, Questions& questions)
{
    int temp_size = board.Board_Size;
    bool red_in_1_move = false;
    int red = 0;
    for (int i = 0; i < 41; i++)
    {
        if (board.board[board.row][i] != questions.can_red_win_in_2_move_with_naive_oppponent[i])
        {
            break;
        }
        if (i == 40)
        {
            if (board.correct_board && !board.blue_won && !board.red_won && board.end_board)
            {
                if (board.blue_pawns == board.red_pawns)
                {
                    board.turn = RED_PAWN;
                }
                else {
                    board.turn = BLUE_PAWN;
                }


                if (board.turn == RED_PAWN)
                {
                    if (board.number_of_empty_places > 2 && board.red_pawns >= temp_size - 2)
                    {
                        for (int x = 0; x < temp_size; x++)
                        {
                            for (int y = 0; y < temp_size; y++)
                            {
                                if (board.square_board[x][y] == '#')
                                {
                                    board.square_board[x][y] = RED_PAWN;
                                    if (isRConnected(board))
                                    {
                                        red++;
                                        board.square_board[x][y] = '#';
                                        continue;
                                    }

                                    for (int s = 0; s < temp_size; s++)
                                    {
                                        for (int z = 0; z < temp_size; z++)
                                        {
                                            if (board.square_board[s][z] == '#')
                                            {
                                                board.square_board[s][z] = RED_PAWN;
                                                board.can_red_win_in_2_move = isRConnected(board);
                                                board.square_board[s][z] = '#';
                                            }
                                            if (board.can_red_win_in_2_move)
                                            {
                                                break;

                                            }
                                        }
                                        if (board.can_red_win_in_2_move)
                                        {
                                            break;

                                        }
                                    }
                                    board.square_board[x][y] = '#';
                                }

                                if (board.can_red_win_in_2_move)
                                {
                                    break;

                                }
                            }
                            if (board.can_red_win_in_2_move)
                            {
                                break;

                            }
                        }
                    }
                }
                else
                {
                    if (board.number_of_empty_places > 3 && board.red_pawns >= temp_size - 2)
                    {
                        for (int x = 0; x < temp_size; x++)
                        {
                            for (int y = 0; y < temp_size; y++)
                            {
                                if (board.square_board[x][y] == '#')
                                {
                                    board.square_board[x][y] = RED_PAWN;
                                    if (isRConnected(board))
                                    {
                                        red++;
                                        board.square_board[x][y] = '#';
                                        continue;
                                    }
                                    for (int s = 0; s < temp_size; s++)
                                    {
                                        for (int z = 0; z < temp_size; z++)
                                        {
                                            if (board.square_board[s][z] == '#')
                                            {
                                                board.square_board[s][z] = RED_PAWN;
                                                board.can_red_win_in_2_move = isRConnected(board);
                                                board.square_board[s][z] = '#';
                                            }
                                            if (board.can_red_win_in_2_move)
                                            {
                                                break;

                                            }
                                        }
                                        if (board.can_red_win_in_2_move)
                                        {
                                            break;

                                        }
                                    }
                                    board.square_board[x][y] = '#';
                                }

                                if (board.can_red_win_in_2_move)
                                {
                                    break;

                                }
                            }
                            if (board.can_red_win_in_2_move)
                            {
                                break;

                            }
                        }
                    }
                }
            }

            if (board.can_red_win_in_2_move && red < board.number_of_empty_places)
            {
                std::cout << "YES" << std::endl;
                //outputFile << "YES" << std::endl;
            }
            else
            {
                std::cout << "NO" << std::endl;
                //outputFile << "NO" << std::endl;

            }

        }
    }
}

void Reset_Board_Row(Board& board)
{
    if (board.end_board)
    {
        board.row = 0;
    }
}

void Characters_In_Line(Board& board)
{
    for (int i = 0; i < MAX_WIDTH; i++)
    {
        if (board.board[board.row][i] == RED_PAWN)
        {
            board.red_pawns++;
            board.pawns_number++;
        }
        if (board.board[board.row][i] == BLUE_PAWN)
        {
            board.blue_pawns++;
            board.pawns_number++;
        }
        if (board.board[board.row][i] == DASH)
        {
            board.start_board_int++;
        }
        else
        {
            board.start_board_int = 0;
        }
        if (board.board[board.row][i] == END_LINE)
        {
            break;
        }
        if (board.start_board && board.start_board_int == NUMBER_OF_DASHES_START_END)
        {
            create_temporary_board(board);
            create_square_board(board);
            clear_temp_board(board);
            board.end_board = true;
            board.start_board = false;
        }
        else if (board.start_board_int == NUMBER_OF_DASHES_START_END)
        {
            board.ResetBoardVariables();
            clear_square_board(board);

        }

    }
}

int main() {
    Board board;
    Questions questions;
    while (fgets(board.board[board.row], MAX_WIDTH, stdin)) {
        if (board.board[board.row][0] == '<') { board.Board_Size = count_size_of_a_board(board.board[board.row]); }
        Characters_In_Line(board);//ADDS RED_PAWNS, BLUE_PAWNS, PAWNS_NUMBER, AND CHECKS IF BOARD HAS STARTED/ENDED
        Check_Board_Size(board, questions);
        Check_Pawns_Number(board, questions);
        Check_Is_Board_Correct(board, questions);
        Check_Is_Game_Over(board);
        Answer_Is_Game_Over(board, questions);
        Check_Is_Board_Possible(board, questions);
        Check_can_red_win_in_1_move_with_naive_oppponent(board, questions);
        Check_can_blue_win_in_1_move_with_naive_oppponent(board, questions);
        Check_can_red_win_in_2_move_with_naive_oppponent(board, questions);
        Check_can_blue_win_in_2_move_with_naive_oppponent(board, questions);
        board.row++;
        Reset_Board_Row(board);
    }
    return 0;
}