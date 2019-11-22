/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList() {
      // Initialize _head as a dummy node
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; 
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }

      iterator& operator ++ () { 
         _node = _node->_next; return *(this); 
      }
      iterator operator ++ (int) { 
         iterator tmp = iterator(_node); _node = _node->_next; return tmp;
      }
      iterator& operator -- () { 
         _node = _node->_prev; return *(this); 
      }
      iterator operator -- (int) { 
         iterator tmp = iterator(_node); _node = _node->_prev; return tmp; 
      }

      iterator& operator = (const iterator& i) { 
         _node = i._node; return *(this); 
      }
      bool operator != (const iterator& i) const { 
         return (_node != i._node)? true : false; 
      }
      bool operator == (const iterator& i) const { 
         return (_node == i._node)? true : false; 
      }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions

   // Point to first node
   iterator begin() const { return _head->_next; }
   
   // Point to dummy node
   iterator end() const { return _head; }

   bool empty() const {  
      return (begin() == end())? true : false;
   }

   size_t size() const { 
      size_t s = 0;
      for (iterator it = begin(); it != end(); ++it) ++s;
      return s; 
   }

   void push_back(const T& x) { 
      DListNode<T>* newNode = new DListNode<T>(x, _head->_prev, _head);
      
      _head->_prev->_next = newNode;
      _head->_prev = newNode;

      _isSorted = false;
   }

   void pop_front() { 
      if (!empty()) {
         DListNode<T>* tmp = _head->_next;
         connect(_head, _head->_next->_next);
         delete tmp;
      }
   }
   
   void pop_back() {
      if (!empty()) {
         DListNode<T>* tmp = _head->_prev;
         connect(_head->_prev->_prev, _head);
         delete tmp;
      }
   }

   // return false if nothing to erase
   bool erase(iterator pos) { 
      for (iterator it = begin(); it != end(); ++it) {
         if (it == pos) {
            DListNode<T>* tmp = it._node;
            connect(tmp->_prev, tmp->_next);
            delete tmp;
            
            return true;
         }
      }

      return false; 
   }

   // return false if nothing to erase
   bool erase(const T& x) { 
      for (iterator it = begin(); it != end(); ++it) {
         if (*it == x) {
            DListNode<T>* tmp = it._node;
            connect(tmp->_prev, tmp->_next);
            delete tmp;

            return true;
         }
      }
      return false; 
   }

   // return end() if not found.
   iterator find(const T& x) { 
      iterator it = begin();

      while (it != end()) {
         if (*it == x) break;
         ++it;
      }

      return it;
   }

   // delete all nodes except for the dummy node
   void clear() {
      if (size()) 
      {
         DListNode<T>* tmp = _head->_next;            // Start from first element
         DListNode<T>* buf;

         _head->_prev->_next = 0;                     // Disconnect the connection between last 
                                                      // element and dummy node. 
         
         while (tmp) {                                // Chain Deletion
            buf = tmp->_next; delete tmp; tmp = buf;
         }
         
         _head->_prev = _head->_next = _head;         // Set Dummy Node Property
      }

      _isSorted = true;
   }  

   /*
      Implement bubble-sort with ascending order
   */
   void sort() const { 
      if (!_isSorted) {

         for (iterator boundary = --end(); boundary != begin(); --boundary) // Right Pointer 
         {
            for (iterator tmp = begin(); tmp != boundary; ++tmp)              // Left Pointer
            {  
               if (tmp._node->_data > tmp._node->_next->_data)                // Swap Condition
                  swap(tmp._node, tmp._node->_next);
            }
         }

         _isSorted = true;
      }
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
   void connect(DListNode<T>* &a, DListNode<T>* &b) {
      a->_next = b; b->_prev = a;
   }   

   // Swap DListNode._data
   void swap(DListNode<T>* &a, DListNode<T>* &b) const {
      T tmp = a->_data; a->_data = b->_data; b->_data = tmp;
   }

   // Swap DListNode._data
   void swap(iterator &a, iterator &b) const {
      T tmp = *a; *a = *b; *b = tmp;
   }

};

#endif // DLIST_H
