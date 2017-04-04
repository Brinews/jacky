#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "bst.h"

/*
 This is simple I/O test client for bst.c that
 uses a very specific input format.

 Failure to follow format might lead to undefined behaviour.

 l: print bst_size
 i INT: bst_insert
 f INT: print bst_find
 s INT: print bst_select
 r INT: bst_remove
 g INT INT: print bst_range
 p INT: bst_print
 x: print bst_to_sorted_array
 a LEN INT INT ... INT: sorted_array_to_bst
                        (destroys old bst first)
 b: bst_rebalance
*/

// get_int() reads in an int from input
// effects: if unable to read in a number, prints message and exits
int get_int(void) {
  int i;
  if (scanf("%d", &i) != 1) {
    printf("exit: invalid number\n");
    exit(1);
  }
  return i;
}

int main(void){
  struct bst *t = bst_create();
  int i;
  int j;
  while (1){
    char cmd;
    if (scanf(" %c", &cmd) != 1) break;
    if (cmd == 'q') break;
    if (cmd == 'l') {
      printf("size: %d\n",bst_size(t));
    } else if (cmd == 'i') {
      i = get_int();
      bst_insert(i, t);
    } else if (cmd == 'f') {
      i = get_int();
      printf("find(%d): %d\n", i, bst_find(i, t));
    } else if (cmd == 's') {
      i = get_int();
      printf("select(%d): %d\n", i, bst_select(i, t));
    } else if (cmd == 'r') {
      i = get_int();
      bst_remove(i, t); 
    } else if (cmd == 'g') {
      i = get_int();
      j = get_int();
      printf("range(%d:%d): %d\n", i, j, bst_range(i, j, t));
    } else if (cmd == 'p') {
      i = get_int();
      printf("print(%d): ", i);
      bst_print(i, t);
    } else if (cmd == 'x') {
      int *a = bst_to_sorted_array(t);
      printf("[");      
      if (a) {
        for (int k=0; k < bst_size(t); ++k) {
          if (k) printf(",");
          printf("%d", a[k]);
        }
        free(a);
      } else {
        printf("empty");
      }
      printf("]\n");      
    } else if (cmd == 'a') {
      bst_destroy(t);
      j = get_int();
      int *a = malloc(j * sizeof(int));
      for (int k=0; k < j; ++k) {
        a[k] = get_int();
      }
      t = sorted_array_to_bst(a, j);
      free(a);
    } else if (cmd == 'b') {
      bst_rebalance(t);

    } else {
      printf("Invalid command (%c).\n",cmd);
      break;
    }
  }
  bst_destroy(t);
}
