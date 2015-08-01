#include<iostream>
#include<cmath>
#include<cstdio>
#include<string.h>
#include <sstream>
#include <fstream>

#include <vector>

#define TYPE double

using namespace std;

const int INF = 0x3fffffff;

struct qNode
{
	int id;
	TYPE k;
	double dist;
	int prevId;

	qNode(int i, TYPE key) : id(i), k(key) {}
};

inline int PARRENT(int x) { return x>>1;}
inline int LEFT(int x) { return x<<1;}
inline int RIGHT(int x) { return (x<<1)|1;}


inline void Swap(qNode &a, qNode &b) { qNode t = a; a = b; b = t;} 

struct node {
	double x;
	double y;
	node(double xx = 0, double yy = 0):x(xx), y(yy){}
};

struct Edge {
	int nodeId; /* destination node */
	double distance;

	Edge(int id, double dis) : nodeId(id), distance(dis) {}
};

vector< vector<Edge> > graph;

vector<node> Point;
vector<qNode> dis;

int stopNum;

double Dis(node a, node b)
{
	return sqrt(1.0*((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)));
}

int HEAPSIZE;

void MinHeapify(vector<qNode> &arr, int i)
{
	int smallest, l = LEFT(i), r = RIGHT(i);

	if(l <= HEAPSIZE && arr[l].k < arr[i].k)
		smallest = l;
	else smallest = i;

	if(r <= HEAPSIZE && arr[r].k < arr[smallest].k)
		smallest = r;

	if(smallest != i)
	{
		Swap(arr[smallest], arr[i]);
		MinHeapify(arr, smallest);
	}

	return;
}

void BuildHeap(vector<qNode> &arr, int size)
{
	HEAPSIZE = size;

	for(int i = size>>1; i > 0; i--)
		MinHeapify(arr, i);

	return;
}

qNode HeapExtractMin(vector<qNode> &arr)
{
	qNode min(0, INF);

	if(HEAPSIZE < 1) {cout<<"Heap Overflow"<<endl;}

	min = arr[1];
	Swap(arr[1], arr[HEAPSIZE]); HEAPSIZE--;

	MinHeapify(arr, 1);

	return min;
}

void HeapDecreaseKey(vector<qNode> &arr, int i, TYPE newk)
{
	if(arr[i].k < newk){cout<<"newk smaller"<<endl; return;}

	arr[i].k = newk;

	while(i > 1 && arr[PARRENT(i)].k > arr[i].k) {
		Swap(arr[PARRENT(i)], arr[i]);
		i = PARRENT(i);
	}

	return;
}

void printShortestPath(int start, int end, int size)
{
	int p = end;

	cout << "Path: ";
	cout << p << " <- " ;

	while (p != start) {
		for (int i = 1; i < size; i++) {

			if (dis[i].id == p) {
				if (dis[i].prevId == start) {
					p = start;
					break;
				}

				cout << dis[i].prevId << " <- ";
				p = dis[i].prevId;

				break;
			}
		}
	}

	cout << start << endl;
}

double DijkstraAstar(int x, int end, int size, int &extends)
{
	dis.resize(size, qNode(0,INF));

	int i, j;

	for(i = 0; i < size; i++) {
		dis[i].id = i;
		dis[i].k = INF;
		dis[i].dist = INF;
	}

	dis[x].k = Dis(Point[x], Point[end]); //INITIALIZE
	dis[x].id = x;
	dis[x].dist = 0;
	dis[x].prevId = x;

	BuildHeap(dis, size-1);

	for(i = 1; i < size; i++)
	{
		int minVex;
		double minValue;

		qNode tmp = HeapExtractMin(dis);

		if (tmp.id == end) {
			printShortestPath(x, end, size);
			return tmp.dist;
		}

		minVex = tmp.id, minValue = tmp.dist;

		for (int m = 0; m < graph[minVex].size(); m++) {
			double adis = graph[minVex][m].distance;
			int nodeJ = graph[minVex][m].nodeId;
			int j = -1;

			/* find node in heap */
			for (int n = 1; n < HEAPSIZE; n++) {
				if (dis[n].id == nodeJ)
					j = n;
			}

			if(dis[j].dist > minValue+adis) {
				dis[j].prevId = minVex; /* record previous node id */

				dis[j].dist = minValue + adis;

				extends++;
				//cout << "Extend Node: " << dis[j].id << endl;
				
				HeapDecreaseKey(dis, j, dis[j].dist+Dis(Point[dis[j].id], Point[end]));
			}
		}
	}

	cout << "No Path Exist.\n";

	return INF;
}

int readPointNode(ifstream &fin)
{
	char buf[1024] = {'\0'};
	int pos;
	double fx, fy;

	int realPos = 1;
	/* start from 1, so Point0 is dummy */
	Point.push_back(node(0, 0));

	while (fin.getline(buf, sizeof(buf))) {
		if (strlen(buf) == 0) break;

		for (int i = 0; i < strlen(buf); i++) {
			if (buf[i] == ':') buf[i] = ' ';
			if (buf[i] == ',') buf[i] = ' ';
		}

		stringstream s(buf);
		s >> pos >> fx >> fy;

		if (pos == realPos) {
			Point.push_back(node(fx, fy));
			realPos ++;
		}
	}

	return Point.size();
}

void readPointEdge(int size, ifstream &fin)
{
	char buf[1024] = {'\0'};

	vector<Edge> eDummy;
	graph.resize(size, eDummy);

	while (fin.getline(buf, sizeof(buf))) {

		for (int i = 0; i < strlen(buf); i++) {
			if (buf[i] == ':') buf[i] = ' ';
			if (buf[i] == ',') buf[i] = ' ';
		}

		int from, to;
		vector<Edge> edges;

		stringstream s(buf);
		s >> from;

		while (s >> to) {
			double dist = Dis(Point[from], Point[to]);
			edges.push_back(Edge(to, dist));
		}

		graph[from] = edges;
	}

	/* for out */
#ifdef DEBUG
	for (int i = 0; i < graph.size(); i++) {
		cout << i << ":" ;
		for (int j = 0; j < graph[i].size(); j++) {
			cout << "(" << graph[i][j].nodeId << "," 
				<< graph[i][j].distance << "), ";
		}
		cout << endl;
	}
#endif
}

int main()
{
	char inFileName[1024];
	int from, to;

	cout << "Input File:";
	cin >> inFileName;

	ifstream fin(inFileName);

	/* build graph */
	stopNum = readPointNode(fin);
	readPointEdge(stopNum, fin);

	/* a* search */
	cout << "Input Search start node:";
	fscanf(stdin, "%d", &from);
	cout << "Input Search target node:";
	fscanf(stdin, "%d", &to);

	int count = 0;
	cout << "Shortest Distance=" << 
		DijkstraAstar(from, to, stopNum, count) << endl;

	cout << "Number of node visited: " << count << endl;

	return 0;
}

