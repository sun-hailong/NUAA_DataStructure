#include<iostream>
#include<algorithm>
#include<fstream>
using namespace std;
#define MAX 100
#define INF 0x3f3f3f3f
typedef struct {
	int vex_num;
	int arc_num;
	char vertex[MAX];
	float matrix[MAX][MAX];
}Graph;
Graph G;

typedef struct {
	int vex1;
	int vex2;
	int weight;
}Edge;
Edge edge[MAX * MAX];
int flag[MAX];

void CreateGraph(Graph* graph)
{
	//cout << "请输入顶点数和边数：";
	//cin >> graph->vex_num >> graph->arc_num;
	ifstream infile("E:\\程序们\\数据结构\\数据结构课设\\类型二（无上限）\\建图.txt");
	infile >> graph->vex_num >> graph->arc_num;
	for (int i = 0; i < graph->vex_num; i++)
		infile >> graph->vertex[i];
		//graph->vertex[i] = 'A' + i;

	for (int i = 0; i < graph->vex_num; i++)//邻接矩阵初始化所有元素为无穷大
		for (int j = 0; j < graph->vex_num; j++)
			graph->matrix[i][j] = INF;
	//cout << "请输入边的信息(顶点是0~n-1)\n";
	for (int k = 0; k < graph->arc_num; k++) {
		int i, j;
		float weight;
		//cin >> i >> j >> weight;//输入一条边，i、j表示边的两个顶点编号
		infile >> i >> j >> weight;
		graph->matrix[i][j] = weight;
		graph->matrix[j][i] = weight;
		edge[k].vex1 = i;
		edge[k].vex2 = j;
		edge[k].weight = weight;
	}
	infile.close();
}

void Prim(Graph* G)
{
	int path[MAX], dist[MAX], flag[MAX];
	int mindist = 0;
	//1、初始化三个数组
	for (int i = 0; i < G->vex_num; i++) {
		flag[i] = false;
		dist[i] = G->matrix[0][i];//从第一个顶点开始搜索
		if (dist[i] < INF)
			path[i] = 0;
		else
			path[i] = -1;
	}
	flag[0] = true;//将起始点设置为被选中状态
	//2、循环选中剩余的顶点
	for (int i = 0; i < G->vex_num - 1; i++) {
		//3、在dist数组中找到标志位为0的元素的最小值
		int MIN = INF;
		int k = -1;
		for (int i = 0; i < G->vex_num; i++) {
			if (flag[i] == false && dist[i] < MIN) {
				MIN = dist[i];
				k = i;
			}
		}
		cout << G->vertex[k] << "<--->" << G->vertex[path[k]] << endl;
		//4、找到后将其设置为选中状态
		flag[k] = true;
		mindist += dist[k];
		if (mindist < INF) {
			//5、更新其他顶点经过点k到源点的距离
			for (int i = 0; i < G->vex_num; i++) {
				//6、如果距离更近则更新
				if (flag[i] == false && dist[i] > G->matrix[k][i]) {
					dist[i] = G->matrix[k][i];
					path[i] = k;
				}
			}
		}
	}
	cout << "最小生成树的权值之和为:" << mindist << endl;
}

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

void Kruskal(Graph* G)
{
	//运用等价类的思想
	int num = 0;//用于计算已选边的数量
	int mindist = 0;
	sort(edge, edge + G->arc_num, cmp);//对边进行升序排序
	for (int i = 0; i < G->arc_num; i++) {
		if (num == G->vex_num - 1)
			break;
		int fa = get(edge[i].vex1);
		int fb = get(edge[i].vex2);
		//如果该边的两个顶点的标志相同说明在同一个集合内，所以会形成圈
		if (fa != fb) {
			if (fa < fb)
				flag[fb] = fa;
			else
				flag[fa] = fb;
			num++;
			mindist += edge[i].weight;
			cout << G->vertex[edge[i].vex1] << "<--->" << G->vertex[edge[i].vex2] << endl;
		}
	}
	cout << "最小生成树的权值之和为:" << mindist << endl;
}
int main()
{
	CreateGraph(&G);
	for (int i = 0; i < G.vex_num; i++)
		flag[i] = i;
	Prim(&G);
	return 0;
}
