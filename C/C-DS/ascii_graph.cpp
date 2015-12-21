#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

#define INF 0x1fffffff

typedef struct node* Node;
typedef struct edge* Edge;

struct edge {
	Node pnode;
	int weight;
	int direction; //0: out, 1: in
};

struct node {
	int pno;
	int edgesNum; // max 4 edges
	int outNum;
	int inNum;
	struct edge edges[4];
	int x,y;
};

typedef int Line[50];

vector<Node> nodeList; /* all nodes */
Line lineList[300]; /* used for graph */

int layer = 0;

Node getNode(int nodeId)
{
	for (int i = 0; i < nodeList.size(); i++) {
		if (nodeList[i]->pno == nodeId) return nodeList[i];
	}

	Node pnode = new node;
	pnode->pno = nodeId;
	pnode->edgesNum = pnode->outNum = pnode->inNum = 0;
	pnode->x = pnode->y = 0;

	nodeList.push_back(pnode);

	return pnode;
}

void readGraph()
{
	string buf;
	int fromNode, toNode, weight;

	ifstream fin("graph.dat");

	while (getline(fin, buf)) {
		//cout << buf << endl;

		stringstream ss;
		ss << buf;
		ss >> fromNode;

		Node pnode = getNode(fromNode);

		while (ss >> toNode >> weight) {
			Node tnode = getNode(toNode);

			Edge e = &(pnode->edges[pnode->edgesNum++]);
			e->pnode = tnode;
			e->weight = weight;
			e->direction = 0;
			pnode->outNum++;

			Edge E = &(tnode->edges[tnode->edgesNum++]);
			E->pnode = pnode;
			E->weight = weight;
			E->direction = 1;
			tnode->inNum++;
		}
	}

	fin.close();
}

void sortEdgeList(Node pnode)
{
	int i, j;
	for (i = 0; i < pnode->edgesNum; i++) {
		for (j = i+1; j < pnode->edgesNum; j++) {
			if (pnode->edges[i].pnode->edgesNum >
					pnode->edges[j].pnode->edgesNum){
				struct edge temp = pnode->edges[i];
				pnode->edges[i] = pnode->edges[j];
				pnode->edges[j] = temp;
			}
		}
	}
}

void sortNodeList()
{
	int i, j;

	for (i = 0; i < nodeList.size(); i++) {
		for (j = i+1; j < nodeList.size(); j++) {
			if (nodeList[i]->edgesNum < nodeList[j]->edgesNum) {
				Node temp = nodeList[i];
				nodeList[i] = nodeList[j];
				nodeList[j]  = temp;
			}
		}
	}

	for (i = 0; i < nodeList.size(); i++) {
		//cout<<"No:"<< nodeList[i]->pno<<"Edges:"<<nodeList[i]->edgesNum;
		sortEdgeList(nodeList[i]);
	}
}

void addLayer()
{
	int i, j;
	for (i = 0; i < 30; i++) {
		for (j = 0; j < 50; j++) lineList[i+layer*30][j] = ' ';
	}

	layer++;
}

void addWeight(int x, int y, int weight)
{
	char id[4];
	sprintf(id, "%d", weight);

	int i;
	for (i = 0; i < strlen(id); i++) {
		lineList[x][y+i] = id[i];
	}
}

void addNodeID(Node pnode)
{
	char id[3];
	sprintf(id, "%d", pnode->pno);

	int i;
	lineList[pnode->x][pnode->y-1] = '[';
	for (i = 0; i < strlen(id); i++) {
		lineList[pnode->x][pnode->y+i] =  id[i];
	}
	lineList[pnode->x][pnode->y+i] = ']';
}

int leftDirection(Node tnode)
{
	int ret = 0;

	int y = tnode->y+1;

	if (lineList[tnode->x+1][tnode->y] == ' ') ret += (1<<0); //below
	if (lineList[tnode->x-1][tnode->y] == ' ') ret += (1<<1); //above

	while (lineList[tnode->x][y] >= '0' && lineList[tnode->x][y] <= '9')
		y++;
	if (lineList[tnode->x][y+1] == ' ') ret += (1<<2); //right

	if (lineList[tnode->x][tnode->y-2] == ' ') ret += (1<<3); //left

	return ret;
}

int right(Node pnode)
{
	int ret = pnode->y;

	while (lineList[pnode->x][ret++] != ']') ;

	return ret;
}

void linkNodes(Node pnode, Node tnode, int weight)
{
	int i, j, k;
	int d = leftDirection(tnode);

	//cout << pnode->pno << "," << tnode->pno << endl;

	if (pnode->y == tnode->y) {
		if (pnode->x < tnode->x) {
			for (i = pnode->x+1; i < tnode->x - 1; i++)
				lineList[i][pnode->y] = '|';

			lineList[i][pnode->y] = 'V';
			j = (pnode->x + tnode->x) /2;
			addWeight(j, pnode->y, weight);
		}
		else {
			for (i = pnode->x - 1; i > tnode->x + 1; i--)
				lineList[i][pnode->y] = '|';
			lineList[i][pnode->y] = '^';
			j = (pnode->x + tnode->x)/2;
			addWeight(j, pnode->y, weight);
		}
	} else if (pnode->x == tnode->x) {
		if (pnode->y < tnode->y) {
			if (d&8) {
				for (i = right(pnode); i < tnode->y - 2; i++)
					lineList[pnode->x][i] = '-';
				lineList[pnode->x][i] = '>';
				j = (pnode->y + tnode->y)/2;
				addWeight(pnode->x, j, weight);
			} else if (d&2) {
				for (i = pnode->x-1; i > pnode->x - 4; i--)
					lineList[i][pnode->y] = '|';
				lineList[i][pnode->y] = '+';
				for (j = pnode->y+1; j < right(tnode)+3; j++) {
					if (lineList[i][j] != ' ') lineList[i][j] = '=';
					else lineList[i][j] = '-';
				}
				lineList[i][j] = '+';
				for (i = i+1; i < tnode->x; i++)
					lineList[i][j] = '|';
				addWeight(i-2, j, weight);
				lineList[i][j] = '+';

				for (j = j-1; j > right(tnode); j--) {
					lineList[i][j] = '-';
				}
				lineList[i][j] = '<';
			}
		}
		else {
			if (d&4) {
				for (i = pnode->y - 2; i > right(tnode); i--)
					lineList[pnode->x][i] = '-';
				lineList[pnode->x][i] = '<';
				j = (pnode->y + tnode->y)/2;
				addWeight(pnode->x, j, weight);
			} else if (d&8) {
				for (i = pnode->x - 1; i > pnode->x-8; i--) {
					if (lineList[i][pnode->y] != ' ') 
						lineList[i][pnode->y] = '=';
					else
						lineList[i][pnode->y] = '|';
				}
				lineList[i][pnode->y] = '+';
				for (j = pnode->y-1; j > tnode->y-5; j--) {
					if (lineList[i][j] != ' ') lineList[i][j] = '=';
					else lineList[i][j] = '-';
				}
				k = (pnode->y + tnode->y)/2;
				addWeight(i, k, weight);
				lineList[i][j] = '+';
				for (i = i + 1 ; i < tnode->x; i++) {
					lineList[i][j] = '|';
				}
				lineList[i][j] = '+';
				for (j = j+1; j < tnode->y-2; j++) {
					lineList[i][j] = '-';
				}
				lineList[i][j] = '>';
			} else if (d&2) {
				for (j = right(pnode); j < right(pnode)+3; j++)
					lineList[pnode->x][j] = '-';
				lineList[pnode->x][j] = '+';
				for (i = pnode->x-1; i > pnode->x-6; i--)
					lineList[i][j] = '|';
				lineList[i][j] = '+';
				for (j = j-1; j > tnode->y; j--) {
					if (lineList[i][j] != ' ') lineList[i][j] = '=';
					else lineList[i][j] = '-';
				}
				lineList[i][j] = '+';
				addWeight(i, (pnode->y+tnode->y)/2, weight);
				for (i = i+1; i < tnode->x-1; i++) {
					if (lineList[i][j] != ' ') lineList[i][j] = '=';
					else lineList[i][j] = '|';
				}
				lineList[i][j] = 'V';
			}
		}
	} else if (pnode->x < tnode->x && pnode->y > tnode->y) {
		// right-up
		int px = tnode->x;
		int py = pnode->y;

		for (i = pnode->x + 1; i < px; i++)
			lineList[i][py] = '|';
		lineList[i][py] = '+';

		for (i = py-1; i > right(tnode); i--)
			lineList[px][i] = '-';
		lineList[px][i] = '<';

		j = (py + tnode->y)/2;
		addWeight(px, j, weight);
	} else if (pnode->x < tnode->x && pnode->y < tnode->y) {
		//left-up
		int py = pnode->y;
		int px = tnode->x;

		for (i = pnode->x + 1; i < px; i++)
			lineList[i][py] = '|';
		lineList[i][py] = '+';

		for (i = py+1; i < tnode->y-2; i++)
			lineList[px][i] = '-';
		lineList[px][i] = '>';

		j = (py + tnode->y)/2;
		addWeight(px, j, weight);
	}
}

void arrangeGraph()
{
	int i, j;
	for (i = 0; i < nodeList.size(); i++) {
		Node pnode = nodeList[i];
		if (pnode->x == 0 && pnode->y == 0) {
			//get a position
			addLayer();
			pnode->x = (layer-1)*30 + 12;
			pnode->y = 25;

			addNodeID(pnode);
		}

		for (j = 0; j < pnode->edgesNum; j++) {
			Node tnode = pnode->edges[j].pnode;

			if (tnode->x == 0 && tnode->y == 0) {
				switch(j) {
					case 0: //down
						tnode->x = (layer-1)*30 + 20;
						tnode->y = 25;
						break;
					case 1: //left
						tnode->x = (layer-1)*30 + 12;
						tnode->y = 10;
						break;
					case 2: // right
						tnode->x = (layer-1)*30 + 12;
						tnode->y = 40;
						break;
					case 3: // up
						tnode->x = (layer-1)*30 + 3;
						tnode->y = 25;
						break;
				}

				addNodeID(tnode);
			}

			if (pnode->edges[j].direction == 0)
				linkNodes(pnode, tnode, pnode->edges[j].weight);
		}
	}
}

void displayGraph()
{
	int i,j;
	for (i = 0; i < layer*30; i++) {
		for (j = 0; j < 50; j++) {
			printf("%c", lineList[i][j]);
		}
		printf("\n");
	}
}

void freeGraph()
{
	int i;
	for (i = 0; i < nodeList.size(); i++)
		delete nodeList[i];
}

int edgeWeight(int from, int to)
{
	int i;
	if (from == to) return 0;

	Node fnode = nodeList[from];
	Node tnode = nodeList[to];

	for (i = 0; i < fnode->edgesNum; i++) {
		if (fnode->edges[i].direction == 0 &&
				fnode->edges[i].pnode->pno == tnode->pno)
			return fnode->edges[i].weight;
	}

	return INF;
}

int M[100][100];

void shortestPath()
{
	int len = nodeList.size();
	int i,j,k;
	for (i = 0; i < len; i++) {
		for (j = 0; j < len; j++) {
			M[i][j] = edgeWeight(i, j);
		}
	}

	/* floyd */
	for (k = 0; k < len; k++)
		for (i = 0; i < len; i++)
			for (j = 0; j < len; j++) {
				if (M[i][j] > M[i][k] + M[k][j])
					M[i][j] = M[i][k] + M[k][j];
			}
}

void printPath()
{
	int i, j;
	int len = nodeList.size();
	printf("ShorestPath\n");
	printf("--------------\n");
	for (i = 0; i < len; i++) {
		if (i == 0)  {
			for (j = 0; j < len; j++) {
				if (j == 0) { printf("    %4d", nodeList[j]->pno);}
				else printf("%4d", nodeList[j]->pno);
			}
			printf("\n");
		}
		for (j = 0; j < len; j++) {
			if (j == 0) {
				printf("%4d", nodeList[i]->pno);
			}

			if (M[i][j] < INF && M[i][j] != 0)
				printf("%4d", M[i][j]);
			else
				printf("   *");
		}
		printf("\n");
	}
}

int main(int argc, char **argv)
{
	readGraph();
	sortNodeList();
	arrangeGraph();
	displayGraph();

	shortestPath();
	printPath();

	freeGraph();
	system("pause");

	return 0;
}
