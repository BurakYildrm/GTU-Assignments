#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "hex_class.h"
using namespace std;
int Hex::marked_cells = 0;
Hex::Hex(const int& size){
    setSize(size);
    setGameMode(0);
    setGameStatus(0);
    setIsloaded(0);
}
Hex::Hex(const int& size, const int& gm){
    setSize(size);
    setGameMode(gm);
    setGameStatus(0);
    setIsloaded(0);
}
Hex::Cell::Cell(){
    setState(Empty);
}
inline void Hex::set_pastx(const int& x){
    past_x = x;
}
inline void Hex::set_pasty(const char& y){
    past_y = y;
}
inline int Hex::get_pastx() const{
    return past_x;
}
inline char Hex::get_pasty() const{
    return past_y;
}
void Hex::playGame(){
    string input;
    auto file = false, invalid = false;
    int x, s = 0;
    char y;
    if(getIsloaded() == 0){//checks if the game is loaded or created for the first time
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
        for(int i = 0; getGameMode() != 1 && getGameMode() != 2; i++){
            if(i == 0){
                cout << "\nSelect a game mode\n1)Player vs Player\n2)Player vs Computer\n";
            }
            cin >> game_mode;
            cin.clear();
            cin.ignore(100,'\n');
            if(getGameMode() != 1 && getGameMode() != 2){
                cout << "Invalid operation. Please enter a valid operation: ";
            }
        }
    }
    for(int i = 0, main = 0; getGameStatus() == 0 && main == 0; i++){
        if(getIsloaded() == 1 && i == 0){
            i = getTurn();
        }
        else{
            setTurn(i);
        }
        if(getGameMode() == 1){
            file = false;
            print_board();
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
                else if(input.substr(0, 4) == "MAIN"){//returns to main menu
                    main = 1;
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
            if(!file && main == 0){
                if(i%2 == 0){//player turn
                    play(x - 1, static_cast<int>(y) - 97, Player1);
                }
                else{
                    play(x - 1, static_cast<int>(y) - 97, Player2);
                }
                if(i%2 == 0){
                    if(isConnected(Player1) == true){
                        find_beginning(Player1);
                        for(int i = 0; i < occupied_indexs(getFirstCoordinates(1)); i++){
                            if(isWinner_P1(Player1, getFirstCoordinates(1)[i], 0, 0) == true){
                                print_board();
                                cout << endl << "PLAYER1 WINS!!!" << endl;
                                setStatic(count_marked_cells()*-1);
                                setGameStatus(1);
                            }
                            resetPastCoordinates(1);
                        }
                    }
                }
                else{
                    if(isConnected(Player2) == true){
                        find_beginning(Player2);
                        for(int i = 0; i < occupied_indexs(getFirstCoordinates(2)); i++){
                            if(isWinner_P2(Player2, 0, getFirstCoordinates(2)[i], 0) == true){
                                print_board();
                                cout << endl << "PLAYER2 WINS!!!" << endl;
                                setStatic(count_marked_cells()*-1);
                                setGameStatus(1);
                            }
                            resetPastCoordinates(2);
                        }
                    }
                }
            }
        }
        else{
            if(i%2 == 1){
                if(!file){
                    print_board();
                    play();
                }
                file = false;
            }
            else{
                print_board();
                file = false;
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
                    play(x - 1, static_cast<int>(y) - 97, Player1);
                    set_pastx(x);//sets the past coordinates for play function to use
                    set_pasty(y);
                }
            }
            if(!file && main == 0){
                if(i%2 == 0){
                    if(isConnected(Player1) == true){
                        find_beginning(Player1);
                        for(int i = 0; i < occupied_indexs(getFirstCoordinates(1)); i++){
                            if(isWinner_P1(Player1, getFirstCoordinates(1)[i], 0, 0) == true){
                                print_board();
                                cout << endl << "PLAYER WINS!!!" << endl;
                                setStatic(count_marked_cells()*-1);
                                setGameStatus(1);
                            }
                            resetPastCoordinates(1);
                        }
                    }
                }
                else{
                    if(isConnected(Bot) == true){
                        find_beginning(Bot);
                        for(int i = 0; i < occupied_indexs(getFirstCoordinates(2)); i++){
                            if(isWinner_P2(Bot, 0, getFirstCoordinates(2)[i], 0) == true){
                                print_board();
                                cout << endl << "COMPUTER WINS!!!" << endl;
                                setStatic(count_marked_cells()*-1);
                                setGameStatus(1);
                            }
                            resetPastCoordinates(2);
                        }
                    }
                }
            }
        }
    }
}
inline void Hex::Cell::setColumn(const int& a){
    column = a;
}
inline void Hex::Cell::setRow(const int& b){
    row = b;
}
inline void Hex::Cell::setState(const cell_state& c){
    state = c;
}
inline char Hex::Cell::getColumn() const{
    return column;
}
inline int Hex::Cell::getRow() const{
    return row;
}
inline char Hex::Cell::getState() const{ 
    return state;
}
void Hex::print_board() const{//prints the game board
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
            cout << getBoard()[i][j].getState() << " ";
        }
        cout << endl;
    }
}
void Hex::setSize(const int& s){
    board_size = s;
    setBoard().resize(getSize());
    for(int i = 0; i < getSize(); i++){
        setBoard()[i].resize(getSize());
    }
    for(int i = 0; i < getSize(); i++){
        for(int j = 0; j < getSize(); j++){
            setBoard()[i][j].setRow(i);
            setBoard()[i][j].setColumn(j); 
        }
    }
}
void Hex::play(const int& x, const int& y, const cell_state& player){
    setBoard()[x][y].setState(player);
    setStatic(1);
}
bool Hex::check_cell_state(const int& x, const char& y) const{//returns true if the cell is empty
    if(x - 1 >= 0 && x - 1 <= getSize() - 1 && static_cast<int>(y) - 97 >= 0 && static_cast<int>(y) - 97 <= getSize() - 1){
        if(getBoard()[x - 1][static_cast<int>(y) - 97].getState() == Empty){
            return true;
        }
    }
    return false;
}
inline void Hex::setStatic(const int& a){
    marked_cells += a;
}
int Hex::count_marked_cells() const{
    int counter = 0;
    for(int i = 0; i < board_size; i++){
        for(int j = 0; j < board_size; j++){  
            if(getBoard()[i][j].getState() != Empty){
                counter++;
            }
        }
    }
    return counter;
}
inline void Hex::setGameMode(const int& mode){
    game_mode = mode;
}
inline int Hex::getGameMode() const{
    return game_mode;
}
void Hex::load_game(int& player_turn, string filename){
    int marked_cells;
    ifstream fin;
    string s;
    fin.open(filename);
    if(fin){
        marked_cells = count_marked_cells();
        getline(fin, s);
        setSize(stoi(s));
        getline(fin, s);
        setGameMode(stoi(s));
        getline(fin, s);
        player_turn = stoi(s) - 1;
        getline(fin, s);
        for(int i = 0, k = 0; i < getSize(); i++){
            for(int j = 0; j < getSize(); j++, k++){
                setBoard()[i][j].setState(static_cast<cell_state>(s[k]));
            }
        }
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
        fout << getSize() << endl << getGameMode() << endl << player_turn%2 + 1 << endl;
        for(int i = 0; i < getSize(); i++){
            for(int j = 0; j < getSize(); j++){
                fout << getBoard()[i][j].getState();
            }
        }
    }
    else{ 
        cerr << "An error occured while opening " << filename << endl;
    }
    fout.close();
}
inline int Hex::getSize() const{
    return board_size;
}
bool Hex::isWinner_P1(const cell_state& player, const int& x, const int& y, int rotation){
    if(getBoard()[x][y].getState() != player){
        return false;
    }
    else if(getBoard()[x][y].getState() == player && y == getSize() - 1){
        setBoard()[x][y].setState(static_cast<cell_state>(Player1 - 32));
        return true;
    }
    else{
        //function determines the coordinates of the previous node by finding the opposite rotation of current one and records these coordinates so that function doesn't check the same coordinates over and over again and this prevents the function from getting into an infinite loop
        if(rotation == 1){
            setPastCoordinates(1)[occupied_indexs(getPastCoordinates(1))].setx(x + 1);
            setPastCoordinates(1)[occupied_indexs(getPastCoordinates(1))].sety(y);
        }
        if(rotation == 2){
            setPastCoordinates(1)[occupied_indexs(getPastCoordinates(1))].setx(x + 1);
            setPastCoordinates(1)[occupied_indexs(getPastCoordinates(1))].sety(y - 1);
        }
        if(rotation == 3){
            setPastCoordinates(1)[occupied_indexs(getPastCoordinates(1))].setx(x);
            setPastCoordinates(1)[occupied_indexs(getPastCoordinates(1))].sety(y - 1);
        }
        if(rotation == 4){
            setPastCoordinates(1)[occupied_indexs(getPastCoordinates(1))].setx(x - 1);
            setPastCoordinates(1)[occupied_indexs(getPastCoordinates(1))].sety(y);
        }
        if(rotation == 5){
            setPastCoordinates(1)[occupied_indexs(getPastCoordinates(1))].setx(x - 1);
            setPastCoordinates(1)[occupied_indexs(getPastCoordinates(1))].sety(y + 1);
        }
        if(rotation == 6){
            setPastCoordinates(1)[occupied_indexs(getPastCoordinates(1))].setx(x);
            setPastCoordinates(1)[occupied_indexs(getPastCoordinates(1))].sety(y + 1);
        }
        //rotations starts from up and goes on clockwise
        //if isWinner_P1 is true it uppercases the current node so that only the winning path is uppercased 
        if(x - 1 >= 0 && check_coordinates(x - 1, y, 1) == true){
            rotation = 1;
            if(isWinner_P1(player, x - 1, y, 1) == true){
                if(getBoard()[x][y].getState() == Player1){
                    setBoard()[x][y].setState(static_cast<cell_state>(Player1 - 32));
                }
                return true;
            }
        }
        if(x - 1 >= 0 && y + 1 <= getSize() - 1 && check_coordinates(x - 1, y + 1, 1) == true){
            rotation = 2;
            if(isWinner_P1(player, x - 1, y + 1, 2) == true){
                if(getBoard()[x][y].getState() == Player1){
                    setBoard()[x][y].setState(static_cast<cell_state>(Player1 - 32));
                }
                return true;
            }
        }
        if(y + 1 <= getSize() - 1 && check_coordinates(x, y + 1, 1) == true){
            rotation = 3;
            if(isWinner_P1(player, x, y + 1, 3) == true){
                if(getBoard()[x][y].getState() == Player1){
                    setBoard()[x][y].setState(static_cast<cell_state>(Player1 - 32));
                }
                return true;
            }
        }
        if(x + 1 <= getSize() - 1 && check_coordinates(x + 1, y, 1) == true){
            rotation = 4;
            if(isWinner_P1(player, x + 1, y, 4) == true){
                if(getBoard()[x][y].getState() == Player1){
                    setBoard()[x][y].setState(static_cast<cell_state>(Player1 - 32));
                }
                return true;
            }
        }
        if(x + 1 <= getSize() - 1 && y - 1 >= 0 && check_coordinates(x + 1, y - 1, 1) == true){
            rotation = 5;
            if(isWinner_P1(player, x + 1, y - 1, 5) == true){
                if(getBoard()[x][y].getState() == Player1){
                    setBoard()[x][y].setState(static_cast<cell_state>(Player1 - 32));
                }
                return true;
            }
        }
        if(y - 1 >= 0 && check_coordinates(x, y - 1, 1) == true){
            rotation = 6;
            if(isWinner_P1(player, x, y - 1, 6) == true){
                if(getBoard()[x][y].getState() == Player1){
                    setBoard()[x][y].setState(static_cast<cell_state>(Player1 - 32));
                }
                return true;
            }
        }
        return false;
    }
}
bool Hex::isWinner_P2(const cell_state& player, const int& x, const int& y, int rotation){
    if(getBoard()[x][y].getState() != player){
        return false;
    }
    else if(getBoard()[x][y].getState() == player && x == getSize() - 1){
        setBoard()[x][y].setState(static_cast<cell_state>(Player2 - 32));
        return true;
    }
    else{
        //function determines the coordinates of the previous node by finding the opposite rotation of current one and records these coordinates so that function doesn't check the same coordinates over and over again and this prevents the function from getting into an infinite loop
        if(rotation == 1){
            setPastCoordinates(2)[occupied_indexs(getPastCoordinates(2))].setx(x);
            setPastCoordinates(2)[occupied_indexs(getPastCoordinates(2))].sety(y - 1);
        }
        if(rotation == 2){
            setPastCoordinates(2)[occupied_indexs(getPastCoordinates(2))].setx(x - 1);
            setPastCoordinates(2)[occupied_indexs(getPastCoordinates(2))].sety(y);
        }
        if(rotation == 3){
            setPastCoordinates(2)[occupied_indexs(getPastCoordinates(2))].setx(x - 1);
            setPastCoordinates(2)[occupied_indexs(getPastCoordinates(2))].sety(y + 1);
        }
        if(rotation == 4){
            setPastCoordinates(2)[occupied_indexs(getPastCoordinates(2))].setx(x);
            setPastCoordinates(2)[occupied_indexs(getPastCoordinates(2))].sety(y + 1);
        }
        if(rotation == 5){
            setPastCoordinates(2)[occupied_indexs(getPastCoordinates(2))].setx(x + 1);
            setPastCoordinates(2)[occupied_indexs(getPastCoordinates(2))].sety(y);
        }
        if(rotation == 6){
            setPastCoordinates(2)[occupied_indexs(getPastCoordinates(2))].setx(x + 1);
            setPastCoordinates(2)[occupied_indexs(getPastCoordinates(2))].sety(y - 1);
        }
        //rotations starts from right and goes on clockwise
        //if isWinner_P2 is true it uppercases the current node so that only the winning path is uppercased 
        if(y + 1 <= getSize() - 1 && check_coordinates(x, y + 1, 2) == true){
            rotation = 1;
            if(isWinner_P2(player, x, y + 1, 1) == true){
                if(getBoard()[x][y].getState() == Player2){
                    setBoard()[x][y].setState(static_cast<cell_state>(Player2 - 32));
                }
                return true;
            }
        }
        if(x + 1 <= getSize() - 1 && check_coordinates(x + 1, y, 2) == true){
            rotation = 2;
            if(isWinner_P2(player, x + 1, y, 2) == true){
                if(getBoard()[x][y].getState() == Player2){
                    setBoard()[x][y].setState(static_cast<cell_state>(Player2 - 32));
                }
                return true;
            }
        }
        if(x + 1 <= getSize() - 1 && y - 1 >= 0 && check_coordinates(x + 1, y - 1, 2) == true){
            rotation = 3;
            if(isWinner_P2(player, x + 1, y - 1, 3) == true){
                if(getBoard()[x][y].getState() == Player2){
                    setBoard()[x][y].setState(static_cast<cell_state>(Player2 - 32));
                }
                return true;
            }
        }
        if(y - 1 >= 0 && check_coordinates(x, y - 1, 2) == true){
            rotation = 4;
            if(isWinner_P2(player, x, y - 1, 4) == true){
                if(getBoard()[x][y].getState() == Player2){
                    setBoard()[x][y].setState(static_cast<cell_state>(Player2 - 32));
                }
                return true;
            }
        }
        if(x - 1 >= 0 && check_coordinates(x - 1, y, 2) == true){
            rotation = 5;
            if(isWinner_P2(player, x - 1, y, 5) == true){
                if(getBoard()[x][y].getState() == Player2){
                    setBoard()[x][y].setState(static_cast<cell_state>(Player2 - 32));
                }
                return true;
            }
        }
        if(x - 1 >= 0 && y + 1 <= getSize() - 1 && check_coordinates(x - 1, y + 1, 2) == true){
            rotation = 6;
            if(isWinner_P2(player, x - 1, y + 1, 6) == true){
                if(getBoard()[x][y].getState() == Player2){
                    setBoard()[x][y].setState(static_cast<cell_state>(Player2 - 32));
                }
                return true;
            }
        }
        return false;
    }
}
int Hex::occupied_indexs(vector<Hex::past_coordinates> vec){
    int counter = 0;
    for(int i = 0; i < vec.size(); i++){
        if(vec[i].gety() != -1){
            counter++;
        }
    }
    return counter;
}
int Hex::occupied_indexs(vector<int> vec){
    int counter = 0;
    for(int i = 0; i < vec.size(); i++){
        if(vec[i] != -1){
            counter++;
        }
    }
    return counter;
}
inline vector<vector<Hex::Cell> >& Hex::setBoard(){
    return hexCells;
}
inline const vector<vector<Hex::Cell> > Hex::getBoard() const{
    return hexCells;
}
inline vector<Hex::past_coordinates>& Hex::setPastCoordinates(int player){
    if(player == 1){
        return p1_past_coordinates;
    }
    else{
        return p2_past_coordinates;
    }
}
inline const vector<Hex::past_coordinates> Hex::getPastCoordinates(int player){
    if(player == 1){
        return p1_past_coordinates;
    }
    else{
        return p2_past_coordinates;
    }
}
inline vector<int>& Hex::setFirstCoordinates(int player){
    if(player == 1){
        return p1_first_coordinates;
    }
    else{
        return p2_first_coordinates;
    }
}
inline const vector<int> Hex::getFirstCoordinates(int player){
    if(player == 1){
        return p1_first_coordinates;
    }
    else{
        return p2_first_coordinates;
    }
}
void Hex::initializeFirstCoordinates(){//initializes the vectors with -1 according to the size
    p1_first_coordinates.resize(getSize(), -1);
    p2_first_coordinates.resize(getSize(), -1);
}
void Hex::initializePastCoordinates(){//initializes the vectors with -1 according to the size
    p1_past_coordinates.resize(getSize()*getSize()/2);
    p2_past_coordinates.resize(getSize()*getSize()/2);
    for(int i = 0; i < getSize()*getSize()/2; i++){
        p1_past_coordinates[i].setx(-1);
        p1_past_coordinates[i].sety(-1);
        p2_past_coordinates[i].setx(-1);
        p2_past_coordinates[i].sety(-1);
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
void Hex::past_coordinates::setx(const int& a){
    x = a;
}
void Hex::past_coordinates::sety(const int& b){
    y = b;
}
bool Hex::isConnected(cell_state player){//determines if a player or computer has a node on both sides
    int check1 = 0, check2 = 0;
    if(player == Player1){
        for(int i = 0; i < getSize() && check1 == 0; i++){
            if(getBoard()[i][0].getState() == player){
                check1++;
            }
        }
        for(int i = 0; i < getSize() && check2 == 0; i++){
            if(getBoard()[i][getSize() - 1].getState() == player){
                check2++;
            }
        }
    }
    else{
        for(int i = 0; i < getSize() && check1 == 0; i++){
            if(getBoard()[0][i].getState() == player){
                check1++;
            }
        }
        for(int i = 0; i < getSize() && check2 == 0; i++){
            if(getBoard()[getSize() - 1][i].getState() == player){
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
            if(getBoard()[i][0].getState() == player){
                setFirstCoordinates(1)[j] = i;
                j++;
            }
        }
    }
    else{
        for(int i = 0; i < getSize(); i++){
            if(getBoard()[0][i].getState() == player){
                setFirstCoordinates(2)[j] = i;
                j++;
            }
        }
    }
}
inline int Hex::past_coordinates::getx() const{
    return x;
}
inline int Hex::past_coordinates::gety() const{
    return y;
}
bool Hex::check_coordinates(const int& x, const int& y, int player){//checks if the node is already been visited by isWinner function 
    for(int i = 0; i < getSize()*getSize()/2; i++){
        if(getPastCoordinates(player)[i].getx() == x && getPastCoordinates(player)[i].gety() == y){
            return false;
        }
    }
    return true;
}
void Hex::play(){//makes moves for bot
    if(check_cell_state(get_pastx(), get_pasty() + 1) == true){
        setBoard()[get_pastx() - 1][static_cast<int>(get_pasty()) - 96].setState(Bot);
        cout << "\nComputer's move: " << static_cast<char>(get_pasty() + 1) << get_pastx() << endl;
    }
    else if(check_cell_state(get_pastx() - 1, get_pasty() + 1) == true){
        setBoard()[get_pastx() - 2][static_cast<int>(get_pasty()) - 96].setState(Bot);
        cout << "\nComputer's move: " << static_cast<char>(get_pasty() + 1) << get_pastx() - 1 << endl;
    }
    else if(check_cell_state(get_pastx() + 1, get_pasty()) == true){
        setBoard()[get_pastx()][static_cast<int>(get_pasty()) - 97].setState(Bot);
        cout << "\nComputer's move: " << get_pasty() << get_pastx() + 1 << endl;
    }
    else if(check_cell_state(get_pastx() - 1, get_pasty()) == true){
        setBoard()[get_pastx() - 2][static_cast<int>(get_pasty()) - 97].setState(Bot);
        cout << "\nComputer's move: " << get_pasty() << get_pastx() - 1 << endl;
    }
    else if(check_cell_state(get_pastx() + 1, get_pasty() - 1) == true){
        setBoard()[get_pastx()][static_cast<int>(get_pasty()) - 98].setState(Bot);
        cout << "\nComputer's move: " << static_cast<char>(get_pasty() - 1) << get_pastx() + 1 << endl;
    }
    else if(check_cell_state(get_pastx(), get_pasty() - 1) == true){
        setBoard()[get_pastx() - 1][static_cast<int>(get_pasty()) - 98].setState(Bot);
        cout << "\nComputer's move: " << static_cast<char>(get_pasty() - 1) << get_pastx() << endl;
    }
    else{
        for(int i = 0; i < getSize(); i++){
            for(int j = 0; j < getSize(); j++){
                if(check_cell_state(i, j) == true){
                    setBoard()[i][j].setState(Bot);
                    cout << "\nComputer's move: " << static_cast<char>(j + 97) << i + 1 << endl;
                    break;
                }
            }
        }
    }
    setStatic(1);
}
inline void Hex::setGameStatus(int gs){
    game_status = gs;
}
inline int Hex::getGameStatus() const{
    return game_status;
}
bool Hex::compareGames(const Hex& other) const{//compares the marked cells of two games
    if(count_marked_cells() > other.count_marked_cells()){
        return true;
    }
    else{
        return false;
    }
}
void Hex::setName(const std::string& n){
    name = n;
}
string Hex::getName() const{
    return name;
}
inline void Hex::setIsloaded(const int& a){
    isloaded = a;
}
inline void Hex::setTurn(const int& a){
    turn = a;
}
inline int Hex::getIsloaded() const{
    return isloaded;
}
inline int Hex::getTurn() const{
    return turn;
}