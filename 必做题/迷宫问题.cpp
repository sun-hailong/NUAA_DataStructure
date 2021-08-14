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
point End; //迷宫起点和终点

typedef struct {
	int inc_x;//x的增量
	int inc_y;//y的增量
}direction;
direction dir[4] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };//方向按照右下左上进行

int matrix[100][100]; //0表示无障碍，1表示有障碍，-1表示已经走过的点
int row, col;//用于记录迷宫的行数与列数
void solve_migong()
{
	int solve_flag = 0;//用于判断是否找到可以走出的迷宫路线
	int stack_flag = 0;//用于判断弹栈以后是否能找到另外一条路,0表示没有弹栈，1表示已经弹栈
	stack<point> s;
	point current = Start;//记录当前走到的坐标
	point prev = Start;//记录前一次的坐标 
	s.push(Start);//将起点压栈，且不能将起始点置为-1，否则会出错
	//matrix[Start.x][Start.y] = -1;
	while (!s.empty()) {
		if (current.x == End.x && current.y == End.y) {
			solve_flag = 1;
			break;
		}
		int dir_flag = 0;//用于判断目前点是否存在可走的方向，0为没有可走的方向
		for (int i = 0; i < 4; i++) {
			prev.x = current.x;
			prev.y = current.y;
			int x = current.x + dir[i].inc_x;
			int y = current.y + dir[i].inc_y;
			//cout << "x y:" << x << ',' << y << endl;//调试
			//如果目前点的坐标超出迷宫范围则该方向不行
			if (x >= col || y >= row || x < 0 || y < 0)
				continue;
			// 如果存在通路
			if (matrix[x][y] == 0) {
				matrix[x][y] = -1;
				current.x = x;
				current.y = y;
				//cout << "current:" << current.x << ',' << current.y << endl;//调试
				if(stack_flag == 1)
					s.push(prev);
				s.push(current);//将目前走到的点压栈
				dir_flag = 1;
				break;
			}
		}
		//如果不存在可以走的方向，则返回上一个点
		if (dir_flag == 0) {
			//cout << "四个方向都堵死啦！" << endl; // 调试
			current = s.top();
			stack_flag = 1;
			//cout << "current:" << current.x << ',' << current.y << endl;//调试
			s.pop();
		}
	}//end while

	if (solve_flag == 0) {
		cout << "输入的起始点和终点在此迷宫下不存在路径！" << endl;
		return;
	}
	else {
		while (!s.empty()) {
			point p = s.top();
			s.pop();
			matrix[p.x][p.y] = 2;
		}
		//输出迷宫路径
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
	ifstream infile("E:\\程序们\\数据结构\\数据结构课设\\必做题\\迷宫路.txt");
	infile >> row >> col;
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			infile >> matrix[i][j];//将迷宫输入矩阵内
	infile.close();
	//生成模拟迷宫图
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
	cout << "请输入迷宫的起点和终点(x,y):" << endl;
	cin >> Start.x >> Start.y;
	if (Start.x < 0 || Start.x >= col || Start.y < 0 || Start.y >= row) {
		cout << "起点坐标不在迷宫内！" << endl;
		return 0;
	}
	cin >> End.x >> End.y;
	if (End.x < 0 || End.x >= col || End.y < 0 || End.y >= row) {
		cout << "终点坐标不在迷宫内！" << endl;
		return 0;
	}
	solve_migong();

	return 0;
}
