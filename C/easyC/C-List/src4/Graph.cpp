#include "Graph.h"

BitMatrixGraph::BitMatrixGraph()
{
    bitAdjacencyMatrix = NULL;
    n = 0;
}

BitMatrixGraph::BitMatrixGraph(int numNodes)
{
    n = numNodes;

    int size = 0;
    if (numNodes%8 == 0)
        size = numNodes/8;
    else
        size = (int)(numNodes/8)+1;

    bitAdjacencyMatrix = new unsigned char* [numNodes];

    for (int i = 0; i < numNodes; i++) {
        bitAdjacencyMatrix[i] = new unsigned char[size];
    }

    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < size; j++) {
            bitAdjacencyMatrix[i][j] = 0x00 << 8;
        }
    }
}

BitMatrixGraph::BitMatrixGraph(const BitMatrixGraph &otherGraph)
{
    if (this != &otherGraph) {
        this->n = otherGraph.n;

        int size = 0;
        int numNodes = otherGraph.n;

        if (numNodes%8 == 0)
            size = numNodes/8;
        else
            size = (int)(numNodes/8)+1;

        bitAdjacencyMatrix = new unsigned char* [numNodes];

        for (int i = 0; i < numNodes; i++) {
            bitAdjacencyMatrix[i] = new unsigned char[size];
        }

        for (int i = 0; i < numNodes; i++) {
            for (int j = 0; j < size; j++) {
                bitAdjacencyMatrix[i][j] = otherGraph.bitAdjacencyMatrix[i][j];
            }
        }
    }
}

BitMatrixGraph& BitMatrixGraph::operator=(const BitMatrixGraph &otherGraph)
{
    if (this != &otherGraph) {
        this->n = otherGraph.n;

        int size = 0;
        int numNodes = otherGraph.n;

        if (numNodes%8 == 0)
            size = numNodes/8;
        else
            size = (int)(numNodes/8)+1;

        bitAdjacencyMatrix = new unsigned char* [numNodes];

        for (int i = 0; i < numNodes; i++) {
            bitAdjacencyMatrix[i] = new unsigned char[size];
        }

        for (int i = 0; i < numNodes; i++) {
            for (int j = 0; j < size; j++) {
                bitAdjacencyMatrix[i][j] = otherGraph.bitAdjacencyMatrix[i][j];
            }
        }
    }
}

void printBits(ostream& out, unsigned char s) 
{
    //print the bit pattern
    for (int i = 0; i < 8; i++) {
        if (s&(1<<(7-i)))
            out << 1;
        else
            out << 0;
    }
}

ostream& operator<<(ostream& out, BitMatrixGraph &graph)
{
    int size = 0;

    if (graph.n%8 == 0) size = graph.n/8;
    else size = (int)(graph.n/8)+1;

    for (int i = 0; i < graph.n; i++) {
        for (int j = 0; j < size; j++) {
            printBits(out, graph.bitAdjacencyMatrix[i][j]);
            out << endl;
        }
    }

    return out;
}

void BitMatrixGraph::addEdge(int u, int v)
{
    int idx = v/8;
    int off_set = v%8;

    //cout << "u=" << u << ",idx="<<idx<<",off="<<off_set<<endl;

    bitAdjacencyMatrix[u][idx] |= (0x01<<(7-off_set));

    int idx1 = u/8;
    int off_set1 = u%8;

    bitAdjacencyMatrix[v][idx1] |= 0x01 <<(7-off_set1);
}

bool BitMatrixGraph::isAnEdge(int u, int v) const
{
    if (u < 0 || u >= n || v < 0 || v >= n) return false;

    int idx = v/8;
    int off_set = v%8;

    unsigned ret = bitAdjacencyMatrix[u][idx] & (0x01 <<(7-off_set));
    if (ret == 0) return false;

    return true;
}
