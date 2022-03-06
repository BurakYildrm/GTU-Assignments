#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;
class Relations{
    public:
        int element_num, r_num;
        vector<char> reflexive_pairs;
        vector<vector<char> > transitive_pairs;
        vector<vector<char> > non_anti_symmetrics;
        vector<vector<char> > non_transitives;
        vector<vector<char> > relations;
        vector<vector<char> > copy_relations;
        vector<char> elements;
        vector<char> invalid_input;
        bool isReflexive();
        bool isAnti_Symmetric();
        bool isTransitive();
        void print_poset(ofstream& fout, const int& check);
        bool check_validity();
        void print_set(ofstream& fout);
        void makeReflexive();
        void makeAntiSymmetric();
        void makeTransitive();
        bool checkPoset();
        void convertToPoset();
        void removeReflexive();
        void removeTransitive();
};
int main(){
    ifstream fin;
    ofstream fout;
    string input;
    fin.open("input.txt");  //opens input.txt to read
    fout.open("output.txt");    //opens output.txt to write
    Relations var;
    while(!fin.eof()){   //checks if the file ended
        getline(fin, input);
        if(input.size() == 0){  //a safety measure for the empty lines at bottom of the file
            break;
        }
        else{
            var.r_num = stoi(input); //assigns number of relations to r_num variable
            getline(fin, input); //read the whole line
            var.relations.resize(var.r_num); //allocates memory according to the r_num variable
            for(int i = 0; i < var.r_num; i++) var.relations[i].resize(2);
            var.element_num = (input.length() + 1)/2; //assigns number of elements to element_num variable
            var.elements.resize(var.element_num);//allocates memory according to the element_num variable
            for(int i = 0, k = 0; i < input.length(); i++){ //stores elements in a vector
                if(input[i] != ','){ //skips comma character
                    var.elements[k] = input[i];
                    k++;
                }
            }
            for(int i = 0; i < var.r_num; i++){ //stores relations in a 2d vector
                getline(fin, input);
                var.relations[i][0] = input[0];
                var.relations[i][1] = input[2]; //uses index number 2 to skip comma character in index number 1
            }
            
            if(var.check_validity() == true){ //checks if there is any invalid input in relations
                if(var.checkPoset() == false){ //checks if the given relations set is a poset
                    var.convertToPoset(); //if not then converts it to poset
                }
                var.copy_relations.resize(var.relations.size()); //allocates memory according to the size of relations vector
                for(int i = 0; i < var.relations.size(); i++) var.copy_relations[i].resize(2);
                //copies relations vector's all contents to copy relations vector before the remove functions so that print_poset function can print the poset after remove functions
                for(int i = 0; i < var.relations.size(); i++) for(int j = 0; j < 2; j++) var.copy_relations[i][j] = var.relations[i][j]; 
                var.removeReflexive(); //removes all reflexive pairs
                var.removeTransitive(); //removes all transitive pairs
                var.print_poset(fout, 0); //prints the results. 0 means check_validity functions returned true
            }
            else{
                var.print_poset(fout, 1);//prints the results. 1 means check_validity functions returned false
            }
        }
    }
    fin.close(); //closes input.txt
    fout.close(); //closes output.txt
    return 0;
}
bool Relations::isReflexive(){
    reflexive_pairs.clear(); //clears all the content of reflexive_pairs vector so that every time this function is called it stores new information in an empty vector, not in a vector full of old, useless information.
    for(int i = 0; i < relations.size(); i++){
        if(relations[i][0] == relations[i][1]){ //checks if the current relation is reflexive
            //if it is then stores it in reflexive pairs vector
            reflexive_pairs.resize(reflexive_pairs.size() + 1);
            reflexive_pairs[reflexive_pairs.size() - 1] = relations[i][0];
            //then deletes the reflexive element from elements vector so that if there is no element left in the elements vector that means that given relations set is reflexive
            for(int k = 0; k < elements.size(); k++){
                if(relations[i][0] == elements[k]){
                    elements.erase(elements.begin() + k);
                }
            }
        }
    }
    if(elements.size() == 0) return true;
    else return false; //if there are elements in elements vector that means that this relations set does not have a reflexive pair for all the elements
}
bool Relations::isAnti_Symmetric(){
    //clears all the content of non_anti_symmetrics vector so that every time this function is called it stores new information in an empty vector, not in a vector full of old, useless information.
    for(int i = 0; i < non_anti_symmetrics.size(); i++) non_anti_symmetrics[i].clear();
    non_anti_symmetrics.clear();
    for(int i = 0, check, check2; i < relations.size(); i++){
        check = 0;
        if(relations[i][0] != relations[i][1]){ //checks only non reflexive pairs because reflexive pairs does not affect the anti symmetric property
            for(int j = 0; j < relations.size(); j++){
                if(relations[j][0] == relations[i][1] && relations[j][1] == relations[i][0]){//checks if there is a symmetric version of the current relation
                    check++; //if there is then increments the check by 1
                    break;
                }
            }
            if(check == 1){
                check2 = 0;
                for(int j = 0; j < non_anti_symmetrics.size(); j++){ //checks if the symmetric version of the the current relations is already stored
                    if(non_anti_symmetrics[j][0] == relations[i][1] && non_anti_symmetrics[j][1] == relations[i][0]){
                        check2++; //if it is then increments the check2 by 1
                        break;
                    }
                }
                if(check2 == 0){ //if it is not then stores the current relation in non_anti_symmetric vector
                    non_anti_symmetrics.resize(non_anti_symmetrics.size() + 1);
                    non_anti_symmetrics[non_anti_symmetrics.size() - 1].push_back(relations[i][0]);
                    non_anti_symmetrics[non_anti_symmetrics.size() - 1].push_back(relations[i][1]);
                }
            }
        }
    }
    if(non_anti_symmetrics.size() == 0) return true; //this means there is no non anti symmetric relation so that given relations set is anti symmetric
    return false; //this means there is at least one non anti symmetric relation so that given relations set is not anti symmetric
}
bool Relations::isTransitive(){
    char a, b, c;
    //clears all the content of non_transitives vector so that every time this function is called it stores new information in an empty vector, not in a vector full of old, useless information.
    for(int i = 0; i < non_transitives.size(); i++) non_transitives[i].clear();
    non_transitives.clear();
    //clears all the content of transitive_pairs vector so that every time this function is called it stores new information in an empty vector, not in a vector full of old, useless information.
    for(int i = 0; i < transitive_pairs.size(); i++) transitive_pairs[i].clear();
    transitive_pairs.clear();
    for(int i = 0, check, counter; i < relations.size(); i++){//check transitive property
        if(relations[i][0] != relations[i][1]){ //checks only non reflexive pairs because reflexive pairs does not affect the transitive property
            a = relations[i][0];
            b = relations[i][1];
            for(int j = 0; j < relations.size(); j++){ //checks all the relations that starts with b which are non reflexive 
                counter = 0;
                if(relations[j][0] == b && relations[j][1] != b){
                    c = relations[j][1];
                    for(int k = 0; k < relations.size(); k++){
                        if(relations[k][0] == a && relations[k][1] == c){ //checks if there is a relation (a,c)
                            //if there is then stores it in transitive_pairs vector
                            transitive_pairs.resize(transitive_pairs.size() + 1);
                            for(int l = 0; l < transitive_pairs.size(); l++) transitive_pairs[l].resize(2);
                            transitive_pairs[transitive_pairs.size() - 1][0] = a;
                            transitive_pairs[transitive_pairs.size() - 1][1] = c;
                            counter++;
                            break;
                        }
                    }
                    if(counter == 0){ //if there is not then stores the current relation in non_transitive vector
                        check = 0;
                        for(int k = 0; k < non_transitives.size(); k++){ //checks if the same a and c are already stored
                            if(non_transitives[k][0] == a && non_transitives[k][1] == c){
                                check++; //if they are then increments check by 1
                                break;
                            }
                        }
                        if(check == 0){ //if they are not then stores a and c in non_transitives vector
                            non_transitives.resize(non_transitives.size() + 1);
                            non_transitives[non_transitives.size() - 1].push_back(a);
                            non_transitives[non_transitives.size() - 1].push_back(c);
                        }
                    }
                }
            }
        }
    }
    if(non_transitives.size() == 0) return true; //this means there is no non transitive relation so that given relations set is transitive
    else return false; //this means there is at least one non transitive relation so that given relations set is not transitive
}
bool Relations::check_validity(){
    for(int i = 0, counter1, counter2; i < relations.size(); i++){
        counter1 = 0;
        counter2 = 0;
        for(int j = 0; j < elements.size(); j++){//compares the first and the second argument of the current relation with all the elements
            if(elements[j] == relations[i][0]){ //if the first argument is one of the elements then increments counter1 by 1
                counter1++;//
            }
            if(elements[j] == relations[i][1]){ //if the second argument is one of the elements then increments counter2 by 1
                counter2++;
            }
        }
        if(counter1 == 0 || counter2 == 0){ //this means that at least one of the arguments is not in the elements vector so that it is a invalid input
            //stores the argument of the current relation in invalid_input vector
            invalid_input.push_back(relations[i][0]);
            invalid_input.push_back(relations[i][1]);
            if(counter1 == 0 && counter2 == 0){ //this means both the first and the second argument of the current relation are invalid
                invalid_input.push_back('3'); //3 is for both the arguments
            }
            else if(counter2 == 0){ //this means the second argument of the current relation is invalid
                invalid_input.push_back('2'); //2 is for the second argument
            }
            else{ //this means the first argument of the current relation is invalid
                invalid_input.push_back('1'); //1 is for the first argument
            }
            return false;
        }
    }
    return true;
}
void Relations::print_set(ofstream& fout){ //prints the elements
    for(int i = 0; i < elements.size(); i++){
        if(i == 0){
            fout << "{" << elements[i] << ",";
        }
        else if(i == elements.size() - 1){
            fout << elements[i] << "}";
        }
        else{
            fout << elements[i] << ",";
        }
    }
}
bool Relations::checkPoset(){
    if(isReflexive() == true && isAnti_Symmetric() == true && isTransitive() == true) return true; //checks if the relations set is a poset
    return false;
}
void Relations::convertToPoset(){
    srand(time(NULL)); //for the random choice in makeAntiSymmetric function
    for(int i = 0, counter; counter != 3; i++){ //loop continues until the relation is reflexive, anti symmetric and transitive
        counter = 0;
        if(isReflexive() == false) makeReflexive();
        else counter++;
        if(isAnti_Symmetric() == false) makeAntiSymmetric();
        else counter++;
        if(isTransitive() == false) makeTransitive();
        else counter++;
    }
}
void Relations::makeReflexive(){
    for(int i = 0; i < elements.size(); i++){ //adds the remaining elements as reflexive pairs to the relations vector
        relations.resize(relations.size() + 1);
        relations[relations.size() - 1].push_back(elements[i]);
        relations[relations.size() - 1].push_back(elements[i]);
    }
}
void Relations::makeAntiSymmetric(){
    for(int i = 0, k, j, chance; i < non_anti_symmetrics.size(); i++){
        for(k = 0; k < relations.size(); k++){ //k is the index of the current non anti symmetric relation in relations vector
            if(relations[k][0] == non_anti_symmetrics[i][0] && relations[k][1] == non_anti_symmetrics[i][1]) break;
        }
        for(j = 0; j < relations.size(); j++){ //j is the index of the symmetric version of the current non anti symmetric relation in relations vector
            if(relations[j][0] == non_anti_symmetrics[i][1] && relations[j][1] == non_anti_symmetrics[i][0]) break;
        }
        chance = rand() % 2; //chance is either 0 or 1
        if(chance == 0){
            relations.erase(relations.begin() + k); //the current non anti symmetric relation is erased
        } 
        else{
            relations.erase(relations.begin() + j); //the symmetric version of the current non anti symmetric relation is erased
        }
    }
}
void Relations::makeTransitive(){
    for(int i = 0; i < non_transitives.size(); i++){ //adds the relations in the non_transitive vector to the relations vector
        relations.resize(relations.size() + 1);
        relations[relations.size() - 1].push_back(non_transitives[i][0]);
        relations[relations.size() - 1].push_back(non_transitives[i][1]);
    }
}
void Relations::print_poset(ofstream& fout, const int& check){
    if(check == 1){ //this means there is invalid input
        if(invalid_input[2] == '3'){ //this mean both of the arguments of the relation is invalid
            fout << "\nBoth of the elements of the relation (" << invalid_input[0] << ", " << invalid_input[1] << ") is not found in the set A = ";
        }
        else{ //invalid_input[static_cast<int>(invalid_input[2]) - 49] is the invalid argument of the relation
            fout << "\nThe element " << invalid_input[static_cast<int>(invalid_input[2]) - 49] << " of the relation (" << invalid_input[0] << ", " << invalid_input[1] << ") is not found in the set A = ";
        }
        print_set(fout); //prints elements
        fout << endl;
    }
    else{
        fout << "n" << endl << "POSET: ";
        for(int i = 0; i < copy_relations.size(); i++){ //prints the poset before remove functions
            if(i != copy_relations.size() - 1){
                fout << "(" << copy_relations[i][0] << ", " << copy_relations[i][1] << "), ";
            }
            else{
                fout << "(" << copy_relations[i][0] << ", " << copy_relations[i][1] << ")\n";
            }
        }
        for(int i = 0; i < relations.size(); i++){ //prints the relations vector after remove functions
            fout << relations[i][0] << "," << relations[i][1] << endl;
        }
    }
}
void Relations::removeReflexive(){
    for(int i = 0, k; i < reflexive_pairs.size(); i++){ //erases all the reflexive pairs from the relations vector
        for(k = 0; relations[k][0] != reflexive_pairs[i] || relations[k][1] != reflexive_pairs[i]; k++);
        relations.erase(relations.begin() + k);
    }
}
void Relations::removeTransitive(){
    for(int i = 0, j, k; i < transitive_pairs.size(); i++){ //erases all the transitive pairs from the relations vector
        for(j = 0; j < relations.size() && (transitive_pairs[i][0] != relations[j][0] || transitive_pairs[i][1] != relations[j][1]); j++);
        if(j != relations.size()){
            for(k = 0; relations[k][0] != transitive_pairs[i][0] || relations[k][1] != transitive_pairs[i][1]; k++);
            relations.erase(relations.begin() + k);
        }
    }
}