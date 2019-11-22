/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
// TODO: design your own class!!
template <class T>
class BSTreeNode
{
private:
   friend class BSTree<T>;
   friend class BSTree<T>::iterator;

   BSTreeNode() {}
   BSTreeNode(const T& d, BSTreeNode<T>* l = 0, BSTreeNode<T>* r = 0): _data(d), _left(l), _right(r) {}
   ~BSTreeNode() { }

   T               _data;
   BSTreeNode<T>*  _left;
   BSTreeNode<T>*  _right;
};

// TODO: design your own class!!
template <class T>
class BSTree
{
public:
   // Initialize with a Dummy Node
   BSTree() { _root = 0; }; 
   ~BSTree() { delete _root; }
   
   class iterator 
   { 
   public:
      friend class BSTree;
      
      iterator(BSTreeNode<T>* n = 0): _node(n) { _trace.clear(); _trace.push_back(n); }
      iterator(const iterator& i) : _node(i._node) { _trace = i._trace; _direction = i._direction; }
      ~iterator() {}

      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }

      iterator& operator ++ () { 
         if (!_node)    
            return (*(this));
         
         BSTreeNode<T>* tmp = BSTree::successor(_node);
         bool dir;
         
         if (!tmp)                           // Find the next element from upper level
         {
            while (_direction.size())           // Check if it's not the _root
            {
               tmp = _trace.back();                // Find the parent
               _trace.pop_back();

               dir = _direction.back();            // Find the direction
               _direction.pop_back();

               if (!dir)                        // If it's a left path (Found the element): 
               {                                   // Save the address and Return 
                  _node = tmp;
                  return *(this);
               }
            }

            _node = 0;                          // If not found
         }
         else                                // Find the next element from lower level
         {
            goRight();                          // Finding it's successor.
            while (_node->_left) goLeft();
         }
         
         return *(this);
      }

      iterator& operator -- () { 
         if (!_node) 
         {
            _node = _trace.back(); 

            if (_node)
            {
               while (_node->_right)   goRight();

               return (*(this));
            }
         }

         BSTreeNode<T>* tmp = BSTree::predecessor(_node);
         bool dir;

         if (!tmp)                           // Find the next element from upper level
         {
            while (_direction.size())           // Check if it's not the _root
            {
               tmp = _trace.back();                // Find the parent
               _trace.pop_back();

               dir = _direction.back();            // Find the direction
               _direction.pop_back();

               if (dir)                        // If it's a right path (Found the element): 
               {                                   // Save the address and Return
                  _node = tmp;
                  return (*(this));
               }
            }

            _node = 0;                          // If not found
         }
         else                                // Find the next element from lower level
         {
            goLeft();                           // Finding it's predecessor.
            while (_node->_right) goRight();
         }
         
         return *(this);
      }

      iterator operator ++ (int) { iterator tmp = iterator(_node); ++(*(this)); return tmp; }
      iterator operator -- (int) { iterator tmp = iterator(_node); --(*(this)); return tmp; }

      iterator& operator = (const iterator& i) { 
         _node = i._node; _trace = i._trace; _direction = i._direction; return *(this); 
      }

      bool operator != (const iterator& i) const { 
         return (_node != i._node)? true : false; 
      }

      bool operator == (const iterator& i) const {
         return (_node == i._node)? true : false; 
      }

   private:
      BSTreeNode<T>*           _node;
      
      vector<BSTreeNode<T>*>   _trace;             // Remember the path from _root to _node.

      vector<short int>        _direction;         // Remember the direction (left - 0 / right - 1) 
                                                   // from _root to _node.
   
      void track(BSTreeNode<T>* n) { }
      iterator& goLeft() { _trace.push_back(_node); _node = _node->_left; _direction.push_back(false); return *(this); }
      iterator& goRight() { _trace.push_back(_node); _node = _node->_right; _direction.push_back(true); return *(this); }
      iterator& goParent() { _node = _trace.back(); _trace.pop_back(); _direction.pop_back(); return *(this); }
      iterator getLeft() { iterator it = (*(this)); it.goLeft(); return it; }
      iterator getRight() { iterator it = (*(this)); it.goRight(); return it; }
      iterator getParent() { iterator it = (*(this)); it.goParent(); return it; }
      bool isRoot() { return !(_direction.size()); }
   };

   // TODO: implement these functions
   iterator begin() const { 
      iterator it = iterator(_root);   
      
      if (_root) 
      {   
         while (it._node->_left) 
            it.goLeft();
      } 
      
      return it; 
   }
   
   iterator end() const { 
      iterator it = iterator(_root);   it._node = 0;  return it; 
   }

   bool empty() const { return (_root == 0)? true : false; }

   size_t size() const { 
      size_t s = 0;
      for (iterator it = begin(); it != end(); ++it) ++s;
      return s; 
   }

   void insert(const T& x) { 
      BSTreeNode<T>* newNode = new BSTreeNode<T>(x);
      BSTreeNode<T>* pos = _root;

      if (empty())
      {   
         _root = newNode;  
         return;
      }
      else 
      {
         while (true) 
         {
            if (newNode->_data > pos->_data)          // Larger -> right side 
            {
               if (pos->_right == 0)                     // If right side is empty
               {                                            // Save to right side
                  pos->_right = newNode; 
                  break;
               } 
               else                                      // If right side is not empty
               {                                            // Go deeper to left side
                  pos = pos->_right;
               }
            }
            else                                      // Smaller or Equal -> left side
            {
               if (pos->_left == 0)                      // If left side is empty
               {                                            // Save to left side
                  pos->_left = newNode; 
                  break;
               } 
               else                                      // If left side is not empty
               {                                            // Go deeper to left side
                  pos = pos->_left;
               }
            }
         }
      }

      return;
   }

   /*
      Pop the minimum element
   */
   void pop_front() { 
      if (!empty()) erase(begin());
   }
   
   /*
      Pop the maximum element
   */
   void pop_back() { 
      if (!empty()) erase(--end());
   }

   /*
      When deleting a node, only need to consider the parent
      and the left / right subtree

      Notes:
      1. If erasing the parent, remember to modify _root
      
      @return false if nothing to erase
   */
   bool erase(iterator pos) {
      iterator successor = pos;
      iterator parent = pos.getParent();

      if (pos._node->_left && pos._node->_right)      // Case 1 : Full Node
                                                      // Find the successor to alternate the erasing node.
      {
         ++successor;
         *pos = *successor;                              // Copy the data of successor
         erase(successor);                               // Recursive call erase(successor);

         return true;
      } 
      else if (pos._node->_left || pos._node->_right) // Case 2 : Have one-side of Subtree
                                                      // Get Address to Re-linking
      {                                               
         if (pos._node->_left)
            successor.goLeft();
         else 
            successor.goRight();

         if (pos.isRoot())
            _root = successor._node;
         else if (!pos._direction.back())                
            parent._node->_left = successor._node;
         else
            parent._node->_right = successor._node;
            
         delete pos._node;

         return true;
      }
      else                                            // Case 3 : Leaf
                                                      // Empty the parent's link
      {
         if (pos.isRoot())
            _root = 0;
         else if (!pos._direction.back())
            parent._node->_left = 0;
         else 
            parent._node->_right = 0;
         
         delete pos._node; 
         
         return true;
      }
         
      return false;
   }

   /*
      @return false if nothing to erase
   */
   bool erase(const T& x) { 
      iterator tmp = find(x);

      if (tmp != end()) {
         erase(tmp);
         return true;
      }

      return false;
   }

   /*
      Implement Binary Tree Search

      @return iterator point to target, end() if not found.
   */
   iterator find(const T& x) { 
      iterator it = iterator(_root);

      while (it._node) {
         if (*it == x) { return it; }
         else if (*it < x) { it.goRight(); }
         else if (*it > x) { it.goLeft(); } 
      }

      return end();
   }


   void clear() { 
      if (!empty()) {
         if (_root->_left)  clear(_root->_left);
         if (_root->_right) clear(_root->_right);
         delete _root; _root = 0;
      }
   }

   /*
      Clean the entire subtree (given the root of subtree)

      @params node the root of tree want to clean
   */
   void clear(BSTreeNode<T>* &node) { 
      if (!empty()) {
         if (node->_left)  clear(node->_left);
         if (node->_right) clear(node->_right);
         delete node; node = 0;
      }
   }

   // Done!
   void sort() { }
   void print() const { }

private:
   BSTreeNode<T>* _root;

   unsigned int height(BSTreeNode<T>* node) {
      BSTreeNode<T>* tmp = _root;
      unsigned int i = 0;

      while (node != tmp) {
         
         if (node->_data > tmp->_data)   
            tmp = tmp->_right;
         else  
            tmp = tmp->_left; 
         
         ++i;
      }

      return i;
   }

   /*
      @return the mininum element at the subtree.
   */
   static BSTreeNode<T>* min(BSTreeNode<T>* node) {
      while (node->_left) node = node->_left;
      return node;
   }

   static BSTreeNode<T>* max(BSTreeNode<T>* node) {
      while (node->_right) node = node->_right;
      return node;
   }

   /*
      get the next larger node in this node's subtree
      
      @return 0 if not find.
   */
   static BSTreeNode<T>* successor(BSTreeNode<T>* &node) {
      return (!(node->_right))? 0 : min(node->_right);
   }

   /*
      get the previous larger node in this node's subtree
      
      @return 0 if not find.
   */
   static BSTreeNode<T>* predecessor(BSTreeNode<T>* &node) {
      return (!(node->_left))? 0 : max(node->_left);
   }

   /*
      @return true if the node is the leaf
   */
   static bool isLeaf(BSTreeNode<T>* &node) {
      return (node->_left == 0 && node->_right == 0)? true : false;
   }

   static bool isLeaf(iterator &it) {
      return isLeaf(it._node);
   }

};

#endif // BST_H
