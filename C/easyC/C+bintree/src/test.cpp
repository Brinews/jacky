/*
 *    CS110 Computer Architecture Homework 7
 *
 */

#include <iostream>
#include "bintree.h"


using namespace std;

int main(){

    binTree<int> tree;
    int nums[] = {3, 2 , -1 , 1 , 8 , 4 , 6,  5 , 9, -2};
    for(int i = 0; i < sizeof(nums) / sizeof(int); i++){
        tree.insert(nums[i]);
        cout<<"Inserted "<<nums[i]<<endl;
    }

   cout<<"\nIterator:\n"<<endl;
   binTree<int>::iterator it = tree.begin(); 
   cout << *it << endl;

   for(binTree<int>::iterator it = tree.begin(); it != tree.end(); it++){
       cout<<"Printing "<<*it<<endl;
   }

   cout<<"\nIterator Preorder:\n"<<endl;
   for(binTree<int>::preorder_iterator it = tree.pre_begin(); it != tree.pre_end(); it++){
       cout<<"Printing "<<*it<<endl;

       // editing is not really usefull because it might cause the bintree to not be a binary search tree anymore ...
       // but maybe a complex type is used where only one element (the "key") is not changed but the other is...
       //*it += 5;
       
   }
   cout<<endl;

    cout<<"\nIterator Inorder:\n"<<endl;
    for(binTree<int>::inorder_iterator it = tree.in_begin(); it != tree.in_end(); it++){
      cout<<"Printing "<<*it<<endl;
    //   *it += 5;    // this shouldn't work!!!
    }

    cout<<"\nIterator Postorder:\n"<<endl;
    for(binTree<int>::postorder_iterator it = tree.post_begin(); it != tree.post_end(); it++){
      cout<<"Printing "<<*it<<endl;
    //   *it += 5;    // this shouldn't work!!!
    }


   const int ARRAY_SIZE = 32;

    int arr[ARRAY_SIZE];

    cout << "Postorder:\n";
    tree.postorder(arr,ARRAY_SIZE);
    for(int i = 0; i < tree.size(); i++){
        cout<<" "<< arr[i];
    }
    cout<<endl;

    cout << "Preorder:\n";
    tree.preorder(arr,ARRAY_SIZE);
    for(int i = 0; i < tree.size(); i++){
        cout<<" "<< arr[i];
    }
    cout<<endl;

    cout << "Inorder:\n";
    tree.inorder(arr,ARRAY_SIZE);
    for(int i = 0; i < tree.size(); i++){
        cout<<" "<< arr[i];
    }
    cout<<endl;



    /* Correct orders for the given example:

    Input:     int nums[] = {3, 2 , -1 , 1 , 8 , 4 , 6,  5 , 9, -2};

    Preorder:
    3 2 -1 -2 1 8 4 6 5 9
    Inorder:
    -2 -1 1 2 3 4 5 6 8 9
    Postorder:
    -2 1 -1 2 5 6 4 9 8 3

    */


    const binTree<int> treeConst(tree);

    cout<<"\nConst Iterator Preorder:\n"<<endl;
    for(binTree<int>::const_preorder_iterator it = treeConst.pre_begin(); it != treeConst.pre_end(); it++){
      cout<<"Printing "<<*it<<endl;
       *it += 5;    // this shouldn't work!!!
    }

    /*
    cout << "Preorder:\n";
    treeConst.preorder(arr,ARRAY_SIZE);
    for(int i = 0; i < treeConst.size(); i++){
        cout<<" "<< arr[i];
    }
    cout<<endl;
    treeConst.inorder(arr,ARRAY_SIZE);
    for(int i = 0; i < treeConst.size(); i++){
        cout<<" "<< arr[i];
    }
    cout<<endl;
    */


    return 0;
} 
