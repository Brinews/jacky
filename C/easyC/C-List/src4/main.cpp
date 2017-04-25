#include "Graph.h"

#include <iostream>
#include <queue>

using namespace std;

int* BFS(const BitMatrixGraph &graph, int startp)
{
    int *parent = new int[graph.getSize()];
    int *visit = new int[graph.getSize()];

    for (int i = 0; i < graph.getSize(); i++) {
        if (i == startp) parent[i] = -1;
        else parent[i] = 0;

        visit[i] = 0; // not visit
    }

    queue<int> Q;
    Q.push(startp);
    visit[startp] = 1;

    while (!Q.empty()) {
        int v = Q.front(); Q.pop();

        for (int i = 0; i < graph.getSize(); i++) {
            if (graph.isAnEdge(v, i)) {
                if (visit[i] == 0) {
                    parent[i] = v;
                    visit[i] = 1;
                    Q.push(i);
                }
            }
        }
    }

    delete []visit;

    return parent;
}

void DFS_recur(const BitMatrixGraph &G, int u, int *parent)
{
    for (int i = 0; i < G.getSize(); i++) {
        if (G.isAnEdge(u, i) && parent[i] == 0) {
            parent[i] = u;
            DFS_recur(G, i, parent);
        }
    }
}

int* DFS(const BitMatrixGraph &G, int u)
{
    int *parent = new int[G.getSize()];

    for (int i = 0; i < G.getSize(); i++)
        parent[i] = 0;

    parent[u] = -1;
    DFS_recur(G, u, parent);

    return parent;
}

int main()
{
    //test case

    int n, from, to;

    cout << "Input nodes number: ";
    cin >> n;
    BitMatrixGraph bmg(n);

    cout << "Input edges(i, j), (-1, -1) for end\n";
    while (cin >> from >> to) {
        if (from < 0 || to < 0) break;
        bmg.addEdge(from, to);
    }

    // test output
    cout << "Graph Display:\n";
    cout << bmg << endl;


    // test constructor
    BitMatrixGraph copy(bmg);
    cout << "Graph copy:\n";
    cout << copy << endl;

    // test =
    BitMatrixGraph asgn = bmg;
    cout << "Graph copy:\n";
    cout << asgn << endl;

    int *parent = BFS(bmg, 0);

    cout << "Nodes:\n";
    for (int i = 0; i < bmg.getSize(); i++) {
        cout << i << ", ";
    }

    cout << "\nParents of BFS:\n";
    for (int i = 0; i < bmg.getSize(); i++) {
        cout << parent[i] << ", ";
    }
    cout << endl;

    int *pa1 = DFS(bmg, 0);

    cout << "Nodes:\n";
    for (int i = 0; i < bmg.getSize(); i++) {
        cout << i << ", ";
    }

    cout << "\nParents of DFS:\n";
    for (int i = 0; i < bmg.getSize(); i++) {
        cout << pa1[i] << ", ";
    }
    cout << endl;

    delete parent;
    delete pa1;

    return 0;
}
