#include <iostream>
using namespace std;
#include "Process_ZX.h"

ProInfo prolist[1024];
int     pronum;

//----------------------�ṹ�嶨��--------------------//

typedef struct snode{
	ProInfo elem;
	struct snode* next;
}Snode;//������

typedef struct {
	Snode* head;	//�������ͷ���
	int length;		//������ĳ���
}Single;
Single sgList;

typedef struct dnode{
	ProInfo elem;
	struct dnode* prev;
	struct dnode* next;
}Dnode;//˫����

typedef struct {
	Dnode* head;	//˫�����ͷ���
	int length;		//˫����ĳ���
}Double;
Double dbList;

//----------------------��������-----------------------//

int LocateElem(Single* list, ProInfo elem)
{
	Snode* node = list->head->next;
	for (int i = 0; i < list->length; i++)
	{
		if (node->elem.ID == elem.ID)
			return i;
		node = node->next;
	}
	return -1;
}

void sg_InitList(Single* list)
{
	list->head = (Snode*)malloc(sizeof(Snode));
	list->head->next = NULL;
	list->length = 0;
}

void db_InitList(Double* list)
{
	list->head = (Dnode*)malloc(sizeof(Dnode));
	list->head->next = list->head;
	list->head->prev = list->head;
	list->length = 0;
}

Snode* sg_listGetNode(Single* list, int index) //���ڵõ��ض�λ�õĽ��
{
	int i;
	Snode* node = list->head->next;

	for (i = 0; i < index; i++)
		node = node->next;

	return node;
}

void sg_InsertElem(Single* list, int i, ProInfo elem)
{
	if (i < 0 || i > list->length)
		printf("�����±����󣬲���ʧ�ܣ�\n");
	else
	{
		Snode* node = (Snode*)malloc(sizeof(Snode));
		node->next = NULL;
		node->elem = elem;

		Snode* prev = (Snode*)malloc(sizeof(Snode));
		if (i == 0)
			prev = list->head;
		else
			prev = sg_listGetNode(list, i - 1);
		Snode* next = prev->next;
		prev->next = node;
		node->next = next;
		list->length++;
	}
}

Dnode* db_listGetNode(Double* list, int index) //���ڵõ��ض�λ�õĽ��
{
	int i;
	Dnode* node = list->head->next;

	for (i = 0; i < index; i++)
		node = node->next;

	return node;
}

void db_InsertElem(Double* list, int i, ProInfo elem)
{
	if (i < 0 || i > list->length)
		printf("�����±����󣬲���ʧ�ܣ�\n");
	else
	{
		Dnode* node = (Dnode*)malloc(sizeof(Dnode));
		node->next = NULL;
		node->elem = elem;

		Dnode* prev = (Dnode*)malloc(sizeof(Dnode));
		if (i == 0)
			prev = list->head;
		else
			prev = db_listGetNode(list, i - 1);
		Dnode* next = prev->next;

		prev->next = node;
		node->next = next;
		next->prev = node;
		node->prev = prev;
		list->length++;
	}
}

void Select_sort_Single(Single* list)//ѡ�������ڴ�ʹ���Զൽ������
{
	//1�����ֻ��һ��Ԫ�ػ���û��Ԫ�أ��򲻽��в�����
	if (list->length == 0 || list->length == 1)
		return;

	Snode* head = list->head->next;//���ڼ�¼�������ͷ�ڵ�
	list->head->next = NULL;//��ԭ�����ͷ��������Ͽ�
	while (head)
	{
		Snode* prev = (Snode*)malloc(sizeof(Snode));//���ڼ�¼��Сֵ��ǰ�ڵ㡣
		Snode* pmin = head;//���ڼ�¼��Сֵ�Ľڵ㡣
		Snode* cur = head;
		//2��Ѱ����Сֵ�ڵ㼰��ǰһ�ڵ㡣
		while (cur->next)
		{
			if (cur->next->elem.Memory < pmin->elem.Memory)
			{
				prev = cur;
				pmin = prev->next;
			}
			cur = cur->next;
		}
		if (pmin != head)      //�����Сֵ�ڵ㲻Ϊ�������ͷ���
			prev->next = pmin->next;//����Сֵ�Ľڵ��������жϿ���
		else
			head = head->next;    //�������ͷ������
		//3��������Ѱ�ҳ�����Сֵ�ڵ����ͷ�塣
		pmin->next = list->head->next;
		list->head->next = pmin;
	}
}

void Select_sort_Double(Double* list)//ѡ�����򣬳���ʱ�����ٵ�������
{
	//1�����ֻ��һ��Ԫ�ػ���û��Ԫ�أ��򲻽��в�����
	if (list->length == 0 || list->length == 1)
		return;

	Dnode* head = list->head->next;//���ڼ�¼�������ͷ�ڵ�
	list->head->next = NULL;//��ԭ�����ͷ��������Ͽ�
	while (head)
	{
		Dnode* prev = (Dnode*)malloc(sizeof(Dnode));//���ڼ�¼���ֵ��ǰ�ڵ㡣
		Dnode* pmax = head;//���ڼ�¼���ֵ�Ľڵ㡣
		Dnode* cur = head;
		//2��Ѱ�����ֵ�ڵ㼰��ǰһ�ڵ㡣
		while (cur->next)
		{
			int t1 = cur->next->elem.DuraTime.minute * 60 + cur->next->elem.DuraTime.second;
			int t2 = pmax->elem.DuraTime.minute * 60 + pmax->elem.DuraTime.second;
			if (t1 > t2)
			{
				prev = cur;
				pmax = prev->next;
			}
			cur = cur->next;
		}
		if (pmax != head)      //������ֵ�ڵ㲻Ϊ�������ͷ���
			prev->next = pmax->next;//�����ֵ�Ľڵ��������жϿ���
		else
			head = head->next;    //�������ͷ������
		//3��������Ѱ�ҳ������ֵ�ڵ����ͷ�塣
		pmax->next = list->head->next;
		list->head->next = pmax;
	}
}

int main()
{
	Single list;//�ܽ��̵������ܽ���-��ǰ����=��������
	sg_InitList(&list);

	while (1) {
		//����
		system("cls");

		//�������˫�����ʼ��
		sg_InitList(&sgList);
		db_InitList(&dbList);

		//��ʼ��
		AdjustPurview();
		//ִ��
		bool flag = GetCurrentProcesses(prolist, pronum, 1024);
		if (flag == false) {
			cout << "��ȡ��Ϣʧ�ܣ�" << endl;
			return -1;
		}

		//cout << "����" << pronum << "������" << "\n";
		//for (int i = 0; i <= pronum - 1; i++)
		//{
		//	cout << i << "\n";
		//	PrintProInfo(prolist[i]);
		//}
		for (int i = 0; i < pronum; i++) {
			sg_InsertElem(&sgList, sgList.length, prolist[i]);
			if (LocateElem(&list, prolist[i]) == -1)
				sg_InsertElem(&list, list.length, prolist[i]);//����ܽ�������û�е�ǰ���̣�������ȥ
		}
		Select_sort_Single(&sgList);
		Snode* node = sgList.head->next;
		//�����ǰϵͳ�Ľ���
		for (int i = 0; i < sgList.length; i++) {
			cout << i << endl;
			PrintCurProInfo(node->elem);
			node = node->next;
		}
		//����ѽ����Ľ���
		Snode* temp = list.head->next;
		for (int i = 0; i < list.length; i++) {
			if (LocateElem(&sgList, temp->elem) == -1)//�������Ľ��̼��뵽˫����
			{
				db_InsertElem(&dbList, dbList.length, temp->elem);

				FILETIME begintime, endtime, ipker, ipuser;
				DWORD ProID = temp->elem.ID;
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProID);
				GetProcessTimes(hProcess, &begintime, &endtime, &ipker, &ipuser);//��ʼ������
				LPSYSTEMTIME pSystemTime = new SYSTEMTIME;
				DWORD etime = 0;

				FileTimeToSystemTime(&endtime, pSystemTime);
				pSystemTime->wHour = (pSystemTime->wHour + 8) % 24;  //��UTCʱ��(ʱ���׼ʱ��)ת��Ϊ����ʱ��
				etime = pSystemTime->wHour * 60 * 60 + pSystemTime->wMinute * 60 + pSystemTime->wSecond;//����DWORD��ʽ����ʱ�䣬��λΪS

				temp->elem.EndTime.Year = pSystemTime->wYear;
				temp->elem.EndTime.Month = pSystemTime->wMonth;
				temp->elem.EndTime.Day = pSystemTime->wDay;
				temp->elem.EndTime.Hour = pSystemTime->wHour;
				temp->elem.EndTime.Minute = pSystemTime->wMinute;
				temp->elem.EndTime.Second = pSystemTime->wSecond;
			}
			temp = temp->next;
		}
		Select_sort_Double(&dbList);
		Dnode* p = dbList.head->next;
		for (int i = 0; i < dbList.length; i++) {
			cout << i << endl;
			PrintEndProInfo(p->elem);
			p = p->next;
		}


		//ÿ��5s��ȡһ�ε�ǰ����
		Sleep(5000);
	}

	system("pause");
	return 0;
}