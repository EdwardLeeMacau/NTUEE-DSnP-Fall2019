/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine constant (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/

CirMgr::CirMgr() : _M(0), _I(0), _L(0), _O(0), _A(0)
{

}

// Release the memory allocated by CirGates.
CirMgr::~CirMgr()
{
   for (auto it : _gates)  delete it;
}

/*
   Return the pointer of specified Gate.

   @return CirGate*
      0 if not found
*/
CirGate*
CirMgr::getGate(unsigned int gid) const
{
   return (gid > _M + _O)? NULL : _gates[gid];
}

bool
CirMgr::readCircuit(const string& fileName)
{
   // Connecting Circuit DFS Variables
   stack<CirGate*> frontier;

   // Parsing Circuit Variables
   unsigned int i1, i2, i3;
   streampos pos_o;
   fstream file(fileName);
   string tmp, tmp2;
   char type;
   bool isFloating;

   // Open File
   if (!file.good())
   {
      cerr << "Cannot open design \"" << fileName << "\"!!" << endl;
      return false;
   }

   // Set LineNo
   lineNo = 1;

   // Get Header
   getline(file, tmp);
   readHeader(tmp);  
   ++lineNo;

   // Set Attribute (1 (CONST 0) + M (PI / AIG) + O (PO)) 
   _gates.resize(_M + _O + 1); fill(_gates.begin(), _gates.end(), (CirGate*)NULL);

   // Set Constant False
   _gates[0] = new CirConstGate();

   // Number ID of PIns in range [1, _M], Record LineNo also.
   for (int i = 0; i < _I; ++i)
   {
      file >> i1;
      loadInput(i1 / 2);
      ++lineNo;
   }

   // Record Marker
   pos_o = file.tellg();

   // Number ID of POs in range [_M + 1, _M + _O]
   for (int o = 0; o < _O; ++o)
   {
      file >> i1;
      _pout.push_back(_M + o + 1);
      _gates[_M + o + 1] = new CirPOGate(_M + o + 1, lineNo);
      ++lineNo;      
   }
   
   // Record Marker
   // paig_tellg = file.tellg();

   // Number ID of AIGs
   for (int a = 0; a < _A; ++a)
   {
      file >> i1 >> i2 >> i3;
      _aig.push_back(i1 / 2);
      _gates[i1 / 2] = new CirAIGate(i1 / 2, lineNo);
      ++lineNo;
   }

   /*
    * Load Symbol
    * 
    * If getline() and sscanf() successfully
   */
   while (true)
   {
      if (file.peek() == '\n') file.get();
      if (file.peek() == 'c') break;
      if (file.peek() == EOF) break;

      getline(file, tmp);
      
      if (sscanf(tmp.c_str(), "%c%u %s", &type, &i1, buf) == 3)
      {
         tmp2 = string(buf);
         if (type == 'i')  loadSymbol(i1 + 1, tmp2);
         if (type == 'o')  loadSymbol(_M + i1 + 1, tmp2);
      }
      else
      {
         break;
      }
      
   }

   /*
    * Load Comment
   */
   // loadComment(tmp);

   // Reset LineNo to 0
   lineNo = 0;

   /*
    * Connecting AIG.
    *  
    * Before connecting the circuit... the connecting information 
    * is keeped by only CirAIGate._fanout.
    * 
    * After connecting the circuit... each CirGate can see the 
    * ascenders (CirGate._fanin) and descenders (CirGate._fanout).
    * 
    * - CirPOGate
    * - CirAIGate
    * 
    * Find out the Undefined Gate
    * 
    * Find out the Floating Gate
    */

   // Load the fanin information.
   file.clear();  file.seekg(pos_o, ios::beg); 

   // Connect Output
   for (int o = 0; o < _O; ++o)
   {
      file >> i1;
      _gates[_M + o + 1]->addFanin(getGate(i1 / 2), i1 % 2);
      _gates[i1 / 2]->addFanout(getGate(_M + o + 1), i1 % 2);
   }

   // Connect And-Inverter Gate
   for (int a = 0; a < _A; ++a)
   {
      file >> i1 >> i2 >> i3;

      // if i2 is Undefined
      if (!_gates[i2 / 2])
      {
         // _floating.push_back(i1 / 2);
         _gates[i2 / 2] = new CirUndefGate(i2 / 2);
      }   
      
      // if i3 is Undefined
      if (!_gates[i3 / 2])
      {
         // _floating.push_back(i1 / 2);
         _gates[i3 / 2] = new CirUndefGate(i3 / 2);
      }
         
      // i2 and i3 are the fanins of i1
      _gates[i1 / 2]->addFanin(getGate(i2 / 2), i2 % 2);  
      _gates[i1 / 2]->addFanin(getGate(i3 / 2), i3 % 2);
      
      // i1 is the fanin of i2 and i3
      _gates[i2 / 2]->addFanout(getGate(i1 / 2), i2 % 2); 
      _gates[i3 / 2]->addFanout(getGate(i1 / 2), i3 % 2);
   }

   // Find out the defined but not in used gates
   for (auto it : _aig)
   {
      if (!getGate(it)->_fanout.size())
         _notused.push_back(it);
   }

   for (auto it : _pin)
   {
      if (!getGate(it)->_fanout.size())
         _notused.push_back(it);
   }

   // Find out the undefined gates (floating)
   for (auto it : _aig)
   {
      for (auto it2 : getGate(it)->_fanin)
      {
         if (CirGate::gate(it2)->isFloating())
            _floating.push_back(it);
      }
   }

   for (auto it : _pout)
   {
      for (auto it2 : getGate(it)->_fanin)
      {
         if (CirGate::gate(it2)->isFloating())
            _floating.push_back(it);
      }
   }

   // Sorting the vector
   sort(_floating.begin(), _floating.end());
   sort(_notused.begin(), _notused.end());

   return true;
}

/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/

/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/

void
CirMgr::printSummary() const
{
   cout << endl
        << "Circuit Statistics" << endl
        << "==================" << endl
        << "  PI   " << right << setw(9) << _pin.size()  << endl
        << "  PO   " << right << setw(9) << _pout.size() << endl
        << "  AIG  " << right << setw(9) << _aig.size()  << endl
        << "------------------" << endl
        << "  Total" << right << setw(9) << _pin.size() + _pout.size() + _aig.size() << endl;
}

/*
   Forward (from PIn to POut) printing
*/
void
CirMgr::printNetlist() const
{
   CirGate* tmp;
   stack<CirGate*> frontier;
   stack<CirGate*> explored_set;
   vector<CirGate*> printed_set;

   // Raise up global marker
   CirGate::raiseGlobalMarker();
   
   // Reset Parameters
   lineNo = 0;

   // Pre-Spacing
   cout << endl;

   /*
    * Algorithm description:
    * 
    * Search for all POut, stop when only all PIns were found.
    * (To be optimized)
   */
   for (auto it : _pout)
   {
      frontier.push(getGate(it));
   
      while (frontier.size())
      {
         // Take the element out
         tmp = frontier.top();  frontier.pop(); 

         // Save to the explored stack
         explored_set.push(tmp); 

         // If have already seen the element, ignore it
         // if (tmp->isMarked()) continue; 

         // Mark the Gate, and push the element in frontier
         tmp->mark();

         // Only push defined gate to the frontier.
         for (auto it2 : tmp->_fanin)
         {
            if (!CirGate::gate(it2)->isFloating()) frontier.push(CirGate::gate(it2));
         }
      }

      while (explored_set.size())
      {
         // Take the element out
         tmp = explored_set.top();  explored_set.pop();
         
         // Don't print again
         if (find(printed_set.begin(), printed_set.end(), tmp) != printed_set.end()) continue;

         // Marked with set
         printed_set.push_back(tmp);

         // Print the gate type
         cout << '[' << lineNo << ']' << ' ' << setw(4) << left << tmp->getTypeStr() << tmp->_gateId;
         
         // Print fanins if it is AIG gate
         for (auto it2 : tmp->_fanin)
         {
            cout << ' ';
            if (CirGate::gate(it2)->isFloating())     cout << '*';
            if (CirGate::isInv(it2))                  cout << '!';
            cout << CirGate::gate(it2)->_gateId;
         }

         // Print if the gate has Symbol
         if (tmp->hasSymbol())   cout << " (" << tmp->_symbol << ')'; 

         cout << endl;

         ++lineNo;
      }
   }
}

void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit:";
   for (auto it : _pin) cout << ' ' << it;
   cout << endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit:";
   for (auto it : _pout) cout << ' ' << it;
   cout << endl;
}

/*
   Print the Floating Gates Message

   Attribute
   - Floating fanins
   - Defined but not used 
*/
void
CirMgr::printFloatGates() const
{
   if (!_floating.empty())
   {
      cout << "Gates with floating fanin(s):";
      for (auto it : _floating) cout << ' ' << it;
      cout << endl;
   }

   if (!_notused.empty())
   {
      cout << "Gates defined but not used  :";
      for (auto it : _notused)  cout << ' ' << it;
      cout << endl;
   }
}

void
CirMgr::writeAag(ostream& outfile) const
{

}

void
CirMgr::reset()
{
   _M = _I = _L = _O = _A = 0;   
   for (auto it : _gates)  delete it;
   cirMgr = NULL;
}

/**********************************************************/
/*   HELPER FUNCTION                                      */
/**********************************************************/

/*
   DepthFirstSearch Algorithm.
   
   Search the gate from POut to PIn.

   @param gateID
      The gateID to start searching.
   @param depth
      The depth of graph to execute.
*/
void
CirMgr::DepthFirstTraversal(const unsigned int &gateID, const unsigned int &depth = 0)
{
   stack<CirGate*> frontier;
   frontier.push(getGate(gateID));
   DepthFirstTraversal(frontier, 0);
}

/*
   DepthFirstSearch Algorithm.
   
   Search the gate from POut(s) to PIn(s).

   @param frontier
      The gateID(s) to start searching.
   @param depth
      The depth of graph to execute.
*/
void
CirMgr::DepthFirstTraversal(stack<CirGate*> &frontier, const unsigned int &depth = 0)
{
   CirGate* tmp;

   while (frontier.size())
   {
      // Take the element out
      tmp = frontier.top();  frontier.pop();

      // If have already seen the element
      if (tmp->isMarked()) continue;

      // If have never seen the ascenders
      tmp->mark();

      for (auto it : tmp->_fanin)
      {
         frontier.push(CirGate::gate(it));
         CirGate::gate(it)->_fanout.push_back(tmp);
      }  
   }
}

/*
   DepthFirstSearch Algorithm.
   
   Search the gate from POut(s) to PIn(s).

   @param frontier
      The gateID to start searching.
   @param dfslist
      The gateID List
   @param depth
      The depth of graph to execute.
*/
void
CirMgr::DepthFirstTraversal(stack<CirGate*> &frontier, vector<CirGate*> &dfslist, const unsigned int &depth = 0)
{
   CirGate* tmp;

   while (frontier.size())
   {
      // Take the element out
      tmp = frontier.top();  frontier.pop();

      // If have already seen the element, ignore it
      if (tmp->isMarked()) continue;

      // Else, mark the Gate
      tmp->mark();

      for (auto it : tmp->_fanin)
         frontier.push(it);
   }
}

bool
CirMgr::readHeader(const string &s)
{
   if (sscanf(s.c_str(), "aag %d %d %d %d %d", &_M, &_I, &_L, &_O, &_A) == 5) { return true; }

   return false;
}

/*
   Input pin will have no fanin, but multiple fanout

   @param id
      The ID Number of the Gate
   @return bool
      True if successfully load a PIn
*/
bool
CirMgr::loadInput(const unsigned int &id)
{
   _pin.push_back(id);
   _gates[id] = new CirPIGate(id, lineNo);

   return true;
}

/*
   Output pin will have no fanout, but 1 fanin

   @param id
      The ID Number of the Gate
   @param fanin
      The ID Number of the Fanin
   @return bool
      True if successfully load a POut
*/
bool 
CirMgr::loadOutput(const unsigned int &id, const unsigned int &fanin)
{
   _pout.push_back(id);
   _gates[id] = new CirPOGate(id, lineNo);
   
   return true;
}

/*
   And-Inverter Gate will have 2 fanin, variant fanout.

   @param id
      The ID Number of the Gate Output
   @param i1, i2
      The ID Number of the fanin (input)
*/
bool
CirMgr::loadAIG(const unsigned int &id, const unsigned int &i1, const unsigned int &i2)
{
   _aig.push_back(id);
   _gates[id] = new CirAIGate(id, lineNo);

   return true;
}

bool 
CirMgr::loadSymbol(const unsigned int &id, const string &s)
{
   _gates[id]->_symbol = s;
   return true;
}

bool
CirMgr::loadComment(const string &s)
{
   return true;
}

/*
   Connect 2 gates.

   @param ascender, descender
      Fanin / Fanout
   @return bool
      True if successfully connect the Circuit
*/
bool
CirMgr::connect(const unsigned int &ascender, const unsigned int &descender)
{
   // getGate(ascender / 2)->addFanout(getGate(descender / 2), ascender % 2);
   // getGate(descender / 2)->addFanin(getGate(ascender / 2), ascender % 2);

   return true;
}