/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool
initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
      )) {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------

CmdExecStatus
MTResetCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      int b;
      if (!myStr2Int(token, b) || b < int(toSizeT(sizeof(MemTestObj)))) {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
      #ifdef MEM_MGR_H
      mtest.reset(toSizeT(b));
      #else
      mtest.reset();
      #endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
   cout << setw(15) << left << "MTReset: " 
        << "(memory test) reset memory manager" << endl;
}


//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------

CmdExecStatus
MTNewCmd::exec(const string& option)
{
   // TODO

   vector<string> tokens;
   int numObjects = 0;      // n times
   int arraySize = 0;       // array size

   if (!lexOptions(option, tokens)) {
      return errorOption(CMD_OPT_MISSING, option);
   }

   for (size_t i = 0; i < tokens.size(); ++i) {
      // Parsing [-Array (size_t arraySize)]
      if (!myStrNCmp("-Array", tokens[i], 2)) 
      {
         // Check Argument Repeat
         if (arraySize)
            return errorOption(CMD_OPT_EXTRA, tokens[i]);
         // Check Argument Missing
         if (i == tokens.size() - 1)
            return errorOption(CMD_OPT_MISSING, tokens[i]);
         // Check Next Token is vaild
         if (!myStr2Int(tokens[++i], arraySize) || arraySize <= 0)
            return errorOption(CMD_OPT_ILLEGAL, tokens[i]);
      }
      // Parsing <(size_t numObjects)>
      else 
      {
         if (numObjects)
            return errorOption(CMD_OPT_EXTRA, tokens[i]);
         if (!myStr2Int(tokens[i], numObjects) || numObjects <= 0)
            return errorOption(CMD_OPT_ILLEGAL, tokens[i]);
      }
   }

   // If Parse no numObjects
   if (numObjects == 0)
      return errorOption(CMD_OPT_MISSING, "");
   
   // Use try-catch to catch the bad_alloc exception
   try 
   {
      if (arraySize)
         mtest.newArrs(numObjects, arraySize);
      else
         mtest.newObjs(numObjects);
   }
   catch(bad_alloc BadAllocError) 
   {
      return CMD_EXEC_ERROR;
   }
   return CMD_EXEC_DONE;
}

void
MTNewCmd::usage(ostream& os) const
{  
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
   cout << setw(15) << left << "MTNew: " 
        << "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------

CmdExecStatus
MTDeleteCmd::exec(const string& option)
{
   // TODO
   vector<string> tokens;
   bool randomOpt = false;
   bool arrayOpt = false;
   int id = -1;
   
   if (!lexOptions(option, tokens))
      return errorOption(CMD_OPT_MISSING, option);

   for (size_t i = 0; i < tokens.size(); ++i) {
      // Parsing [-Array]
      if (!myStrNCmp("-Array", tokens[i], 2)) 
      {
         if (arrayOpt)
            return errorOption(CMD_OPT_EXTRA, tokens[i]);
         else
            arrayOpt = true;
      }
      // Parsing <-Random (size_t numRandiD)>
      else if (!myStrNCmp("-Random", tokens[i], 2)) 
      {
         if (id != -1)
            return errorOption(CMD_OPT_EXTRA, tokens[i]);
         if (++i == tokens.size())
            return errorOption(CMD_OPT_MISSING, "-R");
         if (!myStr2Int(tokens[i], id) || id <= 0)
            return errorOption(CMD_OPT_ILLEGAL, tokens[i]);

         randomOpt = true;
      }
      // Parsing <-Index (size_t objId)>
      else if (!myStrNCmp("-Index", tokens[i], 2))
      {
         if (id != -1)
            return errorOption(CMD_OPT_EXTRA, tokens[i]);
         if (++i == tokens.size())
            return errorOption(CMD_OPT_MISSING, "-I");
         if (!myStr2Int(tokens[i], id) || id < 0)
            return errorOption(CMD_OPT_ILLEGAL, tokens[i]);
      }
      else
      {
         return errorOption(CMD_OPT_ILLEGAL, tokens[i]);
      }
   }

   if (randomOpt) 
   {
      if (arrayOpt) 
      {
         if (!mtest.getArrListSize()) {
            cerr << "Size of array list is " << mtest.getArrListSize() << "!!" << endl;
            return errorOption(CMD_OPT_ILLEGAL, to_string(id));
         }
         for (int i = 0; i < id; ++i)
            mtest.deleteArr(rnGen(mtest.getArrListSize()));
      }
      else
      {
         if (!mtest.getObjListSize()) {
            cerr << "Size of object list is " << mtest.getObjListSize() << "!!" << endl;
            return errorOption(CMD_OPT_ILLEGAL, to_string(id));
         }   
         for (int i = 0; i < id; ++i)
            mtest.deleteObj(rnGen(mtest.getObjListSize()));
      }
   }
   else 
   {
      if (arrayOpt)
      {
         if (id < mtest.getArrListSize())
            mtest.deleteArr(id);
         else {
            cerr << "Size of array list (" << mtest.getArrListSize() << ") is <= " << id << "!!" << endl;
            return errorOption(CMD_OPT_ILLEGAL, to_string(id));
         }
      }
      else
      {
         if (id < mtest.getObjListSize())
            mtest.deleteObj(id);
         else {
            cerr << "Size of object list (" << mtest.getObjListSize() << ") is <= " << id << "!!" << endl;
            return errorOption(CMD_OPT_ILLEGAL, to_string(id));
         }
      }
   }

   
   return CMD_EXEC_DONE;
}

void
MTDeleteCmd::usage(ostream& os) const
{  
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
   cout << setw(15) << left << "MTDelete: " 
        << "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------

CmdExecStatus
MTPrintCmd::exec(const string& option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
   os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
   cout << setw(15) << left << "MTPrint: " 
        << "(memory test) print memory manager info" << endl;
}


