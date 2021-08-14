#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<cstring>
#include<queue>
#include<stack>
#define MAX 10
using namespace std;
//--------------�ṹ�嶨��------------//

typedef char Elem;
typedef struct {
	int num;//��������
	int alive;//�Ƿ���(1��ʾ���ڣ�0��ʾ����)
	int marry;//�Ƿ���(1��ʾ�ѻ飬0��ʾδ�飩
	string name;//����
	string birth_data;//��������
	string die_data;//��������
	string address;//��ַ
} TData;

typedef struct tree{
	TData data;//������
	struct tree* child[MAX];//����ָ��
	struct tree* parent = NULL;//˫��ָ��
}Tree;
Tree tree;

//------------��������---------------//

int menu_select();
int level(Tree* t);//ȷ������Ϊ�ڼ���
Tree* treeNew(string name, string birth_data, string address);//����һ������
void showAllInformation();				//��ͼ�η�ʽ��ʾ����
void showInformation(Tree* tree);		//��ʾĳ���˵���Ϣ
void showNInformation(int n);			//��ʾ��n�������˵���Ϣ
Tree* searchPersonName(string name);	//����������ѯ�������Ա��Ϣ�������䱾�ˡ����ס����ӵ���Ϣ����
void searchPersonData(string data);		//���ճ������ڲ�ѯ��Ա����
void relation(string name1, string name2);//��������������ȷ�����ϵ
void addPerson(string p);				//ĳ��Ա��Ӻ���
void deletePerson(string name);			//ɾ��ĳ��Ա�����仹�к������һ��ɾ����
void changeInfo(string name);			//�޸�ĳ��Ա��Ϣ
void saveToFile();						//�ļ��洢�Ͷ�ȡ
Tree* ImportDatabase();		//�����ݿ����Ϣ����


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
			cout << "������Ҫ���ҵڼ�������Ϣ��";
			cin >> n;
			showNInformation(n);
			system("pause");
			break; 
		}
		case 3: {
			cout << "��������Ҫ���ҵ����֣�";
			string name;
			cin >> name;
			searchPersonName(name);
			system("pause");
			break;
		}
		case 4: {
			cout << "��������Ҫ���ҵĳ������ڣ�����20010202��ʾ2002��2��2�գ�:";
			string data;
			cin >> data;
			searchPersonData(data);
			system("pause");
			break;
		}
		case 5: {
			cout << "��������Ҫ���ҵ������˵��������ÿո���������磺���� �����£���";
			string name1, name2;
			cin >> name1 >> name2;
			relation(name1, name2);
			system("pause");
			break; 
		}
		case 6: {
			string parent;
			cout << "��������Ҫ���ĸ���Ա��Ӻ��ӣ����磺���ƣ���";
			cin >> parent;
			addPerson(parent);
			system("pause");
			break;
		}
		case 7: {
			string name;
			cout << "��������Ҫɾ���ĳ�Ա�����磺���ƣ���";
			cin >> name;
			deletePerson(name);
			system("pause");
			break;
		}
		case 8: {
			string name;
			cout << "��������Ҫ�޸ĵĳ�Ա�����磺���ƣ���";
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
			cout << "�����Ƿ�ȷ��Ҫ�˳�(y/n)" << endl;
			cin >> ch;
			if (ch == 'y') {
				saveToFile();//�˳�ǰ�ȱ�������
				cout << "	***************************************************   " << endl;
				cout << "	*                                                 *   " << endl;
				cout << "	*                ��л����ʹ��!                    *   " << endl;
				cout << "	*                                                 *   " << endl;
				cout << "	*      designer:�ﺣ��     std_number:181910720   *   " << endl;
				cout << "	*                                                 *   " << endl;
				cout << "	***************************************************   " << endl;
				system("pause");
				exit(0);
			}
		}
	}
	return 0;
}

int level(Tree* t)//ȷ������Ϊ�ڼ���
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
		cout << "�ڴ���䲻�ɹ���" << endl;
		return NULL;
	}
}

void showInformation(Tree* tree)
{
	if (tree == NULL) {
		cout << "���˲����ڣ�" << endl;
		return;
	}
	cout << "������" << tree->data.name;
	if (tree->data.alive == 1)
		cout << "��" << tree->data.birth_data << "����	��" << ",";
	else
		cout << "��" << tree->data.birth_data << "����" << tree->data.die_data << "��,";
	cout << "��ַ��" << tree->data.address << ",";
	if (tree->data.marry == 1)
		cout << "�ѻ�\n" << endl;
	else
		cout << "δ��\n" << endl;

}

void showNInformation(int n)			//��ʾ��n�������˵���Ϣ
{
	queue<Tree*> q;
	int flag = 0;//�����ж��Ƿ��ҵ���n����
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
		cout << "�������룬������û�е�" << n << "����" << endl;
	cout << endl;
}

void showAllInformation()
{
	Tree* flag1 = treeNew("[", "0", "0");//��������ÿһ�㸸�׺ͺ��ӵĹ�ϵ
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

Tree* searchPersonName(string name)	//����������ѯ�������Ա��Ϣ�������䱾�ˡ����ס����ӵ���Ϣ����			
{
	queue<Tree*> q;
	q.push(&tree);
	while (!q.empty()) {
		Tree* t = q.front();
		q.pop();

		if (t->data.name == name) {
			cout << "\n���������ڼ����в��ҳɹ���" << endl;
			showInformation(t);
			cout << "����";
			if (t->parent)
				showInformation(t->parent);
			else
				cout << name << "�����ȣ��Ҳ�������Ŷ��" << endl;
			for (int i = 0; i < t->data.num; i++) {
				cout << "����" << i + 1;
				showInformation(t->child[i]);
			}
			if (t->data.num == 0)
				cout << name << "��û�к���Ŷ��" << endl;
			return t;
		}
		
		for (int i = 0; i < t->data.num; i++) {
			q.push(t->child[i]);
		}
	}

	cout << "�����в��޴��ˣ���������������Ƿ�����" << endl;
	return NULL;
}

void searchPersonData(string data)
{
	queue<Tree*> q;
	q.push(&tree);
	int flag = 0;//�ж��Ƿ��ڼ������ҵ�
	while (!q.empty()) {
		Tree* t = q.front();
		q.pop();

		if (t->data.birth_data == data) {
			if(flag == 0)
				cout << "���ճ��������ڼ����в��ҳɹ���" << endl;
			flag = 1;
			showInformation(t);
		}

		for (int i = 0; i < t->data.num; i++) {
			q.push(t->child[i]);
		}
	}
	if(flag == 0)
		cout << "�����в��޴��ˣ���������ĳ��������Ƿ�����" << endl;
}

void relation(string name1, string name2)//��������������ȷ�����ϵ
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
	//������ҵ�ǰ��ͬһ�����ֵ���
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
	//Ȼ������ջͬʱ���ϱ���
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

void addPerson(string parent)						//ĳ��Ա��Ӻ���
{
	Tree* p = searchPersonName(parent);
	if (p) {
		string name, data, address;
		int alive;
		int marry;
		cout << "������ú��ӵ����������գ���ַ�����磺������ 20200202 ����ʡ�Ͼ��У���";
		cin >> name >> data >> address;
		Tree* child = treeNew(name, data, address);


		cout << "���ʸú����Ƿ��ڣ�1��ʾ���ڣ�0��ʾȥ������";
		while (true) {
			cin >> alive;
			if (alive != 1 && alive != 0) {
				cout << "�����������������룡" << endl;
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
			cout << "�������������ڣ����磺20201213��:";
			cin >> die_data;
			child->data.die_data = die_data;
		}

		cout << "���ʸú����Ƿ��飨1��ʾ�ѻ飬0��ʾδ�飩��";
		while (true) {
			cin >> marry;
			if (marry != 1 && marry != 0) {
				cout << "�����������������룡" << endl;
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

		//�����Ӻ͸�����������
		child->parent = p;
		p->child[p->data.num] = child;
		p->data.num++;
	}
	else
		return;
}

void deletePerson(string name)			//ɾ��ĳ��Ա�����仹�к������һ��ɾ����
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
		cout << "\nɾ���ɹ���\n";
	}
	else {
		cout << "ɾ��ʧ�ܣ�\n";
		return;
	}
}

void changeInfo(string name)			//�޸�ĳ��Ա��Ϣ
{
	Tree* t = searchPersonName(name);
	if (t) {
		showInformation(t);
		int n;
		string name1, birth_data, address, die_data;
		cout << "�Ƿ���Ҫ�޸����֣�1��ʾ�ǣ�0��ʾ�񣩣�";
		cin >> n;
		if (n == 1) {
			cout << "�������޸ĺ�����֣����磺���ƣ���";
			cin >> name1;
			t->data.name = name1;
		}

		cout << "�Ƿ���Ҫ�޸ĵ�ַ��1��ʾ�ǣ�0��ʾ�񣩣�";
		cin >> n;
		if (n == 1) {
			cout << "�������޸ĺ�ĵ�ַ�����磺����ʡ�Ͼ��У���";
			cin >> address;
			t->data.address = address;
		}

		cout << "�Ƿ���Ҫ�޸ĳ������ڣ�1��ʾ�ǣ�0��ʾ�񣩣�";
		cin >> n;
		if (n == 1) {
			cout << "�������޸ĺ�ĳ������ڣ����磺20020202��ʾ2002��2��2�գ���";
			cin >> birth_data;
			t->data.birth_data = birth_data;
		}

		cout << "�Ƿ���Ҫ�޸Ļ�����Ϣ��1��ʾ�ǣ�0��ʾ�񣩣�";
		cin >> n;
		if (n == 1) {
			int marry;
			cout << "���������״̬��1��ʾ�ѻ飬0��ʾδ�飩��";
			cin >> marry;
			t->data.marry = marry;
		}

		cout << "�Ƿ���Ҫ�޸�������Ϣ��1��ʾ�ǣ�0��ʾ�񣩣�";
		cin >> n;
		if (n == 1) {
			int flag;
			cout << "���ʴ����Ƿ��ڣ�1��ʾ���ڣ�0��ʾ������";
			cin >> flag;
			if (flag == 1)
				t->data.alive = 1;
			if (flag == 0) {
				cout << "�������������ڣ����磺20201213��ʾ2020��12��13�գ���";
				cin >> die_data;
				t->data.die_data = die_data;
			}
		}

		cout << "\n�޸���ɣ��޸ĺ����Ϣ���£�\n";
		showInformation(t);
	}
	else
		return;
}

void saveToFile()						//�ļ��洢�Ͷ�ȡ
{
	ofstream outfile("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\datafile.txt");
	queue<Tree*> q;
	q.push(&tree);
	while (!q.empty()) {
		Tree* t = q.front();
		q.pop();
		outfile << t->data.name << ' ' << t->data.num << ' ' << t->data.alive << ' ' << t->data.birth_data << ' ';
		if (t->data.alive == 1) {
			outfile << "��������" << ' ' << t->data.marry << ' ' << t->data.address << endl;
		}
		else {
			outfile << t->data.die_data << ' ' << t->data.marry << ' ' << t->data.address << endl;
		}
		for (int i = 0; i < t->data.num; i++) {
			q.push(t->child[i]);
		}
	}//end while
}

Tree* ImportDatabase()//�Ƚ������˶�����У�Ȼ�����ν��ɼ���
{
	Tree* tree = new Tree;
	queue<Tree*> q;//������е��Ӵ�
	char ch;
	char temp[100];
	memset(temp, 0, sizeof(temp));
	int i = 0;
	int flag = 0;//�ж϶����ĸ��ַ�������
	int k = 0;//�ж�tab�Ϳո����������
	ifstream infile("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\database.txt");
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

	queue<Tree*> q2 = q;//���������Ӵ��͸���
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
	cout << "�����ݿ⵼��ɹ���" << endl;
	return root;
}

int menu_select()
{
	char* menu[11] = {
				 "     1.  ��ʾ������Ϣ",
				 "     2.  ��ʾ��n�������˵���Ϣ",
				 "     3.  ����������ѯ��Ա����",
				 "     4.  ���ճ������ڲ�ѯ��Ա����",
				 "     5.  ��������������ȷ�����ϵ",
				 "     6.  ����µļ�ͥ��Ա",
				 "     7.  ɾ����Ա",
				 "     8.  �޸ĳ�Ա��Ϣ",
				 "     9.  �����������ļ�",
				 "     0.  �˳�",
				 NULL };
	int choice;
	do {
		system("cls"); //  ����
		cin.clear();//�������״̬
		cin.ignore(cin.rdbuf()->in_avail());//���������
		cout << "\n =================���׹���ϵͳϵͳ================= \n\n";
		for (int i = 0; menu[i]; i++)
			cout << menu[i] << endl;
		cout << "\n ================================================== \n";

		cout << "������ѡ��:";
		cin >> choice;

	} while (choice < 0 || choice > 9);

	return (choice);
}