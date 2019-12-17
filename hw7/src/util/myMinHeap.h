/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
   MinHeap(size_t s = 0) { if (s != 0) _data.reserve(s); }
   ~MinHeap() {}

   void clear() { _data.clear(); }

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const 
   { 
      return _data[i]; 
   }   
   
   Data& operator [] (size_t i) 
   { 
      return _data[i]; 
   }

   size_t size() const 
   { 
      return _data.size(); 
   }

   // TODO
   
   /*
      Return the reference of the minimum element (_data[0]);
   */
   const Data& min() const 
   { 
      return _data[0];
   }

   void insert(const Data& d) 
   { 
      _data.push_back(d);
      
      size_t index = _data.size() - 1;
      Data tmp = _data[index];

      // Float up
      while (index && (_data[index] < _data[(index - 1) / 2]))
      {
         // End Condition
         // if (_data[index] < _data[(index - 1) / 2]) 
         // { 
         // Swap
         tmp = _data[index]; 
         _data[index] = _data[(index - 1) / 2]; 
         _data[(index - 1) / 2] = tmp;
         
         index = (index - 1) / 2;
         // }
      }
   }
   
   void delMin() 
   { 
      delData(0);
   }
   
   /*
      Notes: Index is start from 0, therefore the childIndex is 
             index * 2 + 1
   */
   void delData(size_t i) 
   { 
      size_t childIndex = i * 2 + 1;
      size_t parentIndex = (i - 1) / 2;
      Data tmp = _data.back();

      // Copy the last element to the _data[i]
      _data[i] = _data.back(); _data.pop_back();

      // Sink down if child exists, Selecting the smallest side
      while (childIndex < _data.size())
      { 
         // If Right Child exist, update childIndex if need
         if (childIndex + 1 < _data.size())
         {
            // End Condition
            if (_data[i] < _data[childIndex] && _data[i] < _data[childIndex + 1]) { break; }

            // Select Right Child if it's smaller than left child
            if (_data[childIndex + 1] < _data[childIndex]) { ++childIndex; }
         }

         // Check if satisfy end condition.
         if (_data[i] < _data[childIndex]) { break; }

         // Swap and next iteration
         tmp = _data[i]; 
         _data[i] = _data[childIndex]; 
         _data[childIndex] = tmp;

         // Next Iteration
         i = childIndex;
         childIndex = i * 2 + 1;         
      }

      // Float up if the parent is larger that the child
      while (parentIndex)
      {
         if (_data[parentIndex] < _data[i]) { break; }

         // Swap and next iteration
         tmp = _data[i]; 
         _data[i] = _data[parentIndex]; 
         _data[parentIndex] = tmp;

         // Next Iteration
         i = parentIndex;
         parentIndex = (i - 1) / 2;         

      }
   }

private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
