#include <iostream>
#include <string>

#include <queue>
#include <stack>

using namespace std;

template <typename T>
class Graph {
private:
	const static int VECMAX=64;
	/* edge */
	struct ArcNode {
		int vecidx;
		int weight;
		ArcNode *next;

		ArcNode (int vi, int w, ArcNode *n) {
			vecidx = vi; /* node index, [0, vecCnt) */
			weight = w;
			next = n;
		}
	};

	/* vertex */
	struct VecNode {
		T data;
		ArcNode *first;
		VecNode(T d, ArcNode *p) { data = d; first = p; }
	};

	VecNode *graph[VECMAX];
	int vecCnt;
	bool vecVisited[VECMAX];
	int dist[VECMAX]; /* record distance */
	int path[VECMAX]; /* record path */

public:
	Graph() {
		vecCnt = 0;
		memset(vecVisited, 0, sizeof(vecVisited));

		for (int i = 0; i < VECMAX; i++) 
			graph[i] = NULL;
	}

	Graph(const Graph &g) {
		for (int i = 0; i < VECMAX; i++) {

			if (g.graph[i] == NULL) continue;

			graph[i] = new VecNode(g.graph[i]->data, NULL);

			ArcNode *current = NULL;
			ArcNode *an = g.graph[i]->first;

			while (an != NULL) {
				ArcNode *n = new ArcNode(an->vecidx, an->weight, NULL);

				if (current == NULL) {
					current = n;
					graph[i]->first = n;
				}
				else {
					current->next = n;
					current = n; 
				}

				an = an->next;
			}

			vecVisited[i] = g.vecVisited[i];
			this->vecCnt++;
		}
	}

	/* return add vertex index */
	int addVertex(T d) {
		if (isFull()) {
			std::cout << "Vectex Full." << endl;
			return -1;
		}

		int i;
		for (i = 0; i < VECMAX; i++)
			if (graph[i] == NULL) break;

		graph[i] = new VecNode(d, NULL);

		vecCnt++;

		return i+1;
	}

	/* Non-Diected Graph */
	int addEdge(int from, int to, int wgt) {
		if (from < 0 || to < 0 || from == to 
				|| from > VECMAX || to > VECMAX) {
			return -1;
		}

		VecNode *fnode = graph[from-1];

		if (fnode == NULL || graph[to-1] == NULL) return -1;

		ArcNode *edge = fnode->first;

		while (edge != NULL) {
			if (edge->vecidx == to-1) { /* already exist edge */
				break;
			}

			edge = edge->next;
		}

		if (edge != NULL) return -1;

		/* add edge to node from->to */
		ArcNode *ftEdge = new ArcNode(to-1, wgt, fnode->first);
		fnode->first = ftEdge;

		/* add edge to node to->from */
		ArcNode *tfEdge = new ArcNode(from-1, wgt, graph[to-1]->first);
		graph[to-1]->first = tfEdge;

		return 0;
	}

	int deleteVertexI(int idx) /* delete node by index */
	{
		if (idx < 1 || idx > VECMAX) return -1;

		if (graph[idx-1] == NULL) return -1;

		/* first delete the edge of node idx */
		ArcNode *head = graph[idx-1]->first;
		while (head != NULL) {
			deleteEdge(idx, head->vecidx+1);
			head = graph[idx-1]->first;
		}

		/* delete node */
		delete graph[idx-1];
		graph[idx-1] = NULL;

		vecCnt--;

		return 0;
	}

	int deleteVertex(T d) {
		for (int i = 0; i < VECMAX; i++) {
			if (graph[i] != NULL && graph[i]->data == d) {
				deleteVertexI(i+1);
			}
		}
	}

	int deleteEdge(int from, int to) {
		if (from < 1 || to < 1 || from > VECMAX
				|| to > VECMAX || from == to) return -1;

		if (graph[from-1] == NULL || graph[to-1] == NULL) return -1;

		ArcNode *p = graph[from-1]->first;
		ArcNode *head = p, *ahead = p;

		while (p != NULL) {
			if (p->vecidx == to-1) break;
			ahead = p;
			p = p->next;
		}

		/* not find the edge */
		if (p == NULL) return -1;

		/* delete header */
		if (p == head) {
			graph[from-1]->first = p->next;
			delete p;
		}
		else if (p != NULL) {
			ahead->next = p->next;
			delete p;
		}

		deleteEdge(to, from);

		return 0; /* delete success */
	}

	bool isFull() {
		if (vecCnt > VECMAX) return true;

		return false;
	}

	bool isEmpty() {
		return vecCnt == 0;
	}

	void makeEmpty() {
		vecCnt = 0;
		for (int i = 0; i < VECMAX; i++) {
			deleteVertexI(i);
		}
	}

	/* from, to [1, vecCnt] */
	int getWeight(int from, int to) {
		if (from < 1 || to < 1 || from == to
				|| from > VECMAX || to > VECMAX)
			return -1;

		VecNode *f = graph[from-1];

		if (f == NULL || graph[to-1] == NULL) return -1;

		ArcNode *an = f->first;

		while (an != NULL) {
			if (an->vecidx == to-1) return an->weight;
		}

		return -1;
	}

	/* v belong to [1, vecCnt] */
	ArcNode *getVertices(int v) {
		if (v < 1 || v > VECMAX) return NULL;

		if (graph[v-1] == NULL) return NULL;

		return graph[v-1]->first;
	}

	void resetVisited() {
		for (int i = 0; i < VECMAX; i++) vecVisited[i] = false;
	}

	/* v belong to [1, vecCnt] */
	bool isVisited(int v) {
		return vecVisited[v-1];
	}

	void displayGraph()
	{
		for (int i = 0; i < VECMAX; i++) {
			if (graph[i] != NULL) {
				printf("Node.%d (%d):", i+1, graph[i]->data);

				ArcNode *an = graph[i]->first;
				while (an != NULL) {
					printf("Edge to(%d,%d),", an->vecidx+1, an->weight);
					an = an->next;
				}

				printf("\n");
			}
		}
	}

	void DFS(int idx) /* between [0, vecCnt) */
	{
		printf("visit node(%d, value=%d)\n", idx+1, graph[idx]->data);
		vecVisited[idx] = true;

		ArcNode *f = graph[idx]->first;

		while (f) {
			if (!vecVisited[f->vecidx]) DFS(f->vecidx);
			f = f->next;
		}
	}

	/* depth first search */
	void graphDFS()
	{
		resetVisited();

		for (int i = 0; i < VECMAX; i++) {
			if (graph[i] != NULL && vecVisited[i] == false) {
				DFS(i);
			}
		}
	}

	/* broad first search */
	void BFS(int idx) /* idx between [0, vecCnt) */
	{
		queue<int> Q;

		Q.push(idx);

		while (!Q.empty()) {
			int next = Q.front();
			Q.pop();

			/* visit */
			printf("visit node(%d, value=%d)\n", 
					next+1, graph[next]->data);
			vecVisited[next] = true;

			ArcNode *an = graph[next]->first;
			while (an != NULL) {
				if (vecVisited[an->vecidx] != true)
					Q.push(an->vecidx);
				an = an->next;
			}
		}
	}

	void graphBFS()
	{
		resetVisited();

		for (int i = 0; i < VECMAX; i++)
			if (graph[i] != NULL && vecVisited[i] == false)
				BFS(i);
	}

	void dijkstraPath(int vs) /* start from vs, vs between [0, vecCnt)*/
	{
		int v0 = vs;
		if (graph[v0] == NULL) return;

		resetVisited();

		for (int i = 0; i < VECMAX; i++) {
			dist[i] = INT_MAX;
			path[i] = -1;
		}

		/* init first node-edge */
		ArcNode *an = graph[v0]->first;
		while (an != NULL) {
			dist[an->vecidx] = an->weight;
			path[an->vecidx] = v0;

			an = an->next;
		}

		vecVisited[v0] = true;

		for (int i = 1; i < vecCnt; i++) { /* iteration times */
			int min = INT_MAX;
			int next;

			/* get next min-weight node */
			for (int j = 0; j < VECMAX; j++)
			{
				if (vecVisited[j] == false && dist[j] < min)
				{
					min = dist[j];
					next = j;
				}
			}

			if (min == INT_MAX) break;

			/* update dist and path */
			vecVisited[next] = true;

			an = graph[next]->first;
			while (an != NULL) {

				if (vecVisited[an->vecidx] == false && 
						min + an->weight < dist[an->vecidx])
				{
					dist[an->vecidx] = min + an->weight;
					path[an->vecidx] = next;
				}

				an = an->next;
			}
		}
	}

	/* from, to belong to [1, VECCNT] */
	int getShortestWeight(int from, int to)
	{
		if (from == to || from < 1 || from > VECMAX
				|| to < 1 || to > VECMAX) 
			return 0;

		if (graph[from-1] == NULL || graph[to-1] == NULL)
			return 0;

		dijkstraPath(from-1);

		if (dist[to-1] != INT_MAX)
			return dist[to-1];

		return 0;
	}

	void showShortestPath(int from, int to)
	{
		stack<int> s;

		/* transfer to node storage index */
		from = from - 1;
		to   = to - 1;

		while (to != from) {
			s.push(to);
			to = path[to];
		}

		while (!s.empty()) {
			std::cout << s.top() + 1 << " <- ";
			s.pop();
		}

		std::cout << from+1 << std::endl;
	}

	~Graph() {
		for (int i = 0; i < VECMAX; i++) {
			if (graph[i] != NULL) {
				VecNode *vec = graph[i];
				ArcNode *arc = vec->first;

				/* delete edge */
				while (arc != NULL) {
					ArcNode *tmp = arc->next;
					delete arc;
					arc = tmp;
				}

				delete vec;
			}
		}
	}
};

void menuLoop(Graph<int> *g)
{
	int value, from, to, weight, option;

	while (1) {
		printf("Option:\n\t0:add vertex\n\t1:add edge\n");
		printf("\t2:delete vertex\n\t3:delete edge\n");
		printf("\t4:get weight\n\t5:make empty\n");
		printf("\t6:display graph\n\t7:DFS graph\n");
		printf("\t8:BFS graph\n\t9:Find Shortest Path <from, to>\n");
		printf("\t10:Exit\n");

		std::cin >> option;
		if (option < 0 || option > 10) continue;

		switch(option) {
			case 0:
				std::cout << "Input Node Value:" << std::endl;
				std::cin >> value;
				if (g->addVertex(value) != -1) {
					std::cout <<"Add Node Success.\n";
				}
				else {
					std::cout <<"Add Node Failed.\n";
				}

				break;
			
			case 1:
				std::cout << "Add Edge <from, to, weight>\n";
				std::cin >> from >> to >> weight;
				if (g->addEdge(from, to, weight) == 0) {
					std::cout << "Add Edge Success\n";
				}
				else {
					std::cout << "Add Edge Failed\n";
				}

				break;

			case 2:
				std::cout << "Delete Vertex i:" << std::endl;
				std::cin >> from;
				if (g->deleteVertexI(from) == 0) {
					std::cout << "Delete Vertex Success\n";
				}
				else {
					std::cout << "Delete Vertex Failed\n";
				}

				break;

			case 3:
				std::cout << "Delete Edge <from, to>:\n";
				std::cin >> from >> to;
				if (g->deleteEdge(from, to) == 0) {
					std::cout <<"Delete Edge Success.\n";
				}
				else {
					std::cout <<"Delete Edge Failed.\n";
				}

				break;

			case 4:
				std::cout <<"Get Edge Weight <from, to>:\n";
				std::cin >> from >> to;
				weight = g->getWeight(from, to);

				if (weight != -1) {
					std::cout << "Weight=" << weight << std::endl;
				}

				break;

			case 5:

				g->makeEmpty();
				std::cout << "Make Empty Success\n";

				break;

			case 6:
				g->displayGraph();
				break;

			case 7:
				g->graphDFS();
				break;

			case 8:
				g->graphBFS();
				break;

			case 9:
				std::cout <<"Shortest Path <from, to>, input:";
				std::cin >> from >> to;
				
				value = g->getShortestWeight(from, to);

				if (value != 0) {
					std::cout << "Get Shortest Weight: " << value 
						      << std::endl;

					std::cout << "The Path is :";
					g->showShortestPath(from, to);
				}
				else {
					std::cout << "No Path Found.\n";
				}

				break;

			case 10:
				return ;
		}
	}
}

int main()
{
	Graph<int> *g = new Graph<int>();

	menuLoop(g);

	delete g;

	return 0;
}
