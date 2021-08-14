#include<iostream>
#include<fstream>
#include<cstring>
#include<queue>
#include<stdio.h>
#include<stdlib.h>
#define MAX_CHAR_NUMS 128
using namespace std;

//--------------�ṹ�弰ȫ�ֱ����Ķ���----------------//

typedef struct {
	char ch;
	int weight;//���ʵĴ���
	int num;//����ĳ���
	int code[100];
}Char;

typedef struct tree{
	char ch = '+';//�ַ�
	int weight;//Ȩֵ
	struct tree* left;
	struct tree* right;
	struct tree* parent;
	int num = 0;//���ڼ�¼��������ĳ���
	int code[100];//��������
}HuffmanTree;

int N = 0;//��¼�ַ����࣬�ɱ�
int maxNums = N;//��¼�ַ����࣬����
Char chars[MAX_CHAR_NUMS];//��¼�ַ�������ʴ���

//--------------------����������---------------------//
HuffmanTree* treeNew(int weight, char ch);//����һ���½��
int treeDepth(HuffmanTree* T);//���������
HuffmanTree* Select(HuffmanTree** arr, int n);//Ѱ����СȨֵ�ڵ�
HuffmanTree* CreateHuffmanTree(HuffmanTree** arr);//����huffman��
void Code(HuffmanTree* tree);//����
void deCode(HuffmanTree* tree);//����
void treePreOrderVisit(HuffmanTree* tree);//����������ݹ�ʵ��
void treeInOrderVisit(HuffmanTree* tree);//����������ݹ�ʵ��
void treeLevelOrderVisit(HuffmanTree* tree);//��α���������ʵ��
void showInfo(HuffmanTree* tree);//��ʾ������Ϣ
void getCharNums();//���ļ���ȡ�ַ�
void charSelect();//���ַ������ʴ�����������
void saveToDat();//�����������뱣����dat�ļ�
void saveToTxt(HuffmanTree* tree);//�洢�ļ�(Huffman.txt)
void decTobin(int num, int arr[]);//ʮ����ת������

//------------------������----------------------//

int main()
{
	getCharNums();//���Ҳ���¼���ַ�
	charSelect();//���շ��ʴ�����������
	maxNums = N;
	HuffmanTree** arr = new HuffmanTree * [N];//����һ��ָ�����飬�����ڴ�Ž��ָ��

	for (int i = 0; i < maxNums; i++)//������Ȩ�ع�������Ľ��
	{
		int weight = chars[i].weight;
		char ch = chars[i].ch;
		arr[i] = treeNew(weight, ch);
	}

	HuffmanTree* tree = CreateHuffmanTree(arr);

	Code(tree);
	//showInfo(tree);
	saveToDat();
	saveToTxt(tree);
	deCode(tree);
	return 0;
}

//---------------Huffman���Ķ��弰����---------------//

HuffmanTree* treeNew(int weight, char ch = '+')//����һ���½��
{
	HuffmanTree* tree = (HuffmanTree*)malloc(sizeof(HuffmanTree));
	tree->ch = ch;
	tree->weight = weight;
	tree->left = NULL;
	tree->right = NULL;
	tree->parent = NULL;
	return tree;
}

int treeDepth(HuffmanTree* T)
{
	if (T == NULL)
		return 0;
	int left_depth = treeDepth(T->left);
	int right_depth = treeDepth(T->right);
	if (left_depth < right_depth)
		return right_depth + 1;
	else
		return left_depth + 1;
}

HuffmanTree* Select(HuffmanTree** arr, int n)//Ѱ����СȨֵ�ڵ�
{
	int min_pos = 0;
	//1���ҳ���Сֵ�Ľ��
	for (int i = 0; i < n; i++)
		if (arr[i]->weight <= arr[min_pos]->weight)// ��<=���Ա�֤�½����������
			min_pos = i;
	//2�����沢��������ɾ���ýڵ�
	HuffmanTree* T = arr[min_pos];
	for (int i = min_pos; i < n - 1; i++)
		arr[i] = arr[i + 1];
	return T;
}

HuffmanTree* CreateHuffmanTree(HuffmanTree** arr)//����huffman��
{
	while (N >= 2)
	{
		//1��Ѱ��������Сֵ���
		HuffmanTree* tree1 = Select(arr, N);
		N--;
		HuffmanTree* tree2 = Select(arr, N);
		//2������������㹹����½��
		HuffmanTree* tree3 = treeNew(tree1->weight + tree2->weight);
		tree3->left = tree1;
		tree3->right = tree2;
		tree1->parent = tree3;
		tree2->parent = tree3;
		//3�����½����뵽ָ�����鵱��
		arr[N - 1] = tree3;
	}
	return arr[0];
}

void Code(HuffmanTree* tree)
{
	queue<HuffmanTree*>q;
	int depth = 1;//��¼��ǰ�ѵ��˹��������ĵڼ���
	if (tree)
		q.push(tree);
	q.push(NULL);
	while (q.size() != 1)
	{
		HuffmanTree* cur = q.front();
		q.pop();
		if (cur == NULL) {
			depth++;
			q.push(NULL);
		}
		else {
			if (cur->left)
				q.push(cur->left);
			if (cur->right)
				q.push(cur->right);
		}
		//����ýڵ�Ϊԭʼ�ڵ㣬����Ϊ�½��Ľڵ�
		if (cur != NULL && cur->ch != '+') {
			HuffmanTree* temp = cur;
			int n = depth - 1;
			cur->num = n;
			while (temp->parent != NULL) {
				HuffmanTree* parent = temp->parent;
				if (temp == parent->left) {
					cur->code[--n] = 0;
				}
				else {
					cur->code[--n] = 1;
				}
				temp = parent;
			}
			//���������뱣�����ṹ�������ڣ����ڱ���ͽ���
			for (int i = 0; i < maxNums; i++) {
				if (chars[i].ch == cur->ch) {
					chars[i].num = cur->num;
					for (int j = 0; j < cur->num; j++) {
						chars[i].code[j] = cur->code[j];
					}
				}
			}
		}
	}
}

void deCode(HuffmanTree* tree)
{
	ifstream in("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\code.dat", ios::in | ios::binary);
	ofstream out("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\recode.txt");
	if (!in)
	{
		cout << "code.dat��ʧ�ܣ�" << endl;
		exit(1);
	}
	char ch;
	int arr[100000];
	memset(arr, -1, sizeof(arr));
	int t = 0;
	while (in.get(ch)) {
		int dec = (ch + 256) % 256;
		decTobin(dec, arr + t);
		t += 8;
	}
	HuffmanTree* root = tree;
	for (int i = 0; i < t; i++) {
		if (tree->ch != '+') {
			out << tree->ch;
			tree = root;
		}
		if (arr[i] == 0)
			tree = tree->left;
		else
			tree = tree->right;
	}
	cout << "\n������ϣ��ļ�����ɹ���\n";
	out.close();
	in.close();
}

void treePreOrderVisit(HuffmanTree* tree)//����������ݹ�ʵ��
{
	if (tree) {
		//if (tree->left == NULL && tree->right == NULL)
		cout << tree->weight << ' ';
		treePreOrderVisit(tree->left);
		treePreOrderVisit(tree->right);
	}
}

void treeInOrderVisit(HuffmanTree* tree)//����������ݹ�ʵ��
{
	if (tree) {
		treeInOrderVisit(tree->left);
		//if (tree->left == NULL && tree->right == NULL)
		cout << tree->weight << ' ';
		treeInOrderVisit(tree->right);
	}
}

void treeLevelOrderVisit(HuffmanTree* tree)
{
	queue<HuffmanTree*>q;
	if (tree)
		q.push(tree);
	q.push(NULL);//���ڷֲ�
	while (q.size() != 1)
	{
		HuffmanTree* cur = q.front();
		q.pop();
		//���ڰ�����ʾ
		if (cur == NULL) {
			cout << '\n';
			q.push(NULL);
		}
		else {
			cout << cur->ch << ' ';
			if (cur->left)
				q.push(cur->left);
			if (cur->right)
				q.push(cur->right);
		}
	}
}

void showInfo(HuffmanTree* tree)
{
	queue<HuffmanTree*>q;
	if (tree)
		q.push(tree);
	while (q.size() != 0)
	{
		HuffmanTree* cur = q.front();
		q.pop();
		if (cur->ch != '+') {
			if (cur->ch == ' ')
				cout << "�ַ�:" << "Space" << "\t" << "���ʴ���:" << cur->weight << '\t' << "����:";
			else if (cur->ch == '\n')
				cout << "�ַ�:" << "LineFeed" << "\t" << "���ʴ���:" << cur->weight << '\t' << "����:";
			else
				cout << "�ַ�:" << cur->ch << "\t\t" << "���ʴ���:" << cur->weight << '\t' << "����:";
			for (int i = 0; i < cur->num; i++)
				cout << cur->code[i];
			cout << endl;
		}
		if (cur->left)
			q.push(cur->left);
		if (cur->right)
			q.push(cur->right);
	}
}

//-------------------���ݴ���------------------------//

void getCharNums()//���ļ���ȡ�ַ�
{
	ifstream infile("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\source.txt");
	if (!infile)
	{
		cout << "source.txt�ı���ʧ��!" << endl;
		exit(1);
	}
	char ch;
	while (infile.get(ch))
	{
		int flag = 0;//�ж�Ŀǰ���������Ƿ���������ַ���1Ϊ��
		//infile >> ch;
		for (int i = 0; i < N; i++)//��������������и��ַ�������ʴ���+1
		{
			if (ch == chars[i].ch)
			{
				chars[i].weight++;
				flag = 1;
				break;
			}
		}
		if (flag == 0)//Ŀǰ�������޸��ַ�
		{
			chars[N].ch = ch;
			chars[N].weight = 1;
			N++;
		}
	}
	infile.close();
}

void charSelect()//���ַ������ʴ�����������
{
	for (int i = 0; i < N - 1; i++)
	{
		int max = i;
		for (int j = i + 1; j < N; j++)
		{
			if (chars[j].weight > chars[max].weight)
				max = j;
		}
		if (max != i)
		{
			Char temp;
			temp = chars[max];
			chars[max] = chars[i];
			chars[i] = temp;
		}
	}
}

void saveToDat()//�����������뱣����dat�ļ�
{
	ifstream in("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\source.txt");
	fstream file("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\code.dat", ios::out | ios::binary);
	if (!in)
	{
		cout << "source.txt�ı���ʧ��!" << endl;
		exit(1);
	}
	char ch;
	int arr[100000];
	int t = 0;
	while (in.get(ch)) {
		for (int i = 0; i < maxNums; i++) {
			if (ch == chars[i].ch) {
				for (int j = 0; j < chars[i].num; j++)
					arr[t++] = chars[i].code[j];
				break;
			}
		}
	}
	//������ȫ������һ�������ڣ�Ȼ��ÿ��λ���һ���ַ�����dat�ļ�
	int sum = 0;
	for (int i = 0; i < t; i++) {
		if (i % 8 == 0 && i != 0) {
			file << (char)sum;
			sum = 0;
		}
		sum += pow(2, 7 - (i % 8)) * arr[i];
	}
	cout << "\n������ϣ��ļ�����ɹ���\n";
	file.close();
	in.close();
}

void saveToTxt(HuffmanTree* tree)//�洢�ļ�(Huffman.txt)
{
	ofstream out("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\Huffman.txt");
	queue<HuffmanTree*>q;
	if (tree)
		q.push(tree);
	while (q.size() != 0)
	{
		HuffmanTree* cur = q.front();
		q.pop();
		if (cur->ch != '+') {
			if (cur->ch == ' ')
				out << "�ַ�:" << "Space" << "\t" << "���ʴ���:" << cur->weight << '\t' << "����:";
			else if (cur->ch == '\n')
				out << "�ַ�:" << "LineFeed" << "\t" << "���ʴ���:" << cur->weight << '\t' << "����:";
			else
				out << "�ַ�:" << cur->ch << "\t\t" << "���ʴ���:" << cur->weight << '\t' << "����:";
			for (int i = 0; i < cur->num; i++)
				out << cur->code[i];
			out << endl;
		}
		if (cur->left)
			q.push(cur->left);
		if (cur->right)
			q.push(cur->right);
	}
}

void decTobin(int num, int arr[])
{
	//����ȡ�෨
	int i = 7;
	while (num != 0) {
		arr[i] = (num % 2);
		num /= 2;
		i--;
	}
	for (int j = 0; j <= i; j++) {
		arr[j] = 0;
	}
}