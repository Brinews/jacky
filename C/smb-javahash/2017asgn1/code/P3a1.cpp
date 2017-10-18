#include <stdio.h>
#include <malloc.h>
#include <iostream>
#include <queue>

using namespace std;

typedef struct BiNode{
    double data;
    double max;
    struct BiNode *lch;
    struct BiNode *rch;
}BiNode,*BiTree;

void CreateTree(BiTree &T, int total)
{
    double value;
    int cur = 0;
    int two = 0;

    BiTree temp, parent = NULL;
    queue<BiTree> Q;

    cur++;
    scanf("%lf", &value);

    temp = (BiNode*) malloc(sizeof(BiNode));
    temp->data = value;
    temp->lch = NULL;
    temp->rch = NULL;
    temp->max = 0.0;

    Q.push(temp);
    T = temp; // set root node

    while (cur < total) {
        if (two == 0) {
            parent = Q.front();
            Q.pop();
        }

        cur++;

        scanf("%lf", &value);

        temp = (BiNode*) malloc(sizeof(BiNode));
        temp->data = value;
        temp->lch = NULL;
        temp->rch = NULL;
        temp->max = 0.0;

        Q.push(temp);

        if (two == 0) {
            parent->lch = temp;
        } else {
            parent->rch = temp;
        }

        two = (two+1)%2;
    }
}

void Preorder (BiTree T)
{
    if(T) {
        printf("%lf ",T->data); 

        Preorder(T->lch);
        Preorder(T->rch);
    }
} 

void Postorder (BiTree T)
{
    if(T) {
        Postorder(T->lch);
        Postorder(T->rch);

        printf("%lf ",T->data); 
    }
} 

void destroy (BiTree T)
{
    if (T != NULL) {
        destroy(T->lch);
        destroy(T->rch);

        free(T);
    }
}

bool isLeaf(BiTree t)
{
    return t != NULL && t->lch == NULL && t->rch == NULL;
}

double HouseDP(BiTree T)
{
    if (T == NULL) return 0;
    if (T->max != 0.0) return T->max;
    if (isLeaf(T)) return T->data;
    if (isLeaf(T->lch) && isLeaf(T->rch)) {
        if (T->data > T->lch->data + T->rch->data) return T->data;
        else return T->lch->data + T->rch->data;
    }

    double v1 = 0.0, v2 = 0.0;

    // not select root node
    v1 = HouseDP(T->lch) + HouseDP(T->rch);

    // select root node
    v2 = T->data + HouseDP(T->lch->lch) + HouseDP(T->lch->rch)
        + HouseDP(T->rch->lch) + HouseDP(T->rch->rch);

    if (v1 > v2) {
        T->max = v1;
        return v1;
    } else {
        T->max = v2;
        return v2;
    }
}

int main()
{
    BiTree T;
    int total;

    scanf("%d", &total);
    CreateTree(T, total);

    printf("%.2lf\n", HouseDP(T));

    destroy(T);

    return 0;
}
