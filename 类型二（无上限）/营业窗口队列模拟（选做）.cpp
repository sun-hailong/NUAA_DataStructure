#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define N 500                                                    //���˶��ٵĿͻ�
int MidOut = 0;                                                  //��;�뿪�Ŀͻ�ͳ��
#define MaxSize N                                                //�����еĳ��������˶����������壨�������ȵ��°�Ŀͻ���
int window = 4;                                                  //������

//����ʱ�� 
typedef   struct Time
{
    int h;
    int m;
}T;

//����ÿ���ͻ��ĵ��ﴰ��ʱ�䣬�ȴ�ʱ�䣬��ʼ����ʱ�䣬
//ҵ����ʱ�䣬����ҵ��ʱ��(�뿪ʱ��)�����
struct customer
{
    T        ArriveTime;                                         //��������ʱ�� 
    T        WaitTime;                                           //�ȴ�ʱ�� 
    T        StartTime;                                          //��ʼ����ҵ��ʱ�� 
    T        FinishTime;                                         //��������ҵ��ʱ��    
    int      BusinessTime;                                       //ҵ����ʱ��(����ҵ����ʱ��Ϊ����) 
    int      Number;                                             //�ͻ��������е���ţ����������     
    int      VIP;                                                //�����Ƿ���VIP(����)
};

//�������ݽ��
typedef struct _tag_Node
{
    customer data;
    struct _tag_Node* next;
} DataNode;

//����ͷ���
typedef struct
{
    int ServeNum;
    DataNode* front;
    DataNode* rear;
} LinkQueue;
//��������
void CreateQueue(LinkQueue*& queue)         //��������
{
    queue = (LinkQueue*)malloc(sizeof(LinkQueue));
    memset(queue, 0, sizeof(LinkQueue));
    queue->front = queue->rear = NULL;
}
//���ٶ���
void DestroyQueue(LinkQueue* queue)                                //��������
{
    CreateQueue(queue);
    free(queue);
}
//�ӳ�
int QueueLength(LinkQueue* queue)           //���ض���������Ԫ�ظ���
{
    DataNode* p = queue->front;
    int i = 0;
    while (p)
    {
        i++;
        p = p->next;
    }
    //printf("%d\n", i);
    return(i);
}
//���Ƿ�Ϊ��
bool QueueEmpty(LinkQueue* queue)           //�ж������Ƿ�Ϊ��
{
    return(queue->rear == NULL);
}
//���
void enQueue(LinkQueue*& queue, customer e)                        //���
{
    DataNode* p = NULL;
    p = (DataNode*)malloc(sizeof(DataNode));
    memset(p, 0, sizeof(DataNode));


    p->data = e;
    if (queue->rear == NULL && queue->front == NULL)
        queue->front = queue->rear = p;
    else
    {
        queue->rear->next = p;
        queue->rear = p;
    }
}
//����
bool deQueue(LinkQueue*& queue, customer& e)  
{
    DataNode* p;

    if (queue->rear == NULL)
        return false;

    p = queue->front;
    if (queue->front == queue->rear)
        queue->front = queue->rear = NULL;
    else
        queue->front = queue->front->next;
    e = p->data;
    free(p);
    return true;
}
//���еĸ���
void CopyQueue(LinkQueue*& queue, LinkQueue* &temp)
{
    CreateQueue(temp);
    DataNode* change=queue->front;
    while (change)
    {
        enQueue(temp,change->data);
        change = change->next;
    }
}
LinkQueue* temp = NULL;
//��;�뿪
bool IsLeaf()                                               //20��֮һ�ĸ�����;�뿪
{
    bool IF = false;
    int i;
    i = rand() % 20;
    if (i == 1)
        IF = true;
    return IF;
}
//����ģ��
bool IsCome()                                                   //ʮ��֮һ�ļ�������
{
    bool IF = false;
    int i;
    i = rand() % 10;
    if (i == 1)
        IF = true;
    return IF;
}
//�Ƿ���vip
bool IsVip(customer people)                    
{
    bool IF = false;
    if (people.VIP == 0)
        IF = true;
    return IF;
}
//���ý��ӵ�ʱ��
void SetTime(LinkQueue*& queue, customer &people,int time)
{
    people.ArriveTime.h = time / 60;
    people.ArriveTime.m = time % 60;
    if (IsVip(people))
    {
        people.WaitTime.h = 0;
        people.WaitTime.m = 0;
        people.StartTime.h = people.ArriveTime.h;
        people.StartTime.m = people.ArriveTime.m;

     }
    
    else if (QueueEmpty(queue))                                 //�������� ����ʱ������ۼƼ�ʱ��
    { 
        people.WaitTime.h = 0;
        people.WaitTime.m = 0;
        people.StartTime.h = people.ArriveTime.h;
        people.StartTime.m = people.ArriveTime.m;


        people.FinishTime.h = people.StartTime.h + (people.StartTime.m + people.BusinessTime) / 60;
        people.FinishTime.m = (people.StartTime.m + people.BusinessTime) % 60;
    }
    else if (!QueueEmpty(queue))
    {
        people.StartTime.h = queue->rear->data.FinishTime.h;
        people.StartTime.m = queue->rear->data.FinishTime.m;
        people.FinishTime.h = people.StartTime.h + (people.StartTime.m + people.BusinessTime) / 60;
        people.FinishTime.m = (people.StartTime.m + people.BusinessTime) % 60;

        people.WaitTime.h = people.StartTime.h - people.ArriveTime.h;
        people.WaitTime.m = people.StartTime.m - people.ArriveTime.m;
    }
}
//�����ӡʱ���number�ĳ�ֵ��Ϊ����1�����ж����ǲ��ǹ˿�
void TimeList(customer people[])
{
    int i;
    for ( i = 0; people[i].Number != -1; i++)
    {
        printf("��%2d�Ź˿͵ĵ���ʱ�䣺%02d:%02d      ��ʼʱ�䣺%02d:%02d     ҵ����ʱ�䣺%2d ����    �뿪ʱ�䣺%02d:%02d       �ȴ�ʱ�䣺%3d ����\n",
                people[i].Number, people[i].ArriveTime.h, people[i].ArriveTime.m,
                people[i].StartTime.h, people[i].StartTime.m,
                people[i].BusinessTime,
                people[i].FinishTime.h, people[i].FinishTime.m,
                people[i].WaitTime.h * 60 + people[i].WaitTime.m);
        
    }
    printf("�ܹ����ٵĹ˿͹���%d �˴�\n", i);
}
//����ͳ��
void PeopleList(LinkQueue* queue[])
{
    for (int i = 1; i < window; i++)
    {
        printf("%d�Ŵ��ڷ����˴�:%d\n",i, queue[i]->ServeNum);
    }
    printf("VIP���ڷ����˴�:%d\n",queue[0]->ServeNum);
    printf("��;�뿪�˴�:%d", MidOut);
}
//������ʱ�����ֵ���м���һ��   ���������������Ժϲ���һ������
void TimeCaculate(customer people[])
{
    for (int i = 0; people[i].Number != -1; i++)
    {
        people[i].WaitTime.h = people[i].StartTime.h - people[i].ArriveTime.h;
        people[i].WaitTime.m = people[i].StartTime.m - people[i].ArriveTime.m;
    }
}

//���г�ʼ��  �Ŷӣ��ͻ���ҵ��ʱ�䣬
void Init(customer people[],LinkQueue *queue[])
{
    int i;
    for (i = 0; i < window; i++)
    {
        CreateQueue(queue[i]);
        queue[i]->ServeNum = 0;                          //��ʼ���ڷ���˿͸��� 
    }

    for (int i = 0; i < MaxSize; i++)
    { //��ʼ�˿�ʱ�� 
        people[i].BusinessTime = 5+rand() % 20;               //����ʱ�����
        people[i].Number = -1;                              //-1Ϊ ��ʼ��
        people[i].VIP = rand() % 100;                  //100��֮һ�ĸ���
        if(IsVip(people[i]))
            people[i].BusinessTime = rand() % 5+1;               
    }
}
//��ʾ�Ŷӵ�����
void ShowQueue(LinkQueue* queue[MaxSize])
{
    for (int i = 1; i < window; i++)
    {
        printf("%d�Ŵ���:", i );
        DataNode* m = queue[i]->front;
        while (m != NULL)
        {
            printf("%d�ſͻ� ", m->data.Number);
            m = m->next;
        }
        printf("\n");
    }
    printf("VIP����:");
    DataNode* m = queue[0]->front;
    while (m != NULL)
    {
        printf("%d�ſͻ� ", m->data.Number);
        m = m->next;
    }
    printf("\n");
}
//�ҳ���С�Ķӣ�Ȼ���Ŷ�
int  FindMinQueueNum(LinkQueue*queue[])
{
    int i,Num = 1;                              //numѰ����С�Ķ���
    int TimeArray[MaxSize];                  //���˿���ÿ��������Ҫ�ȴ���ʱ�����һ��������

    for (i = 0; i < MaxSize; i++)
    {
        TimeArray[i] = 0;
    }
    for ( i = 1; i < window ; i++)                   
    {
        int AllTime;
          
        LinkQueue* temp;
        customer e;
        CopyQueue(queue[i], temp);                  //�����Լ���дһ�����ƺ������Է���ԭ���Ķ���Ū��
        if (!QueueEmpty(temp))
        {
            AllTime = temp->front->data.BusinessTime;              //����ÿ�����е�����ҵ��ʱ��֮�ͣ���alltime
            while (!QueueEmpty(temp))
            {
                deQueue(temp, e);
                AllTime += e.BusinessTime;
            }
            TimeArray[i] = AllTime;
        }
    }
    //�ҳ���Ҫʱ����̵��Ǹ�����   
    for (i = 1; i < window; i++)                    //��1��ʼ�ǽ�0����vip����
    {
        if (TimeArray[i] < TimeArray[Num])
            Num = i;
    }

    return Num;
}
//��;��Ӻ���,//��һ�ֱ߳��ӣ��߽���
void MidOut1(LinkQueue* &queue, int i )
{
    int j = 0;//����
    LinkQueue* temp;
    CreateQueue(temp);
    customer p1;
    //CopyQueue(queue,temp);
    
    for (j; j < i ; j++)
    {
        deQueue(queue, p1);
        enQueue(temp, p1);
        printf("%d\n", p1.Number);

    }
    deQueue(queue, p1);
    //printf("����������������%d : %d����������������\n", time / 60, time % 60);
    printf("%2d�ſͻ���;�����뿪���ڡ�\n", p1.Number);
    //people[p1.Number].FinishTime.h = people[p1.Number-1].StartTime.h = time % 60;              //�뿪�ͻ�ʱ��ĵ���
    //people[loc].FinishTime.m = people[loc].StartTime.m = time / 60;
   // people[loc].BusinessTime = 0;

    //ShowQueue(queue);
    for (j; j < QueueLength(queue); j++)
    {
        deQueue(queue, p1);
        enQueue(temp, p1);
        printf("%d\n", p1.Number);
    }
    //�����Ӻ�Ķ��д���ԭ����
    CopyQueue(temp, queue);
    DestroyQueue(temp);
}
//��;��Ӻ���,//��һ�ֱ߳��ӣ��߽��ӣ��Ըģ�
void  MidOut2(LinkQueue* &queue, customer people[], int time )
{
    //int loc = rand() % (QueueLength(queue[j]) - 1) + 1;         //���������һ��λ�õ����뿪
    int i = rand() % (QueueLength(queue) - 1) + 1;

    int j = 0;//����
    LinkQueue* temp;
    CreateQueue(temp);
    DataNode* m = queue->front;
    DataNode* p = queue->front;

    for (j; j < QueueLength(queue)&&!QueueEmpty(queue)&&m; j++)                    //&&m
    {
        if (j == i )    //��������ˡ���==д��=  ����~~~
        {
            printf("%2d�ſͻ���;�����뿪���ڡ�\n", m->data.Number);

            people[m->data.Number - 1].FinishTime.h = people[m->data.Number - 1].StartTime.h = time / 60;          //�뿪�ͻ�ʱ��ĵ���
            people[m->data.Number - 1].FinishTime.m = people[m->data.Number - 1].StartTime.m = time % 60;
            people[m->data.Number - 1].BusinessTime = 0;

        }
        else if(j==0)//temp����Ϊ�յ�ʱ��
        {
            enQueue(temp, m->data);
            p = p->next;
        }
        else//temp��Ϊ����
        {
            //��ȥ����������ʱ��ĵ���
            people[m->data.Number - 1].StartTime.h = p->data.StartTime.h;
            people[m->data.Number - 1].StartTime.m = p->data.StartTime.m;
            enQueue(temp,people[m->data.Number - 1]);
            p = p->next;

        }
        m = m->next;

    }
    //�����Ӻ�Ķ��д���ԭ����
    CopyQueue(temp, queue);
    DestroyQueue(temp);

}
//ѡ�����Ӫҵʱ��ĸı�
void Choose(int *x,int* y)
{
    int ch=20200724;

    while (ch != 1) 
    {
        printf("������ʱ�䣺������09:00-18:00������9 18,�ٰ��س��� ��\n");
        scanf("%d %d", &*x,& *y);
        printf("��Ҫģ���ʱ���ǣ�%02d:00-%02d:00 ,������������� 1 �󣬰��س���ʼģ�⡣��������0����������ʱ�䡣\n",*x,*y);
        //fflush(stdin);
        scanf("%d", &ch);
   }
}
//��Ŀ�˵�
bool Menu(int *x, int *y, bool *Action)
{
    int ch = 0;
    printf("%s\n", "����������������������������������������Ŀ��Ҫ�󩤩����������������������������������� ");
    printf("%s\n", "��Ӫҵ���ڶ���ģ�⣨***��                                                           �� \n");
    printf("%s\n", "��Ҫ��ʵ�־���n�����ڵ���ʵ����ģ�⣬ͳ��ÿ�˵ĵȴ�ʱ�䡣                         �� \n");
    printf("%s\n", "��1).��������˿͵ĵ���ʱ��ͷ���ʱ����̡�                                         �� \n");
    printf("%s\n", "��2).ʵ�ֶ��еĲ����ɾ����                                                         �� \n");
    printf("%s\n", "��3).���й˿��뿪ʱ�����ݶ��г��ȵ�����β��                                         �� \n");
    printf("%s\n", "��4).���ǹ˿���;��ӵ������                                                       �� \n");
    printf("%s\n", "��5).���ǹ˿;������ȼ��������                                                     �� \n");
    printf("%s\n", "�������������������������������������������������������������������������������������� \n");
    
    printf("\n");
    //1.����Ĭ��ģ��ʱ��Ϊ09:00-18:00
    //2.��������ʱ��ģ�⣬��������Ŀ�ʼ�ͽ���ʱ��
    //0.�˳�
    printf("%s\n", "�������Щ�����������������������������������ѡ������������������������������������� \n");
    printf("%s\n", "�������੤���������������������������������������������������������������������������� \n");
    printf("%s\n", "��1.  ������Ĭ��ģ��ʱ��Ϊ 09:00-18:00                                              �� \n");
    printf("%s\n", "��2.  ����������ʱ��ģ�⣬��������Ŀ�ʼ�ͽ���ʱ��                                  �� \n");
    printf("%s\n", "��0.  ���˳�ģ��                                                                    �� \n");
    printf("%s\n", "�������ة����������������������������������������������������������������������������� \n");

    printf("���������ѡ��\n");
    scanf("%d", &ch);
    switch (ch)
    {
    case 0:
        printf("�����˳�ģ�⣡\n");
        *Action = false;
        break;
    case 1:
        printf("��Ҫģ���ʱ���ǣ�09:00-18:00\n");
        break;
    case 2:
        Choose(&*x, &*y);
        break;
      
    default:
        *Action = false;
        break;
    }
    return Action;
}

//ģ����Ŷ�
void Serve(int t1, int t2)
{
    srand((unsigned int)time(NULL));			                 //�����
    int i=0,j=0, time=0;
    customer people[MaxSize];                                   //
    LinkQueue* queue[20];
    Init( people, queue);                                       //�����У��ͻ����г�ʼ��
    customer temp;                                               //���ӵĿͻ�


    //Choose(&t1, &t2);
    printf("%d,%d", t1,t2);
    //begain
    //ģ������庯��
    printf("����������������������������������������������������������ʼӪҵ��������������������������������������������������������\n");
    //����Ӫҵ
    for (time =t1*60; time < t2 * 60; time++)
    {       
        //�Ƿ��й˿���
        if (IsCome())
        {
            int t = FindMinQueueNum(queue);            //t���������̵��Ǹ�����
            people[i].Number = i + 1;                   //��ʼ���ͻ��ı��
            printf("��������������������������������������������������������%02d:%02d����������������������������������������������������������\n", time / 60, time % 60);
            if (IsVip(people[i]))
            {
                printf("�����������������������������������������װ��Ĺ���ͻ�����ǰ��VIP���ڣ�лл!����������������������������������������\n");
                SetTime(queue[0], people[i], time);
                enQueue(queue[0], people[i]);
            }
            //����vip������в�������ʱ��
            else
            {
                SetTime(queue[t], people[i], time);        ///������ʱ�������
                enQueue(queue[t], people[i]);                   //���
                printf("����������������������������������������������%2d�ſͻ�����%2d�Ŵ����Ŷӡ�����������������������������������������������\n", people[i].Number, t);
            }
            ShowQueue(queue);
            i++;           //i++Ӧ�÷�����i��ĩβ������ǰ�����ʱ����         
        }
        //���ӿͻ������ ��ͷ�ͻ� ����ʱ��+ҵ��ʱ��͵�ǰʱ����ͬ��˵����������ˣ��������뿪��
        for (j= 0; j < window; j++)
        {
            if (!QueueEmpty(queue[j]))
            {
                bool queuein = true;
                int alltime = 0;                                                                //��alltime�����һ��������
                //��ʼʱ��h+m+ҵ��ʱ��
                alltime = (queue[j]->front->data.StartTime.h) * 60 + queue[j]->front->data.StartTime.m + queue[j]->front->data.BusinessTime;
                if (alltime == time && queuein == true)
                {
                    LinkQueue* tempqueue;
                    CreateQueue(tempqueue);
                    CopyQueue(queue[j], tempqueue);
                    deQueue(queue[j], temp);                                       //����
                    queue[j]->ServeNum++;                                       //��Աͳ��

                    people[temp.Number - 1].FinishTime.h = time / 60;           //��¼������ʱ��
                    people[temp.Number - 1].FinishTime.m = time % 60;

                    printf("��������������������������������������������������������%02d:%02d����������������������������������������������������������\n", time / 60, time % 60);
                    printf("����������������������������������������������������%2d�ſͻ��뿪���ڡ���������������������������������������������������\n", temp.Number);
                    queuein = false;
                    ShowQueue(queue);

                }
                //��;���,��δ����ͷ�����뿪
                if (IsLeaf() && QueueLength(queue[j]) > 2 && queuein == true)
                {
                    printf("��������������������������������������������������������%02d:%02d��������������������������������������������������������\n", time / 60, time % 60);
                    MidOut2(queue[j], people, time);                    
                    MidOut++;
                    queuein = false;
                    ShowQueue(queue);

                }
            }
        }
    }
    printf("��������������������������������������������������������%02d:%02d��������������������������������������������������������\n", time / 60, time % 60);
    //ʱ�䵽�ˣ�ʣ�µĳ���
    for (int j = 0; j < window; j++)
    {
        while (!QueueEmpty(queue[j]))
        {
            deQueue(queue[j], temp);                              //����
            MidOut++;                                             //��;�뿪����
            printf("�������������������������������������������������ʱ�䵽��Ŷ�����������ɣ�����������������������������������������������\n");
            printf(" %d�ſͻ��뿪��\n", temp.Number);
            people[temp.Number - 1].FinishTime.h = t2;            //��¼������ʱ��
            people[temp.Number - 1].FinishTime.m = 0;
            people[temp.Number - 1].StartTime.h = t2;
            people[temp.Number - 1].StartTime.m = 0;
            people[temp.Number - 1].BusinessTime = (people[temp.Number - 1].FinishTime.h - people[temp.Number - 1].StartTime.h) * 60 + people[temp.Number - 1].FinishTime.m - people[temp.Number - 1].StartTime.m;

            ShowQueue(queue);
        }
    }
    printf("\n");

    printf("����������������������������������������������������������Աͳ�ơ�������������������������������������������������������\n");
    TimeCaculate(people);
    TimeList(people);                           //����Աʱ��ͳ�Ʊ��ӡ����
    //ÿ�����з�������Լ���;�뿪���˵�ͳ��
    PeopleList(queue);
    //�������Ķ��н������
    for (i = 0; i < window; i++)
    {
        DestroyQueue(queue[i]);
    }

    //
}

int main()
{

    int t1 = 9, t2 = 18;
    bool IsAction=true;
    Menu(&t1,&t2,&IsAction);
    if(IsAction)
        Serve(t1, t2);

    return 0;
}