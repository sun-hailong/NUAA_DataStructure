#include<iostream>
#include<stack>
#include<fstream>
#include<windows.h> 
using namespace std;
#define GREEN SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN)
#define WHITE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7)
#define	RED	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED)
#define BLUE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE)
typedef struct {
	int x;
	int y;
}point;
point Start;
point End; //�Թ������յ�

typedef struct {
	int inc_x;//x������
	int inc_y;//y������
}direction;
direction dir[4] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };//�������������Ͻ���

int matrix[100][100]; //0��ʾ���ϰ���1��ʾ���ϰ���-1��ʾ�Ѿ��߹��ĵ�
int row, col;//���ڼ�¼�Թ�������������
void solve_migong()
{
	int solve_flag = 0;//�����ж��Ƿ��ҵ������߳����Թ�·��
	int stack_flag = 0;//�����жϵ�ջ�Ժ��Ƿ����ҵ�����һ��·,0��ʾû�е�ջ��1��ʾ�Ѿ���ջ
	stack<point> s;
	point current = Start;//��¼��ǰ�ߵ�������
	point prev = Start;//��¼ǰһ�ε����� 
	s.push(Start);//�����ѹջ���Ҳ��ܽ���ʼ����Ϊ-1����������
	//matrix[Start.x][Start.y] = -1;
	while (!s.empty()) {
		if (current.x == End.x && current.y == End.y) {
			solve_flag = 1;
			break;
		}
		int dir_flag = 0;//�����ж�Ŀǰ���Ƿ���ڿ��ߵķ���0Ϊû�п��ߵķ���
		for (int i = 0; i < 4; i++) {
			prev.x = current.x;
			prev.y = current.y;
			int x = current.x + dir[i].inc_x;
			int y = current.y + dir[i].inc_y;
			//cout << "x y:" << x << ',' << y << endl;//����
			//���Ŀǰ������곬���Թ���Χ��÷�����
			if (x >= col || y >= row || x < 0 || y < 0)
				continue;
			// �������ͨ·
			if (matrix[x][y] == 0) {
				matrix[x][y] = -1;
				current.x = x;
				current.y = y;
				//cout << "current:" << current.x << ',' << current.y << endl;//����
				if(stack_flag == 1)
					s.push(prev);
				s.push(current);//��Ŀǰ�ߵ��ĵ�ѹջ
				dir_flag = 1;
				break;
			}
		}
		//��������ڿ����ߵķ����򷵻���һ����
		if (dir_flag == 0) {
			//cout << "�ĸ����򶼶�������" << endl; // ����
			current = s.top();
			stack_flag = 1;
			//cout << "current:" << current.x << ',' << current.y << endl;//����
			s.pop();
		}
	}//end while

	if (solve_flag == 0) {
		cout << "�������ʼ����յ��ڴ��Թ��²�����·����" << endl;
		return;
	}
	else {
		while (!s.empty()) {
			point p = s.top();
			s.pop();
			matrix[p.x][p.y] = 2;
		}
		//����Թ�·��
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				if (matrix[i][j] == 1)
					WHITE;
				if (matrix[i][j] == 2)
					GREEN;
				if (matrix[i][j] == -1)
					RED;
				if (matrix[i][j] == 0)
					BLUE;
				if (matrix[i][j] == -1)
					cout << "x" << ' ';
				else
					cout << matrix[i][j] << ' ';
			}
			cout << endl;
		}
	}
	WHITE;
}

int main()
{
	ifstream infile("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\�Թ�·.txt");
	infile >> row >> col;
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			infile >> matrix[i][j];//���Թ����������
	infile.close();
	//����ģ���Թ�ͼ
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (matrix[i][j] == 1)
				WHITE;
			if (matrix[i][j] == 0)
				BLUE;
			cout << matrix[i][j] << ' ';
		}
		cout << endl;
	}
	WHITE;
	cout << "�������Թ��������յ�(x,y):" << endl;
	cin >> Start.x >> Start.y;
	if (Start.x < 0 || Start.x >= col || Start.y < 0 || Start.y >= row) {
		cout << "������겻���Թ��ڣ�" << endl;
		return 0;
	}
	cin >> End.x >> End.y;
	if (End.x < 0 || End.x >= col || End.y < 0 || End.y >= row) {
		cout << "�յ����겻���Թ��ڣ�" << endl;
		return 0;
	}
	solve_migong();

	return 0;
}
