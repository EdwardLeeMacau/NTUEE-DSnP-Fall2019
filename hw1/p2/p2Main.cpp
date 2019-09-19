/****************************************************************************
  FileName     [ p2Main.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define main() function ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2016-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include "p2Json.h"

using namespace std;

/*
    !Called when the command needs some argument(s) but not exists
*/
void missingArgumentError(){
    cout << "Error: Missing argument!!" << endl;
}

/*
    !Called when the argument is invaild

    @param argument
*/
void illegalArgumentError(const string& argument){
    cout << "Error: Illegal argument \"" << argument << "\"!!" << endl;
}

/*
    !Called when the number of arguments is over than the command specified.

    @param argument
*/
void ExtraArgumentError(const string& argument){
    cout << "Error: Extra argument \"" << argument << "\"!!" << endl;
}

/*
    @param str 

    @return true if the string is representing a number.
*/
bool is_number(string& str){
    return !str.empty() && find_if(str.begin(), str.end(), [](char c) { return !isdigit(c); }) == str.end();
}

/*
    !Separate the input string as multiple part: <Command> <Argument>

    @str The string user inputed

    @container

    @delim 
*/
template <class Container>
void parseCmd(string& str, Container& container, const string& delim = " "){
    int lcursor = 0;
    int rcursor = 0;
    
    do {
        rcursor = str.find(delim, lcursor);
        // cout << str.substr(lcursor, rcursor - lcursor) << endl;
        container.push_back(str.substr(lcursor, rcursor - lcursor));
    
        lcursor = str.find_first_not_of(delim, rcursor);
    } while (lcursor != string::npos);
}

/*
    !Separate the input string as two part: <Command> <Argument>

    @str The string user inputed

    @argument The variable to save the argument in string format

    @delim 

    @return cmd
*/
string parseCmd(string& str, string& argument, const string& delim = " "){
    string cmd = str.substr(0, str.find(delim));
    argument = str.substr(cmd.size(), string::npos);

    if (argument.find_first_not_of(delim) == string::npos){
        argument.empty();
    } else {
        argument = argument.substr(argument.find_first_not_of(delim), string::npos);
    }

    return cmd;
}

int main()
{
    Json json;
    vector<string> args;
    string buffer;
    string command;
    string key;

    // Read in the csv file. Do NOT change this part of code.
    string jsonFile;
    cout << "Please enter the file name: ";
    cin >> jsonFile;
    
    if (json.read(jsonFile))
        cout << "File \"" << jsonFile << "\" was read in successfully." << endl;
    else {
        cerr << "Failed to read in file \"" << jsonFile << "\"!" << endl;
        exit(-1); // jsonFile does not exist.
    }

    /*
        7 commands are available:
        - ADD <key> <value>
        - AVE
        - EXIT
        - MAX
        - MIN
        - PPRINT
        - SUM
    */

    while (true) {
        // Reset command and arguments as empty
        command = "";
        args.clear();

        cout << "Enter command: ";

        // Get the command and arguments      
        while (command.empty()){
            getline(cin, command);
        }
        command = parseCmd(command, buffer);

        // Main Flow
        if (command == "SUM") {
            if (!buffer.empty()) {
                ExtraArgumentError(buffer);
            } else if (json.isEmpty()){
                json.EmptyError();
            } else {
                cout << "The summation of the values is: " << json.sum() << "." << endl;
            }
        } 
        
        else if (command == "AVE") {
            if (!buffer.empty()) {
                ExtraArgumentError(buffer);
            } else if (json.isEmpty()){
                json.EmptyError();
            } else {
                // Set precision as 1 digits
                cout.precision(1);
                cout << fixed;

                cout << "The average of the values is: " << json.average() << "." << endl;
            }
        } 
        
        else if (command == "MAX") {
            if (!buffer.empty()){
                ExtraArgumentError(buffer);
            } else if (json.isEmpty()) {
                json.EmptyError();
            } else {
                cout << "The maximum element is: { " << json.maxElement() << " }." << endl;
            }
        } 
        
        else if (command == "MIN") {
            if (!buffer.empty()) {
                ExtraArgumentError(buffer);
            } else if (json.isEmpty()) {
                json.EmptyError();
            } else {
                cout << "The minimum element is: { " << json.minElement() << " }." << endl;
            }
        } 
        
        else if (command == "PRINT") {
            if (!buffer.empty()){
                ExtraArgumentError(buffer);
            } else {
                json.print();
            }
        } 
        
        else if (command == "ADD") {
            // If argument (key or value) is empty
            parseCmd<vector<string>>(buffer, args);
            
            // The size of arguments should equal to 2
            if (args.size() < 2) {
                missingArgumentError();
                continue;
            } else if (args.size() > 2){
                ExtraArgumentError(args[2]);
                continue;
            }

            // If value is illegal (not a number)
            if (!is_number(args[1])) {
                illegalArgumentError(args[1]);
                continue;
            }

            // Add element if key doesn't repeat.
            if (json.isExist(args[0])){
                json.KeyExistError(args[0]);
            } else {
                json.add(args[0], stoi(args[1]));
            }
        } 
        
        else if (command == "EXIT") {
            break;
        }

        else {
            cout << "Error: unknown command \"" << command << "\"" << endl;
        }
    }

    // Exit the program when jump out the loop.
    exit(-1);
}
