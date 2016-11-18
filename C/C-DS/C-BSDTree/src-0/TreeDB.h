//declarations for function in class TreeDB
#ifndef _TREEDB_H
#define _TREEDB_H

#include "TreeNode.h"
#include "DBentry.h"

class TreeDB {
private:
   TreeNode* root;
   int probesCount;

   int activeCount;

   // You will need to add additional private functions
   bool insertFunc(DBentry* newEntry, TreeNode* cur);
   DBentry* findFunc(string name, TreeNode* cur, int probes);
   void activeFunc(TreeNode* cur);
   void clearFunc(TreeNode* cur);
   TreeNode* removeNext(TreeNode *cur);

public:
   // the default constructor, creates an empty database.
   TreeDB();

   // the destructor, deletes all the entries in the database.
   ~TreeDB();

   // inserts the entry pointed to by newEntry into the database.
   // If an entry with the same key as newEntry's exists
   // in the database, it returns false. Otherwise, it returns true.
   bool insert(DBentry* newEntry);

   // searches the database for an entry with a key equal to name.
   // If the entry is found, a pointer to it is returned.
   // If the entry is not found, the NULL pointer is returned.
   // Also sets probesCount
   DBentry* find(string name);

   // deletes the entry with the specified name (key)  from the database.
   // If the entry was indeed in the database, it returns true.
   // Returns false otherwise.
   // See section 6 of the lab handout for the *required* removal method.
   // If you do not use that removal method (replace deleted node by
   // maximum node in the left subtree when the deleted node has two children)
   // you will not match exercise's output.
   bool remove(string name);

   // deletes all the entries in the database.
   void clear();

   // prints the number of probes stored in probesCount
   void printProbes() const;

   // computes and prints out the total number of active entries
   // in the database (i.e. entries with active==true).
   void countActive();

   TreeNode *getRoot() const { return root; }
   void printNode(ostream &out, TreeNode* cur) const;
   // Prints the entire tree, in ascending order of key/name
   // The entries are printed one per line, using the 
   // operator<< for DBentry. 
   friend ostream& operator<< (ostream& out, const TreeDB& rhs);
};

#endif
