#include<iostream>
using namespace std;

int main()
{
	int count[2000] = { 0 };//���ڼ�¼����i���ֵĴ���
	int n;
	int k;
	int nums = 0;//���ڼ�¼һ���ж�������
	cin >> n;
	for(int i = 0; i < n; i++) {
		cin >> k;
		count[k]++;
	}
	//Ѱ��һ���ж�������
	for(int i = 0; i < 2000; i++) {
		if(count[i] > 0)
			nums++;
	}
	//�����ֵ
	for(int i = 0; i < nums; i++){
		int max_pos = 0;
		for(int j = 0; j < 2000; j++){
			if(count[j] > count[max_pos])
				max_pos = j;
		}
		if(count[max_pos] > 0){
			cout << max_pos << ' ' << count[max_pos] << endl;
			count[max_pos] = -1;
		}
	}
	return 0;
}
