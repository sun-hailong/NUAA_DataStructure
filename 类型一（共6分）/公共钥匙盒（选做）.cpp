#include<iostream>
#include<algorithm>
using namespace std;
typedef struct {
	int no;
	int start;
	int time;
	int end;
}Teacher;
Teacher t1[1000];//��¼�Ͽ�ʱ����Ⱥ�˳��
Teacher t2[1000];//��¼�¿�ʱ����Ⱥ�˳��
int key[1000+1];//��ʾԿ�׺У�key[i]��ʾԿ�ױ��
typedef struct {
	int flag;//0��ʾ�ϿΣ�1��ʾ�¿�
	Teacher t;
}Node;
Node node[2000];
bool cmp1(Teacher t1, Teacher t2)
{
	if (t1.start < t2.start)
		return true;
	else
		return false;

}

bool cmp2(Teacher t1, Teacher t2)
{
	if (t1.end < t2.end)
		return true;
	else if (t1.end == t2.end)
		return t1.no < t2.no;
	else
		return false;

}
int main()
{
	int n, k;
	cin >> n >> k;
	//��ʼ��Կ�׺�
	for (int i = 1; i <= n; i++) {
		key[i] = i;
	}
	//������ʦ���¿���Ϣ
	for (int i = 0; i < k; i++) {
		cin >> t1[i].no >> t1[i].start >> t1[i].time;
		t1[i].end = t1[i].start + t1[i].time;
		t2[i] = t1[i];
	}
	sort(t1, t1 + k, cmp1);//�Ͽ�ʱ����������
	sort(t2, t2 + k, cmp2);;//�¿�ʱ����������

	int a = 0;
	int b = 0;
	//���桢��Կ�׵�˳���¼������
	for (int i = 0; i < 2 * k; i++) {
		//a < k ����жϺ���Ҫ��
		if (a < k && t1[a].start < t2[b].end) {
			node[i].t = t1[a];
			node[i].flag = 0;
			a++;
		}
		else {
			node[i].t = t2[b];
			node[i].flag = 1;
			b++;
		}
		//cout << "flag:" << node[i].flag << endl;
		//cout << "no:" << node[i].t.no << endl;
		int no = node[i].t.no;
		//1��ȡԿ��
		if (node[i].flag == 0) {
			for (int j = 1; j <= n; j++) {
				if (key[j] == no)
				{
					key[j] = 0;
					break;
				}
			}
		}
		//2����Կ��
		else {
			for (int j = 1; j <= n; j++) {
				if (key[j] == 0)
				{
					key[j] = no;
					break;
				}
			}
		}
	}
	for (int i = 1; i <= n; i++)
		cout << key[i] << ' ';
	return 0;
}