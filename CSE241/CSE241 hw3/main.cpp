#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "hex_class.h"
using namespace std;
int main(){
    string game_name, gn1, gn2;
    int index1, index2, choice = 0, check = -1, check2 = 0, check3 = 0;
    vector<Hex> games;
    for(int i = 0; choice != 5; i++){
        cout << "\tMAIN MENU\n\n";
        if(games.size() == 0){
            cout << "1)Start a game\n";
        }
        else{
            cout << "1)Start another game\n";
        }
        cout << "2)Load a game\n3)Compare two games\n4)Show total marked cells\n5)Quit the menu\n";
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
                        if(games[j].getName() == game_name){
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
                games[games.size() - 1].setName(game_name);
                games[games.size() - 1].playGame();
                if(games[games.size() - 1].getGameStatus() == 1){//if the game is won by a user or bot program removes the game from vector
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
                    cout << j + 1 << ")" << games[j].getName() << endl;
                }
                cout << "Which game do you want to play?(enter q to return to main menu): ";
                cin >> game_name;
                if(game_name[0] != 'q'){
                    for(int j = 0; check == -1; j++){//checks if the name exists
                        for(int k = 0; k < games.size(); k++){
                            if(games[k].getName() == game_name){
                                check = k;
                                break;
                            }
                        }
                        if(check == -1){
                            cout << "There is no entry for " << game_name << ". Please enter a valid name: ";
                            cin >> game_name;
                        }
                    }
                    games[check].setIsloaded(1);//sets loaded to 1 to bypass setsize function
                    games[check].playGame();
                    if(games[check].getGameStatus() == 1){//if the game is won by a user or bot program removes the game from vector
                        games.erase(games.begin() + check);
                    }
                    check = -1;
                }
            }
        }
        else if(choice == 3){
            cout << "List of active games\n";
            for(int j = 0; j < games.size(); j++){
                cout << j + 1 << ")" << games[j].getName() << endl;
            }
            cout << "Please enter the name of first game(enter q to return to main menu): ";
            cin >> gn1;
            if(gn1[0] != 'q'){
                cout << "Please enter the name of second game: ";
                cin >> gn2;
                for(int k = 0; k < games.size(); k++){
                    if(games[k].getName() == gn1){
                        index1 = k;
                        break;
                    }
                }
                for(int k = 0; k < games.size(); k++){
                    if(games[k].getName() == gn2){
                        index2 = k;
                        break;
                    }
                }
                if(games[index1].compareGames(games[index2]) == true){
                    cout << games[index1].getName() << " has marked cells more than " << games[index2].getName() << endl << endl;
                }
                else{
                    cout << games[index2].getName() << " has marked cells more than or equal to " << games[index1].getName() << endl << endl;
                }
            }
        }
        else if(choice == 4){
            cout << "Total marked cells: " << games[0].getStatic() << endl << endl;
        }
        else{
            cout << "Goodbye!\n";
        }
    }
    return 0;
}