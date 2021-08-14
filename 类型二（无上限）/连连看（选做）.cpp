#include<iostream>
#include<cstdlib>
#include<ctime>
#include<algorithm>
#include<windows.h> 
//���ڵ�ɫ
void setColour(int x) {
    HANDLE h = GetStdHandle(-11);
    SetConsoleTextAttribute(h, x);
}

using namespace std;
int matrix[10 + 2][20 + 2];//����ͼ����һȦ�����ߵ�·��

bool horizontalCheck(int x1, int y1, int x2, int y2)
{
    if (x1 == x2 && y1 == y2) {
        cout << "����ѡ�е���ͬһ�����飬������ѡ��" << endl;
        return false;
    }
    if (x1 != x2)//��λ��ͬһ��
        return false;
    int min_y = min(y1, y2);
    int max_y = max(y1, y2);
    if (max_y - min_y == 1)//������������
        return true;
    //����������߼��з�������������
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
    if (y1 != y2)//��λ��ͬһ��
        return false;
    int min_x = min(x1, x2);
    int max_x = max(x1, x2);
    if (max_x - min_x == 1)//������������
        return true;
    //����������߼��з�������������
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
    //��¼�յ�
    int p1 = x1;
    int p2 = y2;
    int q1 = x2;
    int q2 = y1;
    //���ڹյ�˵����������ͬһ�к�ͬһ��
    if (x1 != x2 && y1 != y2) {
        //�յ�û�з���
        if (matrix[p1][p2] == -1)
            if (horizontalCheck(x1, y1, p1, p2) && verticalCheck(p1, p2, x2, y2))
                return true;
        //�յ�û�з���
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
            //��Ϊ�յķ��鲻����Ϊ�յ�
            if (matrix[i][j] != -1)
                continue;
            //�뱻ѡ�㲻��ͬһ�л�ͬһ�еĲ�����Ϊ�յ�
            if (i != x1 && i != x2 && j != y1 && j != y2)
                continue;
            //��һ���յ���ͬ������ų�
            if ((i == x1 && j == y2) || (i == x2 && j == y1))
                continue;
            //�����սǼ�� = һ���սǼ�� && (ˮƽ��� || ��ֱ���)
            if (turnOnceCheck(x1, y1, i, j) && (horizontalCheck(i, j, x2, y2) || verticalCheck(i, j, x2, y2)))
                return true;
            if (turnOnceCheck(i, j, x2, y2) && (horizontalCheck(x1, y1, i, j) || verticalCheck(x1, y1, i, j)))
                return true;
            //�����սǷֱ��ڷ������������������ս�λ��ͬһ�л���ͬһ��
          
        }
    }
    return false;
}

bool canConnect(int x1, int y1, int x2, int y2)
{
    //���жϺ������ͨ
    if (horizontalCheck(x1, y1, x2, y2))
        return true;
    //���ж��������ͨ
    if (verticalCheck(x1, y1, x2, y2))
        return true;
    //�ж�һ���յ�Ŀ���ͨ���
    if (turnOnceCheck(x1, y1, x2, y2))
        return true;
    //�ж������յ�Ŀ���ͨ���
    if (turnTwiceCheck(x1, y1, x2, y2))
        return true;
    //��������ͨ������false
    return false;
}

int arr[10];//���ڼ�¼��ͼ����������ȷ��Ϊż�� 
void start()
{
    srand(time(NULL));
    memset(matrix, -1, sizeof(matrix));//��ʼ��ȫΪ-1

    //�������ԭʼ���ݣ�������ǰ9��
    for (int i = 1; i < 10; i++) {
        for (int j = 1; j < 21; j++) {
            matrix[i][j] = rand() % 10;
            arr[matrix[i][j]]++;
        }
    }
    //���ɵ�ʮ�У����˹�ȷ��Ϊż��
    for (int j = 1; j < 21; j++) {
        if (j <= 10) {
            matrix[10][j] = rand() % 10;
            arr[matrix[10][j]]++;
        }
        else {
            int flag = 0;//������������ͼ��
            for (int i = 0; i < 10; i++) {
                //�����Ϊż��
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
    cout << "\n������������ͬ���������ֵ(�±��1��ʼ):\n";
    cin >> x1 >> y1 >> x2 >> y2;
    if (matrix[x1][y1] != matrix[x2][y2]) {
        cout << "��ѡͼ������ͬ��ɾ��ʧ�ܣ�" << endl;
    }
    else if (canConnect(x1, y1, x2, y2)) {
        cout << "�����ɹ���" << endl;
        matrix[x1][y1] = -1;
        matrix[x2][y2] = -1;
    }
    else
        cout << "���������ڵ���3��ֱ��������ɾ��ʧ�ܣ�" << endl;
}

int main()
{
    while (true) {
        system("cls");
        cout << "1����ʼ��Ϸ\n"
                "2��������Ϸ\n"
                "3��������Ϸ\n";
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
            cout << "��л����ʹ�ã�" << endl;
            return 0;
        default:
            cout << "�����������������룡" << endl;
            system("pause");
            continue;
        }
        system("pause");
    }
	return 0;
}
