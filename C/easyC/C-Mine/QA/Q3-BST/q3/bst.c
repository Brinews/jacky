#include "bst.h"

#include <stdlib.h>
#include <stdio.h>

const int PRE_ORDER = 0;
const int IN_ORDER = 1;
const int POST_ORDER = 2;

struct bst *bst_create(void)
{
    struct bst * bs = (struct bst *) malloc(sizeof(struct bst));
    bs->root = NULL;

    return bs;
}

void free_node(struct bstnode *root)
{
    if (root) {
        free_node(root->left);
        free_node(root->right);
        free(root);
    }
}

void bst_destroy(struct bst *t)
{
    free_node(t->root);
    free(t);
}

int bst_size(struct bst *t)
{
    return t->root->size-1;
}

struct bstnode* bstnode_insert(int i, struct bstnode *root, int *succ)
{
    if (root == NULL) {
        root = (struct bstnode *)malloc(sizeof(*root));
        root->item = i;
        root->left = root->right = 0;
        root->size = 1;
        *succ = 1;

        return root;

    } else if (i < root->item) {
        root->left = bstnode_insert(i, root->left, succ);
        if (*succ == 1) root->left->size++;
    } else if (i > root->item) {
        root->right = bstnode_insert(i, root->right, succ);
        if (*succ == 1) root->right->size++;
    }

    if (*succ == 1) root->size++;

    return root;
}

void bst_insert(int i, struct bst *t)
{
    int succ = 0;
    t->root = bstnode_insert(i, t->root, &succ);
}

bool find(int i, struct bstnode *root)
{
    if (root) {
        if (root->item == i) return true;
        else if (root->item > i) {
            return find(i, root->left);
        } else {
            return find(i, root->right);
        }
    }

    return false;
}

bool bst_find(int i, struct bst *t)
{
    return find(i, t->root);
}

int select(int k, struct bstnode *root)
{
    if (root && root->size > k) {
        if (root->left && root->left->size == k) {
            return root->item;
        }
        else if (root->left && root->left->size < k) {
            return select(k-root->left->size-1, root->right);
        } else if (root->left && root->left->size > k) {
            return select(k, root->left);
        } else  {
            if (k == 0) return root->item;
            else return select(k-1, root->right);
        }
    }

    return 0;
}

int bst_select(int k, struct bst *t)
{
    return select(k, t->root);
}

struct bstnode* findMinimum(struct bstnode *t) {
    if (t == NULL) {
        return t;
    }
    if (t->left == NULL) {
        return t;
    }
    return findMinimum(t->left);
}

struct bstnode *Remove(int i, struct bstnode *t)
{
    struct bstnode * tmp;

    if (t == NULL) {
        return t;
    }

    if (i < t->item) {
        t->left = Remove(i, t->left);
    } else {
        if (i > t->item) {
            t->right = Remove(i, t->right);
        } else {
            if (t->left && t->right) {
                tmp = findMinimum(t->right);
                t->item = tmp->item;
                t->right = Remove(t->item, t->right);
            } else {
                tmp = t;
                if (t->left == NULL) {
                    t = t->right;
                } else {
                    if (t->right == NULL) {
                        t = t->left;
                    }
                    free(tmp);
                }
            }
        }
    }

    return t;
    
}

void bst_remove(int i, struct bst *t)
{
    t->root = Remove(i, t->root);
}

int getCount(struct bstnode *root, int low, int high)
{
    if (!root) return 0;
 
    if (root->item == high && root->item == low)
        return 1;
 
    if (root->item <= high && root->item >= low)
         return 1 + getCount(root->left, low, high) +
                    getCount(root->right, low, high);
    else if (root->item < low)
         return getCount(root->right, low, high);
    else return getCount(root->left, low, high);
}


int bst_range(int start, int end, struct bst *t)
{
    return getCount(t->root, start, end);
}

void pre_order(struct bstnode *root)
{
    if (!root) return;
    printf("%d,", root->item);
    pre_order(root->left);
    pre_order(root->right);
}

void in_order(struct bstnode *root)
{
    if (!root) return;
    pre_order(root->left);
    printf("%d,", root->item);
    pre_order(root->right);
}

void post_order(struct bstnode *root)
{
    if (!root) return;
    pre_order(root->left);
    pre_order(root->right);
    printf("%d,", root->item);
}


void bst_print(int o, struct bst *t)
{
    if (!t->root) {
        printf("[empty]\n");
    } else {
        printf("[");
        if (o == 0) pre_order(t->root);
        else if (o == 1) in_order(t->root);
        else post_order(t->root);
        printf("\b");
        printf("]\n");
    }
}

int idx = 0;

void build_array(int *a, struct bstnode *root)
{
    if (root == NULL) return;

    build_array(a, root->left);
    a[idx++] = root->item;
    build_array(a, root->right);
}

int *bst_to_sorted_array(struct bst *t)
{
    int *a = (int *) malloc(t->root->size * sizeof(int));

    idx = 0;
    build_array(a, t->root);

    return a;
}

struct bstnode *bt_insert(int *a, int begin, int end)
{
    if (begin > end) return NULL;

    int i = 0;
    int mid = (begin+end)/2;
    
    struct bstnode *root = (struct bstnode *)malloc(sizeof(struct bstnode));
    root->item = a[mid];
    root->size = end-begin+1;

    root->left = bt_insert(a, begin, mid-1);
    root->right = bt_insert(a, mid+1, end);

    return root;
}

struct bst *sorted_array_to_bst(int *a, int len)
{
    struct bst *t = bst_create();

    t->root = bt_insert(a, 0, len-1);

    return t;
}

void bst_rebalance(struct bst *t)
{
    int *a = bst_to_sorted_array(t);
    int len = t->root->size;
    free_node(t->root);
    t->root = bt_insert(a, 0, len-1);
    free(a);
}
