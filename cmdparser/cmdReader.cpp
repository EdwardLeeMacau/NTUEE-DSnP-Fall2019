/****************************************************************************
  FileName     [ cmdReader.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command line reader member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <algorithm>
#include <cassert>
#include <cstring>
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    Extrenal funcitons
//----------------------------------------------------------------------
void mybeep();
char mygetc(istream&);
ParseChar getChar(istream&);


//----------------------------------------------------------------------
//    Member Function for class Parser
//----------------------------------------------------------------------
void
CmdParser::readCmd()
{
   if (_dofile.is_open()) {
      readCmdInt(_dofile);
      _dofile.close();
   }
   else
      readCmdInt(cin);
}

void
CmdParser::readCmdInt(istream& istr)
{
    resetBufAndPrintPrompt();

    while (1) {
        ParseChar pch = getChar(istr);
        if (pch == INPUT_END_KEY) break;
        switch (pch) {
            case LINE_BEGIN_KEY :
            case HOME_KEY       : moveBufPtr(_readBuf); break;
            case LINE_END_KEY   :
            case END_KEY        : moveBufPtr(_readBufEnd); break;
            case BACK_SPACE_KEY : /* TODO */ 
                                  if (moveBufPtr(_readBufPtr - 1)) {deleteChar();} break;
            case DELETE_KEY     : deleteChar(); break;
            case NEWLINE_KEY    : addHistory();
                                  cout << char(NEWLINE_KEY);
                                  resetBufAndPrintPrompt(); break;
            case ARROW_UP_KEY   : moveToHistory(_historyIdx - 1); break;
            case ARROW_DOWN_KEY : moveToHistory(_historyIdx + 1); break;
            case ARROW_RIGHT_KEY: /* TODO */ moveBufPtr(_readBufPtr + 1); break;
            case ARROW_LEFT_KEY : /* TODO */ moveBufPtr(_readBufPtr - 1); break;
            case PG_UP_KEY      : moveToHistory(_historyIdx - PG_OFFSET); break;
            case PG_DOWN_KEY    : moveToHistory(_historyIdx + PG_OFFSET); break;
            case TAB_KEY        : /* TODO */ insertChar(' ', ( TAB_POSITION - (_readBufPtr - _readBuf)  % TAB_POSITION ) ); break;
            case INSERT_KEY     : // not yet supported; fall through to UNDEFINE
            case UNDEFINED_KEY:   mybeep(); break;
            default:  // printable character
                insertChar(char(pch)); break;
        }
        #ifdef TA_KB_SETTING
        taTestOnly();
        #endif
    }
}


/*
    This function moves _readBufPtr to the "ptr" pointer
    It is used by left/right arrowkeys, home/end, etc.

    Suggested steps:
    1. Make sure ptr is within [_readBuf, _readBufEnd].
       If not, make a beep sound and return false. (DON'T MOVE)

    2. Move the cursor to the left or right, depending on ptr

    3. Update _readBufPtr accordingly. The content of the _readBuf[] will
        not be changed

    [Note] 
    This function can also be called by other member functions below
    to move the _readBufPtr to proper position.

    @params ptr

    @return bool True if pointer is legal to move.
*/
bool
CmdParser::moveBufPtr(char* const ptr)
{
    // TODO...

    // Check **ptr** is within [_readBuf, _readBufEnd]
    if (ptr < _readBuf || ptr > _readBufEnd) {
        mybeep();
        return false;
    }

    // Case if **ptr** < **_readBufPtr**
    while (_readBufPtr > ptr) {
        cout << '\b';
        --_readBufPtr;
    }

    // Case if **ptr** > **_readBufPtr**
    while (ptr > _readBufPtr) {
        cout << *_readBufPtr;
        ++_readBufPtr;
    }

    return true;
}

/*
    [Notes]
    1. Delete the char at _readBufPtr
    2. mybeep() and return false if at _readBufEnd
    3. Move the remaining string left for one character
    4. The cursor should stay at the same position
    5. Remember to update _readBufEnd accordingly.
    6. Don't leave the tailing character.
    7. Call "moveBufPtr(...)" if needed.

    For example, (^ is the cursor position)
    cmd> This is the command
                 ^

    After calling deleteChar()
    cmd> This is he command
                 ^

    [Notes]
    1. Need to handle if _readBufPtr point to _readBufEnd?
*/
bool
CmdParser::deleteChar()
{
    // TODO...

    /* 
        If _readBufPtr at the end of _readBuf, call mybeep();
    */
    if (_readBufPtr == _readBufEnd) {
        mybeep();
        return false;
    }

    /*
        Else, delete 1 char
        1. Looping to move the remain characters in array forward, 
           and cout them
        2. cout << ' ' to replace the last char
        3. Move the cursor to **_readBufPtr**
        4. --readBufEnd
    */
    for (char* ptr = _readBufPtr; ptr < _readBufEnd; ++ptr) {
        *ptr = *(ptr + 1);
        cout << *ptr;
    }

    // Print a ' ' to replace the char
    cout << ' '; 
    
    // Move the cursor
    for (char* ptr = _readBufPtr; ptr < _readBufEnd; ++ptr)
        cout << '\b';

    --_readBufEnd;

    return true;
}

/*
    Insert characters in _readBuf.

    [Notes]
    1. Insert character 'ch' for "repeat" times at _readBufPtr
    2. Move the remaining string right for "repeat" characters
    3. The cursor should move right for "repeats" positions afterwards
    4. Default value for "repeat" is 1. You should assert that (repeat >= 1).

    For example, (^ is the cursor position)
    cmd> This is the command
                 ^                

    After calling insertChar('k', 3) ---
    cmd> This is kkkthe command
                    ^
    
    @params ch

    @params repeat 
*/
void
CmdParser::insertChar(char ch, int repeat)
{
    // TODO...
    assert(repeat >= 1);

    // Extend the size of _readBuf
    _readBufEnd += repeat;

    // Move the chars in array
    // Append new char in array, and move _readBufPtr
    for (char *ptr = _readBufEnd - 1; ptr >= _readBufPtr + repeat; --ptr) {
        *ptr = *(ptr - repeat);
    }

    // Append new char in array and move _readBufPtr
    for (char *ptr = _readBufPtr; ptr < _readBufPtr + repeat; ++ptr) {
        *ptr = ch;
        cout << ch;
    }

    _readBufPtr += repeat;

    // Print The remain char if **_readBufPtr** != _readBufEnd at start
    // Send '\b' to move cursor
    for (char *ptr = _readBufPtr; ptr < _readBufEnd; ++ptr) cout << *(ptr);
    for (char *ptr = _readBufPtr; ptr < _readBufEnd; ++ptr) cout << '\b';
}

/*
    [Notes]
    1. Delete the line that is currently shown on the screen
    2. Reset _readBufPtr and _readBufEnd to _readBuf
    3. Make sure *_readBufEnd = 0

    [Example] (^ is the cursor position)
    cmd> This is the command
                 ^                

    After calling deleteLine()---
    cmd>
        ^
*/
void
CmdParser::deleteLine()
{
    // TODO...

    /*
        Empty _readBuf, set pointers (_readBufPtr, _readBufEnd) point to the begin of _readBuf
    */
    moveBufPtr(_readBuf);
    
    // Cover all letters show in screen
    while (_readBufPtr < _readBufEnd){
        cout << ' ';
        *_readBufPtr = 0;
        ++_readBufPtr;
    }

    // Move the cursor to the leading of _readBuf
    moveBufPtr(_readBuf);
    _readBufEnd = _readBuf;
}

/*
    This functions moves _historyIdx to index and display _history[index]
    on the screen.

    [Note] 
    Need to consider:
    - If moving up... (i.e. index < _historyIdx)
        1. If already at top (i.e. _historyIdx == 0), beep and do nothing.
        2. If at bottom, temporarily record _readBuf to history.
        (Do not remove spaces, and set _tempCmdStored to "true")
        3. If index < 0, let index = 0.

    - If moving down... (i.e. index > _historyIdx)
        1. If already at bottom, beep and do nothing
        2. If index >= _history.size(), let index = _history.size() - 1.

    - Assign _historyIdx to index at the end.

    @params index
*/
void
CmdParser::moveToHistory(int index)
{
    // TODO...
    assert(index != _historyIdx);

    /* 
        To move up:
        1. Beep() and return if _historyIdex is 0
        2. If index < 0, clip it to 0.
    */
    if (index < _historyIdx) {
        if (_historyIdx == 0) {
            mybeep();
            return;
        }
        
        if (index <= 0)
            index = 0;
    }

    /* 
        To move down:
        1. Beep() and return if _historyIdx is at the end
        2. If index > size() - 1, clip it to size() - 1
    */
    if (index > _historyIdx) {
        if (_historyIdx == _history.size()) {
            mybeep();
            return;
        }
        
        if (index >= _history.size() - 1)
            index = _history.size() - 1;
    }

    // Save as temp if it is a new command
    if (_historyIdx == _history.size() && !_tempCmdStored) {
        _history.push_back(string(_readBuf));
        _tempCmdStored = true;
    }

    // Show History, set _historyIdx to index
    _historyIdx = index;
    retrieveHistory();

    // Delete last history if it is temp
    if (index == _history.size() - 1 && _tempCmdStored) {
        _history.pop_back();
        _tempCmdStored = false;
    }
}

/*
    This function adds the string in _readBuf to the _history.
    The size of _history may or may not change. Depending on whether 
    there is a temp history string.

    [Notes]
    1. Remove ' ' at the beginning and end of _readBuf
    2. If not a null string, add string to _history.
       Be sure you are adding to the right entry of _history.
    3. If it is a null string, don't add anything to _history.
    4. Make sure to clean up "temp recorded string" (added earlier by up/pgUp,
       and reset _tempCmdStored to false
    5. Reset _historyIdx to _history.size() // for future insertion
*/
void
CmdParser::addHistory()
{
    // TODO...
    string cmd = _readBuf;
    size_t lcursor = cmd.find_first_not_of(' ');
    size_t rcursor = cmd.find_last_not_of(' ');

    // If **_readBuf** is empty, cmd is an empty string ""

    // Clean up temp recorded string
    if (_tempCmdStored) {
        _history.pop_back();
        _tempCmdStored = false;
    }

    // Else if **_readBuf** is not empty, strip
    if (lcursor != string::npos) {
        if (rcursor == string::npos) cmd = cmd.substr(lcursor);
        else cmd = cmd.substr(lcursor, rcursor - lcursor + 1);

        _history.push_back(cmd);
    }

    // Move _historyIdx to the end of _history
    _historyIdx = _history.size();

    // Clean up _readBuf
    for (char *ptr = _readBuf; ptr < _readBufEnd; ++ptr){
        *ptr = 0;
    }
}

/*
    1. Replace current line with _history[_historyIdx] on the screen
    2. Set _readBufPtr and _readBufEnd to end of line

    [Note] 
    1. Do not change _history.size().
    2. char* strcpy(char* dest, const char* src);
       string.h in C++ standard library, to copy the str to dest from src.
*/
void
CmdParser::retrieveHistory()
{
   deleteLine();
   strcpy(_readBuf, _history[_historyIdx].c_str());
   cout << _readBuf;
   _readBufPtr = _readBufEnd = _readBuf + _history[_historyIdx].size();
}
