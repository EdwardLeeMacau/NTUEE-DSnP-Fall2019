/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

// Initialize static data member
unsigned int CirGate::_globalMarker = 0;

/**************************************/
/*   class CirGate member functions   */
/**************************************/

/*
   Append CirGate* to _fanin

   @param c
      Fanin in type CirGate*
   @param isInv
      If True, store 0x1 at the LSB of CirGate*
*/
void 
CirGate::addFanin(CirGate* c, bool isInv)
{
   if (isInv) c = CirGate::setInv(c);
   _fanin.push_back(c);
}

/*
   Append CirGate* to _fanout

   @param c
      Fanout in type CirGate*
   @param isInv
      If True, store 0x1 at the LSB of CirGate*
*/
void
CirGate::addFanout(CirGate* c, bool isInv)
{
   if (isInv) c = CirGate::setInv(c);
   _fanout.push_back(c);
}

/*************************************************
==================================================
= AIG(100), line 312                             =
==================================================
*************************************************/

void
CirGate::reportGate() const
{
   stringstream ss;
   cout << "==================================================" << endl;

   // Store informations in stringstream   
   ss << "= " << getTypeStr() << '(' << _gateId << ")";
   if (_symbol != "") { ss << '"' << _symbol << '"';}
   ss << ", line " << _lineno;
   
   // Get Lenght of StringStream, fill with space
   ss.seekp(0, ios::end);  
   if (49 - ss.tellp() > 0) { ss << string(49 - ss.tellp(), ' '); ss << '='; }
   ss.seekp(0, ios::beg);

   // Release string to cout.
   cout << ss.str() << endl;

   cout << "==================================================" << endl;
}

void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);


}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);

   
}

/***************************************/
/* class CirConstGate member functions */
/***************************************/

void 
CirConstGate::printGate() const
{
   
}

/***************************************/
/* class CirUndefGate member functions */
/***************************************/

void 
CirUndefGate::printGate() const
{
   
}

/***************************************/
/* class CirPIGate member functions    */
/***************************************/

void
CirPIGate::printGate() const
{

}


/***************************************/
/* class CirPOGate member functions    */
/***************************************/

void
CirPOGate::printGate() const
{

}


/***************************************/
/* class CirAIGate member functions    */
/***************************************/

void 
CirAIGate::printGate() const
{
   
}