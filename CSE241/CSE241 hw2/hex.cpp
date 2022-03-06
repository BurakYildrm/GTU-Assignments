#include <iostream>
#include <fstream>
#include <string>
#include "hex.h"
using namespace std;
bool isEmpty(char board[12][12], int x, char y){    //to determine if the selected node is empty
    if(board[x - 1][static_cast<int>(y) - 97] == Empty){
        return true;
    }
    else{
        return false;
    }
}
bool isConnected(char board[12][12], cell_state player, int board_size){  //to determine if a player or computer has a node on both sides
    int check1 = 0, check2 = 0;
    if(player == Player1){
        for(int i = 0; i < board_size && check1 == 0; i++){
            if(board[i][0] == player){
                check1++;
            }
        }
        for(int i = 0; i < board_size && check2 == 0; i++){
            if(board[i][board_size - 1] == player){
                check2++;
            }
        }
    }
    else{
        for(int i = 0; i < board_size && check1 == 0; i++){
            if(board[0][i] == player){
                check1++;
            }
        }
        for(int i = 0; i < board_size && check2 == 0; i++){
            if(board[board_size - 1][i] == player){
                check2++;
            }
        }
    }
    if(check1 == 1 && check2 == 1){
        return true;
    }
    else{
        return false;
    }
}
bool isWinner_P1(char board[12][12], int board_size, cell_state player, int x, int y, int rotation, int past_coordinates_x[73], int past_coordinates_y[73]){
    if(board[x][y] != player){
        return false;
    }
    else if(board[x][y] == player && y == board_size - 1){
        board[x][y] -= 32;
        return true;
    }
    else{
        //function determines the coordinates of the previous node by finding the opposite rotation of current one and records these coordinates so that function doesn't check the same coordinates over and over again and this prevents the function from getting into an infinite loop
        if(rotation == 1){
            past_coordinates_x[sizeofarray(past_coordinates_x)] = x + 1;
            past_coordinates_y[sizeofarray(past_coordinates_y)] = y;
        }
        if(rotation == 2){
            past_coordinates_x[sizeofarray(past_coordinates_x)] = x + 1;
            past_coordinates_y[sizeofarray(past_coordinates_y)] = y - 1;
        }
        if(rotation == 3){
            past_coordinates_x[sizeofarray(past_coordinates_x)] = x;
            past_coordinates_y[sizeofarray(past_coordinates_y)] = y - 1;
        }
        if(rotation == 4){
            past_coordinates_x[sizeofarray(past_coordinates_x)] = x - 1;
            past_coordinates_y[sizeofarray(past_coordinates_y)] = y;
        }
        if(rotation == 5){
            past_coordinates_x[sizeofarray(past_coordinates_x)] = x - 1;
            past_coordinates_y[sizeofarray(past_coordinates_y)] = y + 1;
        }
        if(rotation == 6){
            past_coordinates_x[sizeofarray(past_coordinates_x)] = x;
            past_coordinates_y[sizeofarray(past_coordinates_y)] = y + 1;
        }
        //rotations starts from up and goes on clockwise
        //if isWinner_P1 is true it uppercases the current node so that only the winning path is uppercased 
        if(x - 1 >= 0 && check_coordinates(past_coordinates_x, past_coordinates_y, x - 1, y) == true){
            rotation = 1;
            if(isWinner_P1(board, board_size, player, x - 1, y, 1, past_coordinates_x, past_coordinates_y) == true){
                if(board[x][y] == Player1){
                    board[x][y] -= 32;
                }
                return true;
            }
        }
        if(x - 1 >= 0 && y + 1 <= board_size - 1 && check_coordinates(past_coordinates_x, past_coordinates_y, x - 1, y + 1) == true){
            rotation = 2;
            if(isWinner_P1(board, board_size, player, x - 1, y + 1, 2, past_coordinates_x, past_coordinates_y) == true){
                if(board[x][y] == Player1){
                    board[x][y] -= 32;
                }
                return true;
            }
        }
        if(y + 1 <= board_size - 1 && check_coordinates(past_coordinates_x, past_coordinates_y, x, y + 1) == true){
            rotation = 3;
            if(isWinner_P1(board, board_size, player, x, y + 1, 3, past_coordinates_x, past_coordinates_y) == true){
                if(board[x][y] == Player1){
                    board[x][y] -= 32;
                }
                return true;
            }
        }
        if(x + 1 <= board_size - 1 && check_coordinates(past_coordinates_x, past_coordinates_y, x + 1, y) == true){
            rotation = 4;
            if(isWinner_P1(board, board_size, player, x + 1, y, 4, past_coordinates_x, past_coordinates_y) == true){
                if(board[x][y] == Player1){
                    board[x][y] -= 32;
                }
                return true;
            }
        }
        if(x + 1 <= board_size - 1 && y - 1 >= 0 && check_coordinates(past_coordinates_x, past_coordinates_y, x + 1, y - 1) == true){
            rotation = 5;
            if(isWinner_P1(board, board_size, player, x + 1, y - 1, 5, past_coordinates_x, past_coordinates_y) == true){
                if(board[x][y] == Player1){
                    board[x][y] -= 32;
                }
                return true;
            }
        }
        if(y - 1 >= 0 && check_coordinates(past_coordinates_x, past_coordinates_y, x, y - 1) == true){
            rotation = 6;
            if(isWinner_P1(board, board_size, player, x, y - 1, 6, past_coordinates_x, past_coordinates_y) == true){
                if(board[x][y] == Player1){
                    board[x][y] -= 32;
                }
                return true;
            }
        }
        return false;
    }
}
bool isWinner_P2(char board[12][12], int board_size, cell_state player, int x, int y, int rotation, int past_coordinates_x[73], int past_coordinates_y[73]){
    if(board[x][y] != player){
        return false;
    }
    else if(board[x][y] == player && x == board_size - 1){
        board[x][y] -= 32;
        return true;
    }
    else{
        //function determines the coordinates of the previous node by finding the opposite rotation of current one and records these coordinates so that function doesn't check the same coordinates over and over again and this prevents the function from getting into an infinite loop
        if(rotation == 1){
            past_coordinates_x[sizeofarray(past_coordinates_x)] = x;
            past_coordinates_y[sizeofarray(past_coordinates_y)] = y - 1;
        }
        if(rotation == 2){
            past_coordinates_x[sizeofarray(past_coordinates_x)] = x - 1;
            past_coordinates_y[sizeofarray(past_coordinates_y)] = y;
        }
        if(rotation == 3){
            past_coordinates_x[sizeofarray(past_coordinates_x)] = x - 1;
            past_coordinates_y[sizeofarray(past_coordinates_y)] = y + 1;
        }
        if(rotation == 4){
            past_coordinates_x[sizeofarray(past_coordinates_x)] = x;
            past_coordinates_y[sizeofarray(past_coordinates_y)] = y + 1;
        }
        if(rotation == 5){
            past_coordinates_x[sizeofarray(past_coordinates_x)] = x + 1;
            past_coordinates_y[sizeofarray(past_coordinates_y)] = y;
        }
        if(rotation == 6){
            past_coordinates_x[sizeofarray(past_coordinates_x)] = x + 1;
            past_coordinates_y[sizeofarray(past_coordinates_y)] = y - 1;
        }
        //rotations starts from right and goes on clockwise
        //if isWinner_P2 is true it uppercases the current node so that only the winning path is uppercased 
        if(y + 1 <= board_size - 1 && check_coordinates(past_coordinates_x, past_coordinates_y, x, y + 1) == true){
            rotation = 1;
            if(isWinner_P2(board, board_size, player, x, y + 1, 1, past_coordinates_x, past_coordinates_y) == true){
                if(board[x][y] == Player2){
                    board[x][y] -= 32;
                }
                return true;
            }
        }
        if(x + 1 <= board_size - 1 && check_coordinates(past_coordinates_x, past_coordinates_y, x + 1, y) == true){
            rotation = 2;
            if(isWinner_P2(board, board_size, player, x + 1, y, 2, past_coordinates_x, past_coordinates_y) == true){
                if(board[x][y] == Player2){
                    board[x][y] -= 32;
                }
                return true;
            }
        }
        if(x + 1 <= board_size - 1 && y - 1 >= 0 && check_coordinates(past_coordinates_x, past_coordinates_y, x + 1, y - 1) == true){
            rotation = 3;
            if(isWinner_P2(board, board_size, player, x + 1, y - 1, 3, past_coordinates_x, past_coordinates_y) == true){
                if(board[x][y] == Player2){
                    board[x][y] -= 32;
                }
                return true;
            }
        }
        if(y - 1 >= 0 && check_coordinates(past_coordinates_x, past_coordinates_y, x, y - 1) == true){
            rotation = 4;
            if(isWinner_P2(board, board_size, player, x, y - 1, 4, past_coordinates_x, past_coordinates_y) == true){
                if(board[x][y] == Player2){
                    board[x][y] -= 32;
                }
                return true;
            }
        }
        if(x - 1 >= 0 && check_coordinates(past_coordinates_x, past_coordinates_y, x - 1, y) == true){
            rotation = 5;
            if(isWinner_P2(board, board_size, player, x - 1, y, 5, past_coordinates_x, past_coordinates_y) == true){
                if(board[x][y] == Player2){
                    board[x][y] -= 32;
                }
                return true;
            }
        }
        if(x - 1 >= 0 && y + 1 <= board_size - 1 && check_coordinates(past_coordinates_x, past_coordinates_y, x - 1, y + 1) == true){
            rotation = 6;
            if(isWinner_P2(board, board_size, player, x - 1, y + 1, 6, past_coordinates_x, past_coordinates_y) == true){
                if(board[x][y] == Player2){
                    board[x][y] -= 32;
                }
                return true;
            }
        }
        return false;
    }
}
void print_board(char board[12][12], int board_size){//to print the playing board
    cout << endl <<  "  ";
    for(int i = 97, k = 0; k < board_size; k++){
        cout << static_cast<char>(i++) << " ";
    }
    cout << endl;
    for(int i = 0; i < board_size; i++){
        cout << i + 1;
        if(i >= 9){//to aline the rows
            for(int k = 1; k < i + 1; k++){
                cout << " ";
            }
        }
        else{
            for(int k = 0; k < i + 1; k++){
                cout << " ";
            }
        }
        for(int j = 0; j < board_size; j++){
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}
void find_beginning(char board[12][12], int board_size, cell_state player, int coordinates[13]){//to find the beginning point/points of the isWinner functions
    int j = 0;
    if(player == Player1){
        for(int i = 0; i < board_size; i++){
            if(board[i][0] == player){
                coordinates[j] = i;
                j++;
            }
        }
    }
    else{
        for(int i = 0; i < board_size; i++){
            if(board[0][i] == player){
                coordinates[j] = i;
                j++;
            }
        }
    }
}
int sizeofarray(int coordinates[]){
    int counter = 0;
    for(int i = 0; coordinates[i] != -1; i++){
        counter++;
    }
    return counter;
}
bool check_coordinates(int past_coordinates_x[73], int past_coordinates_y[73], int x, int y){//checks if the node is already been visited by isWinner function 
    for(int i = 0; i < 73; i++){
        if(past_coordinates_x[i] == x && past_coordinates_y[i] == y){
            return false;
        }
    }
    return true;
}
//bot blocks the player's moves rightward
//prioritization is first right and top rigth then up and down then left and bottom left
int block_player(char board[12][12], int x, int y){
    if(board[x][y + 1] == Empty){
        return 1;
    }
    else if(board[x - 1][y + 1] == Empty){
        return 2;
    }
    else if(board[x + 1][y] == Empty){
        return 3;
    }
    else if(board[x - 1][y] == Empty){
        return 4;
    }
    else if(board[x + 1][y - 1] == Empty){
        return 5;
    }
    else if(board[x][y - 1] == Empty){
        return 6;
    }
    else{//returns seven if there is no place near the player
        return 7;
    }
}
//finds a random place on the right side of the player
void find_cell(char board[12][12], int board_size, int y, int* cell_x, int* cell_y){
    if(y + 1 == board_size){
        for(int i = 0; i < board_size; i++){
            for(int j = 0; j < board_size; j++){
                if(board[i][j] == Empty){
                    *cell_x = j;
                    *cell_y = i;
                    break;
                }
            }
        }
    }
    else{
        for(int i = y + 1; i < board_size; i++){
            for(int j = 0; j < board_size; j++){
                if(board[j][i] == Empty){
                    *cell_x = j;
                    *cell_y = i;
                    break;
                }
            }
        }
    }
}
bool isFull(char board[12][12], int board_size){//checks if the playing is full
    for(int i = 0; i < board_size; i++){
        for(int j = 0; j < board_size; j++){
            if(board[i][j] == Empty){
                return false;
            }
        }
    }
    return true;
}
void load_game(char board[12][12], int& board_size, int& game_mode, int& player_turn, string filename){
    string s;
    ifstream fin;
    fin.open(filename);
    if(fin){    //checks if the file is opened properly
        //reads size of the board, game mode(player vs player or player vs computer), player turn(player1 or player2) and the board itself in the given order
        getline(fin, s);
        board_size = stoi(s);
        getline(fin, s);
        game_mode = stoi(s);
        getline(fin, s);
        player_turn = stoi(s) - 1;
        getline(fin, s);
        for(int i = 0, k = 0; i < board_size; i++){
            for(int j = 0; j < board_size; j++, k++){
                board[i][j] = s[k];
            }
        }
    }
    else{    //if the file is not opened it prints an error message
        cerr << "An error occured while opening " << filename << endl;
    }
    fin.close();
}
void save_game(const char board[12][12], const int& board_size, const int& game_mode, const int& player_turn, string filename){
    ofstream fout;
    fout.open(filename);
    if(fout){   //checks if the file is opened properly
        //writes size of the board, game mode(player vs player or player vs computer), player turn(player1 or player2) and the board itself in the given order
        fout << board_size << endl << game_mode << endl << player_turn%2 + 1 << endl;
        for(int i = 0; i < board_size; i++){
            for(int j = 0; j < board_size; j++){
                fout << board[i][j];
            }
        }
    }
    else{   //if the file is not opened it prints an error message
        cerr << "An error occured while opening " << filename << endl;
    }
    fout.close();
}