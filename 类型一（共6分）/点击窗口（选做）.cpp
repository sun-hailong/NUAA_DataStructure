#include<iostream>
#include<algorithm>
using namespace std;
typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;//窗口的一对顶点坐标
    int no;//窗口的序号
}Node;
Node node[10];
int main()
{
    int n, m;
    int x, y;
    cin >> n >> m;
    //输入所有窗口
    for (int i = 0; i < n; i++) {
        cin >> node[i].x1 >> node[i].y1 >> node[i].x2 >> node[i].y2;
        node[i].no = i + 1;
    }
    //输入鼠标点击的坐标并排序
    for (int i = 0; i < m; i++) {
        cin >> x >> y;
        int flag = 0;//用于判断鼠标点击处是否有窗口
        for (int j = n - 1; j >= 0; j--) {
            //从顶层往底层遍历
            if (x >= node[j].x1 && x <= node[j].x2 && y >= node[j].y1 && y <= node[j].y2) {
                flag = 1;
                cout << node[j].no << endl;
                for (int k = j; k < n - 1; k++) {
                    swap(node[k], node[k + 1]);
                }
                break;
            }
        }
        if (flag == 0)
            cout << "IGNORED" << endl;
    }

    return 0;
}