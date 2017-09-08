#include <stdio.h>
#include <stdlib.h>

typedef struct point {
    double x;
    double y;
    int impl;
} Point;

void Merge(Point sourceArr[],Point tempArr[], int startIndex, int midIndex, int endIndex,int n)
{
    int i = startIndex, j=midIndex+1, k = startIndex;
    while(i!=midIndex+1 && j!=endIndex+1)
    {
        if(sourceArr[i].x > sourceArr[j].x)
            tempArr[k++] = sourceArr[j++];
        else if (sourceArr[i].x == sourceArr[j].x 
                && sourceArr[i].y > sourceArr[j].y) {
            tempArr[k++] = sourceArr[j++];
        } else {
            tempArr[k++] = sourceArr[i++];
        }
    }

    while(i != midIndex+1)
        tempArr[k++] = sourceArr[i++];
    while(j != endIndex+1)
        tempArr[k++] = sourceArr[j++];

    for(i=startIndex; i<=endIndex; i++)
        sourceArr[i] = tempArr[i];
}

void MergeSort(Point sourceArr[], Point tempArr[], int startIndex, int endIndex,int n)
{
    int midIndex;
    if(startIndex < endIndex)
    {
        midIndex = (startIndex + endIndex) / 2;
        MergeSort(sourceArr, tempArr, startIndex, midIndex,n);
        MergeSort(sourceArr, tempArr, midIndex+1, endIndex,n);
        Merge(sourceArr, tempArr, startIndex, midIndex, endIndex,n);
    }
}

void ImplMerge(Point *p1, Point *p2, int N1, int N2)
{
    int i, j, counter;

    i = j = counter = 0;

    while (i+j < N1+N2) {
        if (i >= N1 || (j < N2 && p2[j].y <= p1[i].y)) {
            p2[j].impl += counter;
            //printf("%f,%f - ", p2[j].x, p2[j].y);
            //printf("%d\n", p2[j].impl);
            j += 1;
        } else {
            //printf("%f,%f - ", p1[i].x, p1[i].y);
            //printf("%d\n", p1[i].impl);
            counter += 1;
            i += 1;
        }
    }
}

int main()
{
    int N, N1, N2;

    scanf("%d", &N);
    scanf("%d", &N1);

    Point *P, *Q;
    Point *p1, *p2;

    P = (Point *)malloc(sizeof(Point)*N);
    Q = (Point *)malloc(sizeof(Point)*N);

    p1 = P;
    p2 = P + N1;

    for (int i = 0; i < N1; i++) {
        scanf("%lf%lf%d", &p1[i].x, &p1[i].y, &p1[i].impl);
    }

    scanf("%d", &N2);

    for (int i = 0; i < N2; i++) {
        scanf("%lf%lf%d", &p2[i].x, &p2[i].y, &p2[i].impl);
    }

    ImplMerge(p1, p2, N1, N2);

    MergeSort(P, Q, 0, N-1, N);

    for (int i = 0; i < N; i++) {
        printf("%d\n", P[i].impl);
    }

    free(P);
    free(Q);

    return 0;
}
