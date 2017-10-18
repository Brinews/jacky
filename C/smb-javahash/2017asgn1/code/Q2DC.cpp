#include <stdio.h>
#include <stdlib.h>

typedef struct point {
    double x;
    double y;
    int impl;
} Point;

void MergeByX(Point sourceArr[],Point tempArr[], 
        int startIndex, int midIndex, int endIndex)
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

void MergeByY(Point sourceArr[],Point tempArr[], 
        int startIndex, int midIndex, int endIndex)
{
    int i = startIndex, j=midIndex+1, k = startIndex;
    while(i!=midIndex+1 && j!=endIndex+1)
    {
        if (sourceArr[i].y > sourceArr[j].y)
            tempArr[k++] = sourceArr[j++];
        else if (sourceArr[i].y == sourceArr[j].y
                && sourceArr[i].x > sourceArr[j].x) {
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


void MergeSort(Point sourceArr[], Point tempArr[], 
        int startIndex, int endIndex, 
        void (*merge) (Point p1[], Point p2[], int s, int m, int e))
{
    int midIndex;
    if(startIndex < endIndex)
    {
        midIndex = (startIndex + endIndex) / 2;
        MergeSort(sourceArr, tempArr, startIndex, midIndex, merge);
        MergeSort(sourceArr, tempArr, midIndex+1, endIndex, merge);
        merge(sourceArr, tempArr, startIndex, midIndex, endIndex);
    }
}

void ImplMerge(Point *p1, Point *p2, int start, int mid, int end)
{
    int i = start, j = mid+1, k = start, counter = 0;

    while (i != mid+1 || j != end+1) {
        if (i >= mid+1 || (j < end+1 && p1[j].y <= p1[i].y)) {
            p1[j].impl += counter;
            p2[k++] = p1[j];
            //printf("%f,%f - ", p1[j].x, p1[j].y);
            //printf("%d\n", p1[j].impl);
            j += 1;
        } else {
            //printf("%f,%f - ", p1[i].x, p1[i].y);
            //printf("%d\n", p1[i].impl);
            p2[k++] = p1[i];
            counter += 1;
            i += 1;
        }
    }

    for (i = start; i <= end; i++) // copy back
        p1[i] = p2[i];
}

void DivideConquer(Point p1[], Point p2[], int start, int end)
{
    int mid;
    if (start < end) {
        mid = (start+end)/2;

        DivideConquer(p1, p2, start, mid);
        DivideConquer(p1, p2, mid+1, end);

        ImplMerge(p1, p2, start, mid, end);
    }
}

int main()
{
    int N;

    scanf("%d", &N);

    Point *P, *Q;
    Point *p1, *p2;

    P = (Point *)malloc(sizeof(Point)*N);
    Q = (Point *)malloc(sizeof(Point)*N);

    p1 = P;

    for (int i = 0; i < N; i++) {
        scanf("%lf%lf", &p1[i].x, &p1[i].y);
        p1[i].impl = 0;
    }

    MergeSort(P, Q, 0, N-1, MergeByX); // 1. sort by x1-coordinate

    DivideConquer(P, Q, 0, N-1); // 2. divide and merge two sub-set with impl
    MergeSort(P, Q, 0, N-1, MergeByX); // 3. sort by x1-coordinate

    for (int i = 0; i < N; i++) {
        //printf("%lf,%lf - ", P[i].x, P[i].y);
        printf("%d\n", P[i].impl);
    }

    free(P);
    free(Q);

    return 0;
}
