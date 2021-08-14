#include<iostream>
#include<cstdlib>
#include<fstream>
#include<queue>
#include<ctime>
#include<Windows.h>
using namespace std;
#define MAXSIZE 100000  //˳������󳤶�

//--------------------�ṹ�嶨��---------------//

typedef int KeyType;
typedef struct {
	KeyType key;//�ؼ�����
}RecordType;	//�����¼����

typedef struct {
	RecordType arr[MAXSIZE + 1];//0�ŵ�Ԫ����
	int length;//˳�����
}SqList;	//˳�������

SqList L1[10];//���ԭʼ����
SqList L2[10];//���������
//----------------ֱ�Ӳ����㷨----------------//

void Insert(SqList* L, int i)
{
	RecordType temp = L->arr[i];
	int j;
	for (j = i; j > 1; j--) {
		if (temp.key < L->arr[j - 1].key) {
			L->arr[j] = L->arr[j - 1];//�����ǰ���С�򽻻�
		}
		else
			break;//��ȷ������λ��
	}
	L->arr[j] = temp;
}

void InsertSort(SqList* L)
{
	for (int i = 2; i < L->length; i++) {
		Insert(L, i);
	}
}

//--------------ϣ������----------------//   

void shellInsert(SqList* L, int d)//�Լ��d�������н���ֱ�Ӳ�������
{
	for (int i = d + 1; i <= L->length; i++) {
		if (L->arr[i].key < L->arr[i - d].key) {
			RecordType temp = L->arr[i];
			int j;
			for (j = i; j > d; j -= d) {
				if (temp.key < L->arr[j - d].key) {
					L->arr[j] = L->arr[j - d];//�����ǰ���С�򽻻�
				}
				else
					break;//��ȷ������λ��
			}
			L->arr[j] = temp;
		}
	}
}

void shellSort(SqList* L, int d[], int t)//����������d[0,...,t-1]����˳���L����ϣ������
{
	for (int i = 0; i < t; i++)
		shellInsert(L, d[i]);
}

//-----------------ð������----------------//

void BubbleSort(SqList* L)
{
	//����n-1��ð�ݹ���
	for (int i = 1; i <= L->length - 1; i++) {
		//ǰn-i���ؼ��ֱȽ�
		int flag = 0;//�����ж�һ��������û�н��н���
		for (int j = 1; j <= L->length - i; j++) {
			if (L->arr[j].key > L->arr[j + 1].key) {
				RecordType temp = L->arr[j];
				L->arr[j] = L->arr[j + 1];
				L->arr[j + 1] = temp;
				flag = 1;
			}
		}
		//���û�н�����˵�������Ѿ����
		if (flag == 0)
			break;
	}
}

//-----------------��������----------------//

int Partition(SqList* L, int low, int high)
{
	int mid = (low + high) / 2;
	RecordType temp = L->arr[mid];
	KeyType pivotkey = L->arr[mid].key;//�����¼�ؼ���
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
		KeyType pivotloc = Partition(L, low, high);//һ�˿�������
		QSort(L, low, pivotloc - 1);//��ǰ�������н��еݹ��������
		QSort(L, pivotloc + 1, high);//�Ժ������н��еݹ��������
	}
}

void QuickSort(SqList* L)
{
	QSort(L, 1, L->length);
}

//------------------��ѡ������--------------//

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

//--------------������-----------------//

void HeapAdjust(SqList& L, int low, int high)//�ѵ�ɸѡ
{
	RecordType temp = L.arr[low];
	int i = low, j = 2 * i;
	while (j <= high) {
		if ((j + 1) <= high && L.arr[j].key < L.arr[j + 1].key)//��������ѡ����
			j++;
		if (temp.key >= L.arr[j].key)
			break;
		L.arr[i] = L.arr[j];//�������ƣ�����������ɸѡ
		i = j;
		j = 2 * i;
	}
	L.arr[i] = temp;
}

void HeapSort(SqList& L)
{
	for (int i = L.length / 2; i > 0; i--) {
		HeapAdjust(L, i, L.length);//���ɴ󶥶�
	}
	for (int i = L.length; i > 1; i--) { //����������������
		RecordType temp = L.arr[1];
		L.arr[1] = L.arr[i];
		L.arr[i] = temp;//���Ѷ���¼�͵�ǰ���һ����¼����
		HeapAdjust(L, 1, i - 1);//��[1����������i-1]���µ���Ϊ�󶥶�
	}
}
//--------------�鲢����---------------//

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
//--------------��������---------------//

void RadixSort(SqList& L)
{
	queue<int> q[10];
	int radix = 1;
	for (int k = 1; k <= 5; k++) {
		radix = radix * 10;
		int i;
		int m;
		for (i = 1; i <= L.length; i++) {
			m = (L.arr[i].key % radix) / (radix / 10);//ȡ�ؼ��ֵġ�λ��
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


//---------------�����������뺯��-----------------//

void input()//���ļ�����ֵ���Ա���
{
	ifstream infile[10];
	for (int i = 0; i < 10; i++) {
		if(i == 0)
			infile[i].open("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\�����������\\data1.txt");
		else if (i == 1)
			infile[i].open("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\�����������\\data2.txt");
		else if (i == 2)
			infile[i].open("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\�����������\\data3.txt");
		else if (i == 3)
			infile[i].open("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\�����������\\data4.txt");
		else if (i == 4)
			infile[i].open("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\�����������\\data5.txt");
		else if (i == 5)
			infile[i].open("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\�����������\\data6.txt");
		else if (i == 6)
			infile[i].open("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\�����������\\data7.txt");
		else if (i == 7)
			infile[i].open("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\�����������\\data8.txt");
		else if (i == 8)
			infile[i].open("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\�����������\\data9.txt");
		else
			infile[i].open("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\�����������\\data10.txt");

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
	//ofstream outfile("E:\\������\\���ݽṹ\\���ݽṹ����\\������\\�����������\\data1.txt");
	//srand((unsigned)time(NULL));//srand()��������һ���Ե�ǰʱ�俪ʼ���������
	//for (int i = 0; i < 50000; i++)
	//	outfile << rand() % RAND_MAX << ' ';//RAND_MAXΪ���ֵ���������Ϊ0~32766
	
	input();//���ļ������ݵ������Ա���
	//���а˴�ѭ�����ð��ֲ�ͬ�������㷨����ʱ��Ƚ�
	double sum[8] = { 0 };
	for (int k = 0; k < 8; k++) {
		for (int i = 0; i < 10; i++)
			L2[i] = L1[i];
		double t[10] = { 0 };//���ڼ�¼ʮ������ʱ��
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
				cout << "��" << j + 1 << "��ֱ�Ӳ��������ʱ��Ϊ��" << t[j] << "ms" << endl;
			}
			cout << endl;
			cout << "ʮ��ֱ�Ӳ��������ƽ��ʱ��Ϊ" << sum[0] / 10 << "ms" << endl;
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
				cout << "��" << j + 1 << "��ϣ�������ʱ��Ϊ��" << t[j] << "ms" << endl;
			}
			cout << endl;
			cout << "ʮ��ϣ�������ƽ��ʱ��Ϊ" << sum[1] / 10 << "ms" << endl;
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
				cout << "��" << j + 1 << "��ð�������ʱ��Ϊ��" << t[j] << "ms" << endl;
			}
			cout << endl;
			cout << "ʮ��ð�������ƽ��ʱ��Ϊ" << sum[2] / 10  << "ms" << endl;
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
				cout << "��" << j + 1 << "�ο��������ʱ��Ϊ��" << t[j] << "ms" << endl;
			}
			cout << endl;
			cout << "ʮ�ο��������ƽ��ʱ��Ϊ" << sum[3] / 10 << "ms" << endl;
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
				cout << "��" << j + 1 << "�μ�ѡ�������ʱ��Ϊ��" << t[j] << "ms" << endl;
			}
			cout << endl;
			cout << "ʮ�μ�ѡ�������ƽ��ʱ��Ϊ" << sum[4] / 10 << "ms" << endl;
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
				cout << "��" << j + 1 << "�ζ������ʱ��Ϊ��" << t[j] << "ms" << endl;
			}
			cout << endl;
			cout << "ʮ�ζ������ƽ��ʱ��Ϊ" << sum[5] / 10 << "ms" << endl;
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
				cout << "��" << j + 1 << "�ι鲢�����ʱ��Ϊ��" << t[j] << "ms" << endl;
			}
			cout << endl;
			cout << "ʮ�ι鲢�����ƽ��ʱ��Ϊ" << sum[6] / 10 << "ms" << endl;
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
				cout << "��" << j + 1 << "�λ��������ʱ��Ϊ��" << t[j] << "ms" << endl;
			}
			cout << endl;
			cout << "ʮ�λ��������ƽ��ʱ��Ϊ" << sum[7] / 10 << "ms" << endl;
			break;
		}
		}//end switch
	}
	cout << "------------------------------------------\n" << endl;
	cout << "	ֱ�Ӳ�������		" << sum[0] / 10 << "ms" << endl;
	cout << "	ϣ������		" << sum[1] / 10 << "ms" << endl;
	cout << "	ð������		" << sum[2] / 10 << "ms" << endl;
	cout << "	��������		" << sum[3] / 10 << "ms" << endl;
	cout << "	��ѡ������		" << sum[4] / 10 << "ms" << endl;
	cout << "	������			" << sum[5] / 10 << "ms" << endl;
	cout << "	�鲢����		" << sum[6] / 10 << "ms" << endl;
	cout << "	��������		" << sum[7] / 10 << "ms" << endl;
	cout << "------------------------------------------\n" << endl;
	return 0;
}
