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

/****************************************************************************
 * ERROR MESSAGE
****************************************************************************/

/*
    !Print if the command needs argument(s) but not exists
*/
void 
missingArgumentError()
{
    cout << "Error: Missing argument!!" << endl;
}

/*
    !Print if the argument is invaild for that command

    @param argument
*/
void 
illegalArgumentError(const string& argument)
{
    cout << "Error: Illegal argument \"" << argument << "\"!!" << endl;
}

/*
    !Print if the number of arguments is over than the command specified.

    @param argument
*/
void 
ExtraArgumentError(const string& argument)
{
    cout << "Error: Extra argument \"" << argument << "\"!!" << endl;
}

/****************************************************************************
 * UTILS FUNCTION
****************************************************************************/

/*
    @return bool true if the string is representing a number.
*/
bool 
is_number(string& str)
{
    return !str.empty() && find_if(str.begin(), str.end(), [](char c) { return !isdigit(c); }) == str.end();
}

void 
ltrim(string &s) 
{
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
        return !isspace(ch);
    }));
}

void 
rtrim(string &s) 
{
    s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
        return !isspace(ch);
    }).base(), s.end());
}

void 
trim(string &s) 
{
    ltrim(s); rtrim(s);
}

/*
    !Separate the input string as multiple part: <Command> <Argument>

    @param str The string user inputed

    @param container

    @param delim 
*/
void 
parseCmd(string& str, vector<string>& container, const string& delim = " ")
{
    unsigned long lcursor = 0;
    unsigned long rcursor = 0;
    
    do {
        rcursor = str.find(delim, lcursor);
        // cout << str.substr(lcursor, rcursor - lcursor) << endl;
        container.push_back(str.substr(lcursor, rcursor - lcursor));
    
        lcursor = str.find_first_not_of(delim, rcursor);
    } while (lcursor != string::npos);
}

/*
    !Separate the input string as two part: <Command> <Argument>

    @param str The string user inputed

    @param argument The variable to save the argument in string format

    @param delim 

    @return cmd
*/
string 
parseCmd(string& str, string& argument, const string& delim = " ")
{
    string cmd = str.substr(0, str.find(delim));
    argument = str.substr(cmd.size(), string::npos);

    if (argument.find_first_not_of(delim) == string::npos){
        argument.empty();
    } else {
        argument = argument.substr(argument.find_first_not_of(delim), string::npos);
    }

    return cmd;
}

int 
main()
{
    Json json;
    vector<string> args;
    string jsonFile;
    string buffer;
    string command;
    string key;

    // Do NOT change this part of code.
    cout << "Please enter the file name: ";

    // Read filename
    getline(cin, jsonFile); trim(jsonFile);

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

        // TODO: Optimize this (ignore input ' ')
        // Get the command and arguments
        while (command.empty()) {
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
            parseCmd(buffer, args);
            
            // The size of arguments should equal to 2
            if (args.size() < 2) 
                { missingArgumentError(); continue; } 
            else if (args.size() > 2)
                { ExtraArgumentError(args[2]); continue; }

            // If value is illegal (not a number)
            if (!is_number(args[1])) 
                { illegalArgumentError(args[1]); continue; }

            // Add element if key doesn't repeat.
            if (json.isExist(args[0]))
                { json.KeyExistError(args[0]); } 
            else 
                { json.add(args[0], stoi(args[1])); }
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
