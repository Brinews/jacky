#ifndef _GRAPH_
#define _GRAPH_

#include <iostream>
#include <fstream>

using namespace std;

class BitMatrixGraph {
protected:
    unsigned char **bitAdjacencyMatrix;
    int n; // number of nodes in the graph
public:
    BitMatrixGraph();
    BitMatrixGraph(int numNodes);
    BitMatrixGraph(const BitMatrixGraph& otherGraph);
    //overloaded = operator
    BitMatrixGraph& operator=(const BitMatrixGraph& otherGraph);
    void addEdge(int u, int v);
    bool isAnEdge(int u, int v) const;
    int getSize() const { return n; }
    //other that you want and will be useful
    //ostream operator
    friend ostream& operator<<(ostream& out, BitMatrixGraph& graph);
};

#endif
