#include<iostream>
#include<cstdlib>
#include<fstream>
#include<queue>
#include<ctime>
#include<Windows.h>
using namespace std;
#define MAXSIZE 100000  //顺序表的最大长度

//--------------------结构体定义---------------//

typedef int KeyType;
typedef struct {
	KeyType key;//关键字项
}RecordType;	//定义记录类型

typedef struct {
	RecordType arr[MAXSIZE + 1];//0号单元闲置
	int length;//顺序表长度
}SqList;	//顺序表类型

SqList L1[10];//存放原始数据
SqList L2[10];//用于排序后
//----------------直接插入算法----------------//

void Insert(SqList* L, int i)
{
	RecordType temp = L->arr[i];
	int j;
	for (j = i; j > 1; j--) {
		if (temp.key < L->arr[j - 1].key) {
			L->arr[j] = L->arr[j - 1];//如果比前面的小则交换
		}
		else
			break;//已确定插入位置
	}
	L->arr[j] = temp;
}

void InsertSort(SqList* L)
{
	for (int i = 2; i < L->length; i++) {
		Insert(L, i);
	}
}

//--------------希尔排序----------------//   

void shellInsert(SqList* L, int d)//对间隔d的子序列进行直接插入排序
{
	for (int i = d + 1; i <= L->length; i++) {
		if (L->arr[i].key < L->arr[i - d].key) {
			RecordType temp = L->arr[i];
			int j;
			for (j = i; j > d; j -= d) {
				if (temp.key < L->arr[j - d].key) {
					L->arr[j] = L->arr[j - d];//如果比前面的小则交换
				}
				else
					break;//已确定插入位置
			}
			L->arr[j] = temp;
		}
	}
}

void shellSort(SqList* L, int d[], int t)//按增量序列d[0,...,t-1]，对顺序表L进行希尔排序
{
	for (int i = 0; i < t; i++)
		shellInsert(L, d[i]);
}

//-----------------冒泡排序----------------//

void BubbleSort(SqList* L)
{
	//进行n-1次冒泡过程
	for (int i = 1; i <= L->length - 1; i++) {
		//前n-i个关键字比较
		int flag = 0;//用于判断一趟下来有没有进行交换
		for (int j = 1; j <= L->length - i; j++) {
			if (L->arr[j].key > L->arr[j + 1].key) {
				RecordType temp = L->arr[j];
				L->arr[j] = L->arr[j + 1];
				L->arr[j + 1] = temp;
				flag = 1;
			}
		}
		//如果没有交换，说明排序已经完成
		if (flag == 0)
			break;
	}
}

//-----------------快速排序----------------//

int Partition(SqList* L, int low, int high)
{
	int mid = (low + high) / 2;
	RecordType temp = L->arr[mid];
	KeyType pivotkey = L->arr[mid].key;//枢轴记录关键字
	L->arr[mid] = L->arr[low];
	while (low < high) {
		while (low < high && L->arr[high].key >= pivotkey)
			high--;
		L->arr[low] = L->arr[high];
		while (low < high && L->arr[low].key <= pivotkey)
			low++;
		L->arr[high] = L->arr[low];
	}
	L->arr[low] = temp;
	return low;
}

void QSort(SqList* L, int low, int high)
{
	if (low < high) {
		KeyType pivotloc = Partition(L, low, high);//一趟快速排序
		QSort(L, low, pivotloc - 1);//对前部子序列进行递归快速排序
		QSort(L, pivotloc + 1, high);//对后部子序列进行递归快速排序
	}
}

void QuickSort(SqList* L)
{
	QSort(L, 1, L->length);
}

//------------------简单选择排序--------------//

void SelectSort(SqList* L)
{
	for (int i = 1; i < L->length; i++) {
		KeyType min = L->arr[i].key;
		int k = i;
		for (int j = i + 1; j < L->length - 1; j++) {
			if (L->arr[j].key < min) {
				min = L->arr[j].key;
				k = j;
			}
		}
		if (k != i) {
			RecordType temp = L->arr[i];
			L->arr[i] = L->arr[k];
			L->arr[k] = temp;
		}
	}
}

//--------------堆排序-----------------//

void HeapAdjust(SqList& L, int low, int high)//堆的筛选
{
	RecordType temp = L.arr[low];
	int i = low, j = 2 * i;
	while (j <= high) {
		if ((j + 1) <= high && L.arr[j].key < L.arr[j + 1].key)//左右子树选大者
			j++;
		if (temp.key >= L.arr[j].key)
			break;
		L.arr[i] = L.arr[j];//大者上移，并继续向下筛选
		i = j;
		j = 2 * i;
	}
	L.arr[i] = temp;
}

void HeapSort(SqList& L)
{
	for (int i = L.length / 2; i > 0; i--) {
		HeapAdjust(L, i, L.length);//建成大顶堆
	}
	for (int i = L.length; i > 1; i--) { //逐个输出，并调整堆
		RecordType temp = L.arr[1];
		L.arr[1] = L.arr[i];
		L.arr[i] = temp;//将堆顶记录和当前最后一个记录交换
		HeapAdjust(L, 1, i - 1);//将[1，···，i-1]重新调整为大顶堆
	}
}
//--------------归并排序---------------//

void Merge(SqList& L, int low, int mid, int high)
{
	SqList T;
	int i = low;
	int j = mid + 1;
	int k = 0;
	while (i <= mid && j <= high) {
		if (L.arr[i].key <= L.arr[j].key) {
			T.arr[k] = L.arr[i];
			i++, k++;
		}
		else {
			T.arr[k] = L.arr[j];
			j++, k++;
		}
	}
	while (i <= mid) {
		T.arr[k] = L.arr[i];
		i++, k++;
	}
	while (j <= high) {
		T.arr[k] = L.arr[j];
		j++, k++;
	}
	for (k = 0, i = low; i <= high; k++, i++)
		L.arr[i] = T.arr[k];
}

void MSort(SqList& L, int len)
{
	int i = 1;
	while (i + 2 * len <= L.length) {
		Merge(L, i, i + len - 1, i + 2 * len - 1);
		i = i + 2 * len;
	}
	if (i + len <= L.length)
		Merge(L, i, i + len - 1, L.length);
}

void MergeSort(SqList& L)
{
	for (int i = 1; i <= L.length; i = 2 * i) {
		MSort(L, i);
	}
}
//--------------基数排序---------------//

void RadixSort(SqList& L)
{
	queue<int> q[10];
	int radix = 1;
	for (int k = 1; k <= 5; k++) {
		radix = radix * 10;
		int i;
		int m;
		for (i = 1; i <= L.length; i++) {
			m = (L.arr[i].key % radix) / (radix / 10);//取关键字的“位”
			q[m].push(L.arr[i].key);
		}
		for (i = 1, m = 0; m < 10; m++) {
			while (!q[m].empty()) {
				L.arr[i].key = q[m].front();
				q[m].pop();
				i++;
			}
		}
	}
}


//---------------主函数及输入函数-----------------//

void input()//将文件输入值线性表内
{
	ifstream infile[10];
	for (int i = 0; i < 10; i++) {
		if(i == 0)
			infile[i].open("E:\\程序们\\数据结构\\数据结构课设\\必做题\\随机数据样本\\data1.txt");
		else if (i == 1)
			infile[i].open("E:\\程序们\\数据结构\\数据结构课设\\必做题\\随机数据样本\\data2.txt");
		else if (i == 2)
			infile[i].open("E:\\程序们\\数据结构\\数据结构课设\\必做题\\随机数据样本\\data3.txt");
		else if (i == 3)
			infile[i].open("E:\\程序们\\数据结构\\数据结构课设\\必做题\\随机数据样本\\data4.txt");
		else if (i == 4)
			infile[i].open("E:\\程序们\\数据结构\\数据结构课设\\必做题\\随机数据样本\\data5.txt");
		else if (i == 5)
			infile[i].open("E:\\程序们\\数据结构\\数据结构课设\\必做题\\随机数据样本\\data6.txt");
		else if (i == 6)
			infile[i].open("E:\\程序们\\数据结构\\数据结构课设\\必做题\\随机数据样本\\data7.txt");
		else if (i == 7)
			infile[i].open("E:\\程序们\\数据结构\\数据结构课设\\必做题\\随机数据样本\\data8.txt");
		else if (i == 8)
			infile[i].open("E:\\程序们\\数据结构\\数据结构课设\\必做题\\随机数据样本\\data9.txt");
		else
			infile[i].open("E:\\程序们\\数据结构\\数据结构课设\\必做题\\随机数据样本\\data10.txt");

		int j = 1;
		while (!infile[i].eof()) {
			L1[i].length = j;
			infile[i] >> L1[i].arr[j++].key;
		}
	}

	for (int i = 0; i < 10; i++) {
		infile[i].close();
	}
}

int main()
{
	//ofstream outfile("E:\\程序们\\数据结构\\数据结构课设\\必做题\\随机数据样本\\data1.txt");
	//srand((unsigned)time(NULL));//srand()函数产生一个以当前时间开始的随机种子
	//for (int i = 0; i < 50000; i++)
	//	outfile << rand() % RAND_MAX << ' ';//RAND_MAX为最大值，其随机域为0~32766
	
	input();//将文件内数据导入线性表中
	//进行八次循环，用八种不同的排序算法进行时间比较
	double sum[8] = { 0 };
	for (int k = 0; k < 8; k++) {
		for (int i = 0; i < 10; i++)
			L2[i] = L1[i];
		double t[10] = { 0 };//用于记录十次排序时间
		switch (k)
		{
		case 0: {
			cout << "------------------------------------------\n" << endl;
			for (int j = 0; j < 10; j++) {
				double start = GetTickCount64();
				InsertSort(&L2[j]);
				double  end = GetTickCount64();
				t[j] = end - start;
				sum[0] += t[j];
				cout << "第" << j + 1 << "次直接插入排序的时间为：" << t[j] << "ms" << endl;
			}
			cout << endl;
			cout << "十次直接插入排序的平均时间为" << sum[0] / 10 << "ms" << endl;
			break;
		}

		case 1: {
			cout << "------------------------------------------\n" << endl;
			int d[5] = { 9,7,5,3,1 };
			for (int j = 0; j < 10; j++) {
				double start = GetTickCount64();
				shellSort(&L2[j], d, 5);
				double  end = GetTickCount64();
				t[j] = end - start;
				sum[1] += t[j];
				cout << "第" << j + 1 << "次希尔排序的时间为：" << t[j] << "ms" << endl;
			}
			cout << endl;
			cout << "十次希尔排序的平均时间为" << sum[1] / 10 << "ms" << endl;
			break; 
		}

		case 2: {
			cout << "------------------------------------------\n" << endl;
			for (int j = 0; j < 10; j++) {
				double start = GetTickCount64();
				BubbleSort(&L2[j]);
				double  end = GetTickCount64();
				t[j] = end - start;
				sum[2] += t[j];
				cout << "第" << j + 1 << "次冒泡排序的时间为：" << t[j] << "ms" << endl;
			}
			cout << endl;
			cout << "十次冒泡排序的平均时间为" << sum[2] / 10  << "ms" << endl;
			break;
		}

		case 3: {
			cout << "------------------------------------------\n" << endl;
			for (int j = 0; j < 10; j++) {
				double start = GetTickCount64();
				QuickSort(&L2[j]);
				double  end = GetTickCount64();
				t[j] = end - start;
				sum[3] += t[j];
				cout << "第" << j + 1 << "次快速排序的时间为：" << t[j] << "ms" << endl;
			}
			cout << endl;
			cout << "十次快速排序的平均时间为" << sum[3] / 10 << "ms" << endl;
			break;
		}

		case 4: {
			cout << "------------------------------------------\n" << endl;
			for (int j = 0; j < 10; j++) {
				double start = GetTickCount64();
				SelectSort(&L2[j]);
				double  end = GetTickCount64();
				t[j] = end - start;
				sum[4] += t[j];
				cout << "第" << j + 1 << "次简单选择排序的时间为：" << t[j] << "ms" << endl;
			}
			cout << endl;
			cout << "十次简单选择排序的平均时间为" << sum[4] / 10 << "ms" << endl;
			break;
		}

		case 5: {
			cout << "------------------------------------------\n" << endl;
			for (int j = 0; j < 10; j++) {
				double start = GetTickCount64();
				HeapSort(L2[j]);
				double  end = GetTickCount64();
				t[j] = end - start;
				sum[5] += t[j];
				cout << "第" << j + 1 << "次堆排序的时间为：" << t[j] << "ms" << endl;
			}
			cout << endl;
			cout << "十次堆排序的平均时间为" << sum[5] / 10 << "ms" << endl;
			break;
		}

		case 6: {
			cout << "------------------------------------------\n" << endl;
			for (int j = 0; j < 10; j++) {
				double start = GetTickCount64();
				MergeSort(L2[j]);
				double  end = GetTickCount64();
				t[j] = end - start;
				sum[6] += t[j];
				cout << "第" << j + 1 << "次归并排序的时间为：" << t[j] << "ms" << endl;
			}
			cout << endl;
			cout << "十次归并排序的平均时间为" << sum[6] / 10 << "ms" << endl;
			break;
		}
			  
		case 7: {
			cout << "------------------------------------------\n" << endl;
			for (int j = 0; j < 10; j++) {
				double start = GetTickCount64();
				RadixSort(L2[j]);
				double  end = GetTickCount64();
				t[j] = end - start;
				sum[7] += t[j];
				cout << "第" << j + 1 << "次基数排序的时间为：" << t[j] << "ms" << endl;
			}
			cout << endl;
			cout << "十次基数排序的平均时间为" << sum[7] / 10 << "ms" << endl;
			break;
		}
		}//end switch
	}
	cout << "------------------------------------------\n" << endl;
	cout << "	直接插入排序		" << sum[0] / 10 << "ms" << endl;
	cout << "	希尔排序		" << sum[1] / 10 << "ms" << endl;
	cout << "	冒泡排序		" << sum[2] / 10 << "ms" << endl;
	cout << "	快速排序		" << sum[3] / 10 << "ms" << endl;
	cout << "	简单选择排序		" << sum[4] / 10 << "ms" << endl;
	cout << "	堆排序			" << sum[5] / 10 << "ms" << endl;
	cout << "	归并排序		" << sum[6] / 10 << "ms" << endl;
	cout << "	基数排序		" << sum[7] / 10 << "ms" << endl;
	cout << "------------------------------------------\n" << endl;
	return 0;
}
