#include <iostream>
#include <fstream>
#include "HexVector.h"

using namespace std;
using namespace HexVec;
using namespace Abstract;
using namespace Exceptions;

/*prints the current board*/
void HexVector::print(){
    cout << endl <<  "  ";
    for(int i = 97, k = 0; k < getSize(); k++){
        cout << static_cast<char>(i++) << " ";
    }
    cout << endl;
    for(int i = 0; i < getSize(); i++){
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
        for(int j = 0; j < getSize(); j++){
            cout << hexCells[i][j].getState() << " ";
        }
        cout << endl;
    }
}
/*reads a game from file*/
void HexVector::readFromFile(int& player_turn, const string& filename){
    ifstream fin;
    string s;
    fin.open(filename);
    if(fin){
        getline(fin, s);
        setSize(stoi(s));
        getline(fin, s);
        gameMode = stoi(s);
        getline(fin, s);
        player_turn = stoi(s) - 1;
        getline(fin, s);
        moveNum = stoi(s);
        getline(fin, s);
        last_move.setRow(stoi(s));
        getline(fin, s);
        last_move.setColumn(stoi(s));
        getline(fin, s);
        last_move.setState(static_cast<cell_state>(s[0]));
        getline(fin, s);
        for(int i = 0, k = 0; i < getSize(); i++){
            for(int j = 0; j < getSize(); j++, k++){
                hexCells[i][j].setState(static_cast<cell_state>(s[k]));
            }
        }
    }
    else{ 
        cerr << "An error occured while opening " << filename << endl;
    }
    fin.close();
}
/*writes a game to file*/
void HexVector::writeToFile(const int& player_turn, const string& filename) const{
    ofstream fout;
    fout.open(filename);
    if(fout){
        fout << getSize() << endl << getGameMode() << endl << player_turn%2 + 1 << endl << moveNum << endl << last_move.getRow() << endl << last_move.getColumn() << endl << last_move.getState() << endl;
        for(int i = 0; i < getSize(); i++){
            for(int j = 0; j < getSize(); j++){
                fout << hexCells[i][j].getState();
            }
        }
    }
    else{ 
        cerr << "An error occured while opening " << filename << endl;
    }
    fout.close();
}
/*resets all the cell states of all indexes to empty*/
void HexVector::reset(){
    for(int i = 0; i < getSize(); i++){
        for(int j = 0; j < getSize(); j++){
            if(hexCells[i][j].getState() != Empty) hexCells[i][j].setState(Empty);
        }
    }
}
/*sets the size and resets afterward*/
void HexVector::setSize(const int& size){
    boardSize = size;
    hexCells.resize(size);
    for(int i = 0; i < size; i++){
        hexCells[i].resize(size);
    }
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            hexCells[i][j].setRow(i);
            hexCells[i][j].setColumn(j); 
        }
    }
    reset();
}
/*plays a single move for players*/
void HexVector::play(const Cell& coordinates){
    hexCells[coordinates.getRow()][coordinates.getColumn()].setState(static_cast<cell_state>(coordinates.getState()));
    moveNum++;
}
/*plays a single move for computer*/
void HexVector::play(){
    if(check_cell_state(get_pastx(), get_pasty() + 1) == true){
        hexCells[get_pastx() - 1][static_cast<int>(get_pasty()) - 96].setState(Bot);
        cout << "\nComputer's move: " << static_cast<char>(get_pasty() + 1) << get_pastx() << endl;
        last_move.setRow(get_pastx() - 1);
        last_move.setColumn(static_cast<int>(get_pasty()) - 96);
    }
    else if(check_cell_state(get_pastx() - 1, get_pasty() + 1) == true){
        hexCells[get_pastx() - 2][static_cast<int>(get_pasty()) - 96].setState(Bot);
        cout << "\nComputer's move: " << static_cast<char>(get_pasty() + 1) << get_pastx() - 1 << endl;
        last_move.setRow(get_pastx() - 2);
        last_move.setColumn(static_cast<int>(get_pasty()) - 96);
    }
    else if(check_cell_state(get_pastx() + 1, get_pasty()) == true){
        hexCells[get_pastx()][static_cast<int>(get_pasty()) - 97].setState(Bot);
        cout << "\nComputer's move: " << get_pasty() << get_pastx() + 1 << endl;
        last_move.setRow(get_pastx());
        last_move.setColumn(static_cast<int>(get_pasty()) - 97);
    }
    else if(check_cell_state(get_pastx() - 1, get_pasty()) == true){
        hexCells[get_pastx() - 2][static_cast<int>(get_pasty()) - 97].setState(Bot);
        cout << "\nComputer's move: " << get_pasty() << get_pastx() - 1 << endl;
        last_move.setRow(get_pastx() - 2);
        last_move.setColumn(static_cast<int>(get_pasty()) - 97);
    }
    else if(check_cell_state(get_pastx() + 1, get_pasty() - 1) == true){
        hexCells[get_pastx()][static_cast<int>(get_pasty()) - 98].setState(Bot);
        cout << "\nComputer's move: " << static_cast<char>(get_pasty() - 1) << get_pastx() + 1 << endl;
        last_move.setRow(get_pastx());
        last_move.setColumn(static_cast<int>(get_pasty()) - 98);
    }
    else if(check_cell_state(get_pastx(), get_pasty() - 1) == true){
        hexCells[get_pastx() - 1][static_cast<int>(get_pasty()) - 98].setState(Bot);
        cout << "\nComputer's move: " << static_cast<char>(get_pasty() - 1) << get_pastx() << endl;
        last_move.setRow(get_pastx() - 1);
        last_move.setColumn(static_cast<int>(get_pasty()) - 98);
    }
    else{
        for(int i = 0; i < getSize(); i++){
            for(int j = 0; j < getSize(); j++){
                if(check_cell_state(i, j) == true){
                    hexCells[i][j].setState(Bot);
                    last_move.setRow(i);
                    last_move.setColumn(j);
                    cout << "\nComputer's move: " << static_cast<char>(j + 97) << i + 1 << endl;
                    break;
                }
            }
        }
    }
    last_move.setState(Bot);
    moveNum++;
}
/*whole dynamics of the game is in this function*/
void HexVector::playGame(){
    Cell temp;
    string input;
    auto file = false, invalid = false;
    int x, s = 0;
    char y;
    if(isloaded == 0){//checks if the game is loaded or created for the first time
        for(int i = 0; s < 5; i++){
            if(i == 0){
                cout << "Please enter the size of your board(min. 5): ";
            }
            cin >> s;
            cin.clear();
            cin.ignore(100,'\n');
            if(s < 5){
                cout << "Please enter an integer that is bigger than 5: " << endl;
            }
        }
        if(s > 5){
            setSize(s);
        }
        initializePastCoordinates();
        for(int i = 0; getGameMode() != 1 && getGameMode() != 2; i++){
            if(i == 0){
                cout << "\nSelect a game mode\n1)Player vs Player\n2)Player vs Computer\n";
            }
            cin >> gameMode;
            cin.clear();
            cin.ignore(100,'\n');
            if(getGameMode() != 1 && getGameMode() != 2){
                cout << "Invalid operation. Please enter a valid operation: ";
            }
        }
    }
    for(int i = 0, main = 0, lastmv = 0, moves = 0; getGameStatus() == 0 && main == 0; i++){
        if(isloaded == 1 && i == 0){
            i = turn;
        }
        else{
            turn = i;
        }
        if(getGameMode() == 1){
            file = false;
            lastmv = 0;
            moves = 0;
            print();
            do{
                invalid = false;
                if(i%2 == 0){
                    cout << endl << "Player1's move(use lowercase letters): ";
                }
                else{
                    cout << endl << "Player2's move(use lowercase letters): ";
                }
                getline(cin, input);
                if(input.substr(0, 4) == "LOAD"){//loads the given file
                    readFromFile(i, input.substr(5, input.length() - 5));
                    i--;
                    file = true;
                    break;
                }
                else if(input.substr(0, 4) == "SAVE"){//saves to given file
                    writeToFile(i, input.substr(5, input.length() - 5));
                    i--;
                    file = true;
                    break;
                }
                else if(input.substr(0, 4) == "MAIN"){//returns to main menu
                    main = 1;
                    break;
                }
                else if(input == "LASTMOVE"){
                    lastmv = 1;
                    i--;
                    try{
                        temp = lastMove();
                        cout << "Last move is: " << static_cast<char>(temp.getColumn() + 97) << temp.getRow() + 1 << endl;
                    }
                    catch(NoLastMove& noLastMove){
                        cout << noLastMove.what() << endl;
                    }
                    break;
                }
                else if(input == "NUMOFMOVES"){
                    moves = 1;
                    i--;
                    cout << "Number of moves: " << numberOfMoves() << endl;
                    break;
                }
                else if((input[0] >= 'a' && input[0] <= static_cast<char>(getSize() + 96)) && input.length() >= 2){//checks if the input is valid
                    if(static_cast<int>(input[1]) - 48 >= 1 && static_cast<int>(input[1]) - 48 <= 9){
                        x = stoi(input.substr(1, input.length() - 1));
                        y = input[0];
                    }
                }
                else{
                    invalid = true;//if none of the above is satisfied then function marks the input as invalid
                }
            }while(check_cell_state(x, y) == false || invalid == true);
            if(!file && main == 0 && lastmv == 0 && moves == 0){
                temp.setRow(x - 1);
                temp.setColumn(static_cast<int>(y) - 97);
                if(i%2 == 0){//player turn
                    temp.setState(Player1);
                }
                else{
                    temp.setState(Player2);
                }
                play(temp);
                last_move = temp;
                if(i%2 == 0){
                    if(isConnected(Player1) == true){
                        find_beginning(Player1);
                        for(int i = 0; i < p1_first_coordinates.size(); i++){
                            if(isWinner_P1(p1_first_coordinates[i], 0, 0) == true){
                                print();
                                cout << endl << "PLAYER1 WINS!!!" << endl;
                                gameStatus = 1;
                            }
                            resetPastCoordinates(Player1);
                        }
                    }
                }
                else{
                    if(isConnected(Player2) == true){
                        find_beginning(Player2);
                        for(int i = 0; i < p2_first_coordinates.size(); i++){
                            if(isWinner_P2(0, p2_first_coordinates[i], 0) == true){
                                print();
                                cout << endl << "PLAYER2 WINS!!!" << endl;
                                gameStatus = 1;
                            }
                            resetPastCoordinates(Player2);
                        }
                    }
                }
            }
        }
        else{
            if(i%2 == 1){
                if(!file){
                    print();
                    play();
                }
                file = false;
            }
            else{
                print();
                file = false;
                lastmv = 0;
                moves = 0;
                do{
                    invalid = false;
                    cout << endl << "Player's move(use lowercase letters): ";
                    getline(cin, input);
                    if(input.substr(0, 4) == "LOAD"){
                        readFromFile(i, input.substr(5, input.length() - 5));
                        i--;
                        file = true;
                        break;
                    }
                    else if(input.substr(0, 4) == "SAVE"){
                        writeToFile(i, input.substr(5, input.length() - 5));
                        i--;
                        file = true;
                        break;
                    }
                    else if(input.substr(0, 4) == "MAIN"){
                        main = 1;
                        break;
                    }
                    else if(input == "LASTMOVE"){
                        lastmv = 1;
                        i--;
                        try{
                            temp = lastMove();
                            cout << "Last move is: " << static_cast<char>(temp.getColumn() + 97) << temp.getRow() + 1 << endl;
                        }
                        catch(NoLastMove& noLastMove){
                            cout << noLastMove.what() << endl;
                        }
                        break;
                    }
                    else if(input == "NUMOFMOVES"){
                        moves = 1;
                        i--;
                        cout << "Number of moves: " << numberOfMoves() << endl;
                        break;
                    }
                    else if((input[0] >= 'a' && input[0] <= static_cast<char>(getSize() + 96)) && input.length() >= 2){
                        if(static_cast<int>(input[1]) - 48 >= 1 && static_cast<int>(input[1]) - 48 <= 9){
                            x = stoi(input.substr(1, input.length() - 1));
                            y = input[0];
                        }
                    }
                    else{
                        invalid = true;
                    }
                }while(check_cell_state(x, y) == false || invalid == true);
                if(!file && main == 0){
                    temp.setRow(x - 1);
                    temp.setColumn(static_cast<int>(y) - 97);
                    temp.setState(Player1);
                    play(temp);
                    last_move = temp;
                    past_x = x;//sets the past coordinates for play function to use
                    past_y = y;
                }
            }
            if(!file && main == 0){
                if(i%2 == 0){
                    if(isConnected(Player1) == true){
                        find_beginning(Player1);
                        for(int i = 0; i < p1_first_coordinates.size(); i++){
                            if(isWinner_P1(p1_first_coordinates[i], 0, 0) == true){
                                print();
                                cout << endl << "PLAYER WINS!!!" << endl;
                                gameStatus = 1;
                            }
                            resetPastCoordinates(Player1);
                        }
                    }
                }
                else{
                    if(isConnected(Bot) == true){
                        find_beginning(Bot);
                        for(int i = 0; i < p2_first_coordinates.size(); i++){
                            if(isWinner_P2(0, p2_first_coordinates[i], 0) == true){
                                print();
                                cout << endl << "COMPUTER WINS!!!" << endl;
                                gameStatus = 1;
                            }
                            resetPastCoordinates(Player2);
                        }
                    }
                }
            }
        }
    }
}
/*returns the proper cell object according to the given coordinates*/
Cell HexVector::operator()(const int& x, const int& y){
    if(x >= this->boardSize || y >= this->boardSize){
        throw OutOfSize();
    }
    return hexCells[x][y];
}
/*checks if the player/player1 wins*/
bool HexVector::isWinner_P1(const int& x, const int& y, int rotation){
    if(hexCells[x][y].getState() != Player1){
        return false;
    }
    else if(hexCells[x][y].getState() == Player1 && y == getSize() - 1){
        hexCells[x][y].setState(static_cast<cell_state>(Player1 - 32));
        return true;
    }
    else{
        //function determines the coordinates of the previous node by finding the opposite rotation of current one and records these coordinates so that function doesn't check the same coordinates over and over again and this prevents the function from getting into an infinite loop
        if(rotation == 1){
            p1_past_coordinates[occupied_indexs(p1_past_coordinates, getSize()*getSize()/2)].setx(x + 1);
            p1_past_coordinates[occupied_indexs(p1_past_coordinates, getSize()*getSize()/2)].sety(y);
        }
        if(rotation == 2){
            p1_past_coordinates[occupied_indexs(p1_past_coordinates, getSize()*getSize()/2)].setx(x + 1);
            p1_past_coordinates[occupied_indexs(p1_past_coordinates, getSize()*getSize()/2)].sety(y - 1);
        }
        if(rotation == 3){
            p1_past_coordinates[occupied_indexs(p1_past_coordinates, getSize()*getSize()/2)].setx(x);
            p1_past_coordinates[occupied_indexs(p1_past_coordinates, getSize()*getSize()/2)].sety(y - 1);
        }
        if(rotation == 4){
            p1_past_coordinates[occupied_indexs(p1_past_coordinates, getSize()*getSize()/2)].setx(x - 1);
            p1_past_coordinates[occupied_indexs(p1_past_coordinates, getSize()*getSize()/2)].sety(y);
        }
        if(rotation == 5){
            p1_past_coordinates[occupied_indexs(p1_past_coordinates, getSize()*getSize()/2)].setx(x - 1);
            p1_past_coordinates[occupied_indexs(p1_past_coordinates, getSize()*getSize()/2)].sety(y + 1);
        }
        if(rotation == 6){
            p1_past_coordinates[occupied_indexs(p1_past_coordinates, getSize()*getSize()/2)].setx(x);
            p1_past_coordinates[occupied_indexs(p1_past_coordinates, getSize()*getSize()/2)].sety(y + 1);
        }
        //rotations starts from up and goes on clockwise
        //if isWinner_P1 is true it uppercases the current node so that only the winning path is uppercased 
        if(x - 1 >= 0 && check_coordinates(x - 1, y, Player1) == true){
            rotation = 1;
            if(isWinner_P1(x - 1, y, 1) == true){
                if(hexCells[x][y].getState() == Player1){
                    hexCells[x][y].setState(static_cast<cell_state>(Player1 - 32));
                }
                return true;
            }
        }
        if(x - 1 >= 0 && y + 1 <= getSize() - 1 && check_coordinates(x - 1, y + 1, Player1) == true){
            rotation = 2;
            if(isWinner_P1(x - 1, y + 1, 2) == true){
                if(hexCells[x][y].getState() == Player1){
                    hexCells[x][y].setState(static_cast<cell_state>(Player1 - 32));
                }
                return true;
            }
        }
        if(y + 1 <= getSize() - 1 && check_coordinates(x, y + 1, Player1) == true){
            rotation = 3;
            if(isWinner_P1(x, y + 1, 3) == true){
                if(hexCells[x][y].getState() == Player1){
                    hexCells[x][y].setState(static_cast<cell_state>(Player1 - 32));
                }
                return true;
            }
        }
        if(x + 1 <= getSize() - 1 && check_coordinates(x + 1, y, Player1) == true){
            rotation = 4;
            if(isWinner_P1(x + 1, y, 4) == true){
                if(hexCells[x][y].getState() == Player1){
                    hexCells[x][y].setState(static_cast<cell_state>(Player1 - 32));
                }
                return true;
            }
        }
        if(x + 1 <= getSize() - 1 && y - 1 >= 0 && check_coordinates(x + 1, y - 1, Player1) == true){
            rotation = 5;
            if(isWinner_P1(x + 1, y - 1, 5) == true){
                if(hexCells[x][y].getState() == Player1){
                    hexCells[x][y].setState(static_cast<cell_state>(Player1 - 32));
                }
                return true;
            }
        }
        if(y - 1 >= 0 && check_coordinates(x, y - 1, Player1) == true){
            rotation = 6;
            if(isWinner_P1(x, y - 1, 6) == true){
                if(hexCells[x][y].getState() == Player1){
                    hexCells[x][y].setState(static_cast<cell_state>(Player1 - 32));
                }
                return true;
            }
        }
        return false;
    }
}
/*checks if the bot/player2 wins*/
bool HexVector::isWinner_P2(const int& x, const int& y, int rotation){
    if(hexCells[x][y].getState() != Player2){
        return false;
    }
    else if(hexCells[x][y].getState() == Player2 && x == getSize() - 1){
        hexCells[x][y].setState(static_cast<cell_state>(Player2 - 32));
        return true;
    }
    else{
        //function determines the coordinates of the previous node by finding the opposite rotation of current one and records these coordinates so that function doesn't check the same coordinates over and over again and this prevents the function from getting into an infinite loop
        if(rotation == 1){
            p2_past_coordinates[occupied_indexs(p2_past_coordinates, getSize()*getSize()/2)].setx(x);
            p2_past_coordinates[occupied_indexs(p2_past_coordinates, getSize()*getSize()/2)].sety(y - 1);
        }
        if(rotation == 2){
            p2_past_coordinates[occupied_indexs(p2_past_coordinates, getSize()*getSize()/2)].setx(x - 1);
            p2_past_coordinates[occupied_indexs(p2_past_coordinates, getSize()*getSize()/2)].sety(y);
        }
        if(rotation == 3){
            p2_past_coordinates[occupied_indexs(p2_past_coordinates, getSize()*getSize()/2)].setx(x - 1);
            p2_past_coordinates[occupied_indexs(p2_past_coordinates, getSize()*getSize()/2)].sety(y + 1);
        }
        if(rotation == 4){
            p2_past_coordinates[occupied_indexs(p2_past_coordinates, getSize()*getSize()/2)].setx(x);
            p2_past_coordinates[occupied_indexs(p2_past_coordinates, getSize()*getSize()/2)].sety(y + 1);
        }
        if(rotation == 5){
            p2_past_coordinates[occupied_indexs(p2_past_coordinates, getSize()*getSize()/2)].setx(x + 1);
            p2_past_coordinates[occupied_indexs(p2_past_coordinates, getSize()*getSize()/2)].sety(y);
        }
        if(rotation == 6){
            p2_past_coordinates[occupied_indexs(p2_past_coordinates, getSize()*getSize()/2)].setx(x + 1);
            p2_past_coordinates[occupied_indexs(p2_past_coordinates, getSize()*getSize()/2)].sety(y - 1);
        }
        //rotations starts from right and goes on clockwise
        //if isWinner_P2 is true it uppercases the current node so that only the winning path is uppercased 
        if(y + 1 <= getSize() - 1 && check_coordinates(x, y + 1, Player2) == true){
            rotation = 1;
            if(isWinner_P2(x, y + 1, 1) == true){
                if(hexCells[x][y].getState() == Player2){
                    hexCells[x][y].setState(static_cast<cell_state>(Player2 - 32));
                }
                return true;
            }
        }
        if(x + 1 <= getSize() - 1 && check_coordinates(x + 1, y, Player2) == true){
            rotation = 2;
            if(isWinner_P2(x + 1, y, 2) == true){
                if(hexCells[x][y].getState() == Player2){
                    hexCells[x][y].setState(static_cast<cell_state>(Player2 - 32));
                }
                return true;
            }
        }
        if(x + 1 <= getSize() - 1 && y - 1 >= 0 && check_coordinates(x + 1, y - 1, Player2) == true){
            rotation = 3;
            if(isWinner_P2(x + 1, y - 1, 3) == true){
                if(hexCells[x][y].getState() == Player2){
                    hexCells[x][y].setState(static_cast<cell_state>(Player2 - 32));
                }
                return true;
            }
        }
        if(y - 1 >= 0 && check_coordinates(x, y - 1, Player2) == true){
            rotation = 4;
            if(isWinner_P2(x, y - 1, 4) == true){
                if(hexCells[x][y].getState() == Player2){
                    hexCells[x][y].setState(static_cast<cell_state>(Player2 - 32));
                }
                return true;
            }
        }
        if(x - 1 >= 0 && check_coordinates(x - 1, y, Player2) == true){
            rotation = 5;
            if(isWinner_P2(x - 1, y, 5) == true){
                if(hexCells[x][y].getState() == Player2){
                    hexCells[x][y].setState(static_cast<cell_state>(Player2 - 32));
                }
                return true;
            }
        }
        if(x - 1 >= 0 && y + 1 <= getSize() - 1 && check_coordinates(x - 1, y + 1, Player2) == true){
            rotation = 6;
            if(isWinner_P2(x - 1, y + 1, 6) == true){
                if(hexCells[x][y].getState() == Player2){
                    hexCells[x][y].setState(static_cast<cell_state>(Player2 - 32));
                }
                return true;
            }
        }
        return false;
    }
}
/*checks if the cell in given coordinates is empty*/
bool HexVector::check_cell_state(const int& x, const char& y) const{
    if(x - 1 >= 0 && x - 1 <= getSize() - 1 && static_cast<int>(y) - 97 >= 0 && static_cast<int>(y) - 97 <= getSize() - 1){
        if(hexCells[x - 1][static_cast<int>(y) - 97].getState() == Empty){
            return true;
        }
    }
    return false;
}
/*checks if the given player has at least 2 nodes at two opposite sides*/
bool HexVector::isConnected(const cell_state& player){
    int check1 = 0, check2 = 0;
    if(player == Player1){
        for(int i = 0; i < getSize() && check1 == 0; i++){
            if(hexCells[i][0].getState() == player){
                check1++;
            }
        }
        for(int i = 0; i < getSize() && check2 == 0; i++){
            if(hexCells[i][getSize() - 1].getState() == player){
                check2++;
            }
        }
    }
    else{
        for(int i = 0; i < getSize() && check1 == 0; i++){
            if(hexCells[0][i].getState() == player){
                check1++;
            }
        }
        for(int i = 0; i < getSize() && check2 == 0; i++){
            if(hexCells[getSize() - 1][i].getState() == player){
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
/*finds starting points at index 0 according to the player*/
void HexVector::find_beginning(const cell_state& player){
    if(player == Player1){
        p1_first_coordinates.clear();
        for(int i = 0; i < getSize(); i++){
            if(hexCells[i][0].getState() == player){
                p1_first_coordinates.push_back(i);
            }
        }
    }
    else{
        p2_first_coordinates.clear();
        for(int i = 0; i < getSize(); i++){
            if(hexCells[0][i].getState() == player){
                p2_first_coordinates.push_back(i);
            }
        }
    }
}
/*constructor*/
HexVector::HexVector() : AbstractHex(){
    setSize(5);
}
/*constructor with size argument*/
HexVector::HexVector(const int& s) : AbstractHex(s){
    setSize(s);
}
/*constructor with size and gamestatus arguments*/
HexVector::HexVector(const int& s, const int& gm) : AbstractHex(s, gm){
    setSize(s);
}