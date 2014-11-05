/******************************************************
  heap.h -- Declarations for Basic Heap-of-Ints Class
*******************************************************/
#pragma once
#include <iostream>
using namespace std;

class Heap{

public:
      // Constructors and Destructor
      Heap();  // New empty Heap with default capacity.
      Heap(int c); // New empty Heap with capacity c.
      Heap(int * B, int s, int c); // New Heap with capacity c containing first s elements from array B.
      Heap( const Heap & H ); // Copy constructor.
      ~Heap(); // Destructor.

      // Size and Capacity
      bool empty() {return h_size == 0;}; // True iff Heap is empty.
      int size(){ return h_size ;}; // Current size of Heap.
      int capacity(){ return h_capacity ;}; // Current capacity.

      // Operators
      Heap operator+( const Heap & H ) const; // New Heap with combined contents and capacity of operands.

      // Modifiers
      void insert(int x); // Insert element x.
      int extract_min(); // Remove and return the minimum element.

      // Display
      void display(); // Print a string representation of the heap contents to standard out.
 
private:
      int* A ; // Array containing heap contents.
      int  h_capacity ; // Max number of elements (= size of A).
      int h_size ; // Current number of elements.

      void trickle_up(int i);// Repairs ordering invariant after changing root.
      void trickle_down(int i);// Repairs ordering invariant after adding a leaf.
      void make_heap();// Restores ordering invariant to entier contents.
};

