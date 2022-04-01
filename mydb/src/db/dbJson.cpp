/****************************************************************************
  FileName     [ dbJson.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Json member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <cmath>
#include <string>
#include <algorithm>
#include "dbJson.h"
#include "util.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream&
operator << (ostream& os, const DBJsonElem& j)
{
    os << "\"" << j._key << "\" : " << j._value;

    return os;
}

istream& 
operator >> (istream& is, DBJson& j)
{
    // TODO: to read in data from Json file and store them in a DB 
    // - You can assume the input file is with correct JSON file format
    // - NO NEED to handle error file format
    assert(j._obj.empty());

    string buffer;
    string key;
    int value;
    
    while (getline(is, buffer, '\n')) {
        buffer.erase(remove_if(buffer.begin(), buffer.end(), ::isspace), buffer.end());

        if (buffer == "{") {
            continue;
        } else if (buffer == "") {
            continue;
        } else if (buffer == "}") {
            break;
        }

        if (buffer[-1] == ',') {
            buffer.resize(buffer.size() - 1);
        }

        key = buffer.substr(1, buffer.find(':') - 2);
        myStr2Int(buffer.substr(buffer.find(':') + 1), value);

        j.add(DBJsonElem(key, value));
    }

    j._exist = true;

    return is;
}

ostream& 
operator << (ostream& os, const DBJson& j)
{
    // TODO
    cout << '{' << endl;

    for (size_t i = 0; i < j.size(); ++i) {
        cout << "  " << j[i];
        
        if (i == j.size() - 1)
            cout << endl;
        else
            cout << ',' << endl;
    }

    cout << '}' << endl;
    cout << "Total JSON elements: " << j.size();

    return os;
}

/**********************************************/
/*   Member Functions for class DBJsonElem    */
/**********************************************/

/*****************************************/
/*   Member Functions for class DBJson   */
/*****************************************/

/*
    Set the DBJson to empty, and set isCreated as false
*/
void
DBJson::reset()
{
    // TODO

    _obj.erase(_obj.begin(), _obj.end());
    _exist = false;
}

/*
    @return false if not read any JSON file.
*/
bool
DBJson::operator !() 
{
    return !_exist;
}

/*
    @return NULL if not read any JSON file, else return this
*/
DBJson::operator void* () const 
{
    if (!_exist) 
        return NULL;

    return (void*)this;
}

/*
    Append 1 (key, value) pair

    @param elm A DBJson Element

    @return bool False if key is repeated, else true
*/
bool
DBJson::add(const DBJsonElem& elm)
{
    // TODO

    // If the key is repeated
    for (vector<const DBJsonElem>::iterator it = _obj.begin(); it < _obj.end(); ++it){
        if (it->key() == elm.key()) {
            return false;
        }
    }

    _obj.push_back(elm);
    return true;
}

// return NAN if DBJson is empty
float
DBJson::ave() const
{
    // TODO

    float average = NAN;

    if (_obj.size()) { average = (float)sum() / _obj.size(); }

    return average;
}

// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::max(size_t& idx) const
{
    // TODO
    int maxN = INT_MIN; 
    idx = _obj.size();

    for (size_t i = 0; i < _obj.size(); i++) {
        if (_obj[i].value() > maxN ) {
            maxN = _obj[i].value();
            idx  = i;
        }
    }

    return maxN;
}

// If DBJson is empty, set idx to size() and return INT_MAX
int
DBJson::min(size_t& idx) const
{
    // TODO
    int minN = INT_MAX;
    idx = _obj.size();

    for (size_t i = 0; i < _obj.size(); ++i){
        if (_obj[i].value() < minN){
            minN = _obj[i].value();
            idx  = i;
        }
    }

    return minN;
}

// return 0 if empty
int
DBJson::sum() const
{
    // TODO
    int s = 0;

    for (vector<const DBJsonElem>::iterator it = _obj.begin(); it != _obj.end(); ++it){
        s += it->value();
    }

    return s;
}

void
DBJson::sort(const DBSortKey& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

void
DBJson::sort(const DBSortValue& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}
