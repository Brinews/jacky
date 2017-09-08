#include <iostream>  
#include <iomanip>

using namespace std;  
const int MAXV = 10001, MAXE = 100001, INF = 10000;  
struct edge{  
    int t, next;  
    float w;
}es[MAXE * 2];  

int h[MAXV] = {0}, cnt = 0, n, m, heap[MAXV], size = 0, pos[MAXV];
float dist[MAXV];  
int aN = 0;

float edges[MAXV][MAXV];

void addedge(int x, int y, float z)  
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

void push(int v, float d)  
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

float prim()  
{  
    float mst = 0;
    int i, p;  
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

int main()  
{  
    int i, j;
    int x, y;
    float z;  

    cin>>n>>m;  

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            edges[i][j] = -1;
        }
    }

    float total_cost = 0.0;

    for(i=1; i<=m; i++){
        cin>>x>>y>>z;  

        edges[x][y] = z;
        edges[y][x] = z;
    } 

    cin >> aN;
    for (i = 1; i <= aN; i++) {
        cin >> x >> y;

        total_cost += edges[x][y];

        edges[x][y] = 0;
        edges[y][x] = 0;
        /*
        if (x > y) {
            int tm = y;
            y = x;
            x = tm;
        }

        for (int k = 0; k < n; k++) {
            if (k != x) {
                if (edges[x][k] == -1 && edges[y][k] != -1) {
                    edges[x][k] = edges[y][k];
                } else if (edges[x][k] != -1 && edges[y][k] != -1
                        && edges[y][k] < edges[x][k]) {
                    edges[x][k] = edges[y][k];
                }
            }

            edges[y][k] = -1;
        }

        for (int k = 0; k < n; k++) {
            if (k != x) {
                if (edges[k][x] == -1 && edges[k][y] != -1) {
                    edges[k][x] = edges[k][y];
                } else if (edges[k][x] != -1 && edges[k][y] != -1
                        && edges[k][y] < edges[k][x]) {
                    edges[k][x] = edges[k][y];
                }
            }

            edges[k][y] = -1;
        }
        */
    }

    /*
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << edges[i][j] << " ";
        }
        cout << endl;
    }
    */

    for (i = 0; i < n; i++) {
        for (j = i+1; j < n; j++) {
            x = i+1;
            y = j+1;
            z = edges[i][j];

            if (z != -1) {
                addedge(x, y, z);  
                addedge(y, x, z);  
            }
        }
    }

    total_cost += prim();

    cout << setiosflags(ios::fixed) << setprecision(2) << total_cost <<endl;  

    return 0;  
}  
