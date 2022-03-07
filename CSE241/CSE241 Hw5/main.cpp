#include <iostream>
#include "HexVector.h"
#include "AbstractHex.h"
#include "HexArray1D.h"
#include "HexAdapter.h"

using namespace std;
using namespace HexVec;
using namespace Abstract;
using namespace Exceptions;
using namespace HexArr;
using namespace HexAdapt;

/*checks if the cellstates of all indexes of all games are valid*/
bool isValid(AbstractHex** arr){
    AbstractHex* temp;
    char c;
    for(int i = 0; i < 5; i++){
        temp = arr[i];
        for(int j = 0; j < temp->getSize(); j++){
            for(int k = 0; k < temp->getSize(); k++){
                c = (*temp)(j, k).getState();
                if(c != 'x' && c != 'o' && c != '.'){
                    return false;
                }
            }
        }
    }
    return true;
}

int main(){
    string game_name, gn1, gn2;
    int index1, index2, choice = 0, check = -1, check2 = 0, check3 = 0, choice2 = 0, choice3 = 0;
    vector<AbstractHex*> games;
    AbstractHex* arr[5];
    /*menu*/
    for(int i = 0; choice != 5; i++){
        cout << "\tMAIN MENU\n\n";
        if(games.size() == 0){
            cout << "1)Start a game\n";
        }
        else{
            cout << "1)Start another game\n";
        }
        cout << "2)Load a game\n3)Compare two games\n4)Show marked cells\n5)Quit the menu\n";
        cin >> choice;
        for(int j = 0; choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5; j++){
            cout << "Invalid operation. Please enter a valid operation: ";
            cin >> choice;
        }
        if(choice == 1){
            cout << "Please enter the name of game(enter q to return to main menu): ";
            cin >> game_name;
            if(game_name[0] != 'q'){
                for(int k = 0; check2 == 0; k++){//checks if the if name exists
                    for(int j = 0; j < games.size(); j++){
                        if(games[j]->getName() == game_name){
                            check3++;
                            break;
                        }
                    }
                    if(check3 == 0){
                        check2++;
                    }
                    else{
                        cout << "This game already exists. Please enter a unique name: ";
                        cin >> game_name;
                        check3 = 0;
                    }
                }
                games.resize(games.size() + 1);
                cout << "Please select a class\n1)HexVector\n2)HexArray1D\n3)HexAdapter\n";
                cin >> choice2;
                if(choice2 == 1) games[games.size() - 1] = new HexVector;
                if(choice2 == 2) games[games.size() - 1] = new HexArray1D;
                if(choice2 == 3){
                    cout << "Please select a STL container\n1)Vector\n2)Deque\n";
                    cin >> choice3;
                    if(choice3 == 1) games[games.size() - 1] = new HexAdapter<vector>;
                    if(choice3 == 2) games[games.size() - 1] = new HexAdapter<deque>;
                }
                games[games.size() - 1]->setName(game_name);
                games[games.size() - 1]->playGame();
                if(games[games.size() - 1]->getGameStatus() == 1){//if the game is won by a user or bot program removes the game from vector
                    games.erase(games.begin() + games.size() - 1);
                }
                check2 = 0;
            }
        }
        else if(choice == 2){
            if(games.size() == 0){
                cout << "\nThere is no game in the memory.\n";
            }
            else{
                cout << "List of active games\n";
                for(int j = 0; j < games.size(); j++){
                    cout << j + 1 << ")" << games[j]->getName() << endl;
                }
                cout << "Which game do you want to play?(enter q to return to main menu): ";
                cin >> game_name;
                if(game_name[0] != 'q'){
                    for(int j = 0; check == -1; j++){//checks if the name exists
                        for(int k = 0; k < games.size(); k++){
                            if(games[k]->getName() == game_name){
                                check = k;
                                break;
                            }
                        }
                        if(check == -1){
                            cout << "There is no entry for " << game_name << ". Please enter a valid name: ";
                            cin >> game_name;
                        }
                    }
                    games[check]->setIsloaded(1);//sets loaded to 1 to bypass setsize function
                    games[check]->playGame();
                    if(games[check]->getGameStatus() == 1){//if the game is won by a user or bot program removes the game from vector
                        games.erase(games.begin() + check);
                    }
                    check = -1;
                }
            }
        }
        else if(choice == 3){
            cout << "List of active games\n";
            for(int j = 0; j < games.size(); j++){
                cout << j + 1 << ")" << games[j]->getName() << endl;
            }
            cout << "Please enter the name of first game(enter q to return to main menu): ";
            cin >> gn1;
            if(gn1[0] != 'q'){
                cout << "Please enter the name of second game: ";
                cin >> gn2;
                for(int k = 0; k < games.size(); k++){
                    if(games[k]->getName() == gn1){
                        index1 = k;
                        break;
                    }
                }
                for(int k = 0; k < games.size(); k++){
                    if(games[k]->getName() == gn2){
                        index2 = k;
                        break;
                    }
                }
                if(*games[index1] == *games[index2]){
                    cout << games[index1]->getName() << " and " << games[index2]->getName() << " have the same board" << endl;
                }
                else{
                    cout << games[index2]->getName() << " and " << games[index1]->getName() << " have different boards" << endl;
                }
            }
        }
        else if(choice == 4){
            cout << "List of active games\n";
            for(int j = 0; j < games.size(); j++){
                cout << j + 1 << ")" << games[j]->getName() << endl;
            }
            cout << "Please enter the name of the game(enter q to return to main menu): ";
            cin >> game_name;
            if(game_name[0] != 'q'){
                for(int j = 0; check == -1; j++){//checks if the name exists
                    for(int k = 0; k < games.size(); k++){
                        if(games[k]->getName() == game_name){
                            check = k;
                            break;
                        }
                    }
                    if(check == -1){
                        cout << "There is no entry for " << game_name << ". Please enter a valid name: ";
                        cin >> game_name;
                    }
                }
            }
            cout << "Marked cells: " << games[check]->numberOfMoves() << endl;
            check = -1;
        }
        else{
            cout << "Goodbye!\n";
        }
    }
    /*test for global function*/
    Cell c1(1, 2, static_cast<cell_state>('x')), c2(5, 6, static_cast<cell_state>('o')), c3(7, 8, static_cast<cell_state>('.')), c4(1, 2, static_cast<cell_state>('h')), c5(1, 5, static_cast<cell_state>('w'));
    HexArray1D temp1, temp2(7);
    HexVector temp3(9);
    HexAdapter<vector> temp4;
    HexAdapter<deque> temp5(6);
    temp1.play(c1);
    temp2.play(c2);
    temp3.play(c3);
    temp4.play(c4);
    temp5.play(c5);
    arr[0] = &temp1;
    arr[1] = &temp2;
    arr[2] = &temp3;
    arr[3] = &temp4;
    arr[4] = &temp5;
    for(int i = 0; i < 5; i++){
        cout << "Game#" << i + 1 << endl;
        arr[i]->print();
        cout << endl;
    }
    cout << "Is valid: " << boolalpha << isValid(arr);
    return 0;
}