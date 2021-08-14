#include<iostream>
using namespace std;
int board[31][31];//棋盘
int temp[31][31];//用于记录要删除的点
int row, col;
int counts = 0;//用于记录连续相同颜色棋子的个数
void solve()
{
    
    //进行列搜索
    for (int j = 0; j < col; j++) {
        for (int i = 0; i < row - 2; i++) {
            if (board[i][j] == board[i + 1][j] && board[i + 1][j] == board[i + 2][j])
                temp[i][j] = temp[i + 1][j] = temp[i + 2][j] = 0;
        }
    }
    //进行行搜索
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col - 2; j++) {
            if (board[i][j] == board[i][j + 2] && board[i][j + 1] == board[i][j + 2])
                temp[i][j] = temp[i][j + 1] = temp[i][j + 2] = 0;
        }
    }
}
int main()
{
    int color;
    cin >> row >> col;
    //输入面板
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            cin >> color;
            board[i][j] = color;
            temp[i][j] = 1;
        }
    }
    solve();
    //对所有要消除的点置为0
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if(temp[i][j] == 0)
                board[i][j] = 0;
        }
    }
    //输出消除后的棋盘
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            cout << board[i][j] << ' ';
        }
        cout << endl;
    }
    return 0;
}
