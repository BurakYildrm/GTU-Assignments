#include <iostream>
#include <fstream> 
#include <string>
#include "hex.h"
using namespace std;
int main(){
    //maximum possible number of beginning points is 12 but the beginning arrays are initialized with 13 slots for sizeofarray function to determine the size of array beacuse the last member of array must be -1 to determine the size
    string coordinates;
    char board[12][12];
    char y;
    int x;
    decltype(x) p1_first_coordinates[13], p2_first_coordinates[13], board_size, p1_past_coordinates_x[73], p1_past_coordinates_y[73], p2_past_coordinates_x[73], p2_past_coordinates_y[73], game_mode, cell_x, cell_y;
    auto file = false, invalid = false;
    for(int i = 0; i < 73; i++){
        p1_past_coordinates_x[i] = -1;
        p1_past_coordinates_y[i] = -1;
        p2_past_coordinates_x[i] = -1;
        p2_past_coordinates_y[i] = -1;
        if(i < 13){
            p1_first_coordinates[i] = -1;
            p2_first_coordinates[i] = -1;
        }
    }
    do{
        cout << "Which size do you want your board to be?(min. 6, max. 12): ";
        cin >> board_size;
        cin.clear();
        cin.ignore(100,'\n');
    }
    while(board_size < 6 || board_size > 12);
    for(int i = 0; i < board_size; i++){
        for(int j = 0; j < board_size; j++){
            board[i][j] = Empty;
        }
    }
    cout << "\nSelect a game mode\n1)Player vs Player\n2)Player vs Computer\n";
    cin >> game_mode;
    cin.clear();
    cin.ignore(100,'\n');
    for(int i = 0; game_mode != 1 && game_mode != 2; i++){
        cout << "Invalid operation. Please enter a valid operation: ";
        cin >> game_mode;
        cin.clear();
        cin.ignore(100,'\n');
    }
    for(int i = 0, check = 0; check == 0; i++){
        if(game_mode == 1){
            file = false;
            if(isFull(board, board_size) == true){
                cout << "BOARD IS FULL. NO ONE WINS" << endl;
                check++;
            }
            else{
                print_board(board, board_size);
                do{
                    invalid = false;
                    if(i%2 == 0){
                        cout << endl << "Player1's move(use lowercase letters): ";
                    }
                    else{
                        cout << endl << "Player2's move(use lowercase letters): ";
                    }
                    getline(cin, coordinates);
                    if(coordinates.substr(0, 4) == "LOAD"){
                        if(coordinates.length() == 4){  //checks if the user entered a file name
                            load_game(board, board_size, game_mode, i);
                        }
                        else{
                            load_game(board, board_size, game_mode, i, coordinates.substr(5, coordinates.length() - 5));
                        }
                        i--;
                        file = true;
                        break;
                    }
                    else if(coordinates.substr(0, 4) == "SAVE"){
                        if(coordinates.length() == 4){  //checks if the user entered a file name
                            save_game(board, board_size, game_mode, i);
                        }
                        else{
                            save_game(board, board_size, game_mode, i, coordinates.substr(5, coordinates.length() - 5));
                        }
                        i--;
                        file = true;
                        break;
                    }
                    else if((coordinates[0] >= 'a' && coordinates[0] <= 'l') && coordinates.length() >= 2){
                        if(static_cast<int>(coordinates[1]) - 48 >= 1 && static_cast<int>(coordinates[1]) - 48 <= 12){
                            x = stoi(coordinates.substr(1, coordinates.length() - 1));
                            y = coordinates[0];
                        }
                    }
                    else{
                        invalid = true;
                    }    
                }while(isEmpty(board, x, y) == false || invalid == true);
                if(!file){
                    if(i%2 == 0){
                        board[x - 1][static_cast<int>(y) - 97] = Player1;
                    }
                    else{
                        board[x - 1][static_cast<int>(y) - 97] = Player2;
                    }
                    if(i%2 == 0){
                        if(isConnected(board, Player1, board_size) == true){//if player has a node on both sides code begins to find a way to link both sides
                            find_beginning(board, board_size, Player1, p1_first_coordinates);
                            for(int i = 0; i < sizeofarray(p1_first_coordinates); i++){
                                if(isWinner_P1(board, board_size, Player1, p1_first_coordinates[i], 0, 0, p1_past_coordinates_x, p1_past_coordinates_y) == true){
                                    print_board(board, board_size);
                                    cout << endl << "PLAYER1 WINS!!!" << endl;
                                    check++;
                                }
                                for(int i = 0; i < 73; i++){
                                    p1_past_coordinates_x[i] = -1;
                                    p1_past_coordinates_y[i] = -1;
                                }
                            }
                        }
                    }
                    else{
                        if(isConnected(board, Player2, board_size) == true){
                            find_beginning(board, board_size, Player2, p2_first_coordinates);
                            for(int i = 0; i < sizeofarray(p2_first_coordinates); i++){
                                if(isWinner_P2(board, board_size, Player2, 0, p2_first_coordinates[i], 0, p2_past_coordinates_x, p2_past_coordinates_y) == true){
                                    print_board(board, board_size);
                                    cout << endl << "PLAYER2 WINS!!!" << endl;
                                    check++;
                                }
                                for(int i = 0; i < 73; i++){
                                    p2_past_coordinates_x[i] = -1;
                                    p2_past_coordinates_y[i] = -1;
                                }
                            }
                        }
                    }
                }
            }
        }
        else{
            if(isFull(board, board_size) == true){
                cout << "BOARD IS FULL. NO ONE WINS" << endl;
                check++;
            }
            else{
                if(i%2 == 1){
                    if(!file){
                    //bot's aim is to prevent player from winning
                    //bot blocks the player's moves rightward
                    //prioritization is first right and top rigth then up and down then left and bottom left
                        print_board(board, board_size);
                        if(block_player(board, x - 1, static_cast<int>(y) - 97) == 1){
                            board[x - 1][static_cast<int>(y) - 96] = Player2;
                            cout << "\nComputer's move: " << static_cast<char>(y + 1) << x << endl;
                        }
                        else if(block_player(board, x - 1, static_cast<int>(y) - 97) == 2){
                            board[x - 2][static_cast<int>(y) - 96] = Player2;
                            cout << "\nComputer's move: " << static_cast<char>(y + 1) << x - 1 << endl;
                        }
                        else if(block_player(board, x - 1, static_cast<int>(y) - 97) == 3){
                            board[x][static_cast<int>(y) - 97] = Player2;
                            cout << "\nComputer's move: " << static_cast<char>(y) << x + 1 << endl;
                        }
                        else if(block_player(board, x - 1, static_cast<int>(y) - 97) == 4){
                            board[x - 2][static_cast<int>(y) - 97] = Player2;
                            cout << "\nComputer's move: " << static_cast<char>(y) << x - 1 << endl;
                        }
                        else if(block_player(board, x - 1, static_cast<int>(y) - 97) == 5){
                            board[x][static_cast<int>(y) - 98] = Player2;
                            cout << "\nComputer's move: " << static_cast<char>(y - 1) << x + 1 << endl;
                        }
                        else if(block_player(board, x - 1, static_cast<int>(y) - 97) == 6){
                            board[x - 1][static_cast<int>(y) - 98] = Player2;
                            cout << "\nComputer's move: " << static_cast<char>(y - 1) << x << endl;
                        }
                        else{
                            find_cell(board, board_size, static_cast<int>(y) - 97, &cell_x, &cell_y);
                            board[cell_x][cell_y] = Player2;
                            cout << "\nComputer's move: " << static_cast<char>(cell_y + 97) << cell_x + 1 << endl;
                        }
                    }
                    file = false;
                }
                else{
                    print_board(board, board_size);
                    file = false;
                    do{
                        invalid = false;
                        cout << endl << "Player's move(use lowercase letters): ";
                        getline(cin, coordinates);
                        if(coordinates.substr(0, 4) == "LOAD"){
                            if(coordinates.length() == 4){  //checks if the user entered a file name
                                load_game(board, board_size, game_mode, i);
                            }
                            else{
                                load_game(board, board_size, game_mode, i, coordinates.substr(5, coordinates.length() - 5));
                            }
                            i--;
                            file = true;
                            break;
                        }
                        else if(coordinates.substr(0, 4) == "SAVE"){
                            if(coordinates.length() == 4){  //checks if the user entered a file name
                                save_game(board, board_size, game_mode, i);
                            }
                            else{
                                save_game(board, board_size, game_mode, i, coordinates.substr(5, coordinates.length() - 5));
                            }
                            i--;
                            file = true;
                            break;
                        }
                        else if((coordinates[0] >= 'a' && coordinates[0] <= 'l') && coordinates.length() >= 2){
                            if(static_cast<int>(coordinates[1]) - 48 >= 1 && static_cast<int>(coordinates[1]) - 48 <= 12){
                                x = stoi(coordinates.substr(1, coordinates.length() - 1));
                                y = coordinates[0];
                            }
                        }
                        else{
                            invalid = true;
                        }
                    }while(isEmpty(board, x, y) == false);
                    if(!file){
                        board[x - 1][static_cast<int>(y) - 97] = Player1;
                    }
                }
                if(!file){
                    if(i%2 == 0){
                        if(isConnected(board, Player1, board_size) == true){
                            find_beginning(board, board_size, Player1, p1_first_coordinates);
                            for(int i = 0; i < sizeofarray(p1_first_coordinates); i++){
                                if(isWinner_P1(board, board_size, Player1, p1_first_coordinates[i], 0, 0, p1_past_coordinates_x, p1_past_coordinates_y) == true){
                                    print_board(board, board_size);
                                    cout << endl << "PLAYER WINS!!!" << endl;
                                    check++;
                                }
                                for(int i = 0; i < 73; i++){
                                    p1_past_coordinates_x[i] = -1;
                                    p1_past_coordinates_y[i] = -1;
                                }
                            }
                        }
                    }
                    else{
                        if(isConnected(board, Player2, board_size) == true){
                            find_beginning(board, board_size, Player2, p2_first_coordinates);
                            for(int i = 0; i < sizeofarray(p2_first_coordinates); i++){
                                if(isWinner_P2(board, board_size, Player2, 0, p2_first_coordinates[i], 0, p2_past_coordinates_x, p2_past_coordinates_y) == true){
                                    print_board(board, board_size);
                                    cout << endl << "COMPUTER WINS!!!" << endl;
                                    check++;
                                }
                                for(int i = 0; i < 73; i++){
                                    p2_past_coordinates_x[i] = -1;
                                    p2_past_coordinates_y[i] = -1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}