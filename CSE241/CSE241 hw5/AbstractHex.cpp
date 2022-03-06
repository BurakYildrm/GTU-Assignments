#include <iostream>
#include "AbstractHex.h"

using namespace std;
using namespace Abstract;
using namespace Exceptions;

/*return true if gameStatus is 1*/
bool AbstractHex::isEnd(){
    if(gameStatus == 1) return true;
    else return false;
}
/*checks if the boards are the same by checking every cell state*/
bool AbstractHex::operator==(AbstractHex& other){
    int counter = 0;
    if(this->boardSize == other.boardSize){
        try{
            for(int i = 0; i < this->boardSize; i++){
                for(int j = 0; j < this->boardSize; j++){
                    if((*this)(i, j).getState() == other(i, j).getState()){
                        counter++;
                    }
                }
            }
            if(counter == this->boardSize*this->boardSize) return true;
        }
        catch(OutOfSize& outOfSize){
            cout << outOfSize.what() << endl;
        }
    }
    return false;
}
/*if there is no last move throws an exception*/
Cell AbstractHex::lastMove(){
    if(last_move.getRow() == -1){
        throw NoLastMove();
    }
    return last_move;
}
/*sets the sizes of past coordinates with size of boardsize and initializes the pastcoordinates' x and y with -1 for occupied_indexs function*/
void AbstractHex::initializePastCoordinates(){
    p1_past_coordinates.resize(getSize()*getSize()/2);
    p2_past_coordinates.resize(getSize()*getSize()/2);
    for(int i = 0; i < getSize()*getSize()/2; i++){
        p1_past_coordinates[i].setx(-1);
        p1_past_coordinates[i].sety(-1);
        p2_past_coordinates[i].setx(-1);
        p2_past_coordinates[i].sety(-1);
    }
}
/*resets pastcoordinates' x and y to -1*/
void AbstractHex::resetPastCoordinates(const cell_state& player){
    if(player == Player1){
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
/*returns the number of occupied indexes*/
int AbstractHex::occupied_indexs(std::vector<past_coordinates> vec, const int& size){
    int counter = 0;
    for(int i = 0; i < size; i++){
        if(vec[i].gety() != -1){
            counter++;
        }
    }
    return counter;
}
/*compares the given coordinates with past coordinates vectors and returns true if the given coordinates are not in the past coordinates vectors*/
bool AbstractHex::check_coordinates(const int& x, const int& y, const cell_state& player){
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
/*constructor*/
AbstractHex::AbstractHex() : gameMode(0), gameStatus(0), moveNum(0), isloaded(0){
    last_move.setRow(-1);
    last_move.setColumn(-1);
    last_move.setState(Empty);
}
/*constructor with size argument*/
AbstractHex::AbstractHex(const int& s) : gameMode(0), gameStatus(0), moveNum(0), isloaded(0){
    last_move.setRow(-1);
    last_move.setColumn(-1);
    last_move.setState(Empty);
}
/*constructor with size and gamestatus arguments*/
AbstractHex::AbstractHex(const int& s, const int& gm) : gameMode(gm), gameStatus(0), moveNum(0), isloaded(0){
    last_move.setRow(-1);
    last_move.setColumn(-1);
    last_move.setState(Empty);
}
/*cell constructor*/
Cell::Cell() : row(0), column(0), state(Empty){/*Intentionally empty*/}
/*cell constructor with row, column and state arguments*/
Cell::Cell(const int& r, const int& c, const cell_state& s) : row(r), column(c), state(s){/*Intentionally empty*/}