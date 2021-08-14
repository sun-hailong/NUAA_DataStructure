#include<iostream>
#include<algorithm>
using namespace std;
typedef struct {
	int no;
	int start;
	int time;
	int end;
}Teacher;
Teacher t1[1000];//记录上课时间的先后顺序
Teacher t2[1000];//记录下课时间的先后顺序
int key[1000+1];//表示钥匙盒，key[i]表示钥匙编号
typedef struct {
	int flag;//0表示上课，1表示下课
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
	//初始化钥匙盒
	for (int i = 1; i <= n; i++) {
		key[i] = i;
	}
	//输入老师上下课信息
	for (int i = 0; i < k; i++) {
		cin >> t1[i].no >> t1[i].start >> t1[i].time;
		t1[i].end = t1[i].start + t1[i].time;
		t2[i] = t1[i];
	}
	sort(t1, t1 + k, cmp1);//上课时间升序排序
	sort(t2, t2 + k, cmp2);;//下课时间升序排序

	int a = 0;
	int b = 0;
	//将存、放钥匙的顺序记录到数组
	for (int i = 0; i < 2 * k; i++) {
		//a < k 这个判断很重要！
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
		//1、取钥匙
		if (node[i].flag == 0) {
			for (int j = 1; j <= n; j++) {
				if (key[j] == no)
				{
					key[j] = 0;
					break;
				}
			}
		}
		//2、放钥匙
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