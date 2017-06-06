/*
 *    CS110 Computer Architecture Homework 7
 *    Header File for binTree
 *    May 24, 2017
 *
 */

#ifndef BIN_TREE_H
#define BIN_TREE_H


/*
 *
 * The Binary Search Tree Class
 *
 */

template <class T>
class binTree {
private:

 
  /* The treeNode that forms the search tree.
     Has to be implemented as subclass of the binTree.
     The .h file only contains declarations, the implementations
     go into the .hpp file!
  */
  class treeNode {
    private:

    // the data to be saved - do not change this declaration! 
    T data;

    treeNode *left;
    treeNode *right;
    treeNode *parent;

    /* Fill in other missing items */
    public:
    treeNode(): left(NULL), right(NULL), parent(NULL) {}
    treeNode(T d, treeNode *l, treeNode *r) : data(d), left(l), right(r) { 
        parent = NULL;
    }
    treeNode(T d);

    T getData();

    friend class binTree;

  };

public:

  // For the iterators you have to implement the following functions: 
  // - Two comparison operators (equal and not equal)
  // - The tow standard data access oerators (one returning T& and one T*)
  //      * The const iterators of course have to return const versions for data access
  // - The pre-increment and the post-increment operator (e.g. ++i  and  i++)
  //
  // Implement, as sub-classes of binTree:
  // preorder_iterator & const_preorder_iterator 
  // inorder_iterator & const_inorder_iterator
  // postorder_iterator & const_postorder_iterator
  //
  // As those iterators share lots of functionality it might be good to use C++ techniques 
  // to re-use the code - e.g. inheritance ( e.g. class preorder_iterator: public iterator{};)
  //

 
  /* Declare the iterator classes here - implement in .hpp file! */
  class iterator {
  public:
      treeNode *nextNode;

      iterator() : nextNode(NULL) {}
      iterator(treeNode *root);
      virtual void increment();

      bool         operator==(const iterator& i2) const;
      bool         operator!=(const iterator& i2) const;

      T&           operator* ();
      const T&     operator* () const;
      T*           operator->();

      iterator&    operator++ ();    // pre-increment
      iterator     operator++ (int); // post-increment

      friend binTree;
  };

  class preorder_iterator : public iterator {
  public:
      preorder_iterator() : iterator() {}
      preorder_iterator(treeNode *root);
      void increment();

      friend binTree;
  };

  class inorder_iterator : public iterator {
  public:
      inorder_iterator() : iterator() {}
      inorder_iterator(treeNode *root);
      void increment();

      friend binTree;
  };

  class postorder_iterator : public iterator {
  public:
      postorder_iterator() : iterator() {}
      postorder_iterator(treeNode *root);
      virtual void increment();

      friend binTree;
  };

  class const_preorder_iterator : public preorder_iterator {
  public:
      const_preorder_iterator() : preorder_iterator() {}
      const_preorder_iterator(treeNode *root) : preorder_iterator(root){ }
  };

  class const_inorder_iterator : public inorder_iterator {
  public:
      const_inorder_iterator() : inorder_iterator() {}
      const_inorder_iterator(treeNode *root) : inorder_iterator(root){ }
  };

  class const_postorder_iterator : public postorder_iterator {
  public:
      const_postorder_iterator() : postorder_iterator() {}
      const_postorder_iterator(treeNode *root) : postorder_iterator(root){ }
  };


  /* Constructors */

  binTree();
  binTree(const binTree<T> & obj);

  treeNode *clone(treeNode *root, treeNode *parent);
  
  /* Destructor */
  ~binTree();

  void destroy(treeNode *&r);

  /* Inserts a node with the value val into the tree. If a node with an identical value exists, simply exit. */
  void insert(const T &val);

  /* Returns if a node with a given value exists in the tree. */
  bool exists(const T &val) const;

  /* Returns the number of items in the tree. */
  int size() const;
  int size(treeNode *root) const;


  /* Generate an array with items organized in preorder order. */
  void preorder(T *, int) const;
  int preorder(T *, int, treeNode *) const;

  /* Generate an array with items organized in inorder order. */
  void inorder(T *, int) const;
  int inorder(T *, int, treeNode *) const;

  /* Generate and array with items orgainzed in postorder order. */
  void postorder(T *, int) const;
  int postorder(T *, int, treeNode *) const;


  /* Various functions returning iterators */
  iterator begin();
  iterator end();

  preorder_iterator pre_begin();
  preorder_iterator pre_end();

  inorder_iterator in_begin();
  inorder_iterator in_end();

  postorder_iterator post_begin();
  postorder_iterator post_end();

  /* Add according const versions for returning the const iterators */
  const_preorder_iterator pre_begin() const;
  const_preorder_iterator pre_end() const;

  const_inorder_iterator in_begin() const;
  const_inorder_iterator in_end() const;

  const_postorder_iterator post_begin() const;
  const_postorder_iterator post_end() const;
private:
    /* Fill in this part */
    treeNode *root;

};


/* Here we automatically include the actual implementation ... */
#include "bintree.hpp"

#endif
