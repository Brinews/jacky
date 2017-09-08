#include <stdlib.h>
#include <stdio.h>

void Merge(int sourceArr[],int tempArr[], int startIndex, int midIndex, int endIndex,int n)
{
    int i = startIndex, j=midIndex+1, k = startIndex;
    while(i!=midIndex+1 && j!=endIndex+1)
    {
        if(sourceArr[i] > sourceArr[j])
            tempArr[k++] = sourceArr[j++];
        else
            tempArr[k++] = sourceArr[i++];
    }
    while(i != midIndex+1)
        tempArr[k++] = sourceArr[i++];
    while(j != endIndex+1)
        tempArr[k++] = sourceArr[j++];
    for(i=startIndex; i<=endIndex; i++)
        sourceArr[i] = tempArr[i];
    /*
    for(i=0; i< n - 1; i++) printf("%d ", sourceArr[i]);
    if (i==n-1) printf("%d\n",sourceArr[i]);
    */
}

void MergeSort(int sourceArr[], int tempArr[], int startIndex, int endIndex,int n)
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

void ImplDynamic(int p[], int imp[], int n)
{
    imp[0] = 0;

    for (int i = 1; i < n; i++) {
        if (p[i] == p[i-1]) {
            imp[i] = imp[i-1];
        } else {
            imp[i] = i;
        }
    }
}

int main(void)
{
    int n, t;
    scanf ("%d",&n);
    int *a = new int[n];
    int *b = new int[n];

    for (t= 0 ; t < n; t++) {
        scanf("%d",&a[t]);
    }

    MergeSort(a, b, 0, n-1,n);

    ImplDynamic(a, b, n);

    for (int i = 0; i < n; i++) {
        //printf("%d-", a[i]);
        printf("%d\n", b[i]);
    }

    delete []a;
    delete []b;

    return 0;
}
