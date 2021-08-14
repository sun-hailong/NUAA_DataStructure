#include<iostream>
using namespace std;

int main()
{
	int count[2000] = { 0 };//用于记录整数i出现的次数
	int n;
	int k;
	int nums = 0;//用于记录一共有多少种数
	cin >> n;
	for(int i = 0; i < n; i++) {
		cin >> k;
		count[k]++;
	}
	//寻找一共有多少种数
	for(int i = 0; i < 2000; i++) {
		if(count[i] > 0)
			nums++;
	}
	//找最大值
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
