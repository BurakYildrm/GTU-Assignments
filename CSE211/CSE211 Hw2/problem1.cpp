#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;
class Relations{
    public:
        int element_num, r_num, reflexive, symmetric, anti_symmetric, transitive;
        vector<vector<char> >reasons_char;
        vector<vector<int> >reasons_int;
        vector<vector<char> > relations_char;
        vector<vector<int> > relations_int;
        vector<char> elements_char;
        vector<int> elements_int;
        vector<char> invalid_input_char;
        vector<int> invalid_input_int;
        //I used flags for functions to determine if a relation consists of integers or characters
        void isReflexive(const int& flag);
        void isSymmetric(const int& flag);
        void isAnti_Symmetric(const int& flag);
        void isTransitive(const int& flag);
        void print_relation(const int& flag, ofstream& fout);
        bool check_validity(const int& flag);
        void print_set(const int& flag, ofstream& fout);
};
int main(){
    ifstream fin;
    ofstream fout;
    string input;
    fin.open("input.txt");  //opens input.txt to read
    fout.open("output.txt");    //opens output.txt to write
    Relations var;
    int index_int = 0, index_char = 0;
    //these are the vectors where the incompatible relations are stored.
    //there are two vectors. one is for character relations and the other one is for integer relations.
    //each index is for a specific property
    //index 0 is for reflexive, index 1 is for symmetric, index 2 is for anti-symmetric and index 3 is for transitive.
    var.reasons_int.resize(4); 
    for(int i = 0; i < 4; i++){
        var.reasons_int[i].resize(3);
    }
    var.reasons_char.resize(4);
    for(int i = 0; i < 4; i++){
        var.reasons_char[i].resize(3);
    }
    var.invalid_input_char.resize(3);
    var.invalid_input_int.resize(3);
    while(fin.peek() != EOF){   //checks if the file ended
        index_char = 0;
        index_int = 0;
        getline(fin, input);
        if(input.size() == 0){  //a safety measure for the empty lines at bottom of the file
            break;
        }
        else{
            var.r_num = stoi(input);
            getline(fin, input);
            if(input[0] - 0 >= 97){ //checks if the first input is a character or an integer
                //adjusts the size of vector which will hold the relations
                var.relations_char.resize(var.r_num);
                for(int i = 0; i < var.r_num; i++){
                    var.relations_char[i].resize(2);
                }
                //adjusts the size of vector which will hold the number of elements
                var.element_num = (input.length() + 1)/2;
                var.elements_char.resize(var.element_num);
                //assigns elements to proper vector
                for(int i = 0, k = 0; i < input.length(); i++){
                    if(input[i] != ','){
                        var.elements_char[k] = input[i];
                        k++;
                    }
                }
                //assigns relations to proper vector
                for(int i = 0; i < var.r_num; i++){
                    getline(fin, input);
                    var.relations_char[index_char][0] = input[0];
                    var.relations_char[index_char][1] = input[2];
                    index_char++;
                }
                if(var.check_validity(1)){//checks the validity of relations
                    //checks the properties one by one
                    var.isReflexive(1);
                    var.isSymmetric(1);
                    var.isAnti_Symmetric(1);
                    var.isTransitive(1);
                    //prints the results
                    var.print_relation(1, fout);
                }
                else{
                    var.print_relation(31, fout);//3 represents the invalid input and 1 represents the character relation
                }
            }
            else{
                //adjusts the size of vector which will hold the relations
                var.relations_int.resize(var.r_num);
                for(int i = 0; i < var.r_num; i++){
                    var.relations_int[i].resize(2);
                }
                //adjusts the size of vector which will hold the number of elements
                var.element_num = (input.length() + 1)/2;
                var.elements_int.resize(var.element_num);
                //assigns elements to proper vector
                for(int i = 0, k = 0, j = 0; i < input.length(); i++){
                    if(input[i] != ','){
                        for(j = i; input[j] != ',' && input[j] != '\n'; j++);
                        var.elements_int[k] = stoi(input.substr(i, j - i));
                        k++;
                        /*if(input[i + 1] != ','){

                        }
                        else{
                            var.elements_int[k] = input[i] - 48;
                            k++;
                        }*/
                    }
                }
                //assigns relations to proper vector
                for(int i = 0, j = 0, k = 0; i < var.r_num; i++){
                    getline(fin, input);
                    for(j = 0; input[j] != ','; j++);
                    var.relations_int[index_int][0] = stoi(input.substr(0, j));
                    var.relations_int[index_int][1] = stoi(input.substr(j + 1, input.size() - j - 1));
                    index_int++;
                }
                if(var.check_validity(2)){//checks the validity of relations
                    //checks the properties one by one
                    var.isReflexive(2);
                    var.isSymmetric(2);
                    var.isAnti_Symmetric(2);
                    var.isTransitive(2);
                    //prints the results
                    var.print_relation(2, fout);
                }
                else{
                    var.print_relation(32, fout);//3 represents the invalid input and 2 represents the integer relation
                }
            }
        }
    }
    fin.close();    //closes input.txt
    fout.close();   //closes output.txt
    return 0;
}
void Relations::isReflexive(const int& flag){
    int counter = 0;
    if(flag == 1){//if flag is 1 then it means relation consists of characters
        //checks if reflexivity is satisfied for all elements.
        for(int i = 0; i < r_num; i++){
            if(relations_char[i][0] == relations_char[i][1]){
                for(int k = 0; k < elements_char.size(); k++){
                    //if a relation satisfies reflexivity for an element then program erases that element from elements vector to determine if there is any element left which has no reflexive relation
                    if(relations_char[i][0] == elements_char[k]){
                        elements_char.erase(elements_char.begin() + k);
                        counter++;
                    }
                }
            }
        }      
    }
    else{//if flag is 2 then it means relation consists of integers
        //checks if reflexivity is satisfied for all elements.
        for(int i = 0; i < r_num; i++){
            if(relations_int[i][0] == relations_int[i][1]){
                for(int k = 0; k < elements_int.size(); k++){
                    //if a relation satisfies reflexivity for an element then program erases that element from elements vector to determine if there is any element left which has no reflexive relation
                    if(relations_int[i][0] == elements_int[k]){
                        elements_int.erase(elements_int.begin() + k);
                        counter++;
                    }
                }
            }
        }
    }
    if(counter == element_num) reflexive = 1;//this means every element satisfies reflexivity
    //if an element left then the function assigns it to the index 0 of reasons vector.
    else{
        reflexive = 0;
        if(flag == 1) reasons_char[0][0] = elements_char[0];
        else reasons_int[0][0] = elements_int[0];
    }
}
void Relations::isSymmetric(const int& flag){
    int counter = 0, counter2 = 0;
    if(flag == 1){
        for(int i = 0; i < r_num; i++){
            counter2 = 0;
            if(relations_char[i][0] == relations_char[i][1]){
                counter++;
            }
            else{
                for(int j = 0; j < r_num; j++){
                    if(j != i && relations_char[j][0] == relations_char[i][1] && relations_char[j][1] == relations_char[i][0]){//checks symmetric property
                        counter2++;
                        counter++;
                    }
                }
                if(counter2 == 0){//if the current relation is not symmetric, function stores the elements of this relation to the index 1 of reasons vector
                    reasons_char[1][0] = relations_char[i][0];
                    reasons_char[1][1] = relations_char[i][1];
                }
            }
        }
    }
    else{
        for(int i = 0; i < r_num; i++){
            counter2 = 0;
            if(relations_int[i][0] == relations_int[i][1]){
                counter++;
            }
            else{
                for(int j = 0; j < r_num; j++){
                    if(j != i && relations_int[j][0] == relations_int[i][1] && relations_int[j][1] == relations_int[i][0]){//checks symmetric property
                        counter2++;
                        counter++;
                    }
                }
                if(counter2 == 0){//if the current relation is not symmetric, function stores the elements of this relation to the index 1 of reasons vector
                    reasons_int[1][0] = relations_int[i][0];
                    reasons_int[1][1] = relations_int[i][1];
                }
            }
        }
    }
    if(counter == r_num){
        symmetric = 1;
    }
    else symmetric = 0;
}
void Relations::isAnti_Symmetric(const int& flag){
    int counter = 0, counter2 = 0;
    if(flag == 1){
        for(int i = 0; i < r_num; i++){
            counter2 = 0;
            if(relations_char[i][0] == relations_char[i][1]){
                counter++;
            }
            else{
                for(int j = 0; j < r_num; j++){
                    if(j != i && (relations_char[j][0] != relations_char[i][1] || relations_char[j][1] != relations_char[i][0])){//checks anti-symmetric property
                        counter2++;
                    }
                }
                if(counter2 == r_num - 1) counter++;
                else{//if the current relation is not anti-symmetric, function stores the elements of this relation to the index 2 of reasons vector
                    reasons_char[2][0] = relations_char[i][0];
                    reasons_char[2][1] = relations_char[i][1];
                }
            }
        } 
    }
    else{
        for(int i = 0; i < r_num; i++){
            counter2 = 0;
            if(relations_int[i][0] == relations_int[i][1]){
                counter++;
            }
            else{
                for(int j = 0; j < r_num; j++){
                    if(j != i && (relations_int[j][0] != relations_int[i][1] || relations_int[j][1] != relations_int[i][0])){//checks anti-symmetric property
                        counter2++;
                    }
                }
                if(counter2 == r_num - 1) counter++;
                else{//if the current relation is not anti-symmetric, function stores the elements of this relation to the index 2 of reasons vector
                    reasons_int[2][0] = relations_int[i][0];
                    reasons_int[2][1] = relations_int[i][1];
                }
            }
        }
    }
    if(counter == r_num){
        anti_symmetric = 1;
    }
    else anti_symmetric = 0;
}
void Relations::isTransitive(const int& flag){
    int a, b, c, check = 0, counter = 0, counter2 = 0, temp;
    if(flag == 1){
        for(int i = 0; i < r_num; i++){//check transitive property
            if(relations_char[i][0] == relations_char[i][1]){
                counter++;
            }
            else{
                counter2 = 0;
                check = 0;
                a = relations_char[i][0];
                b = relations_char[i][1];
                for(int j = 0; j < r_num; j++){
                    if(relations_char[j][0] == b){
                        temp = check;
                        counter2++;
                        c = relations_char[j][1];
                        for(int k = 0; k < r_num; k++){
                            if(relations_char[k][0] == a && relations_char[k][1] == c){
                                check++;
                            }
                        }
                        if(temp == check){//if the current relation is not transitive, function stores the elements of this relation to the index 3 of reasons vector
                            reasons_char[3][0] = a;
                            reasons_char[3][1] = b;
                            reasons_char[3][2] = c;
                        }
                    }
                }
                if(counter2 == 0){
                    counter++;
                }
                else if(check == counter2){
                    counter++;
                }
            }
        }
    }
    else{
        for(int i = 0; i < r_num; i++){//check transitive property
            if(relations_int[i][0] == relations_int[i][1]){
                counter++;
            }
            else{
                counter2 = 0;
                check = 0;
                a = relations_int[i][0];
                b = relations_int[i][1];
                for(int j = 0; j < r_num; j++){
                    if(relations_int[j][0] == b){
                        temp = check;
                        counter2++;
                        c = relations_int[j][1];
                        for(int k = 0; k < r_num; k++){
                            if(relations_int[k][0] == a && relations_int[k][1] == c){
                                check++;
                            }
                        }
                        if(temp == check){//if the current relation is not transitive, function stores the elements of this relation to the index 3 of reasons vector
                            reasons_int[3][0] = a;
                            reasons_int[3][1] = b;
                            reasons_int[3][2] = c;
                        }
                    }
                }
                if(counter2 == 0){
                    counter++;
                }
                else if(check == counter2){
                    counter++;
                }
            }
        }
    }
    if(counter == r_num) transitive = 1;
    else transitive = 0;
}
void Relations::print_relation(const int& flag, ofstream& fout){//prints the properties of the current relation
    fout << "n" << endl;
    for(int i = 0; i < r_num; i++){
        if(flag == 1) fout << "(" << relations_char[i][0] << "," << relations_char[i][1] << ")";
        else fout << "(" << relations_int[i][0] << "," << relations_int[i][1] << ")";
    }
    if(flag == 31 || flag == 32){
        if(flag == 31){
            if(invalid_input_char[2] == '3'){
                fout << "\nBoth of the elements of the relation (" << invalid_input_char[0] << ", " << invalid_input_char[1] << ") is not found in the set A = ";
            }
            else{
                fout << "\nThe element " << invalid_input_char[static_cast<int>(invalid_input_char[2]) - 49] << " of the relation (" << invalid_input_char[0] << ", " << invalid_input_char[1] << ") is not found in the set A = ";
            }
            print_set(1, fout); 
        }
        else{
            if(invalid_input_int[2] == 3){
                fout << "\nBoth of the elements of the relation (" << invalid_input_int[0] << ", " << invalid_input_int[1] << ") is not found in the set A = ";
            }
            else{
                fout << "\nThe element " << invalid_input_int[invalid_input_int[2] - 1] << " of the relation (" << invalid_input_int[0] << ", " << invalid_input_int[1] << ") is not found in the set A = ";
            }
            print_set(2, fout); 
        }
        fout << endl;
    }
    else{
        if(reflexive == 1){
            fout << "\nReflexive: Yes, all elements are present." << endl;
        }
        else{
            if(flag == 1) fout << "\nReflexive: No, (" << reasons_char[0][0] << "," << reasons_char[0][0] << ") is not found." << endl;
            else fout << "\nReflexive: No, (" << reasons_int[0][0] << "," << reasons_int[0][0] << ") is not found." << endl;
        } 
        if(symmetric == 1){
            fout << "Symmetric: Yes, this relation is symmetric." << endl;
        }
        else{
            if(flag == 1){
                fout << "Symmetric: No, (" << reasons_char[1][0] << "," << reasons_char[1][1] << ") is found whereas (" << reasons_char[1][1] << "," << reasons_char[1][0] << ") is not found." << endl;
            }
            else{
                fout << "Symmetric: No, (" << reasons_int[1][0] << "," << reasons_int[1][1] << ") is found whereas (" << reasons_int[1][1] << "," << reasons_int[1][0] << ") is not found." << endl;
            }
        }
        if(anti_symmetric == 1){
            fout << "Antisymmetric: Yes, this relation is antisymmetric"<< endl;
        }
        else{
            if(flag == 1){
                fout << "Antisymmetric: No, (" << reasons_char[2][0] << "," << reasons_char[2][1] << ") and (" << reasons_char[2][1] << "," << reasons_char[2][0] << ") are found." << endl;
            }
            else{
                fout << "Antisymmetric: No, (" << reasons_int[2][0] << "," << reasons_int[2][1] << ") and (" << reasons_int[2][1] << "," << reasons_int[2][0] << ") are found." << endl;
            }
        }
        if(transitive == 1){
            fout << "Transitive: Yes, this relation is transitive."<< endl;
        }
        else{
            if(flag == 1){
                fout << "Transitive: No, (" << reasons_char[3][0] << "," << reasons_char[3][2] << ") is not found whereas (" << reasons_char[3][0] << "," << reasons_char[3][1] << ") and (" << reasons_char[3][1] << "," << reasons_char[3][2] << ") are found." << endl;
            }
            else{
                fout << "Transitive: No, (" << reasons_int[3][0] << "," << reasons_int[3][2] << ") is not found whereas (" << reasons_int[3][0] << "," << reasons_int[3][1] << ") and (" << reasons_int[3][1] << "," << reasons_int[3][2] << ") are found." << endl;
            }
        }
    }
}
bool Relations::check_validity(const int& flag){//checks if there is an invalid input
    int counter1 = 0, counter2 = 0;
    if(flag == 1){
        for(int i = 0; i < relations_char.size(); i++){
            counter1 = 0;
            counter2 = 0;
            for(int j = 0; j < elements_char.size(); j++){
                if(elements_char[j] == relations_char[i][0]){
                    counter1++;
                }
                if(elements_char[j] == relations_char[i][1]){
                    counter2++;
                }
            }
            if(counter1 == 0 || counter2 == 0){
                invalid_input_char[0] =  relations_char[i][0];
                invalid_input_char[1] =  relations_char[i][1];
                if(counter1 == 0 && counter2 == 0){
                    invalid_input_char[2] = '3';
                }
                else if(counter2 == 0){
                    invalid_input_char[2] = '2';
                }
                else{
                    invalid_input_char[2] = '1';
                }
                return false;
            }
        }
    }
    else{
        for(int i = 0; i < relations_int.size(); i++){
            counter1 = 0;
            counter2 = 0;
            for(int j = 0; j < elements_int.size(); j++){
                if(elements_int[j] == relations_int[i][0]){
                    counter1++;
                }
                if(elements_int[j] == relations_int[i][1]){
                    counter2++;
                }
            }
            if(counter1 == 0 || counter2 == 0){
                invalid_input_int[0] =  relations_int[i][0];
                invalid_input_int[1] =  relations_int[i][1];
                if(counter1 == 0 && counter2 == 0){
                    invalid_input_int[2] = 3;
                }
                else if(counter2 == 0){
                    invalid_input_int[2] = 2;
                }
                else{
                    invalid_input_int[2] = 1;
                }
                return false;
            }
        }
    }
    return true;
}
void Relations::print_set(const int& flag, ofstream& fout){//prints the set
    if(flag == 1){
        for(int i = 0; i < elements_char.size(); i++){
            if(i == 0){
                fout << "{" << elements_char[i] << ",";
            }
            else if(i == elements_char.size() - 1){
                fout << elements_char[i] << "}";
            }
            else{
                fout << elements_char[i] << ",";
            }
        }
    }
    else{
        for(int i = 0; i < elements_int.size(); i++){
            if(i == 0){
                fout << "{" << elements_int[i] << ",";
            }
            else if(i == elements_int.size() - 1){
                fout << elements_int[i] << "}";
            }
            else{
                fout << elements_int[i] << ",";
            }
        }
    }
}