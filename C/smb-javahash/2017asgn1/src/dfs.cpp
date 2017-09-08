#include<iostream>  
#include<malloc.h>  
using namespace std;  
 
#define maxNum 1001 
int visited[maxNum];
int label[maxNum];
  
typedef struct  
{  
    int e[maxNum][maxNum];
    int vNum; 
    int eNum;
}graph;  
  
void createGraph(graph *g);
void DFS(graph *g);
  
void dfs(graph *g,int i, int group)  
{  
    visited[i]=1; // mark visited
    label[i] = group; // mark group

    for(int j=0;j<g->vNum;j++)  
    {  
        if(g->e[i][j]!=0 && visited[j]==0) {
            dfs(g,j,group); 
        }
    }  
}  
  
void DFS(graph *g)  
{  
    int i;  
    for(i=0;i<g->vNum;i++) {
        visited[i]=0;  
        label[i] = 0;
    }

    int start = 1;

    for(i=0;i<g->vNum;i++)  {
        if(visited[i]==0) {
            dfs(g,i, start);  
        }
        start++;
    }
}  
  
void createGraph(graph *g)
{  
    cin>>g->vNum;  
    cin>>g->eNum;  

    int i,j;  
    double w;
  
    // init
    for(i=0;i<g->vNum;i++)  
        for(j=0;j<g->vNum;j++)  
            g->e[i][j]=0;  
  
    for(int k=0;k<g->eNum;k++)  
    {
        cin>>i>>j>>w;  
        g->e[i][j]=1;  
        g->e[j][i]=1;  
    } 
}  

void query(int qnum)
{
    int i;

    int from, to;
    for (i = 0; i < qnum; i++) {
        cin >> from >> to;
        if (label[from] != label[to]) {
            cout << 0 << endl;
        } else {
            cout << 1 << endl;
        }
    }
}
 
int main()  
{  
    graph *g;  

    g=(graph*)malloc(sizeof(graph));  

    createGraph(g);  

    DFS(g);  

    int qnum;  
    cin>>qnum;  

    query(qnum);

    free(g);

    return 0;  
}  
  
