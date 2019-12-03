/****************************************************************************
  FileName     [ array.h ]
  PackageName  [ util ]
  Synopsis     [ Define dynamic array package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>
#include <algorithm>

using namespace std;

template <class T>
class Array
{
public:
   // TODO: decide the initial value for _isSorted
   Array() : _data(0), _size(0), _capacity(0), _isSorted(true) {}
   ~Array() { delete []_data; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class Array;

   public:
      iterator(T* n= 0): _node(n) {}
      iterator(const iterator& i): _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return (*_node); }
      T& operator * () { return (*_node); }
      iterator& operator ++ () { 
         _node++; return *(this); 
      }
      iterator operator ++ (int) { 
         T* tmp = _node; _node++; return *(this);
      }
      iterator& operator -- () { 
         _node--; return *(this);
      }
      iterator operator -- (int) { 
         T* tmp = _node; _node--; return tmp; 
      }

      iterator operator + (int i) const {
         T* tmp = _node; tmp += i; return tmp; 
      }
      iterator& operator += (int i) { 
         _node += i; return (*this); 
      }
      iterator operator - (int i) const {
         T* tmp = _node; tmp -= i; return tmp;
      }
      iterator& operator -= (int i) {
         _node -= i; return (*this);
      }

      iterator& operator = (const iterator& i) { 
         _node = i._node; return (*this); 
      }

      bool operator != (const iterator& i) const { 
         return (_node != i._node)? true : false; 
      }
      bool operator == (const iterator& i) const { 
         return (_node == i._node)? true : false; 
      }

   private:
      T*    _node;
   };

   // TODO: implement these functions
   iterator begin() const { return &_data[0]; }
   iterator end() const { return &(_data[_size]); }
   bool empty() const { return (size() == 0)? true : false; }
   size_t size() const { return _size; }

   T& operator [] (size_t i) { return _data[0]; }
   const T& operator [] (size_t i) const { return _data[0]; }

   void push_back(const T& x) { 
      // Handle the case of Container overflow
      if (_size == _capacity){
         (!_capacity)? resize(1) : resize(_capacity * 2);
      }

      _data[_size] = x; _size++; _isSorted = false;
   }
   
   void pop_front() {
      if (_size) {
         --_size; _isSorted = false;
         _data[0] = _data[_size];
      }
   }

   void pop_back() { 
      if (_size) {
         --_size;
      }
   }

   bool erase(iterator pos) { 
      // Return If Out-of-Range
      if (pos._node < _data || pos._node >= (_data + _size)) {
         return false;
      }

      *pos = *(end() - 1);
      _size--; 
      _isSorted = false;

      return true;
   }

   bool erase(const T& x) {
      iterator it = find(x); 
      
      if (it != end()){
         *it = *(end() - 1);
         _size--; _isSorted = false;
         return true;
      }
      
      return false; 
   }

   /*
      [Notes] Can implement binary search if array is sorted.

      @params x 

      @return it Return end() if not find x
   */
   iterator find(const T& x) { 
      iterator it = begin();
      
      while (it != end()) { 
         if (x == *it) 
            break;
         ++it;
      }

      return it;
   }

   /*
      Let the size of Array become 0
   */
   void clear() { 
      _size = 0; _isSorted = true;
   }

   // [Optional TODO] Feel free to change, but DO NOT change ::sort()
   void sort() const { 
      if (!empty() && !_isSorted) {
         ::sort(_data, _data + _size); 
         _isSorted = true; 
      }
   }

   // Nice to have, but not required in this homework...
   void reserve(size_t n) { }
   
   void resize(size_t n) { 
      if (n > _capacity) {
         T* newData = new T[n];

         if (_capacity) {
            copy(_data, _data + _size, newData);         
         }
         
         _capacity = n; 
         delete []_data;   
         _data = newData;
      }
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   T*            _data;
   size_t        _size;       // number of valid elements
   size_t        _capacity;   // max number of elements
   mutable bool  _isSorted;   // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] Helper functions; called by public member functions
};

#endif // ARRAY_H
