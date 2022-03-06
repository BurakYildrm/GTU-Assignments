#include <iostream>
#include <fstream>
#include <string>
#include "hex_class.h"
using namespace std;
int Hex::marked_cells = 0;
int Hex::connectedCells = 1;
void Hex::delete_last_element(){//deletes the last element of all_plays for undo feature
    int temp_size = occupied_indexs(all_plays, getSize()*getSize()) - 1;
    past_coordinates *temp;
    temp = new past_coordinates[temp_size];
    for(int i = 0; i < temp_size; i++){
        temp[i].setx(all_plays[i].getx());
        temp[i].sety(all_plays[i].gety());
    }
    delete[] all_plays;
    all_plays = new past_coordinates[getSize()*getSize()];
    for(int i = 0; i < temp_size; i++){
        all_plays[i].setx(temp[i].getx());
        all_plays[i].sety(temp[i].gety());
    }
    for(int i = temp_size; i < getSize()*getSize(); i++){
        all_plays[i].setx(-1);
        all_plays[i].sety(-1);
    }
    delete[] temp;
}
bool operator==(const Hex& obj1, const Hex& obj2){
    if(obj1.count_marked_cells() > obj2.count_marked_cells()){//returns true if the first game has more marked cells
        return true;
    }
    else{
        return false;
    }
}
Hex& Hex::operator=(const Hex& other){
    name = other.name;
    past_x = other.past_x;
    past_y = other.past_y;
    delete[] p1_first_coordinates;
    delete[] p2_first_coordinates;
    if(other.p1_first_coordinates && other.p2_first_coordinates){
        p1_first_coordinates = new int[other.getSize()];
        p2_first_coordinates = new int[other.getSize()];
        for(int i = 0; i < other.getSize(); i++){
            p1_first_coordinates[i] = other.p1_first_coordinates[i];
            p2_first_coordinates[i] = other.p2_first_coordinates[i];
        }
    }
    else{
        p1_first_coordinates = nullptr;
        p2_first_coordinates = nullptr;
    }
    for(int i = 0; i < board_size; i++){
        delete[] hexCells[i];
    }
    delete[] hexCells;
    hexCells = new Cell*[other.board_size];
    for(int i = 0; i < other.board_size; i++){
        hexCells[i] = new Cell[other.board_size];
    }
    for(int i = 0; i < other.getSize(); i++){
        for(int j = 0; j < other.getSize(); j++){
            hexCells[i][j].setColumn(other.hexCells[i][j].getColumn());
            hexCells[i][j].setRow(other.hexCells[i][j].getRow());
            hexCells[i][j].setState(static_cast<cell_state>(other.hexCells[i][j].getState()));
        }
    }
    board_size = other.board_size;
    game_mode = other.game_mode;
    game_status = other.game_status;
    isloaded = other.isloaded;
    turn = other.turn;
    delete[] p1_past_coordinates;
    delete[] p2_past_coordinates;
    delete[] all_plays;
    delete[] score_coordinates;
    if(other.p1_past_coordinates && other.p2_past_coordinates && other.all_plays && other.score_coordinates){
        p1_past_coordinates = new past_coordinates[other.getSize()*other.getSize()/2];
        p2_past_coordinates = new past_coordinates[other.getSize()*other.getSize()/2];
        all_plays = new past_coordinates[other.getSize()*other.getSize()];
        score_coordinates = new past_coordinates[other.getSize()*other.getSize()/2];
        for(int i = 0; i < other.getSize()*other.getSize()/2; i++){
            p1_past_coordinates[i].setx(other.p1_past_coordinates[i].getx());
            p1_past_coordinates[i].sety(other.p1_past_coordinates[i].gety());
            p2_past_coordinates[i].setx(other.p2_past_coordinates[i].getx());
            p2_past_coordinates[i].sety(other.p2_past_coordinates[i].gety());
            score_coordinates[i].setx(other.score_coordinates[i].getx());
            score_coordinates[i].sety(other.score_coordinates[i].gety());
        }
        for(int i = 0; i < other.getSize()*other.getSize(); i++){
            all_plays[i].setx(other.all_plays[i].getx());
            all_plays[i].sety(other.all_plays[i].gety());
        }
    }
    else{
        p1_past_coordinates = nullptr;
        p2_past_coordinates = nullptr;
        all_plays = nullptr;
        score_coordinates = nullptr;
    }
    return *this;
}
Hex& Hex::operator--(){//undo the last play
    hexCells[all_plays[occupied_indexs(all_plays, getSize()*getSize()) - 1].getx()][all_plays[occupied_indexs(all_plays, getSize()*getSize()) - 1].gety()].setState(Empty); //sets the cell state of the last position to Empty
    delete_last_element();
    return *this;
}
Hex Hex::operator--(int){//undo the last play
    Hex temp;
    temp = *this;
    hexCells[all_plays[occupied_indexs(all_plays, getSize()*getSize()) - 1].getx()][all_plays[occupied_indexs(all_plays, getSize()*getSize()) - 1].gety()].setState(Empty); //sets the cell state of the last position to Empty
    delete_last_element();
    return temp;
}
ostream& operator<<(ostream& outputStream, Hex& game){//prints the gameboard with the scores of players
    outputStream << endl <<  "  ";
    for(int i = 97, k = 0; k < game.getSize(); k++){
        outputStream << static_cast<char>(i++) << " ";
    }
    outputStream << endl;
    for(int i = 0; i < game.getSize(); i++){
        outputStream << i + 1;
        if(i >= 9){//to aline the rows
            for(int k = 1; k < i + 1; k++){
                outputStream << " ";
            }
        }
        else{
            for(int k = 0; k < i + 1; k++){
                outputStream << " ";
            }
        }
        for(int j = 0; j < game.getSize(); j++){
            outputStream << game.hexCells[i][j].getState() << " ";
        }
        outputStream << endl;
    }
    if(game.game_mode == 1){
        outputStream << "Score of player1: " << game.score_of_player(Player1) << "(maximum score is " << game.getSize() << ")" << endl;
        game.reset_score_coordinates();
        outputStream << "Score of player2: " << game.score_of_player(Player2) << "(maximum score is " << game.getSize() << ")" << endl;
        game.reset_score_coordinates();
    }
    else{
        outputStream << "Score of player: " << game.score_of_player(Player1) << "(maximum score is " << game.getSize() << ")" << endl;
        game.reset_score_coordinates();
        outputStream << "Score of bot: " << game.score_of_player(Bot) << "(maximum score is " << game.getSize() << ")" << endl;
        game.reset_score_coordinates();
    }
    return outputStream;
}
ofstream& operator<<(ofstream& outputStream, const Hex& game){//writes the game info to the outputStream
    outputStream << game.getSize() << endl << game.game_mode << endl << game.turn%2 + 1 << endl;
    for(int i = 0; i < game.getSize(); i++){
        for(int j = 0; j < game.getSize(); j++){
            outputStream << game.hexCells[i][j].getState();
        }
    }
    return outputStream;
}
ifstream& operator>>(ifstream& outputStream, Hex& game){//gets the game info from outputStream and assigns it to the proper hex member
    string s;
    getline(outputStream, s);
    game.setSize(stoi(s));
    getline(outputStream, s);
    game.game_mode = stoi(s);
    getline(outputStream, s);
    game.turn = stoi(s) - 1;
    getline(outputStream, s);
    for(int i = 0, k = 0; i < game.getSize(); i++){
        for(int j = 0; j < game.getSize(); j++, k++){
            game.hexCells[i][j].setState(static_cast<cell_state>(s[k]));
        }
    }
    return outputStream;
}
Hex::Hex(): game_mode(0), game_status(0), isloaded(0), p1_first_coordinates(nullptr), p1_past_coordinates(nullptr), p2_first_coordinates(nullptr), p2_past_coordinates(nullptr), all_plays(nullptr), score_coordinates(nullptr){
    setSize(5);
}
Hex::Hex(const int& size) : game_mode(0), game_status(0), isloaded(0), p1_first_coordinates(nullptr), p1_past_coordinates(nullptr), p2_first_coordinates(nullptr), p2_past_coordinates(nullptr), all_plays(nullptr), score_coordinates(nullptr){
    setSize(size);
}
Hex::Cell::Cell() : row(0), column(0), state(Empty){
    /*Intentionally empty*/
}
Hex::Cell::Cell(const int& r, const int& c, const cell_state& s) : row(r), column(c), state(s){
    /*Intentionally empty*/
}
Hex::~Hex(){
    for(int i = 0; i < getSize(); i++){
        delete[] hexCells[i];
    }
    delete[] hexCells;
    if(p1_first_coordinates && all_plays){
        delete[] p1_first_coordinates;
        delete[] p2_first_coordinates;
        delete[] p1_past_coordinates;
        delete[] p2_past_coordinates;
        delete[] all_plays;
        delete[] score_coordinates;
    }
}
Hex::Hex(const Hex& other){
    name = other.name;
    past_x = other.past_x;
    past_y = other.past_y;
    if(other.p1_past_coordinates && other.p2_past_coordinates){
        p1_first_coordinates = new int[other.getSize()];
        p2_first_coordinates = new int[other.getSize()];
        for(int i = 0; i < other.getSize(); i++){
            p1_first_coordinates[i] = other.p1_first_coordinates[i];
            p2_first_coordinates[i] = other.p2_first_coordinates[i];
        }
    }
    else{
        p1_first_coordinates = nullptr;
        p2_first_coordinates = nullptr;
    }
    board_size = other.board_size;
    game_mode = other.game_mode;
    game_status = other.game_status;
    isloaded = other.isloaded;
    turn = other.turn;
    hexCells = new Cell*[other.getSize()];
    for(int i = 0; i < other.getSize(); i++){
        hexCells[i] = new Cell[other.getSize()];
    }
    for(int i = 0; i < other.getSize(); i++){
        for(int j = 0; j < other.getSize(); j++){
            hexCells[i][j].setColumn(other.hexCells[i][j].getColumn());
            hexCells[i][j].setRow(other.hexCells[i][j].getRow());
            hexCells[i][j].setState(static_cast<cell_state>(other.hexCells[i][j].getState()));
        }
    }
    if(other.p1_past_coordinates && other.p2_past_coordinates && other.all_plays && other.score_coordinates){
        p1_past_coordinates = new past_coordinates[other.getSize()*other.getSize()/2];
        p2_past_coordinates = new past_coordinates[other.getSize()*other.getSize()/2];
        all_plays = new past_coordinates[other.getSize()*other.getSize()];
        score_coordinates = new past_coordinates[other.getSize()*other.getSize()/2];
        for(int i = 0; i < other.getSize()*other.getSize()/2; i++){
            p1_past_coordinates[i].setx(other.p1_past_coordinates[i].getx());
            p1_past_coordinates[i].sety(other.p1_past_coordinates[i].gety());
            p2_past_coordinates[i].setx(other.p2_past_coordinates[i].getx());
            p2_past_coordinates[i].sety(other.p2_past_coordinates[i].gety());
            score_coordinates[i].setx(other.score_coordinates[i].getx());
            score_coordinates[i].sety(other.score_coordinates[i].gety());
        }
        for(int i = 0; i < other.getSize()*other.getSize(); i++){
            all_plays[i].setx(other.all_plays[i].getx());
            all_plays[i].sety(other.all_plays[i].gety());
        }
    }
    else{
        p1_past_coordinates = nullptr;
        p2_past_coordinates = nullptr;
        all_plays = nullptr;
        score_coordinates = nullptr;
    }
}
void Hex::setSize(const int& s){
    board_size = s;
    hexCells = new Cell*[board_size];
    for(int i = 0; i < board_size; i++){
        hexCells[i] = new Cell[board_size];
    }
    for(int i = 0; i < board_size; i++){
        for(int j = 0; j < board_size; j++){
            hexCells[i][j].setRow(i);
            hexCells[i][j].setColumn(j); 
        }
    }
}
void Hex::play(const Cell& coordinates){//plays one step for player
    hexCells[coordinates.getRow()][coordinates.getColumn()].setState(static_cast<cell_state>(coordinates.getState()));
    all_plays[occupied_indexs(all_plays, getSize()*getSize())].setx(coordinates.getRow());
    all_plays[occupied_indexs(all_plays, getSize()*getSize())].sety(coordinates.getColumn());
    setStatic(1);
}
bool Hex::check_cell_state(const int& x, const char& y) const{//returns true if the cell is empty
    if(x - 1 >= 0 && x - 1 <= getSize() - 1 && static_cast<int>(y) - 97 >= 0 && static_cast<int>(y) - 97 <= getSize() - 1){
        if(hexCells[x - 1][static_cast<int>(y) - 97].getState() == Empty){
            return true;
        }
    }
    return false;
}
int Hex::count_marked_cells() const{//counts marked cells
    int counter = 0;
    for(int i = 0; i < board_size; i++){
        for(int j = 0; j < board_size; j++){  
            if(hexCells[i][j].getState() != Empty){
                counter++;
            }
        }
    }
    return counter;
}
void Hex::load_game(int& player_turn, string filename){
    int marked_cells;
    ifstream fin;
    fin.open(filename);
    if(fin){
        marked_cells = count_marked_cells();
        fin >> *this;
        setStatic(count_marked_cells() - marked_cells);//calculates the difference between the original game and the loaded game and sets the static value according to the result of that calculation
    }
    else{ 
        cerr << "An error occured while opening " << filename << endl;
    }
    fin.close();
}
void Hex::save_game(const int& player_turn, string filename) const{
    ofstream fout;
    fout.open(filename);
    if(fout){
        fout << *this;
    }
    else{ 
        cerr << "An error occured while opening " << filename << endl;
    }
    fout.close();
}
int Hex::occupied_indexs(const Hex::past_coordinates *vec, const int& size){
    int counter = 0;
    for(int i = 0; i < size; i++){
        if(vec[i].gety() != -1){
            counter++;
        }
    }
    return counter;
}
int Hex::occupied_indexs(const int *vec, const int& size){
    int counter = 0;
    for(int i = 0; i < size; i++){
        if(vec[i] != -1){
            counter++;
        }
    }
    return counter;
}
void Hex::playGame(){
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
        initializeFirstCoordinates();
        initializePastCoordinates();
        for(int i = 0; game_mode != 1 && game_mode != 2; i++){
            if(i == 0){
                cout << "\nSelect a game mode\n1)Player vs Player\n2)Player vs Computer\n";
            }
            cin >> game_mode;
            cin.clear();
            cin.ignore(100,'\n');
            if(game_mode != 1 && game_mode != 2){
                cout << "Invalid operation. Please enter a valid operation: ";
            }
        }
    }
    for(int i = 0, main = 0, undo = 0; getGameStatus() == 0 && main == 0; i++){
        if(isloaded == 1 && i == 0){
            i = turn;
        }
        else{
            turn = i;
        }
        if(game_mode == 1){
            undo = 0;
            file = false;
            cout << *this;
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
                    load_game(i, input.substr(5, input.length() - 5));
                    i--;
                    file = true;
                    break;
                }
                else if(input.substr(0, 4) == "SAVE"){//saves to given file
                    save_game(i, input.substr(5, input.length() - 5));
                    i--;
                    file = true;
                    break;
                }
                else if(input == "MAIN"){//returns to main menu
                    main = 1;
                    break;
                }
                else if(input == "UNDO"){
                    undo = 1;
                    if(occupied_indexs(all_plays, getSize()*getSize()) == 0){
                        cout << "There is no move left to undo." << endl;
                    }
                    else{
                        --*this;
                    }
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
            if(!file && main == 0 && undo == 0){
                temp.setRow(x - 1);
                temp.setColumn(static_cast<int>(y) - 97);
                if(i%2 == 0){//player turn
                    temp.setState(Player1);
                }
                else{
                    temp.setState(Player2);
                }
                play(temp);
                if(i%2 == 0){
                    if(isConnected(Player1) == true){
                        find_beginning(Player1);
                        for(int i = 0; i < occupied_indexs(p1_first_coordinates, getSize()); i++){
                            if(isWinner_P1(p1_first_coordinates[i], 0, 0) == true){
                                cout << *this;
                                cout << endl << "PLAYER1 WINS!!!" << endl;
                                setStatic(count_marked_cells()*-1);
                                game_status = 1;
                            }
                            resetPastCoordinates(1);
                        }
                    }
                }
                else{
                    if(isConnected(Player2) == true){
                        find_beginning(Player2);
                        for(int i = 0; i < occupied_indexs(p2_first_coordinates, getSize()); i++){
                            if(isWinner_P2(0, p2_first_coordinates[i], 0) == true){
                                cout << *this;
                                cout << endl << "PLAYER2 WINS!!!" << endl;
                                setStatic(count_marked_cells()*-1);
                                game_status = 1;
                            }
                            resetPastCoordinates(2);
                        }
                    }
                }
            }
        }
        else{
            if(i%2 == 1){
                if(!file && undo == 0){
                    cout << *this;
                    temp = play();
                    hexCells[temp.getRow()][temp.getColumn()].setState(Bot);
                }
                file = false;
            }
            else{
                cout << *this;
                file = false;
                undo = 0;
                do{
                    invalid = false;
                    cout << endl << "Player's move(use lowercase letters): ";
                    getline(cin, input);
                    if(input.substr(0, 4) == "LOAD"){
                        load_game(i, input.substr(5, input.length() - 5));
                        i--;
                        file = true;
                        break;
                    }
                    else if(input.substr(0, 4) == "SAVE"){
                        save_game(i, input.substr(5, input.length() - 5));
                        i--;
                        file = true;
                        break;
                    }
                    else if(input.substr(0, 4) == "MAIN"){
                        main = 1;
                        break;
                    }
                    else if(input == "UNDO"){
                        undo = 1;
                        if(occupied_indexs(all_plays, getSize()*getSize()) == 0){
                            cout << "There is no move left to undo." << endl;
                        }
                        else{
                            --*this;
                            --*this;
                        }
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
                if(!file && main == 0 && undo == 0){
                    temp.setRow(x - 1);
                    temp.setColumn(static_cast<int>(y) - 97);
                    temp.setState(Player1);
                    play(temp);
                    past_x = x;//sets the past coordinates for play function to use
                    past_y = y;
                }
            }
            if(!file && main == 0 && undo == 0){
                if(i%2 == 0){
                    if(isConnected(Player1) == true){
                        find_beginning(Player1);
                        for(int i = 0; i < occupied_indexs(p1_first_coordinates, getSize()); i++){
                            if(isWinner_P1(p1_first_coordinates[i], 0, 0) == true){
                                cout << *this;
                                cout << endl << "PLAYER WINS!!!" << endl;
                                setStatic(count_marked_cells()*-1);
                                game_status = 1;
                            }
                            resetPastCoordinates(1);
                        }
                    }
                }
                else{
                    if(isConnected(Bot) == true){
                        find_beginning(Bot);
                        for(int i = 0; i < occupied_indexs(p2_first_coordinates, getSize()); i++){
                            if(isWinner_P2(0, p2_first_coordinates[i], 0) == true){
                                cout << *this;
                                cout << endl << "COMPUTER WINS!!!" << endl;
                                setStatic(count_marked_cells()*-1);
                                game_status = 1;
                            }
                            resetPastCoordinates(2);
                        }
                    }
                }
            }
        }
    }
}
bool Hex::isWinner_P1(const int& x, const int& y, int rotation){
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
bool Hex::isWinner_P2(const int& x, const int& y, int rotation){
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
void Hex::initializeFirstCoordinates(){//initializes the vectors with -1 according to the size
    p1_first_coordinates = new int[getSize()];
    p2_first_coordinates = new int[getSize()];
    for(int i = 0; i < getSize(); i++){
        p1_first_coordinates[i] = -1;
        p2_first_coordinates[i] = -1;
    }
}
void Hex::initializePastCoordinates(){//initializes the vectors with -1 according to the size
    p1_past_coordinates = new past_coordinates[getSize()*getSize()/2];
    p2_past_coordinates = new past_coordinates[getSize()*getSize()/2];
    score_coordinates = new past_coordinates[getSize()*getSize()/2];
    all_plays = new past_coordinates[getSize()*getSize()];
    for(int i = 0; i < getSize()*getSize(); i++){
        if(i < getSize()*getSize()/2){
            p1_past_coordinates[i].setx(-1);
            p1_past_coordinates[i].sety(-1);
            p2_past_coordinates[i].setx(-1);
            p2_past_coordinates[i].sety(-1);
            score_coordinates[i].setx(-1);
            score_coordinates[i].sety(-1);
        }
        all_plays[i].setx(-1);
        all_plays[i].sety(-1);
    }
}
void Hex::resetPastCoordinates(int player){//resets vectors to their original states
    if(player == 1){
        for(int i = 0; i < getSize()*getSize()/2; i++){
            p1_past_coordinates[i].setx(-1);
            p1_past_coordinates[i].sety(-1);
        }
    }
    else{
        for(int i = 0; i < getSize()*getSize()/2; i++){
            p2_past_coordinates[i].setx(-1);
            p2_past_coordinates[i].sety(-1);
        }
    }
}
bool Hex::isConnected(cell_state player){//determines if a player or computer has a node on both sides
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
void Hex::find_beginning(cell_state player){//finds the beginning point/points of the isWinner functions
    int j = 0;
    if(player == Player1){
        for(int i = 0; i < getSize(); i++){
            if(hexCells[i][0].getState() == player){
                p1_first_coordinates[j] = i;
                j++;
            }
        }
    }
    else{
        for(int i = 0; i < getSize(); i++){
            if(hexCells[0][i].getState() == player){
                p2_first_coordinates[j] = i;
                j++;
            }
        }
    }
}
bool Hex::check_coordinates(const int& x, const int& y, const cell_state& player){//checks if the node is already been visited by isWinner function 
    if(player == Player1){
        for(int i = 0; i < getSize()*getSize()/2; i++){
            if(p1_past_coordinates[i].getx() == x && p1_past_coordinates[i].gety() == y){
                return false;
            }
        }
    }
    else{
        for(int i = 0; i < getSize()*getSize()/2; i++){
            if(p2_past_coordinates[i].getx() == x && p2_past_coordinates[i].gety() == y){
                return false;
            }
        }
    }
    return true;
}
Hex::Cell Hex::play(){//plays one step for bot
    setStatic(1);
    Cell temp;
    if(check_cell_state(get_pastx(), get_pasty() + 1) == true){
        temp = Cell(get_pastx() - 1, static_cast<int>(get_pasty()) - 96, Bot);
        all_plays[occupied_indexs(all_plays, getSize()*getSize())].setx(get_pastx() - 1);
        all_plays[occupied_indexs(all_plays, getSize()*getSize())].sety(static_cast<int>(get_pasty()) - 96);
        cout << "\nComputer's move: " << static_cast<char>(get_pasty() + 1) << get_pastx() << endl;
        return temp;
    }
    if(check_cell_state(get_pastx() - 1, get_pasty() + 1) == true){
        temp = Cell(get_pastx() - 2, static_cast<int>(get_pasty()) - 96, Bot);
        all_plays[occupied_indexs(all_plays, getSize()*getSize())].setx(get_pastx() - 2);
        all_plays[occupied_indexs(all_plays, getSize()*getSize())].sety(static_cast<int>(get_pasty()) - 96);
        cout << "\nComputer's move: " << static_cast<char>(get_pasty() + 1) << get_pastx() - 1 << endl;
        return temp;
    }
    if(check_cell_state(get_pastx() + 1, get_pasty()) == true){
        temp = Cell(get_pastx(), static_cast<int>(get_pasty()) - 97, Bot);
        all_plays[occupied_indexs(all_plays, getSize()*getSize())].setx(get_pastx());
        all_plays[occupied_indexs(all_plays, getSize()*getSize())].sety(static_cast<int>(get_pasty()) - 97);
        cout << "\nComputer's move: " << get_pasty() << get_pastx() + 1 << endl;
        return temp;
    }
    if(check_cell_state(get_pastx() - 1, get_pasty()) == true){
        temp = Cell(get_pastx() - 2, static_cast<int>(get_pasty()) - 97, Bot);
        all_plays[occupied_indexs(all_plays, getSize()*getSize())].setx(get_pastx() - 2);
        all_plays[occupied_indexs(all_plays, getSize()*getSize())].sety(static_cast<int>(get_pasty()) - 97);
        cout << "\nComputer's move: " << get_pasty() << get_pastx() - 1 << endl;
        return temp;
    }
    if(check_cell_state(get_pastx() + 1, get_pasty() - 1) == true){
        temp = Cell(get_pastx(), static_cast<int>(get_pasty()) - 98, Bot);
        all_plays[occupied_indexs(all_plays, getSize()*getSize())].setx(get_pastx());
        all_plays[occupied_indexs(all_plays, getSize()*getSize())].sety(static_cast<int>(get_pasty()) - 98);
        cout << "\nComputer's move: " << static_cast<char>(get_pasty() - 1) << get_pastx() + 1 << endl;
        return temp;
    }
    if(check_cell_state(get_pastx(), get_pasty() - 1) == true){
        temp = Cell(get_pastx() - 1, static_cast<int>(get_pasty()) - 98, Bot);
        all_plays[occupied_indexs(all_plays, getSize()*getSize())].setx(get_pastx() - 1);
        all_plays[occupied_indexs(all_plays, getSize()*getSize())].sety(static_cast<int>(get_pasty()) - 98);
        cout << "\nComputer's move: " << static_cast<char>(get_pasty() - 1) << get_pastx() << endl;
        return temp;
    }
    for(int i = 0; i < getSize(); i++){
        for(int j = 0; j < getSize(); j++){
            if(check_cell_state(i, j) == true){
                temp = Cell(i, j, Bot);
                all_plays[occupied_indexs(all_plays, getSize()*getSize())].setx(i);
                all_plays[occupied_indexs(all_plays, getSize()*getSize())].sety(j);
                cout << "\nComputer's move: " << static_cast<char>(j + 97) << i + 1 << endl;
                return temp;
            }
        }
    }
    return temp; //I added this line just to avoid getting warning from compiler.
}
int Hex::score_of_player(cell_state player){//returns the score of player
    int number_of_coordinates, max;
    find_occurances(player);
    number_of_coordinates = occupied_indexs(score_coordinates, getSize()*getSize()/2);
    if(number_of_coordinates == 0){
        return 0;
    }
    if(number_of_coordinates == 1){
        return 1;
    }
    connectedCells_storage = new int[number_of_coordinates*(number_of_coordinates - 1)/2];
    for(int i = 0; i < number_of_coordinates*(number_of_coordinates - 1)/2; i++){
        connectedCells_storage[i] = -1;
    }
    for(int i = 0; i < number_of_coordinates; i++){
        for(int j = i + 1; j < number_of_coordinates; j++){
            if(player == Player1){
                connected_cells_p1(score_coordinates[i].getx(), score_coordinates[i].gety(), 0, score_coordinates[j].getx(), score_coordinates[j].gety());
            }
            else{
                connected_cells_p2(score_coordinates[i].getx(), score_coordinates[i].gety(), 0, score_coordinates[j].getx(), score_coordinates[j].gety());
            }
            connectedCells_storage[occupied_indexs(connectedCells_storage, number_of_coordinates*(number_of_coordinates - 1)/2)] = connectedCells;
            connectedCells = 1;
            if(player == Player1){
                resetPastCoordinates(1);
            }
            else{
                resetPastCoordinates(2);
            }
        }
    }
    max = connectedCells_storage[0];
    for(int i = 0; i < number_of_coordinates*(number_of_coordinates - 1)/2; i++){
        if(max < connectedCells_storage[i]){
            max = connectedCells_storage[i];
        }
    }
    delete[] connectedCells_storage;
    return max;
}
bool Hex::connected_cells_p1(const int& x, const int& y, int rotation, const int& dest_x, const int& dest_y){//calculates the score of player or player1
    if(x == dest_x && y == dest_y){
        return true;
    }
    else if(hexCells[x][y].getState() != Player1 && hexCells[x][y].getState() != static_cast<cell_state>(Player1 - 32)){
        return false;
    }
    else{
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
        if(x - 1 >= 0 && check_coordinates(x - 1, y, Player1) == true){
            rotation = 1;
            if(connected_cells_p1(x - 1, y, 1, dest_x, dest_y) == true){
                return true;
            }
        }
        if(x - 1 >= 0 && y + 1 <= getSize() - 1 && check_coordinates(x - 1, y + 1, Player1) == true){
            rotation = 2;
            if(connected_cells_p1(x - 1, y + 1, 2, dest_x, dest_y) == true){
                connectedCells++;
                return true;
            }
        }
        if(y + 1 <= getSize() - 1 && check_coordinates(x, y + 1, Player1) == true){
            rotation = 3;
            if(connected_cells_p1(x, y + 1, 3, dest_x, dest_y) == true){
                connectedCells++;
                return true;
            }
        }
        if(x + 1 <= getSize() - 1 && check_coordinates(x + 1, y, Player1) == true){
            rotation = 4;
            if(connected_cells_p1(x + 1, y, 4, dest_x, dest_y) == true){
                return true;
            }
        }
        if(x + 1 <= getSize() - 1 && y - 1 >= 0 && check_coordinates(x + 1, y - 1, Player1) == true){
            rotation = 5;
            if(connected_cells_p1(x + 1, y - 1, 5, dest_x, dest_y) == true){
                return true;
            }
        }
        if(y - 1 >= 0 && check_coordinates(x, y - 1, Player1) == true){
            rotation = 6;
            if(connected_cells_p1(x, y - 1, 6, dest_x, dest_y) == true){
                return true;
            }
        }
        return false;
    }
}
bool Hex::connected_cells_p2(const int& x, const int& y, int rotation, const int& dest_x, const int& dest_y){//calculates the score of bot or player2
    if(x == dest_x && y == dest_y){
        return true;
    }
    else if(hexCells[x][y].getState() != Player2 && hexCells[x][y].getState() != static_cast<cell_state>(Player2 - 32)){
        return false;
    }
    else{
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
        if(y + 1 <= getSize() - 1 && check_coordinates(x, y + 1, Player2) == true){
            rotation = 1;
            if(connected_cells_p2(x, y + 1, 1, dest_x, dest_y) == true){
                return true;
            }
        }
        if(x + 1 <= getSize() - 1 && check_coordinates(x + 1, y, Player2) == true){
            rotation = 2;
            if(connected_cells_p2(x + 1, y, 2, dest_x, dest_y) == true){
                connectedCells++;
                return true;
            }
        }
        if(x + 1 <= getSize() - 1 && y - 1 >= 0 && check_coordinates(x + 1, y - 1, Player2) == true){
            rotation = 3;
            if(connected_cells_p2(x + 1, y - 1, 3, dest_x, dest_y) == true){
                connectedCells++;
                return true;
            }
        }
        if(y - 1 >= 0 && check_coordinates(x, y - 1, Player2) == true){
            rotation = 4;
            if(connected_cells_p2(x, y - 1, 4, dest_x, dest_y) == true){
                return true;
            }
        }
        if(x - 1 >= 0 && check_coordinates(x - 1, y, Player2) == true){
            rotation = 5;
            if(connected_cells_p2(x - 1, y, 5, dest_x, dest_y) == true){
                return true;
            }
        }
        if(x - 1 >= 0 && y + 1 <= getSize() - 1 && check_coordinates(x - 1, y + 1, Player2) == true){
            rotation = 6;
            if(connected_cells_p2(x - 1, y + 1, 6, dest_x, dest_y) == true){
                return true;
            }
        }
        return false;
    }
}
void Hex::find_occurances(const cell_state& player){//finds all occurences of player
    for(int i = 0; i < getSize(); i++){
        for(int j = 0; j < getSize(); j++){
            if(hexCells[i][j].getState() == player || hexCells[i][j].getState() == static_cast<cell_state>(player - 32)){
                score_coordinates[occupied_indexs(score_coordinates, getSize()*getSize()/2)].setx(i);
                score_coordinates[occupied_indexs(score_coordinates, getSize()*getSize()/2)].sety(j);
            }
        }
    }
}
void Hex::reset_score_coordinates(){//resets score_coordinates to its original initialization
    for(int i = 0; i < getSize()*getSize()/2; i++){
        score_coordinates[i].setx(-1);
        score_coordinates[i].sety(-1);
    }
}