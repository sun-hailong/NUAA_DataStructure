#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include<stack>
#include<queue>
using namespace std;
const int VERTEX_MAX = 10000;
const int INF = 10000;
//-------------------�ṹ�嶨��---------------------//
typedef struct {
    char name[100];        //�������ı��
    int count;     //������������վ������
    int route[100]; //��������·���±�
} Bus;             //1000

typedef struct {
    int bus[100]; //������վ��Ĺ��������±�
    int count;         //������վ��Ĺ���������
    char name[100];     //վ������
} Station;       //10000      

typedef struct {
    Bus bus[1000];          //�ó��еĹ�����
    Station station[10000]; //�ó��е�վ��
    int BusCount;           //����������
    int StationCount;       //վ������
} Map;
Map map;

typedef struct {
    int prev;//��һվ
    int cur;//��ǰվ
    int bus;//��һվ����վ�ĳ���
} Edge;
Edge edge[10000];
int matrix[10000][10000];//�ڽӾ���
int dist[VERTEX_MAX], path[VERTEX_MAX];
bool visit[VERTEX_MAX];//����������������

//----------------����������---------------//
void InitMap();//��ʼ����ͼ
void CreateMap();//�����ļ���Ϣ������ͼ
int LocateStation(char temp[]);//����վ���±�
int LocateBus(char temp[]);//���ҹ������±�
void showBusInfo(char name[]);//չʾ��������Ϣ
void showStationInfo(char name[]);//չʾվ����Ϣ
void SearchRoutes(char name1[], char name2[]);//��ѯ·��
void UpdataBus(int sno, int station_no);//���¹�������Ϣ
void UpdataStation(int sno, int bus_no);//����վ����Ϣ
int CreateStation(int bus_no, char temp[]);//�½�վ��
void CreateBus(char temp[]);//�½�������
void LeastStation(char name1[], char name2[]);//��ѯ����վ�����ٵĳ˳�·��
void LeastTransfers(char name1[], char name2[]);//��ѯת���������ٵĳ˳�·��
void Dijkstra(int v1, int v2);//�Ͻ�˹���������·��������վ������
void CreateMatrix();//�����ڽӾ���
void menu();//�˵�
void BFS(int v1, int v2);//������ȱ���

//-------------------������-----------------//

int main()
{
    InitMap();
    CreateMap();
    CreateMatrix();
    menu();
	return 0;
}

//-----------------�����Ķ���----------------//
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
    ifstream infile("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\�Ͼ�������·.txt");
    if (!infile) {
        cout << "�ļ���ʧ�ܣ�" << endl;
        exit(0);
    }

    char temp[100];
    memset(temp, 0, sizeof(temp));//��ʼ���ַ�����
    int j = 0;//���ڱ�ʾ��ȡ�ַ��ĸ���
    char ch;
    int visit = 0;//��Ϊ��һ���ո���ı�־λ��0��ʾ��ǰû�пո����1��ʾ֮ǰ�пո��
    int i = map.BusCount;//��ʾĿǰ������������
    while (true) {

        if (infile.eof()) {
            int k = LocateStation(temp);
            //1����վ�����Ϣ�洢
            if (k != 0) {
                UpdataStation(k, i);
            }
            else {
                k = CreateStation(i, temp);
            }
            //2���Թ���������Ϣ�洢
            UpdataBus(i, k);
            break;
        }

        infile.get(ch);//��ȡ�������͵��ַ��������ո�ͻ���

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
            //1����վ�����Ϣ�洢
            if (k != 0) {
                UpdataStation(k, i);
            }
            else {
                k = CreateStation(i, temp);
            }
            //2���Թ���������Ϣ�洢
            UpdataBus(i, k);

            memset(temp, 0, sizeof(temp));
            j = 0;
            continue;
        }

        if (ch == '\n') {
            visit = 0;
            int k = LocateStation(temp);
            //1����վ�����Ϣ�洢
            //���map���Ѵ��ڸ�վ��
            if (k != 0) {
                UpdataStation(k, i);
            }
            //���map�ﲻ���ڸ�վ�㣬���½�վ��
            else {
                k = CreateStation(i, temp);
            }
            //2���Թ���������Ϣ�洢
            UpdataBus(i, k);

            memset(temp, 0, sizeof(temp));
            j = 0;
            continue;
        }

        temp[j++] = ch;
    }
    cout << "�ļ���ȡ�ɹ���" << endl;
}

void UpdataBus(int sno, int station_no)//snoΪ�������±꣨���Ǳ�ţ�
{
    int num = ++map.bus[sno].count;
    map.bus[sno].route[num] = station_no;
}

void UpdataStation(int sno, int bus_no)//snoΪվ��ţ�bus_noΪbus���±�
{
    map.station[sno].count++;//������վ��Ĺ�������+1
    int num = map.station[sno].count;//Ŀǰ��վ��Ĺ�������
    map.station[sno].bus[num] = bus_no;
}

int CreateStation(int bus_no, char temp[])
{
    int k = ++map.StationCount;//�½�վ��ı��
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
        cout << map.bus[num].name << "��·�����£�" << endl;
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
        cout << "δ�ҵ��ù�����,���������Ƿ�����!\n";
        system("pause");
    }
}

void showStationInfo(char name[])
{
    int num = LocateStation(name);
    if (num) {
        cout << "������վ��Ĺ������У�";
        for (int i = 1; i <= map.station[num].count; i++) {
            int t = map.station[num].bus[i];
            cout << map.bus[t].name << ' ';
        }
        cout << endl;
        system("pause");
    }
    else {
        cout << "δ�ҵ���վ��,���������Ƿ�����!\n";
        system("pause");
    }
}

void SearchRoutes(char name1[], char name2[])
{
    if (!LocateStation(name1))
    {
        cout << "δ�ҵ����,���������Ƿ�����!\n";
        system("pause");
        return;
    }

    if (!LocateStation(name2))
    {
        cout << "δ�ҵ��յ��,���������Ƿ�����!\n";
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
    //1����ʼ���ڽӾ���
    for (int i = 0; i < VERTEX_MAX; i++) {
        for (int j = 0; j < VERTEX_MAX; j++)
            matrix[i][j] = INF;
    }
    //2�����ڽӾ���ֵ
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
        cout << "����������յ��غ���������Ҫ�������ɵ��" << endl;
            return;
    }

    //1����ʼ����������
    for (int i = 1; i <= map.StationCount; i++) {
        visit[i] = false;
        dist[i] = matrix[v1][i];
        if (dist[i] < INF)
            path[i] = v1;
        else
            path[i] = -1;
    }
    visit[v1] = true;//����ʼ������Ϊ��ѡ��״̬
    //2��ѭ��ѡ��ʣ��Ķ���
    for (int i = 1; i < map.StationCount; i++) {
        if (visit[v2] == true)
            break;
        //3����dist�������ҵ���־λΪ0��Ԫ�ص���Сֵ
        int MIN = INF;
        int k = -1;
        for (int j = 1; j <= map.StationCount; j++) {
            if (visit[j] == false && dist[j] < MIN) {
                MIN = dist[j];
                k = j;
            }
        }
        //4���ҵ���������Ϊѡ��״̬
        visit[k] = true;
        int mindist = dist[k];
        if (mindist < INF) {
            //5�������������㾭����k��Դ��ľ���
            for (int t = 1; t <= map.StationCount; t++) {
                //6�����������������
                if (visit[t] == false && dist[t] > mindist + matrix[k][t]) {
                    dist[t] = mindist + matrix[k][t];
                    path[t] = k;
                }
            }
        }
    }

        cout << "\n������Ҫ����" << dist[v2] + 1 << "վ��������·����\n" << endl;
        //��ջ����·�������
        stack<int> stack;
        int t = v2;
        while (true) {
            stack.push(t);
            t = path[t];
            if (t == v1)
                break;
        }
        stack.push(v1);
        //���·��
        int i, j;//iΪ��ǰվ��jΪ��һվ
        int num = 0;//��������

        while (!stack.empty()) {
            i = stack.top();
            stack.pop();//��ջ

            if (!stack.empty()) {
                j = stack.top();

                int bus_count = map.station[i].count;//������ǰվ��Ĺ���������
                //���ԭ���������Ե���һվ�㣬����ԭ������
                int flag1 = 0;//ԭ�������Ƿ��ܵ���һվ��
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

                //���ԭ���������Բ�����һվ�㣬���ͷ�ҹ�������
                if (flag1 == 0) {
                    for (int k = 1; k <= bus_count; k++) {
                        int flag2 = 0;//�Ƿ��ҵ�������
                        num = map.station[i].bus[k];//num��¼��������

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
                cout << map.station[i].name <<"������"  << map.bus[num].name <<"��������->";
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
        cout << "��ӭʹ���Ͼ�������·��ѯϵͳ\n\n";
        cout << "1����ѯ��������Ϣ\n"
                "2����ѯվ����Ϣ\n"
                "3�����������յ��ѯ��·\n"
                "4���˳�\n";
        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());//���������
        char choice;
        cin >> choice;
        switch (choice) {
            case '1':
                cout << "�����빫�������(�磺99·):";
                cin >> name1;
                showBusInfo(name1);
                break;
            case '2':
                cout << "������վ������(�磺�ϱ���԰վ):";
                cin >> name1;
                showStationInfo(name1);
                break;
            case '3':
                cout << "��������㣺";
                cin >> name1;
                cout << "�������յ㣺";
                cin >> name2;
                cout << "-----------------------------------------------------------";
                SearchRoutes(name1, name2);
                break;
            case '4':
                cout << "��л����ʹ�ã�\n";
                return;
            default:
                cout << "����������������������������룡" << endl;
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

    //�����ж�վ���Ƿ񱻷��ʹ�
    for (int i = 0; i < VERTEX_MAX; i++) {
        visit[i] = false;
    }

    queue<int> q;//���վ��Ķ���
    visit[v1] = true;
    q.push(v1);
    q.push(0);//�൱�ڿ�ָ�룬���ڼ���ת������
    int count = 1;//��¼ת������

    while (true) {
        int temp = q.front();//���ӣ�tempΪվ����
        q.pop();

        if (temp == 0) {
            count++;
            q.push(0);
        }

        int bus_count = map.station[temp].count;//������ǰվ��Ĺ���������
        int num;//���ڼ�¼���������
        int flag = 0;//���ڼ�¼Ŀǰ�������ܷ񵽴��յ�

        for (int i = 1; i <= bus_count; i++) {
            num = map.station[temp].bus[i];

            for (int j = 1; j <= map.bus[num].count; j++) {
                int v = map.bus[num].route[j];//������������վ��
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

    cout << "\n������Ҫת��" << count  << "�Σ�������·����\n" << endl;
    //���·��
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
        cout << "������" << map.bus[e.bus].name << "��������->" << map.station[e.cur].name;
    }
    cout << "\n-----------------------------------------------------------\n\n";
}