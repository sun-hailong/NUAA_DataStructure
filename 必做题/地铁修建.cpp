#include<iostream>
#include<algorithm>
using namespace std;
#define MAXVEX 1000001
typedef struct {
    int a;
    int b;
    int weight;
}Edge;

Edge edge[MAXVEX];
int n, m;//顶点个数、边个数
int flag[MAXVEX];

int cmp(Edge e1, Edge e2)
{
	return e1.weight < e2.weight;
}

int get(int a)
{
	if (flag[a] == a)
		return a;
	else 
		return get(flag[a]);
}

int kruskal()
{
	int mindist = 0;
	sort(edge + 1, edge + m + 1, cmp);//对边进行升序排序
	for(int i = 1; i <= m; i++){

		int fa = get(edge[i].a);
		int fb = get(edge[i].b);

		if (fa != fb){
			if (fa < fb)
				flag[fb] = fa;
			else
				flag[fa] = fb;
		}

		if (get(1) == get(n)) {
			mindist = edge[i].weight;
			break;
		}
	} 
	return mindist;
}

int main()
{
	cin >> n >> m;
	for (int i = 1; i <= n; i++)//给每一个顶点一个编号，运用等价类思想 
		flag[i] = i;
	for(int i = 1; i <= m; i++){
		cin >> edge[i].a >> edge[i].b >> edge[i].weight;
	}
	cout << kruskal() << endl;
    return 0;
}
