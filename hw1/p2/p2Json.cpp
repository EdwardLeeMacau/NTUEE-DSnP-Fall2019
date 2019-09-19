/****************************************************************************
  FileName     [ p2Json.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define member functions of class Json and JsonElem ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "p2Json.h"

using namespace std;

/*
    !Print JSON Element Empty Error
*/
void Json::EmptyError(){
    cout << "Error: No element found!" << endl;
}

/*
    !Print Key Exist Error
*/
void Json::KeyExistError(string& key){
    cout << "Error: Element with key \"" << key << "\" already exists!!";
}

/*
    !Implement member functions of class Row and Table here

    @param jsonFile the filename of the jsonFile

    @return bool Whether the file is successfully read.
*/
bool Json::read(const string& jsonFile){
    ifstream myfile (jsonFile);
    string line;
    string key;
    int value;
    int colon = 0;

    // Parse the JSON line by line
    while (getline(myfile, line, '\n')){
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

        // Case Handling
        if (line == "{") {
            continue;
        } else if (line == ""){
            continue;
        } else if (line == "}"){
            break;
        }
        
        // Remove the last element
        if (line[-1] == ','){
            line = line.substr(0, line.size() - 1);
        }

        // Parse the key and value in the JSON
        // Key has vary length and Value is the integer
        colon = line.find(':');
        key = line.substr(1, colon - 2);
        value = stoi(line.substr(colon + 1));

        // Add (Key, Value) pairs to object
        add(key, value);
    }

    return true;
}

/*
    !Print the elements in the JSON

    @return true
*/
bool Json::print(){
    cout << "{" << endl;

    for (int i = 0; i < size(); i++){
        // Indent 2 spaces
        cout << "  " << _obj[i];
        
        // print "," between rows.
        if (i == size() - 1) 
            cout << endl;
        else 
            cout << "," << endl;
    }
    
    cout << "}" << endl;
    
    return true;
}

/*
    !Check if the JSON is empty

    @return Whether the json is empty
*/
bool Json::isEmpty() {
    if (_obj.size() == 0) return true;

    return false;
}

/*
    !Check if the key in JSON

    @return Whether the key is exists in JSON
*/
bool Json::isExist(string& key){
    if (_table.find(key) != _table.end()){
        return true;
    }

    return false;
}

/*
    @return The number of elements in container
*/
int Json::size() {
    return _obj.size();
}

/*
    !Execute the summation function.

    @return total The total value of element's value in container
*/
int Json::sum() {
    int total = 0;

    for (int i = 0; i < size(); i++){
        total += _obj[i]._value;
    }

    return total;
}

/*
    !Get the element with the maximum value in Object

    @return JsonElem
*/
JsonElem Json::maxElement() {
    int maximum = 0;
    int index = 0;

    for (int i = 0; i < size(); i++){
        if (_obj[i]._value > maximum) {
            maximum = _obj[i]._value;
            index = i;
        }
    }

    return _obj[index];
}

/*
    !Get the element with the minimum value in Object

    @return JsonElem
*/
JsonElem Json::minElement() {
    int minimum = 0;
    int index = 0;

    for (int i=0; i < size(); i++){
        if (_obj[i]._value < minimum) {
            minimum = _obj[i]._value;
            index = i;
        }
    }
    return _obj[index];
}

/*
    !Add an element to the _obj

    @param key Unique key in JSON File 

    @return bool Whether the element is successfully added to container.
*/
bool Json::add(string key, int value){
    _table.insert(key);
    _obj.push_back(JsonElem(key, value));
    
    return true;
}

/*
    !Compute the average of the value in json

    @return average The average of the fucntion.
*/
double Json::average(){
    return 1.0 * sum() / size();    
}

ostream& operator << (ostream& os, const JsonElem& j){
    return (os << "\"" << j._key << "\" : " << j._value);
}

