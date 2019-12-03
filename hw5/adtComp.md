# Report - DSnP Homework 5 ADT Implementation

Student Name: 李華健

Student ID: B05901119

## Outline

1. Dynamic Array
    - Design
    - Performance
2. Doubly Linked List
    - Design
    - Performance
3. Binary Search Tree
    - Design
    - Performance

## ADT: Dynamic Array

### Dynamic Array: Design

Data member (and property description) in Array:

```cpp
class Array 
{
private:
    // Array Pointer
    T*              _data;

    // Number of elements in Array()
    size_t          _size;      

    // Maximum number of elements
    // If full, grow with ratio 2 
    size_t          _capacity;  
    
    // Initialized with value true
    // When push_back(), set to false;
    // When pop_back(), keeps the same;
    // When sort(), set to true;
    mutable bool    _sorted;    

}
```

Array Iterator:

1. By the property of continuous memory, array iterator can be implemented easliy:
   - `begin()` point to the first memory cell of the array
   - `end()` point to the memory after the last cell of the array
   - Move forward / Move backward
     - `++address` / `--address`

2. Not Implemented
   - Apply binary search if the array was sorted.

### Dynamic Array: Experiments

1. Done the processes defined in dofiles quickly

2. Issue: Coping array (using `copy()`) and Sorting (using `::sort()`)

```
adtappend -r 1000000    
// 0.17s (write data, extend (= array copy) needs time )

adtdelete -r 1000000    
// 0.07s (Only decrease _size)

adtappend -r 1000000    
// 0.07s (Increase _size and write data)

adtsort                
// 0.43s (Need time)
```

## ADT: Doubly Linked List

### Doubly Linked List: Design

Data member (and property description) in DList:

```cpp
class DList
{
private:
    DListNode<T>*    _head;     // Dummy Node

    // Initialized with value true
    // When push_back(), set as false;
    //      push_front()
    // When pop_back(), keeps the same;
    //      pop_front()
    // When sort(), set to true;
    mutable bool _isSorted;     
}
```

Why I decided to set the _head as dummy node?
1. No need to modify `_head` when `push_front()` or `pop_front()`
2. `begin()` points to the next element of `_head`
3. `end()` points to `_head`

How DList reset?
- Because `pop_front()` by looping will waste time to connect / disconnect nodes, I choose to get the address for each node (from `begin()` to `end()`) and free the memory to save some time.

How DList sorted?
- By the linked accessed memory, mergeSort() or quickSort() is not available
- Access element from a to b by $\text{time} \propto distance(a, b))$
- I choose to implement bubble sort (with complexity $O(n^2)$)

### Doubly Linked List: Experiments

## ADT: BST

### Binary Search Tree: Design

Main Idea:

I implemented BST using nodes (`BSTreeNode`), without `_parent` and `_dummy`.

Compare with implementation by array:

Pros:
- **Linear Memory Usage**, even though the Tree is biased to one side
- **Easy to implement** `erase()`, constant time operation (exclude `find()` the target element) when deleting a node which have plenty of ascendents
- Smaller overhead

Cons:
- Extra trace (path from `_root` to `node`, implemented in `Iterator`) handling
- Only iterator know the trace from `_root`

Data member (and property description) in BST:

```cpp
class BST
{
private:
    BSTreeNode<T>*  _root;   // NULL when the tree is empty

    // Some useful function when iterating.
    // Static for sharing to Iterator
    static BSTreeNode<T>* min(BSTreeNode<T>* n);
    static BSTreeNode<T>* max(BSTreeNode<T>* n);
    static BSTreeNode<T>* successor(BSTreeNode<T>* n);
    static BSTreeNode<T>* predecessor(BSTreeNode<T>* n);
}
```

BST Iterator

Idea: Always initialized Iterator from `_root`. I have implemented some basic function, such as `goLeft()`, `goRight()`, `goParent()`, and tracking the trace with `_trace` and `_direction`

```cpp
class Iterator
{
private:
    BSTreeNode<T>*          _node;

    // Record BSTreeNode<T>* from _root to _node
    //
    // Because Iterator can't access BSTree::_root, I choose
    // to store _root at the front of _trace, and _trace.size() 
    // will larger than _direction.size() with value 1.
    vector<BSTreeNode<T>* > _trace;         

    // Record direction (0 for left and 1 for right) 
    // from _root to _node
    vector<bool>            _direction;     
}
```

### Binary Search Tree: Experiments

```
adta -r 1000000
// 1.04s, more time than array (append + sort)

adtd -r 1000000
// Long time even though -O3 compile mode
```