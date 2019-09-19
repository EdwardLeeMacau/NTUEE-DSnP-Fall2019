/****************************************************************************
  FileName     [ p2Json.h]
  PackageName  [ p2 ]
  Synopsis     [ Header file for class Json JsonElem ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#ifndef P2_JSON_H
#define P2_JSON_H

#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

class JsonElem
{
public:
    // TODO: define constructor & member functions on your own
    JsonElem() {}
    JsonElem(const string& k, int v): _key(k), _value(v) {}

    friend ostream& operator << (ostream&, const JsonElem&);
    friend class Json;

private:
    string  _key;   // DO NOT change this definition. Use it to store key.
    int     _value; // DO NOT change this definition. Use it to store value.
};

class Json
{
public:
    // TODO: define constructor & member functions on your own
    bool read(const string&);
    bool add(string, int);
    bool print();
    bool isEmpty();
    bool isExist(string&);
    int size();
    int sum();
    JsonElem maxElement();
    JsonElem minElement();
    double average();
    void EmptyError();
    void KeyExistError(string&);

private:
    // DO NOT change this definition, use it to store JSON elements
    vector<JsonElem>       _obj;

    // Unordered set to check key existance.
    unordered_set<string>  _table;
};

#endif // P2_TABLE_H
