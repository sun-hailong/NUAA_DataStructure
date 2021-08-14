#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<cstring>
#include<queue>
#include<stack>
#define MAX 10
using namespace std;
//--------------结构体定义------------//

typedef char Elem;
typedef struct {
	int num;//孩子数量
	int alive;//是否健在(1表示健在，0表示离世)
	int marry;//是否结婚(1表示已婚，0表示未婚）
	string name;//姓名
	string birth_data;//出生日期
	string die_data;//死亡日期
	string address;//地址
} TData;

typedef struct tree{
	TData data;//数据域
	struct tree* child[MAX];//孩子指针
	struct tree* parent = NULL;//双亲指针
}Tree;
Tree tree;

//------------函数声明---------------//

int menu_select();
int level(Tree* t);//确定此人为第几代
Tree* treeNew(string name, string birth_data, string address);//构建一棵新树
void showAllInformation();				//以图形方式显示家谱
void showInformation(Tree* tree);		//显示某个人的信息
void showNInformation(int n);			//显示第n代所有人的信息
Tree* searchPersonName(string name);	//按照姓名查询，输出成员信息（包括其本人、父亲、孩子的信息）。
void searchPersonData(string data);		//按照出生日期查询成员名单
void relation(string name1, string name2);//输入两人姓名，确定其关系
void addPerson(string p);				//某成员添加孩子
void deletePerson(string name);			//删除某成员（若其还有后代，则一并删除）
void changeInfo(string name);			//修改某成员信息
void saveToFile();						//文件存储和读取
Tree* ImportDatabase();		//将数据库的信息读入


int main()
{
	tree = *ImportDatabase();
	int n;
	while (true) {
		system("cls");
		int choice = menu_select();
		switch (choice) {
		case 1: {
			showAllInformation();
			system("pause");
			break; 
		}
		case 2: {
			cout << "请问需要查找第几代的信息：";
			cin >> n;
			showNInformation(n);
			system("pause");
			break; 
		}
		case 3: {
			cout << "请输入需要查找的名字：";
			string name;
			cin >> name;
			searchPersonName(name);
			system("pause");
			break;
		}
		case 4: {
			cout << "请输入需要查找的出生日期（例如20010202表示2002年2月2日）:";
			string data;
			cin >> data;
			searchPersonData(data);
			system("pause");
			break;
		}
		case 5: {
			cout << "请输入需要查找的两个人的姓名（用空格隔开，例如：王浩 王亮德）：";
			string name1, name2;
			cin >> name1 >> name2;
			relation(name1, name2);
			system("pause");
			break; 
		}
		case 6: {
			string parent;
			cout << "请输入需要给哪个成员添加孩子（例如：王浩）：";
			cin >> parent;
			addPerson(parent);
			system("pause");
			break;
		}
		case 7: {
			string name;
			cout << "请输入需要删除的成员（例如：王浩）：";
			cin >> name;
			deletePerson(name);
			system("pause");
			break;
		}
		case 8: {
			string name;
			cout << "请输入需要修改的成员（例如：王浩）：";
			cin >> name;
			changeInfo(name);
			system("pause");
			break;
		}
		case 9:
			saveToFile();
			system("pause");
			break;
		case 0:
			char ch;
			cout << "请问是否确定要退出(y/n)" << endl;
			cin >> ch;
			if (ch == 'y') {
				saveToFile();//退出前先保存数据
				cout << "	***************************************************   " << endl;
				cout << "	*                                                 *   " << endl;
				cout << "	*                感谢您的使用!                    *   " << endl;
				cout << "	*                                                 *   " << endl;
				cout << "	*      designer:孙海龙     std_number:181910720   *   " << endl;
				cout << "	*                                                 *   " << endl;
				cout << "	***************************************************   " << endl;
				system("pause");
				exit(0);
			}
		}
	}
	return 0;
}

int level(Tree* t)//确定此人为第几代
{
	queue<Tree*> q;
	int i = 1;
	q.push(&tree);
	q.push(NULL);
	while (q.size() != 1) {
		Tree* p = q.front();
		q.pop();
		if (p == NULL) {
			i++;
			q.push(NULL);
			continue;
		}
		if (p == t)
			return i;
		for (int j = 0; j < p->data.num; j++)
			q.push(p->child[j]);
	}
}

Tree* treeNew(string name, string birth_data, string address)
{
	Tree* tree = new Tree;
	if (tree) {
		tree->parent = NULL;
		tree->data.alive = 0;
		tree->data.address = address;
		tree->data.num = 0;
		tree->data.marry = 0;
		tree->data.name = name;
		tree->data.birth_data = birth_data;
		return tree;
	}
	else {
		cout << "内存分配不成功！" << endl;
		return NULL;
	}
}

void showInformation(Tree* tree)
{
	if (tree == NULL) {
		cout << "此人不存在！" << endl;
		return;
	}
	cout << "姓名：" << tree->data.name;
	if (tree->data.alive == 1)
		cout << "（" << tree->data.birth_data << "——	）" << ",";
	else
		cout << "（" << tree->data.birth_data << "——" << tree->data.die_data << "）,";
	cout << "地址：" << tree->data.address << ",";
	if (tree->data.marry == 1)
		cout << "已婚\n" << endl;
	else
		cout << "未婚\n" << endl;

}

void showNInformation(int n)			//显示第n代所有人的信息
{
	queue<Tree*> q;
	int flag = 0;//用于判断是否找到第n代人
	int i = 1;
	q.push(&tree);
	q.push(NULL);
	while (q.size() != 1) {
		Tree* t = q.front();
		q.pop();

		if (t == NULL) {
			q.push(NULL);
			i++;
			continue;
		}

		if (i == n) {
			flag = 1;
			cout << t->data.name << " ";
		}

		if (i > n)
			break;

		for (int i = 0; i < t->data.num; i++) {
			q.push(t->child[i]);
		}
	}
	if (flag == 0)
		cout << "请检查输入，家谱中没有第" << n << "代！" << endl;
	cout << endl;
}

void showAllInformation()
{
	Tree* flag1 = treeNew("[", "0", "0");//用于区分每一层父亲和孩子的关系
	Tree* flag2 = treeNew("]", "0", "0");
	queue<Tree*> q;
	q.push(flag1);
	q.push(&tree);
	q.push(flag2);
	q.push(NULL);
	while (q.size() != 1) {
		Tree* t = q.front();
		q.pop();
		if (t == NULL) {
			q.push(NULL);
			cout << endl;
			continue;
		}
		if (t == flag1) {
			cout << "[ ";
			continue;
		}
		if (t == flag2) {
			cout << "] ";
			continue;
		}
		cout << t->data.name << " ";
		q.push(flag1);
		for (int i = 0; i < t->data.num; i++) {
			q.push(t->child[i]);
		}
		q.push(flag2);	
	}
	cout << endl;
}

Tree* searchPersonName(string name)	//按照姓名查询，输出成员信息（包括其本人、父亲、孩子的信息）。			
{
	queue<Tree*> q;
	q.push(&tree);
	while (!q.empty()) {
		Tree* t = q.front();
		q.pop();

		if (t->data.name == name) {
			cout << "\n按照姓名在家谱中查找成功！" << endl;
			showInformation(t);
			cout << "父亲";
			if (t->parent)
				showInformation(t->parent);
			else
				cout << name << "是祖先，找不到父亲哦！" << endl;
			for (int i = 0; i < t->data.num; i++) {
				cout << "孩子" << i + 1;
				showInformation(t->child[i]);
			}
			if (t->data.num == 0)
				cout << name << "还没有孩子哦！" << endl;
			return t;
		}
		
		for (int i = 0; i < t->data.num; i++) {
			q.push(t->child[i]);
		}
	}

	cout << "家谱中查无此人，请检查输入的名字是否有误！" << endl;
	return NULL;
}

void searchPersonData(string data)
{
	queue<Tree*> q;
	q.push(&tree);
	int flag = 0;//判断是否在家谱中找到
	while (!q.empty()) {
		Tree* t = q.front();
		q.pop();

		if (t->data.birth_data == data) {
			if(flag == 0)
				cout << "按照出生日期在家谱中查找成功！" << endl;
			flag = 1;
			showInformation(t);
		}

		for (int i = 0; i < t->data.num; i++) {
			q.push(t->child[i]);
		}
	}
	if(flag == 0)
		cout << "家谱中查无此人，请检查输入的出生日期是否有误！" << endl;
}

void relation(string name1, string name2)//输入两人姓名，确定其关系
{
	Tree* tree1 = searchPersonName(name1);
	Tree* tree2 = searchPersonName(name2);
	if (!tree1 || !tree2)
		return;
	int i = level(tree1);
	int j = level(tree2);
	stack<Tree*> s1;
	stack<Tree*> s2;
	s1.push(tree1);
	s2.push(tree2);
	//后代先找到前辈同一级辈分的人
	while (i < j) {
		tree2 = tree2->parent;
		s2.push(tree2);
		i++;
	}
	while (j < i) {
		tree1 = tree1->parent;
		s1.push(tree1);
		j++;
	}
	//然后两个栈同时向上遍历
	while (tree1 != tree2) {
		tree1 = tree1->parent;
		tree2 = tree2->parent;
		s1.push(tree1);
		s2.push(tree2);
	}
	while (!s1.empty()) {
		Tree* t = s1.top();
		s1.pop();
		cout << t->data.name << ' ';
	}
	cout << endl;
	while (!s2.empty()) {
		Tree* t = s2.top();
		s2.pop();
		cout << t->data.name << ' ';
	}
	cout << endl;
}

void addPerson(string parent)						//某成员添加孩子
{
	Tree* p = searchPersonName(parent);
	if (p) {
		string name, data, address;
		int alive;
		int marry;
		cout << "请输入该孩子的姓名，生日，地址（例如：王亮德 20200202 江苏省南京市）：";
		cin >> name >> data >> address;
		Tree* child = treeNew(name, data, address);


		cout << "请问该孩子是否健在（1表示健在，0表示去世）：";
		while (true) {
			cin >> alive;
			if (alive != 1 && alive != 0) {
				cout << "输入有误，请重新输入！" << endl;
			}
			else {
				break;
			}
		}//end while

		if (alive == 1) {
			child->data.alive = 1;
		}

		else {
			child->data.alive = 0;
			string die_data;
			cout << "请输入死亡日期（例如：20201213）:";
			cin >> die_data;
			child->data.die_data = die_data;
		}

		cout << "请问该孩子是否结婚（1表示已婚，0表示未婚）：";
		while (true) {
			cin >> marry;
			if (marry != 1 && marry != 0) {
				cout << "输入有误，请重新输入！" << endl;
			}
			else {
				break;
			}
		}//end while

		if (marry == 1) {
			child->data.marry = 1;
		}

		else {
			child->data.marry = 0;
		}

		//将孩子和父亲连接起来
		child->parent = p;
		p->child[p->data.num] = child;
		p->data.num++;
	}
	else
		return;
}

void deletePerson(string name)			//删除某成员（若其还有后代，则一并删除）
{
	Tree* t = searchPersonName(name);
	if (t) {
		Tree* parent = t->parent;
		int i;
		for (i = 0; i < t->data.num; i++) {
			if (parent->child[i] == t)
				break;
		}
		for (int j = i; j < parent->data.num - 1; j++) {
			parent->child[j] = parent->child[j + 1];
		}
		parent->data.num--;
		free(t);
		cout << "\n删除成功！\n";
	}
	else {
		cout << "删除失败！\n";
		return;
	}
}

void changeInfo(string name)			//修改某成员信息
{
	Tree* t = searchPersonName(name);
	if (t) {
		showInformation(t);
		int n;
		string name1, birth_data, address, die_data;
		cout << "是否需要修改名字（1表示是，0表示否）：";
		cin >> n;
		if (n == 1) {
			cout << "请输入修改后的名字（例如：王浩）：";
			cin >> name1;
			t->data.name = name1;
		}

		cout << "是否需要修改地址（1表示是，0表示否）：";
		cin >> n;
		if (n == 1) {
			cout << "请输入修改后的地址（例如：江苏省南京市）：";
			cin >> address;
			t->data.address = address;
		}

		cout << "是否需要修改出生日期（1表示是，0表示否）：";
		cin >> n;
		if (n == 1) {
			cout << "请输入修改后的出生日期（例如：20020202表示2002年2月2日）：";
			cin >> birth_data;
			t->data.birth_data = birth_data;
		}

		cout << "是否需要修改婚姻信息（1表示是，0表示否）：";
		cin >> n;
		if (n == 1) {
			int marry;
			cout << "请输入婚姻状态（1表示已婚，0表示未婚）：";
			cin >> marry;
			t->data.marry = marry;
		}

		cout << "是否需要修改死亡信息（1表示是，0表示否）：";
		cin >> n;
		if (n == 1) {
			int flag;
			cout << "请问此人是否健在（1表示健在，0表示离世）";
			cin >> flag;
			if (flag == 1)
				t->data.alive = 1;
			if (flag == 0) {
				cout << "请输入死亡日期（例如：20201213表示2020年12月13日）：";
				cin >> die_data;
				t->data.die_data = die_data;
			}
		}

		cout << "\n修改完成！修改后的信息如下：\n";
		showInformation(t);
	}
	else
		return;
}

void saveToFile()						//文件存储和读取
{
	ofstream outfile("E:\\程序们\\数据结构\\数据结构课设\\必做题\\datafile.txt");
	queue<Tree*> q;
	q.push(&tree);
	while (!q.empty()) {
		Tree* t = q.front();
		q.pop();
		outfile << t->data.name << ' ' << t->data.num << ' ' << t->data.alive << ' ' << t->data.birth_data << ' ';
		if (t->data.alive == 1) {
			outfile << "————" << ' ' << t->data.marry << ' ' << t->data.address << endl;
		}
		else {
			outfile << t->data.die_data << ' ' << t->data.marry << ' ' << t->data.address << endl;
		}
		for (int i = 0; i < t->data.num; i++) {
			q.push(t->child[i]);
		}
	}//end while
}

Tree* ImportDatabase()//先将所有人读入队列，然后依次建成家谱
{
	Tree* tree = new Tree;
	queue<Tree*> q;//存放所有的子代
	char ch;
	char temp[100];
	memset(temp, 0, sizeof(temp));
	int i = 0;
	int flag = 0;//判断读到哪个字符部分了
	int k = 0;//判断tab和空格的连续个数
	ifstream infile("E:\\程序们\\数据结构\\数据结构课设\\必做题\\database.txt");
	if (infile)
	{
		while (true) {
			infile.get(ch);
			//cout << "ch:" << ch;
			//system("pause");
			if (ch == ' ' || ch == '\t') {
				if (k >= 1)
					continue;
				k++;
				if (flag == 0) {
					tree->data.name = temp;
					//cout << "flag = 0 " << temp << endl;
				}
				else if (flag == 1) {
					tree->data.num = (int)temp[0] - 48;
					//cout << "flag = 1 " << (int)temp[0] - 48 << endl;
				}
				else if (flag == 2) {
					tree->data.alive = (int)temp[0] - 48;
					//cout << "flag = 2 " << (int)temp[0] - 48 << endl;
				}
				else if (flag == 3) {
					tree->data.birth_data = temp;
					//cout << "flag = 3 " << temp << endl;
				}
				else if (flag == 4) {
					if (tree->data.alive == 0)
						tree->data.die_data = temp;
					//cout << "flag = 4 " << temp << endl;
				}
				else if (flag == 5) {
					tree->data.marry = (int)temp[0] - 48;
					//cout << "flag = 5 " << (int)temp[0] - 48 << endl;
				}

				i = 0;
				memset(temp, 0, sizeof(temp));
				flag++;
				continue;
			}

			if (ch == '\n') {
				tree->data.address = temp;
				flag = 0;
				i = 0;
				k = 0;
				memset(temp, 0, sizeof(temp));
				//system("pause");
				q.push(tree);
				//showInformation(tree);
				tree = new Tree;
				continue;
			}

			if (infile.eof()) {
				tree->data.address = temp;
				q.push(tree);
				//showInformation(tree);
				break;
			}

			temp[i++] = ch;
			k = 0;
		}
	}
	infile.close();

	queue<Tree*> q2 = q;//用于连接子代和父亲
	q2.pop();
	Tree* root = q.front();
	while (!q.empty()) {
		Tree* temp = q.front();
		q.pop();
		for (int i = 0; i < temp->data.num; i++) {
			Tree* p = q2.front();
			q2.pop();
			temp->child[i] = p;
			p->parent = temp;
		}
	}
	cout << "从数据库导入成功！" << endl;
	return root;
}

int menu_select()
{
	char* menu[11] = {
				 "     1.  显示家谱信息",
				 "     2.  显示第n代所有人的信息",
				 "     3.  按照姓名查询成员名单",
				 "     4.  按照出生日期查询成员名单",
				 "     5.  输入两人姓名，确定其关系",
				 "     6.  添加新的家庭成员",
				 "     7.  删除成员",
				 "     8.  修改成员信息",
				 "     9.  保存数据至文件",
				 "     0.  退出",
				 NULL };
	int choice;
	do {
		system("cls"); //  清屏
		cin.clear();//清除出错状态
		cin.ignore(cin.rdbuf()->in_avail());//清除缓冲区
		cout << "\n =================家谱管理系统系统================= \n\n";
		for (int i = 0; menu[i]; i++)
			cout << menu[i] << endl;
		cout << "\n ================================================== \n";

		cout << "请输入选择:";
		cin >> choice;

	} while (choice < 0 || choice > 9);

	return (choice);
}