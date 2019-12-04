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

void 
CirGate::addFanin(CirGate* c, bool isInv)
{
   if (isInv) c = CirGate::setInv((size_t)c);
   _fanin.push_back(c);
}

void
CirGate::addFanout(CirGate* c, bool isInv)
{
   if (isInv) c = CirGate::setInv((size_t)c);
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
   cout << "==================================================" << endl
        << "= " << getTypeStr() << '(' << _gateId << ") line " << _lineno << endl
        << "==================================================" << endl;
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