#include<iostream>
#include<fstream>
#include<cstring>
#include<queue>
#include<stdio.h>
#include<stdlib.h>
#define MAX_CHAR_NUMS 128
using namespace std;

//--------------结构体及全局变量的定义----------------//

typedef struct {
	char ch;
	int weight;//访问的次数
	int num;//编码的长度
	int code[100];
}Char;

typedef struct tree{
	char ch = '+';//字符
	int weight;//权值
	struct tree* left;
	struct tree* right;
	struct tree* parent;
	int num = 0;//用于记录哈夫曼码的长度
	int code[100];//哈夫曼码
}HuffmanTree;

int N = 0;//记录字符种类，可变
int maxNums = N;//记录字符种类，不变
Char chars[MAX_CHAR_NUMS];//记录字符及其访问次数

//--------------------函数的声明---------------------//
HuffmanTree* treeNew(int weight, char ch);//构造一个新结点
int treeDepth(HuffmanTree* T);//求树的深度
HuffmanTree* Select(HuffmanTree** arr, int n);//寻找最小权值节点
HuffmanTree* CreateHuffmanTree(HuffmanTree** arr);//创建huffman树
void Code(HuffmanTree* tree);//编码
void deCode(HuffmanTree* tree);//解码
void treePreOrderVisit(HuffmanTree* tree);//先序遍历，递归实现
void treeInOrderVisit(HuffmanTree* tree);//中序遍历，递归实现
void treeLevelOrderVisit(HuffmanTree* tree);//层次遍历，队列实现
void showInfo(HuffmanTree* tree);//显示编码信息
void getCharNums();//从文件读取字符
void charSelect();//对字符按访问次数进行排序
void saveToDat();//将哈夫曼编码保存至dat文件
void saveToTxt(HuffmanTree* tree);//存储文件(Huffman.txt)
void decTobin(int num, int arr[]);//十进制转二进制

//------------------主函数----------------------//

int main()
{
	getCharNums();//查找并记录下字符
	charSelect();//按照访问次数进行排序
	maxNums = N;
	HuffmanTree** arr = new HuffmanTree * [N];//构造一个指针数组，数组内存放结点指针

	for (int i = 0; i < maxNums; i++)//将所有权重构造成树的结点
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

//---------------Huffman树的定义及操作---------------//

HuffmanTree* treeNew(int weight, char ch = '+')//构造一个新结点
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

HuffmanTree* Select(HuffmanTree** arr, int n)//寻找最小权值节点
{
	int min_pos = 0;
	//1、找出最小值的结点
	for (int i = 0; i < n; i++)
		if (arr[i]->weight <= arr[min_pos]->weight)// 用<=可以保证新建的树在左边
			min_pos = i;
	//2、保存并在数组内删除该节点
	HuffmanTree* T = arr[min_pos];
	for (int i = min_pos; i < n - 1; i++)
		arr[i] = arr[i + 1];
	return T;
}

HuffmanTree* CreateHuffmanTree(HuffmanTree** arr)//创建huffman树
{
	while (N >= 2)
	{
		//1、寻找两个最小值结点
		HuffmanTree* tree1 = Select(arr, N);
		N--;
		HuffmanTree* tree2 = Select(arr, N);
		//2、将这两个结点构造成新结点
		HuffmanTree* tree3 = treeNew(tree1->weight + tree2->weight);
		tree3->left = tree1;
		tree3->right = tree2;
		tree1->parent = tree3;
		tree2->parent = tree3;
		//3、将新结点加入到指针数组当中
		arr[N - 1] = tree3;
	}
	return arr[0];
}

void Code(HuffmanTree* tree)
{
	queue<HuffmanTree*>q;
	int depth = 1;//记录当前已到了哈夫曼树的第几层
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
		//如果该节点为原始节点，即不为新建的节点
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
			//将哈夫曼码保存至结构体数组内，便于编码和解码
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
	ifstream in("E:\\程序们\\数据结构\\数据结构课设\\必做题\\code.dat", ios::in | ios::binary);
	ofstream out("E:\\程序们\\数据结构\\数据结构课设\\必做题\\recode.txt");
	if (!in)
	{
		cout << "code.dat打开失败！" << endl;
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
	cout << "\n解码完毕，文件保存成功！\n";
	out.close();
	in.close();
}

void treePreOrderVisit(HuffmanTree* tree)//先序遍历，递归实现
{
	if (tree) {
		//if (tree->left == NULL && tree->right == NULL)
		cout << tree->weight << ' ';
		treePreOrderVisit(tree->left);
		treePreOrderVisit(tree->right);
	}
}

void treeInOrderVisit(HuffmanTree* tree)//中序遍历，递归实现
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
	q.push(NULL);//用于分层
	while (q.size() != 1)
	{
		HuffmanTree* cur = q.front();
		q.pop();
		//便于按层显示
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
				cout << "字符:" << "Space" << "\t" << "访问次数:" << cur->weight << '\t' << "编码:";
			else if (cur->ch == '\n')
				cout << "字符:" << "LineFeed" << "\t" << "访问次数:" << cur->weight << '\t' << "编码:";
			else
				cout << "字符:" << cur->ch << "\t\t" << "访问次数:" << cur->weight << '\t' << "编码:";
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

//-------------------数据处理------------------------//

void getCharNums()//从文件读取字符
{
	ifstream infile("E:\\程序们\\数据结构\\数据结构课设\\必做题\\source.txt");
	if (!infile)
	{
		cout << "source.txt文本打开失败!" << endl;
		exit(1);
	}
	char ch;
	while (infile.get(ch))
	{
		int flag = 0;//判断目前数组里面是否有输入的字符，1为有
		//infile >> ch;
		for (int i = 0; i < N; i++)//如果数组里面已有该字符，则访问次数+1
		{
			if (ch == chars[i].ch)
			{
				chars[i].weight++;
				flag = 1;
				break;
			}
		}
		if (flag == 0)//目前数组内无该字符
		{
			chars[N].ch = ch;
			chars[N].weight = 1;
			N++;
		}
	}
	infile.close();
}

void charSelect()//对字符按访问次数进行排序
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

void saveToDat()//将哈夫曼编码保存至dat文件
{
	ifstream in("E:\\程序们\\数据结构\\数据结构课设\\必做题\\source.txt");
	fstream file("E:\\程序们\\数据结构\\数据结构课设\\必做题\\code.dat", ios::out | ios::binary);
	if (!in)
	{
		cout << "source.txt文本打开失败!" << endl;
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
	//将编码全部编入一个数组内，然后每八位组成一个字符存入dat文件
	int sum = 0;
	for (int i = 0; i < t; i++) {
		if (i % 8 == 0 && i != 0) {
			file << (char)sum;
			sum = 0;
		}
		sum += pow(2, 7 - (i % 8)) * arr[i];
	}
	cout << "\n编码完毕，文件保存成功！\n";
	file.close();
	in.close();
}

void saveToTxt(HuffmanTree* tree)//存储文件(Huffman.txt)
{
	ofstream out("E:\\程序们\\数据结构\\数据结构课设\\必做题\\Huffman.txt");
	queue<HuffmanTree*>q;
	if (tree)
		q.push(tree);
	while (q.size() != 0)
	{
		HuffmanTree* cur = q.front();
		q.pop();
		if (cur->ch != '+') {
			if (cur->ch == ' ')
				out << "字符:" << "Space" << "\t" << "访问次数:" << cur->weight << '\t' << "编码:";
			else if (cur->ch == '\n')
				out << "字符:" << "LineFeed" << "\t" << "访问次数:" << cur->weight << '\t' << "编码:";
			else
				out << "字符:" << cur->ch << "\t\t" << "访问次数:" << cur->weight << '\t' << "编码:";
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
	//除二取余法
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