#include <stdio.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <queue>
#include <set>
#include <algorithm>
#include <stdlib.h>
using namespace std;
#define ll int
#define N 550
#define inf 1152921504606846976
struct node{
	double x, y;
	bool operator<(const node&a)const{
		if(a.x==x)return a.y>y;
		return a.x>x;
	}
	void put(){
		printf("(%.0f, %.0f)\n", x, y);
	}
}p[N];
double Dis(node a, node b){return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));}
int n;
double dis[N][N],dp[N][N];
int main(){
    ll i, j, u, v; int T; scanf("%d", &T);
    while(T-->0){
    	scanf("%d", &n);
        for(i=1;i<=n;i++)scanf("%lf %lf",&p[i].x,&p[i].y);
		sort(p+1,p+n+1);
	//	for(int i = 1; i <= n; i++)p[i].put();
		for(i=1;i<=n;i++)for(j=1;j<=n;j++)dis[i][j] = Dis(p[i],p[j]), dp[i][j] = inf;

		dp[1][1] = 0;
		for(i=2;i<=n;i++)
		{
			for(j = 1;j < i; j++)
			{
				dp[i][j] = min(dp[i-1][j]+dis[i][i-1], dp[i][j]);
				dp[i][i-1] = min(dp[i-1][j]+dis[j][i], dp[i][i-1]);
			}
		}
		printf("%.10f\n",dp[n][n-1]+dis[n][n-1]);
    }
    return 0;
}


/*
Input
Output
Sample Input
2
5
0 1
1 2
2 0
3 2
4 1
3
100 1
200 1
300 1
Sample Output
9.300563079746
400
HINT
*/
