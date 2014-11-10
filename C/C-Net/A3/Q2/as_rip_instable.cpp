/* C++ code */

#include<string>
#include<vector>
#include<iomanip>
#include<iostream>

#ifdef _WIN32
#include <windows.h>
#define sleep(x)	Sleep(x*1000)
#else
#include <unistd.h>
#endif

using namespace std;

const int MAX=15; /* max router number */

/* adjecent matrix */
typedef struct ArcCell
{
	int adj;
}ArcCell,Am[MAX][MAX];

/* route table */
struct TNode
{
	string destination;
	int distance;
	string nextjump;
};

/* route node */
struct VNode
{
	string r;			/* route name */
	vector <TNode> t; 	/* table list */
};

/* routes graph */
struct Graph{
	VNode vex[MAX];
	Am arcs;
	unsigned int vexnum,arcnum;
};

/* get route index by name */
unsigned int LocateVex(Graph G,string s)
{
	unsigned int i;
	for(i=0;i<G.vexnum;i++)
		if(s==G.vex[i].r)
			break;
	return i;
}

/* do generation on beginning */
bool InitRouteTable(Graph &G)
{
	unsigned int i,j,k;
	TNode temp;
	string v1,v2;

	G.vexnum = G.arcnum = 6;

	/* route information */
	for(i=0; i < G.vexnum; i++)
	{
		G.vex[i].r = "R";
		G.vex[i].r += ('0' + i + 1);

		/* init route table for each node */
		for (j = i+1; j <= i+2; j++)
		{
			temp.destination = "N";
			temp.destination += ('0'+j);

			temp.distance = 1;
			temp.nextjump = G.vex[i].r;

			G.vex[i].t.push_back(temp);
		}
	}

	/* init the route links */
	for(i=0;i<G.vexnum;i++)
		for(j=0;j<G.vexnum;j++)
			G.arcs[i][j].adj= -1;

	for(k=0;k<G.arcnum;k++)
	{
		i = k;
		j = (k+1) % G.vexnum;

		G.arcs[i][j].adj = 1; /* direct-graph */
	}

	return true;
}
void print(Graph G)
{
	unsigned int i,j;
	for(i=0;i<G.vexnum;i++)
	{
		cout<<"Router : "<<G.vex[i].r<<endl;

		cout<<setw(20)<<"Destination"
			<<setw(8)<<"hops"
			<<setw(20)<<"next-hop-router"
			<<endl;

		for(j=0;j<(G.vex[i].t.size());j++)
			cout<<setw(20)<<G.vex[i].t[j].destination
				<<setw(8)<<G.vex[i].t[j].distance
				<<setw(20)<<G.vex[i].t[j].nextjump<<endl;

		cout<<endl<<endl;
	}
}

bool Exchange(Graph &G)
{
	unsigned int i,j,k,t,count;
	unsigned int changed = 0;
	unsigned int n1_down = 0;

	TNode temp;

	while (1)
	{
		changed = 0;

		/* update from R1->R2->R3->R4->R5->R6 */
		for(i=0;i<G.vexnum;i++) {
			for(j=0;j<G.vexnum;j++) {
				if(G.arcs[i][j].adj==1) {
					for(k=0;k<G.vex[i].t.size();k++)
					{
						temp.destination=G.vex[i].t[k].destination;
						temp.distance=G.vex[i].t[k].distance+1;
						if(temp.distance>MAX+1)temp.distance=MAX+1;
						temp.nextjump=G.vex[i].r;

						count=0;
						for(t=0;t<G.vex[j].t.size();t++)
						{
							/* update exist */
							if(G.vex[j].t[t].destination==temp.destination) {
								count++;
								if(G.vex[j].t[t].nextjump==temp.nextjump) {
									G.vex[j].t[t].distance=temp.distance;
								}
								else if(G.vex[j].t[t].distance>temp.distance) {
									G.vex[j].t[t].distance=temp.distance;
									G.vex[j].t[t].nextjump=temp.nextjump;
									changed = 1;
								}
							}
						}

						/* new add */
						if(count==0) {
							G.vex[j].t.push_back(temp);
							changed = 1;
						}
					}

					cout<<"Router <"<<G.vex[i].r<<"> Send routing tables to Router <"<<G.vex[j].r<<">"<<endl;
					sleep(1);

					print(G);
					cout<<endl<<endl<<endl;

					/* update router<R1> when N1 disconnected */
					if (i == 0 && n1_down == 0) {
						n1_down = 1;

						vector<TNode>::iterator n1;
						n1 = G.vex[i].t.begin();

						G.vex[i].t.erase(n1);
					}
				}
			}
		}

		/* routers converges */
		if (changed == 0) break;
	}

	return true;
}

int main(int argc, char* argv[])
{
	Graph G;
	InitRouteTable(G);

	print(G);

	sleep(1);

	Exchange(G);

	cout << "Press any key for return...";
	getchar();

	return 0;
}
