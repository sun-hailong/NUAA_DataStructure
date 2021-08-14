#include<iostream>
#include<cstdlib>
#include<ctime>
#include<algorithm>
#include<windows.h> 
//用于调色
void setColour(int x) {
    HANDLE h = GetStdHandle(-11);
    SetConsoleTextAttribute(h, x);
}

using namespace std;
int matrix[10 + 2][20 + 2];//方格图外留一圈可以走的路径

bool horizontalCheck(int x1, int y1, int x2, int y2)
{
    if (x1 == x2 && y1 == y2) {
        cout << "两次选中的是同一个方块，请重新选择！" << endl;
        return false;
    }
    if (x1 != x2)//不位于同一列
        return false;
    int min_y = min(y1, y2);
    int max_y = max(y1, y2);
    if (max_y - min_y == 1)//两个方块相邻
        return true;
    //如果两者连线间有方块则不满足条件
    for (int j = min_y + 1; j < max_y; j++)
        if (matrix[x1][j] != -1)
            return false;
    return true;

}

bool verticalCheck(int x1, int y1, int x2, int y2)
{
    if (x1 == x2 && y1 == y2) {
        return false;
    }
    if (y1 != y2)//不位于同一行
        return false;
    int min_x = min(x1, x2);
    int max_x = max(x1, x2);
    if (max_x - min_x == 1)//两个方块相邻
        return true;
    //如果两者连线间有方块则不满足条件
    for (int i = min_x + 1; i < max_x; i++)
        if (matrix[i][y1] != -1)
            return false;
    return true;
}

bool turnOnceCheck(int x1, int y1, int x2, int y2)
{
    if (x1 == x2 && y1 == y2) {
        return false;
    }
    //记录拐点
    int p1 = x1;
    int p2 = y2;
    int q1 = x2;
    int q2 = y1;
    //存在拐点说明不可能在同一行和同一列
    if (x1 != x2 && y1 != y2) {
        //拐点没有方块
        if (matrix[p1][p2] == -1)
            if (horizontalCheck(x1, y1, p1, p2) && verticalCheck(p1, p2, x2, y2))
                return true;
        //拐点没有方块
        if (matrix[q1][q2] == -1)
            if (verticalCheck(x1, y1, q1, q2) && horizontalCheck(q1, q2, x2, y2))
                return true;
        return false;
    }
    return false;

}

bool turnTwiceCheck(int x1, int y1, int x2, int y2)
{
    if (x1 == x2 && y1 == y2) {
        return false;
    }
    
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 22; j++) {
            //不为空的方块不能作为拐点
            if (matrix[i][j] != -1)
                continue;
            //与被选点不在同一行或同一列的不能作为拐点
            if (i != x1 && i != x2 && j != y1 && j != y2)
                continue;
            //与一个拐点相同的情况排除
            if ((i == x1 && j == y2) || (i == x2 && j == y1))
                continue;
            //两个拐角检测 = 一个拐角检测 && (水平检测 || 垂直检测)
            if (turnOnceCheck(x1, y1, i, j) && (horizontalCheck(i, j, x2, y2) || verticalCheck(i, j, x2, y2)))
                return true;
            if (turnOnceCheck(i, j, x2, y2) && (horizontalCheck(x1, y1, i, j) || verticalCheck(x1, y1, i, j)))
                return true;
            //两个拐角分别于方块相连，并且两个拐角位于同一行或者同一列
          
        }
    }
    return false;
}

bool canConnect(int x1, int y1, int x2, int y2)
{
    //先判断横向可连通
    if (horizontalCheck(x1, y1, x2, y2))
        return true;
    //在判断纵向可连通
    if (verticalCheck(x1, y1, x2, y2))
        return true;
    //判断一个拐点的可连通情况
    if (turnOnceCheck(x1, y1, x2, y2))
        return true;
    //判断两个拐点的可连通情况
    if (turnTwiceCheck(x1, y1, x2, y2))
        return true;
    //都不可连通，返回false
    return false;
}

int arr[10];//用于记录各图案的数量，确保为偶数 
void start()
{
    srand(time(NULL));
    memset(matrix, -1, sizeof(matrix));//初始化全为-1

    //随机产生原始数据，先生成前9行
    for (int i = 1; i < 10; i++) {
        for (int j = 1; j < 21; j++) {
            matrix[i][j] = rand() % 10;
            arr[matrix[i][j]]++;
        }
    }
    //生成第十行，并人工确保为偶数
    for (int j = 1; j < 21; j++) {
        if (j <= 10) {
            matrix[10][j] = rand() % 10;
            arr[matrix[10][j]]++;
        }
        else {
            int flag = 0;//不存在奇数的图案
            for (int i = 0; i < 10; i++) {
                //如果不为偶数
                if (arr[i] % 2 != 0) {
                    matrix[10][j] = i;
                    arr[i]++;
                    flag = 1;
                    break;
                }
            }
            if (flag == 0) {
                matrix[10][j] = rand() % 10;;
                arr[matrix[10][j]]++;
            }
                
        }
    }
}

void play()
{
    cout << "-----------------------------------------\n";
    for (int i = 1; i < 11; i++) {
        for (int j = 1; j < 21; j++) {
            int t = matrix[i][j];
            setColour(t + 1);

                if (matrix[i][j] == -1)
                    cout << "  ";
                else
                    cout << matrix[i][j] << ' ';
        }
        cout << endl;
    }
    setColour(15);
    cout << "-----------------------------------------\n";
    int x1, x2, y1, y2;
    cout << "\n请输入两个不同方块的坐标值(下标从1开始):\n";
    cin >> x1 >> y1 >> x2 >> y2;
    if (matrix[x1][y1] != matrix[x2][y2]) {
        cout << "所选图案不相同，删除失败！" << endl;
    }
    else if (canConnect(x1, y1, x2, y2)) {
        cout << "消除成功！" << endl;
        matrix[x1][y1] = -1;
        matrix[x2][y2] = -1;
    }
    else
        cout << "不存在少于等于3的直线相连，删除失败！" << endl;
}

int main()
{
    while (true) {
        system("cls");
        cout << "1、开始游戏\n"
                "2、继续游戏\n"
                "3、结束游戏\n";
        char choice;
        cin >> choice;
        switch (choice)
        {
        case '1':
            start();
        case '2':
            play();
            break;
        case '3':
            cout << "感谢您的使用！" << endl;
            return 0;
        default:
            cout << "输入有误，请重新输入！" << endl;
            system("pause");
            continue;
        }
        system("pause");
    }
	return 0;
}
