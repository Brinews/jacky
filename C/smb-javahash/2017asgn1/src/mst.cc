#include <iostream>  
using namespace std;  

/**** MST solution with heap ****/

const int MAXV = 10001, MAXE = 100001, INF = (~0u)>>2;  

struct edge{  
    int t, w, next;  
}es[MAXE * 2];  

int h[MAXV], cnt, n, m, heap[MAXV], size, pos[MAXV], dist[MAXV];  

void addedge(int x, int y, int z)  
{  
    es[++cnt].t = y;  
    es[cnt].next = h[x];  
    es[cnt].w = z;  
    h[x] = cnt;  
}  

void heapup(int k)  
{  
    while(k > 1){  
        if(dist[heap[k>>1]] > dist[heap[k]]){    
            swap(pos[heap[k>>1]], pos[heap[k]]);  
            swap(heap[k>>1], heap[k]);  
            k>>=1;  
        }else  
            break;  
    }  
}  

void heapdown(int k)  
{  
    while((k<<1) <= size){  
        int j;  
        if((k<<1) == size || dist[heap[(k<<1)]] < dist[heap[(k<<1)+1]])  
            j = (k<<1);  
        else  
            j = (k<<1) + 1;  
        if(dist[heap[k]] > dist[heap[j]]){  
            swap(pos[heap[k]], pos[heap[j]]);  
            swap(heap[k], heap[j]);  
            k=j;  
        }else  
            break;  
    }  
}  
void push(int v, int d)  
{  
    dist[v] = d;  
    heap[++size] = v;  
    pos[v] = size;  
    heapup(size);  
}  
int pop()  
{  
    int ret = heap[1];  
    swap(pos[heap[size]], pos[heap[1]]);  
    swap(heap[size], heap[1]);  
    size--;  
    heapdown(1);  
    return ret;  
}  
int prim()  
{  
    int mst = 0, i, p;  
    push(1, 0);  
    for(i=2; i<=n; i++)  
        push(i, INF);  
    for(i=1; i<=n; i++){  
        int t = pop();  
        mst += dist[t];  
        pos[t] = -1;  
        for(p = h[t]; p; p = es[p].next){  
            int dst = es[p].t;  
            if(pos[dst] != -1 && dist[dst] > es[p].w){  
                dist[dst] = es[p].w;  
                heapup(pos[dst]);  
                heapdown(pos[dst]);  
            }  
        }  
    }  
    return mst;  
}  

/**** simplify graph ******/

#define maxNum 1001 
int visited[maxNum];
int label[maxNum];
  
typedef struct  
{  
    float e[maxNum][maxNum];
    int E[maxNum][maxNum];
    int vNum; 
    int eNum;
    int aNum; // actual edge
}graph;  
  
void createGraph(graph *g);
void DFS(graph *g);
  
void dfs(graph *g,int i, int group)  
{  
    visited[i]=1; // mark visited
    label[i] = group; // mark group

    for(int j=0;j<g->vNum;j++)  
    {  
        if(g->E[i][j] == 1 && visited[j]==0) {
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
            dfs(g, i, start);  
        }
        start++;
    }
}
  
void createGraph(graph *g)
{  
    cin>>g->vNum;  
    cin>>g->eNum;  

    int i,j,k;  
    double w;
  
    // init
    for(i=0;i<g->vNum;i++)  
        for(j=0;j<g->vNum;j++) {
            g->e[i][j]=0;  
            g->E[i][j] = 0;
        }
  
    for(k=0;k<g->eNum;k++)  
    {
        cin>>i>>j>>w;  
        g->e[i][j]=w;  
        g->e[j][i]=w;  
    } 

    cin >> g->aNum;
    for (k = 0; k < g->aNum; k++) {
        cin >> i >> j;
        g->E[i][j] = 1;
        g->E[j][i] = 1;
    }
}  

int main()  
{  
    int n, m;

    graph g;  

    createGraph(&g);  
    DFS(&g);  

    cin>>n>>m;  
    for(int i=1; i<=m; i++){  
        int x, y, z;  
        cin>>x>>y>>z;  
        addedge(x, y, z);  
        addedge(y, x, z);  
    }  

    cout<<prim()<<endl;  

    return 0;  
}  
