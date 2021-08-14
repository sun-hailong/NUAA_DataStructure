#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include<stack>
#include<queue>
using namespace std;
const int VERTEX_MAX = 10000;
const int INF = 10000;
//-------------------结构体定义---------------------//
typedef struct {
    char name[100];        //公交车的编号
    int count;     //公交车经过的站点数量
    int route[100]; //公交车的路线下标
} Bus;             //1000

typedef struct {
    int bus[100]; //经过该站点的公交车的下标
    int count;         //经过该站点的公交车数量
    char name[100];     //站点名称
} Station;       //10000      

typedef struct {
    Bus bus[1000];          //该城市的公交车
    Station station[10000]; //该城市的站点
    int BusCount;           //公交车数量
    int StationCount;       //站点数量
} Map;
Map map;

typedef struct {
    int prev;//上一站
    int cur;//当前站
    int bus;//上一站到本站的车号
} Edge;
Edge edge[10000];
int matrix[10000][10000];//邻接矩阵
int dist[VERTEX_MAX], path[VERTEX_MAX];
bool visit[VERTEX_MAX];//设置三个辅助数组

//----------------函数的声明---------------//
void InitMap();//初始化地图
void CreateMap();//根据文件信息创建地图
int LocateStation(char temp[]);//查找站点下标
int LocateBus(char temp[]);//查找公交车下标
void showBusInfo(char name[]);//展示公交车信息
void showStationInfo(char name[]);//展示站点信息
void SearchRoutes(char name1[], char name2[]);//查询路线
void UpdataBus(int sno, int station_no);//更新公交车信息
void UpdataStation(int sno, int bus_no);//更新站点信息
int CreateStation(int bus_no, char temp[]);//新建站点
void CreateBus(char temp[]);//新建公交车
void LeastStation(char name1[], char name2[]);//查询经过站点最少的乘车路线
void LeastTransfers(char name1[], char name2[]);//查询转车次数最少的乘车路线
void Dijkstra(int v1, int v2);//迪杰斯特拉求最短路径，即求站点最少
void CreateMatrix();//创建邻接矩阵
void menu();//菜单
void BFS(int v1, int v2);//广度优先遍历

//-------------------主函数-----------------//

int main()
{
    InitMap();
    CreateMap();
    CreateMatrix();
    menu();
	return 0;
}

//-----------------函数的定义----------------//
void InitMap()
{
    map.BusCount = 0;
    map.StationCount = 0;
    for (int i = 0; i < 100; i++)
        map.bus[i].count = 0;
    for (int j = 0; j < 10000; j++)
        map.station[j].count = 0;
}

int LocateStation(char temp[])
{
    for (int i = 1; i <= map.StationCount; i++) {
        if (strcmp(map.station[i].name, temp) == 0){
            return i;
        }
    }
    return 0;
}

int LocateBus(char temp[])
{
    for (int i = 1; i <= map.BusCount; i++) {
        if (strcmp(map.bus[i].name, temp) == 0) {
            return i;
        }
    }
    return 0;
}

void CreateMap()
{
    ifstream infile("E:\\程序们\\数据结构\\数据结构课设\\必做题\\南京公交线路.txt");
    if (!infile) {
        cout << "文件打开失败！" << endl;
        exit(0);
    }

    char temp[100];
    memset(temp, 0, sizeof(temp));//初始化字符数组
    int j = 0;//用于表示读取字符的个数
    char ch;
    int visit = 0;//作为第一个空格符的标志位，0表示此前没有空格符，1表示之前有空格符
    int i = map.BusCount;//表示目前公交车的数量
    while (true) {

        if (infile.eof()) {
            int k = LocateStation(temp);
            //1、对站点的信息存储
            if (k != 0) {
                UpdataStation(k, i);
            }
            else {
                k = CreateStation(i, temp);
            }
            //2、对公交车的信息存储
            UpdataBus(i, k);
            break;
        }

        infile.get(ch);//读取所有类型的字符，包括空格和换行

        if (ch == ' ' && visit == 0) {
            CreateBus(temp);
            i = map.BusCount;
            visit = 1;
            continue;
        }

        if (ch == ' ' && visit == 1) {
            memset(temp, 0, sizeof(temp));
            j = 0;
            continue;
        }

        if (ch == ',') {
            int k = LocateStation(temp);
            //1、对站点的信息存储
            if (k != 0) {
                UpdataStation(k, i);
            }
            else {
                k = CreateStation(i, temp);
            }
            //2、对公交车的信息存储
            UpdataBus(i, k);

            memset(temp, 0, sizeof(temp));
            j = 0;
            continue;
        }

        if (ch == '\n') {
            visit = 0;
            int k = LocateStation(temp);
            //1、对站点的信息存储
            //如果map里已存在该站点
            if (k != 0) {
                UpdataStation(k, i);
            }
            //如果map里不存在该站点，则新建站点
            else {
                k = CreateStation(i, temp);
            }
            //2、对公交车的信息存储
            UpdataBus(i, k);

            memset(temp, 0, sizeof(temp));
            j = 0;
            continue;
        }

        temp[j++] = ch;
    }
    cout << "文件读取成功！" << endl;
}

void UpdataBus(int sno, int station_no)//sno为公交车下标（不是编号）
{
    int num = ++map.bus[sno].count;
    map.bus[sno].route[num] = station_no;
}

void UpdataStation(int sno, int bus_no)//sno为站点号，bus_no为bus的下标
{
    map.station[sno].count++;//经过该站点的公交车数+1
    int num = map.station[sno].count;//目前该站点的公交车数
    map.station[sno].bus[num] = bus_no;
}

int CreateStation(int bus_no, char temp[])
{
    int k = ++map.StationCount;//新建站点的编号
    map.station[k].count = 1;
    map.station[k].bus[1] = bus_no;
    strcpy(map.station[k].name, temp);
    return k;
}

void CreateBus(char temp[])
{   
    int k = ++map.BusCount;
    map.bus[k].count = 0;
    strcpy(map.bus[k].name, temp);
}

void showBusInfo(char name[])
{
    int num = LocateBus(name);
    int t;
    int i;
    if (num) {
        cout << map.bus[num].name << "的路线如下：" << endl;
        for (i = 1; i < map.bus[num].count; i++) {
            t = map.bus[num].route[i];
            cout << map.station[t].name << " -> ";
        }
        t = map.bus[num].route[i];
        cout << map.station[t].name;
        cout << endl;
        system("pause");
    }

    else {
        cout << "未找到该公交车,请检查输入是否有误!\n";
        system("pause");
    }
}

void showStationInfo(char name[])
{
    int num = LocateStation(name);
    if (num) {
        cout << "经过该站点的公交车有：";
        for (int i = 1; i <= map.station[num].count; i++) {
            int t = map.station[num].bus[i];
            cout << map.bus[t].name << ' ';
        }
        cout << endl;
        system("pause");
    }
    else {
        cout << "未找到该站点,请检查输入是否有误!\n";
        system("pause");
    }
}

void SearchRoutes(char name1[], char name2[])
{
    if (!LocateStation(name1))
    {
        cout << "未找到起点,请检查输入是否有误!\n";
        system("pause");
        return;
    }

    if (!LocateStation(name2))
    {
        cout << "未找到终点点,请检查输入是否有误!\n";
        system("pause");
        return;
    }
    LeastStation(name1, name2);
    LeastTransfers(name1, name2);
    system("pause");
}

void LeastStation(char name1[], char name2[])
{
    int v1 = LocateStation(name1);
    int v2 = LocateStation(name2);
    Dijkstra(v1, v2);
}

void LeastTransfers(char name1[], char name2[])
{
    int v1 = LocateStation(name1);
    int v2 = LocateStation(name2);
    BFS(v1, v2);
}

void CreateMatrix()
{
    //1、初始化邻接矩阵
    for (int i = 0; i < VERTEX_MAX; i++) {
        for (int j = 0; j < VERTEX_MAX; j++)
            matrix[i][j] = INF;
    }
    //2、给邻接矩阵赋值
    for (int i = 1; i <= map.BusCount; i++) {
        for (int j = 1; j <= map.bus[i].count - 1; j++) {
            int v1 = map.bus[i].route[j];
            int v2 = map.bus[i].route[j + 1];
            matrix[v1][v2] = 1;
            matrix[v2][v1] = 1;
        }
    }
}

void Dijkstra(int v1, int v2)
{
    if (v1 == v2) {
        cout << "输入的起点和终点重合啦，不需要坐车即可到达！" << endl;
            return;
    }

    //1、初始化三个数组
    for (int i = 1; i <= map.StationCount; i++) {
        visit[i] = false;
        dist[i] = matrix[v1][i];
        if (dist[i] < INF)
            path[i] = v1;
        else
            path[i] = -1;
    }
    visit[v1] = true;//将起始点设置为被选中状态
    //2、循环选中剩余的顶点
    for (int i = 1; i < map.StationCount; i++) {
        if (visit[v2] == true)
            break;
        //3、在dist数组中找到标志位为0的元素的最小值
        int MIN = INF;
        int k = -1;
        for (int j = 1; j <= map.StationCount; j++) {
            if (visit[j] == false && dist[j] < MIN) {
                MIN = dist[j];
                k = j;
            }
        }
        //4、找到后将其设置为选中状态
        visit[k] = true;
        int mindist = dist[k];
        if (mindist < INF) {
            //5、更新其他顶点经过点k到源点的距离
            for (int t = 1; t <= map.StationCount; t++) {
                //6、如果距离更近则更新
                if (visit[t] == false && dist[t] > mindist + matrix[k][t]) {
                    dist[t] = mindist + matrix[k][t];
                    path[t] = k;
                }
            }
        }
    }

        cout << "\n至少需要经过" << dist[v2] + 1 << "站，具体线路如下\n" << endl;
        //用栈辅助路径的输出
        stack<int> stack;
        int t = v2;
        while (true) {
            stack.push(t);
            t = path[t];
            if (t == v1)
                break;
        }
        stack.push(v1);
        //输出路径
        int i, j;//i为当前站，j为下一站
        int num = 0;//公交车号

        while (!stack.empty()) {
            i = stack.top();
            stack.pop();//出栈

            if (!stack.empty()) {
                j = stack.top();

                int bus_count = map.station[i].count;//经过当前站点的公交车数量
                //如果原公交车可以到下一站点，则坐原公交车
                int flag1 = 0;//原公交车是否能到下一站点
                if (num != 0) {
                    for (int t = 1; t < map.bus[num].count; t++) {
                        int v1 = map.bus[num].route[t];
                        int v2 = map.bus[num].route[t + 1];
                        if (v1 == i && v2 == j) {
                            flag1 = 1;
                            break;
                        }
                    }
                }

                //如果原公交车可以不到下一站点，则从头找公交车号
                if (flag1 == 0) {
                    for (int k = 1; k <= bus_count; k++) {
                        int flag2 = 0;//是否找到公交车
                        num = map.station[i].bus[k];//num记录公交车号

                        for (int t = 1; t < map.bus[num].count; t++) {
                            int v1 = map.bus[num].route[t];
                            int v2 = map.bus[num].route[t + 1];
                            if (v1 == i && v2 == j) {
                                flag2 = 1;
                                break;
                            }
                        }
                        if (flag2 == 1)
                            break;
                    }
                }
                cout << map.station[i].name <<"（乘坐"  << map.bus[num].name <<"公交车）->";
            }
            else
                cout << map.station[i].name;
        }
        cout << "\n-----------------------------------------------------------\n\n";
}

void menu()
{
    while (true) {
        system("cls");
        char name1[100];
        char name2[100];
        cout << "欢迎使用南京公交线路查询系统\n\n";
        cout << "1、查询公交车信息\n"
                "2、查询站点信息\n"
                "3、根据起点和终点查询线路\n"
                "4、退出\n";
        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());//清除缓冲区
        char choice;
        cin >> choice;
        switch (choice) {
            case '1':
                cout << "请输入公交车编号(如：99路):";
                cin >> name1;
                showBusInfo(name1);
                break;
            case '2':
                cout << "请输入站点名称(如：南堡公园站):";
                cin >> name1;
                showStationInfo(name1);
                break;
            case '3':
                cout << "请输入起点：";
                cin >> name1;
                cout << "请输入终点：";
                cin >> name2;
                cout << "-----------------------------------------------------------";
                SearchRoutes(name1, name2);
                break;
            case '4':
                cout << "感谢您的使用！\n";
                return;
            default:
                cout << "输入有误，请检查您的输入后重新输入！" << endl;
                system("pause");
                break;
        }
    }
}

void BFS(int v1, int v2)
{
    if (v1 == v2)
        return;

    for (int i = 0; i < 10000; i++) {
        edge[i].cur = i;
        edge[i].prev = 0;
        edge[i].bus = 0;
    }

    //用于判断站点是否被访问过
    for (int i = 0; i < VERTEX_MAX; i++) {
        visit[i] = false;
    }

    queue<int> q;//存放站点的队列
    visit[v1] = true;
    q.push(v1);
    q.push(0);//相当于空指针，用于计算转车数量
    int count = 1;//记录转车次数

    while (true) {
        int temp = q.front();//出队，temp为站点编号
        q.pop();

        if (temp == 0) {
            count++;
            q.push(0);
        }

        int bus_count = map.station[temp].count;//经过当前站点的公交车数量
        int num;//用于记录公交车编号
        int flag = 0;//用于记录目前公交车能否到达终点

        for (int i = 1; i <= bus_count; i++) {
            num = map.station[temp].bus[i];

            for (int j = 1; j <= map.bus[num].count; j++) {
                int v = map.bus[num].route[j];//公交车经过的站点
                if (visit[v] == false) {
                    edge[v].prev = temp;
                    edge[v].bus = num;

                    q.push(v);
                    visit[v] = true;
                }
                if (v == v2) {
                    flag = 1;
                    break;
                }
            }
            if (flag == 1)
                break;
        }
        if (flag == 1)
            break;
    }

    cout << "\n至少需要转车" << count  << "次，具体线路如下\n" << endl;
    //输出路程
    stack<Edge> s;
    int cur = v2;
    while (cur != 0) {
        s.push(edge[cur]);
        cur = edge[cur].prev;
    }
    int bus;
    Edge e = s.top();
    s.pop();
    cout << map.station[e.cur].name;
    while (!s.empty()) {
        e = s.top();
        s.pop();
        cout << "（乘坐" << map.bus[e.bus].name << "公交车）->" << map.station[e.cur].name;
    }
    cout << "\n-----------------------------------------------------------\n\n";
}