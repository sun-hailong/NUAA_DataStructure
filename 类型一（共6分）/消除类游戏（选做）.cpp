#include<iostream>
using namespace std;
int board[31][31];//����
int temp[31][31];//���ڼ�¼Ҫɾ���ĵ�
int row, col;
int counts = 0;//���ڼ�¼������ͬ��ɫ���ӵĸ���
void solve()
{
    
    //����������
    for (int j = 0; j < col; j++) {
        for (int i = 0; i < row - 2; i++) {
            if (board[i][j] == board[i + 1][j] && board[i + 1][j] == board[i + 2][j])
                temp[i][j] = temp[i + 1][j] = temp[i + 2][j] = 0;
        }
    }
    //����������
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
    //�������
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            cin >> color;
            board[i][j] = color;
            temp[i][j] = 1;
        }
    }
    solve();
    //������Ҫ�����ĵ���Ϊ0
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if(temp[i][j] == 0)
                board[i][j] = 0;
        }
    }
    //��������������
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            cout << board[i][j] << ' ';
        }
        cout << endl;
    }
    return 0;
}
