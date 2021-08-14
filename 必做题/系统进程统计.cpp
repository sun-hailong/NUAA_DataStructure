#include <iostream>
using namespace std;
#include "Process_ZX.h"

ProInfo prolist[1024];
int     pronum;

//----------------------结构体定义--------------------//

typedef struct snode{
	ProInfo elem;
	struct snode* next;
}Snode;//单链表

typedef struct {
	Snode* head;	//单链表的头结点
	int length;		//单链表的长度
}Single;
Single sgList;

typedef struct dnode{
	ProInfo elem;
	struct dnode* prev;
	struct dnode* next;
}Dnode;//双链表

typedef struct {
	Dnode* head;	//双链表的头结点
	int length;		//双链表的长度
}Double;
Double dbList;

//----------------------函数定义-----------------------//

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

Snode* sg_listGetNode(Single* list, int index) //便于得到特定位置的结点
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
		printf("输入下标有误，操作失败！\n");
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

Dnode* db_listGetNode(Double* list, int index) //便于得到特定位置的结点
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
		printf("输入下标有误，操作失败！\n");
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

void Select_sort_Single(Single* list)//选择排序，内存使用自多到少排序
{
	//1、如果只有一个元素或者没有元素，则不进行操作。
	if (list->length == 0 || list->length == 1)
		return;

	Snode* head = list->head->next;//用于记录新链表的头节点
	list->head->next = NULL;//将原链表的头结点与后面断开
	while (head)
	{
		Snode* prev = (Snode*)malloc(sizeof(Snode));//用于记录最小值的前节点。
		Snode* pmin = head;//用于记录最小值的节点。
		Snode* cur = head;
		//2、寻找最小值节点及其前一节点。
		while (cur->next)
		{
			if (cur->next->elem.Memory < pmin->elem.Memory)
			{
				prev = cur;
				pmin = prev->next;
			}
			cur = cur->next;
		}
		if (pmin != head)      //如果最小值节点不为新链表的头结点
			prev->next = pmin->next;//将最小值的节点在链表中断开。
		else
			head = head->next;    //新链表的头结点后移
		//3、将此轮寻找出的最小值节点进行头插。
		pmin->next = list->head->next;
		list->head->next = pmin;
	}
}

void Select_sort_Double(Double* list)//选择排序，持续时间自少到多排序
{
	//1、如果只有一个元素或者没有元素，则不进行操作。
	if (list->length == 0 || list->length == 1)
		return;

	Dnode* head = list->head->next;//用于记录新链表的头节点
	list->head->next = NULL;//将原链表的头结点与后面断开
	while (head)
	{
		Dnode* prev = (Dnode*)malloc(sizeof(Dnode));//用于记录最大值的前节点。
		Dnode* pmax = head;//用于记录最大值的节点。
		Dnode* cur = head;
		//2、寻找最大值节点及其前一节点。
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
		if (pmax != head)      //如果最大值节点不为新链表的头结点
			prev->next = pmax->next;//将最大值的节点在链表中断开。
		else
			head = head->next;    //新链表的头结点后移
		//3、将此轮寻找出的最大值节点进行头插。
		pmax->next = list->head->next;
		list->head->next = pmax;
	}
}

int main()
{
	Single list;//总进程的链表，总进程-当前进程=结束进程
	sg_InitList(&list);

	while (1) {
		//清屏
		system("cls");

		//单链表和双链表初始化
		sg_InitList(&sgList);
		db_InitList(&dbList);

		//初始化
		AdjustPurview();
		//执行
		bool flag = GetCurrentProcesses(prolist, pronum, 1024);
		if (flag == false) {
			cout << "获取信息失败！" << endl;
			return -1;
		}

		//cout << "共有" << pronum << "个进程" << "\n";
		//for (int i = 0; i <= pronum - 1; i++)
		//{
		//	cout << i << "\n";
		//	PrintProInfo(prolist[i]);
		//}
		for (int i = 0; i < pronum; i++) {
			sg_InsertElem(&sgList, sgList.length, prolist[i]);
			if (LocateElem(&list, prolist[i]) == -1)
				sg_InsertElem(&list, list.length, prolist[i]);//如果总进程里面没有当前进程，则加入进去
		}
		Select_sort_Single(&sgList);
		Snode* node = sgList.head->next;
		//输出当前系统的进程
		for (int i = 0; i < sgList.length; i++) {
			cout << i << endl;
			PrintCurProInfo(node->elem);
			node = node->next;
		}
		//输出已结束的进程
		Snode* temp = list.head->next;
		for (int i = 0; i < list.length; i++) {
			if (LocateElem(&sgList, temp->elem) == -1)//将结束的进程加入到双链表
			{
				db_InsertElem(&dbList, dbList.length, temp->elem);

				FILETIME begintime, endtime, ipker, ipuser;
				DWORD ProID = temp->elem.ID;
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProID);
				GetProcessTimes(hProcess, &begintime, &endtime, &ipker, &ipuser);//开始、结束
				LPSYSTEMTIME pSystemTime = new SYSTEMTIME;
				DWORD etime = 0;

				FileTimeToSystemTime(&endtime, pSystemTime);
				pSystemTime->wHour = (pSystemTime->wHour + 8) % 24;  //将UTC时间(时间标准时间)转换为北京时间
				etime = pSystemTime->wHour * 60 * 60 + pSystemTime->wMinute * 60 + pSystemTime->wSecond;//运算DWORD形式结束时间，单位为S

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


		//每过5s获取一次当前进程
		Sleep(5000);
	}

	system("pause");
	return 0;
}