//运行环境visual studio 2005     C++代码

#include<string>
#include<vector>
#include<windows.h>
#include<iomanip>
#include<iostream>
using namespace std;
const int MAX=15;

typedef struct ArcCell//弧结点
{
  int adj;
}ArcCell,Am[MAX][MAX];

struct TNode//路由表
{
  string destination;
  int distance;
  string nextjump;
};

struct VNode//路由结点
{
  string r;//顶点名
  vector <TNode> t;
};

struct Graph{//图
  VNode vex[MAX];
  Am arcs;
  int vexnum,arcnum;
};

//确定s在图中的位置
int LocateVex(Graph G,string s)
{
  int i;
  for(i=0;i<G.vexnum;i++)
    if(s==G.vex[i].r)
      break;
  return i;
}
//创建图
bool Create(Graph &G)
{
  int i,j,k;
  TNode temp;
  string v1,v2;
  cout<<"输入路由数 链路数"<<endl;
  cin>>G.vexnum>>G.arcnum;
  cout<<"输入路由器名及路由表"<<endl;
  for(i=0;i<G.vexnum;i++)//构造顶点
    {
      cout<<"输入第 "<<i+1<<" 个路由器名"<<endl;
      cin>>G.vex[i].r;
      cout<<"输入其路由表"<<endl;
      cin>>temp.destination>>temp.distance>>temp.nextjump;
      while(1)
	{
 
	  if(temp.distance>16)temp.distance=16;
	  G.vex[i].t.push_back(temp);//构造路由表
	  cin>>temp.destination>>temp.distance>>temp.nextjump;
	  if(temp.destination=="0"&&temp.distance==0&&temp.nextjump=="0")
	    break;
	}

    }
  for(i=0;i<G.vexnum;i++)//初始化邻接矩阵
    for(j=0;j<G.vexnum;j++)
      G.arcs[i][j].adj=10000;
  for(k=0;k<G.arcnum;k++)//构造邻接矩阵
    {
      cout<<"输入第 "<<k+1<<" 条弧的两顶点"<<endl;
      cin>>v1>>v2;
      i=LocateVex(G,v1);
      j=LocateVex(G,v2);
      G.arcs[i][j].adj=1;
      G.arcs[j][i].adj=1;
    }
  return true;
}
void print(Graph G)
{
  int i,j;
  for(i=0;i<G.vexnum;i++)
    {
      cout<<"路由器 "<<G.vex[i].r<<endl;
      cout<<setw(20)<<"目的地址"<<setw(8)<<"距离"<<setw(20)<<"下一跳"<<endl;
      for(j=0;j<(G.vex[i].t.size());j++)
	cout<<setw(20)<<G.vex[i].t[j].destination<<setw(8)<<G.vex[i].t[j].distance<<setw(20)<<G.vex[i].t[j].nextjump<<endl;
      cout<<endl<<endl;
    }
}
bool Exchange(Graph &G)
{
  int i,j,k,t,count;
  TNode temp;
  for(i=0;i<G.vexnum;i++)
    for(j=0;j<G.vexnum;j++)
      {
	if(G.arcs[i][j].adj==1)
	  {
	    for(k=0;k<G.vex[i].t.size();k++)
	      {
		temp.destination=G.vex[i].t[k].destination;
		temp.distance=G.vex[i].t[k].distance+1;
		if(temp.distance>16)temp.distance=16;
		temp.nextjump=G.vex[i].r;
		count=0;
		for(t=0;t<G.vex[j].t.size();t++)
		  {
		    if(G.vex[j].t[t].destination==temp.destination)
		      {
			count++;
			if(G.vex[j].t[t].nextjump==temp.nextjump)
			  G.vex[j].t[t].distance=temp.distance;
			else if(G.vex[j].t[t].distance>temp.distance)
			  {
			    G.vex[j].t[t].distance=temp.distance;
			    G.vex[j].t[t].nextjump=temp.nextjump;
			  }
		      }
		  }
		if(count==0)
		  G.vex[j].t.push_back(temp);

	      }
	    cout<<"路由器 "<<G.vex[i].r<<" 向路由器 "<<G.vex[j].r<<" 发送消息"<<endl;
	    Sleep(3000);
	    print(G);
	    cout<<endl<<endl<<endl;
	  }
      }
  return true;
}

int main(int argc, char* argv[])
{
  string s;
  Graph G;
  Create(G);
  print(G);
  Sleep(1000);
  Exchange(G);
  cin>>s;

  return 0;
}
