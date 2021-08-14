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
	//cout << "�����붥�����ͱ�����";
	//cin >> graph->vex_num >> graph->arc_num;
	ifstream infile("E:\\������\\���ݽṹ\\���ݽṹ����\\���Ͷ��������ޣ�\\��ͼ.txt");
	infile >> graph->vex_num >> graph->arc_num;
	for (int i = 0; i < graph->vex_num; i++)
		infile >> graph->vertex[i];
		//graph->vertex[i] = 'A' + i;

	for (int i = 0; i < graph->vex_num; i++)//�ڽӾ����ʼ������Ԫ��Ϊ�����
		for (int j = 0; j < graph->vex_num; j++)
			graph->matrix[i][j] = INF;
	//cout << "������ߵ���Ϣ(������0~n-1)\n";
	for (int k = 0; k < graph->arc_num; k++) {
		int i, j;
		float weight;
		//cin >> i >> j >> weight;//����һ���ߣ�i��j��ʾ�ߵ�����������
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
	//1����ʼ����������
	for (int i = 0; i < G->vex_num; i++) {
		flag[i] = false;
		dist[i] = G->matrix[0][i];//�ӵ�һ�����㿪ʼ����
		if (dist[i] < INF)
			path[i] = 0;
		else
			path[i] = -1;
	}
	flag[0] = true;//����ʼ������Ϊ��ѡ��״̬
	//2��ѭ��ѡ��ʣ��Ķ���
	for (int i = 0; i < G->vex_num - 1; i++) {
		//3����dist�������ҵ���־λΪ0��Ԫ�ص���Сֵ
		int MIN = INF;
		int k = -1;
		for (int i = 0; i < G->vex_num; i++) {
			if (flag[i] == false && dist[i] < MIN) {
				MIN = dist[i];
				k = i;
			}
		}
		cout << G->vertex[k] << "<--->" << G->vertex[path[k]] << endl;
		//4���ҵ���������Ϊѡ��״̬
		flag[k] = true;
		mindist += dist[k];
		if (mindist < INF) {
			//5�������������㾭����k��Դ��ľ���
			for (int i = 0; i < G->vex_num; i++) {
				//6�����������������
				if (flag[i] == false && dist[i] > G->matrix[k][i]) {
					dist[i] = G->matrix[k][i];
					path[i] = k;
				}
			}
		}
	}
	cout << "��С��������Ȩֵ֮��Ϊ:" << mindist << endl;
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
	//���õȼ����˼��
	int num = 0;//���ڼ�����ѡ�ߵ�����
	int mindist = 0;
	sort(edge, edge + G->arc_num, cmp);//�Ա߽�����������
	for (int i = 0; i < G->arc_num; i++) {
		if (num == G->vex_num - 1)
			break;
		int fa = get(edge[i].vex1);
		int fb = get(edge[i].vex2);
		//����ñߵ���������ı�־��ͬ˵����ͬһ�������ڣ����Ի��γ�Ȧ
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
	cout << "��С��������Ȩֵ֮��Ϊ:" << mindist << endl;
}
int main()
{
	CreateGraph(&G);
	for (int i = 0; i < G.vex_num; i++)
		flag[i] = i;
	Prim(&G);
	return 0;
}
