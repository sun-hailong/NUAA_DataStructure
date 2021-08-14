#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define N 500                                                    //来了多少的客户
int MidOut = 0;                                                  //中途离开的客户统计
#define MaxSize N                                                //将队列的长度用来了多少人来定义（包括最后等到下班的客户）
int window = 4;                                                  //窗口数

//定义时间 
typedef   struct Time
{
    int h;
    int m;
}T;

//定义每个客户的到达窗口时间，等待时间，开始办理时间，
//业务处理时间，结束业务时间(离开时间)，身份
struct customer
{
    T        ArriveTime;                                         //到达银行时间 
    T        WaitTime;                                           //等待时间 
    T        StartTime;                                          //开始办理业务时间 
    T        FinishTime;                                         //结束办理业务时间    
    int      BusinessTime;                                       //业务处理时间(假设业务处理时间为整数) 
    int      Number;                                             //客户进入银行的序号，先用来标记     
    int      VIP;                                                //定义是否是VIP(概率)
};

//链队数据结点
typedef struct _tag_Node
{
    customer data;
    struct _tag_Node* next;
} DataNode;

//链队头结点
typedef struct
{
    int ServeNum;
    DataNode* front;
    DataNode* rear;
} LinkQueue;
//创建队列
void CreateQueue(LinkQueue*& queue)         //创建链队
{
    queue = (LinkQueue*)malloc(sizeof(LinkQueue));
    memset(queue, 0, sizeof(LinkQueue));
    queue->front = queue->rear = NULL;
}
//销毁队列
void DestroyQueue(LinkQueue* queue)                                //销毁链队
{
    CreateQueue(queue);
    free(queue);
}
//队长
int QueueLength(LinkQueue* queue)           //返回队列中数据元素个数
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
//队是否为空
bool QueueEmpty(LinkQueue* queue)           //判断链队是否为空
{
    return(queue->rear == NULL);
}
//入队
void enQueue(LinkQueue*& queue, customer e)                        //入队
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
//出队
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
//队列的复制
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
//中途离开
bool IsLeaf()                                               //20分之一的概率中途离开
{
    bool IF = false;
    int i;
    i = rand() % 20;
    if (i == 1)
        IF = true;
    return IF;
}
//来人模拟
bool IsCome()                                                   //十分之一的几率来人
{
    bool IF = false;
    int i;
    i = rand() % 10;
    if (i == 1)
        IF = true;
    return IF;
}
//是否是vip
bool IsVip(customer people)                    
{
    bool IF = false;
    if (people.VIP == 0)
        IF = true;
    return IF;
}
//设置进队的时间
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
    
    else if (QueueEmpty(queue))                                 //队列中用 结束时间进行累计加时间
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
//输出打印时间表，number的初值置为的是1，来判断这是不是顾客
void TimeList(customer people[])
{
    int i;
    for ( i = 0; people[i].Number != -1; i++)
    {
        printf("第%2d号顾客的到达时间：%02d:%02d      开始时间：%02d:%02d     业务处理时间：%2d 分钟    离开时间：%02d:%02d       等待时间：%3d 分钟\n",
                people[i].Number, people[i].ArriveTime.h, people[i].ArriveTime.m,
                people[i].StartTime.h, people[i].StartTime.m,
                people[i].BusinessTime,
                people[i].FinishTime.h, people[i].FinishTime.m,
                people[i].WaitTime.h * 60 + people[i].WaitTime.m);
        
    }
    printf("总共光临的顾客共：%d 人次\n", i);
}
//人数统计
void PeopleList(LinkQueue* queue[])
{
    for (int i = 1; i < window; i++)
    {
        printf("%d号窗口服务人次:%d\n",i, queue[i]->ServeNum);
    }
    printf("VIP窗口服务人次:%d\n",queue[0]->ServeNum);
    printf("中途离开人次:%d", MidOut);
}
//将所有时间的数值进行计算一遍   上下两个函数可以合并成一个函数
void TimeCaculate(customer people[])
{
    for (int i = 0; people[i].Number != -1; i++)
    {
        people[i].WaitTime.h = people[i].StartTime.h - people[i].ArriveTime.h;
        people[i].WaitTime.m = people[i].StartTime.m - people[i].ArriveTime.m;
    }
}

//进行初始化  排队，客户的业务时间，
void Init(customer people[],LinkQueue *queue[])
{
    int i;
    for (i = 0; i < window; i++)
    {
        CreateQueue(queue[i]);
        queue[i]->ServeNum = 0;                          //初始窗口服务顾客个数 
    }

    for (int i = 0; i < MaxSize; i++)
    { //初始顾客时间 
        people[i].BusinessTime = 5+rand() % 20;               //服务时间随机
        people[i].Number = -1;                              //-1为 初始化
        people[i].VIP = rand() % 100;                  //100分之一的概率
        if(IsVip(people[i]))
            people[i].BusinessTime = rand() % 5+1;               
    }
}
//显示排队的情形
void ShowQueue(LinkQueue* queue[MaxSize])
{
    for (int i = 1; i < window; i++)
    {
        printf("%d号窗口:", i );
        DataNode* m = queue[i]->front;
        while (m != NULL)
        {
            printf("%d号客户 ", m->data.Number);
            m = m->next;
        }
        printf("\n");
    }
    printf("VIP窗口:");
    DataNode* m = queue[0]->front;
    while (m != NULL)
    {
        printf("%d号客户 ", m->data.Number);
        m = m->next;
    }
    printf("\n");
}
//找出最小的队，然后排队
int  FindMinQueueNum(LinkQueue*queue[])
{
    int i,Num = 1;                              //num寻找最小的队列
    int TimeArray[MaxSize];                  //将顾客在每个队列需要等待的时间放在一个数组里

    for (i = 0; i < MaxSize; i++)
    {
        TimeArray[i] = 0;
    }
    for ( i = 1; i < window ; i++)                   
    {
        int AllTime;
          
        LinkQueue* temp;
        customer e;
        CopyQueue(queue[i], temp);                  //这里自己编写一个复制函数，以防将原来的队列弄错
        if (!QueueEmpty(temp))
        {
            AllTime = temp->front->data.BusinessTime;              //计算每个队列的所有业务时间之和，用alltime
            while (!QueueEmpty(temp))
            {
                deQueue(temp, e);
                AllTime += e.BusinessTime;
            }
            TimeArray[i] = AllTime;
        }
    }
    //找出需要时间最短的那个队列   
    for (i = 1; i < window; i++)                    //从1开始是将0给了vip队列
    {
        if (TimeArray[i] < TimeArray[Num])
            Num = i;
    }

    return Num;
}
//中途离队函数,//第一种边出队，边进队
void MidOut1(LinkQueue* &queue, int i )
{
    int j = 0;//计数
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
    //printf("————————%d : %d————————\n", time / 60, time % 60);
    printf("%2d号客户中途有事离开窗口。\n", p1.Number);
    //people[p1.Number].FinishTime.h = people[p1.Number-1].StartTime.h = time % 60;              //离开客户时间的调整
    //people[loc].FinishTime.m = people[loc].StartTime.m = time / 60;
   // people[loc].BusinessTime = 0;

    //ShowQueue(queue);
    for (j; j < QueueLength(queue); j++)
    {
        deQueue(queue, p1);
        enQueue(temp, p1);
        printf("%d\n", p1.Number);
    }
    //将出队后的队列传回原队列
    CopyQueue(temp, queue);
    DestroyQueue(temp);
}
//中途离队函数,//第一种边出队，边进队（略改）
void  MidOut2(LinkQueue* &queue, customer people[], int time )
{
    //int loc = rand() % (QueueLength(queue[j]) - 1) + 1;         //队列中随机一个位置的人离开
    int i = rand() % (QueueLength(queue) - 1) + 1;

    int j = 0;//计数
    LinkQueue* temp;
    CreateQueue(temp);
    DataNode* m = queue->front;
    DataNode* p = queue->front;

    for (j; j < QueueLength(queue)&&!QueueEmpty(queue)&&m; j++)                    //&&m
    {
        if (j == i )    //这里哭死了。。==写成=  呜呜~~~
        {
            printf("%2d号客户中途有事离开窗口。\n", m->data.Number);

            people[m->data.Number - 1].FinishTime.h = people[m->data.Number - 1].StartTime.h = time / 60;          //离开客户时间的调整
            people[m->data.Number - 1].FinishTime.m = people[m->data.Number - 1].StartTime.m = time % 60;
            people[m->data.Number - 1].BusinessTime = 0;

        }
        else if(j==0)//temp还是为空的时候
        {
            enQueue(temp, m->data);
            p = p->next;
        }
        else//temp不为空了
        {
            //出去后，整个队列时间的调整
            people[m->data.Number - 1].StartTime.h = p->data.StartTime.h;
            people[m->data.Number - 1].StartTime.m = p->data.StartTime.m;
            enQueue(temp,people[m->data.Number - 1]);
            p = p->next;

        }
        m = m->next;

    }
    //将出队后的队列传回原队列
    CopyQueue(temp, queue);
    DestroyQueue(temp);

}
//选择进行营业时间的改变
void Choose(int *x,int* y)
{
    int ch=20200724;

    while (ch != 1) 
    {
        printf("请输入时间：（形如09:00-18:00，输入9 18,再按回车键 ）\n");
        scanf("%d %d", &*x,& *y);
        printf("将要模拟的时间是：%02d:00-%02d:00 ,如果无误请输入 1 后，按回车开始模拟。否则输入0，重新输入时间。\n",*x,*y);
        //fflush(stdin);
        scanf("%d", &ch);
   }
}
//题目菜单
bool Menu(int *x, int *y, bool *Action)
{
    int ch = 0;
    printf("%s\n", "┌──────────────────题目及要求──────────────────┐ ");
    printf("%s\n", "│营业窗口队列模拟（***）                                                           │ \n");
    printf("%s\n", "│要求：实现具有n个窗口的现实队列模拟，统计每人的等待时间。                         │ \n");
    printf("%s\n", "│1).随机产生顾客的到达时间和服务时间存盘。                                         │ \n");
    printf("%s\n", "│2).实现队列的插入和删除。                                                         │ \n");
    printf("%s\n", "│3).当有顾客离开时，根据队列长度调整队尾。                                         │ \n");
    printf("%s\n", "│4).考虑顾客中途离队的情况。                                                       │ \n");
    printf("%s\n", "│5).考虑顾客具有优先级的情况。                                                     │ \n");
    printf("%s\n", "└─────────────────────────────────────────┘ \n");
    
    printf("\n");
    //1.进行默认模拟时间为09:00-18:00
    //2.进行整点时间模拟，输入整点的开始和结束时间
    //0.退出
    printf("%s\n", "┌──┬───────────────操作及选项──────────────────┐ \n");
    printf("%s\n", "├──┼──────────────────────────────────────┤ \n");
    printf("%s\n", "│1.  │进行默认模拟时间为 09:00-18:00                                              │ \n");
    printf("%s\n", "│2.  │进行整点时间模拟，输入整点的开始和结束时间                                  │ \n");
    printf("%s\n", "│0.  │退出模拟                                                                    │ \n");
    printf("%s\n", "└──┴──────────────────────────────────────┘ \n");

    printf("请输入你的选择：\n");
    scanf("%d", &ch);
    switch (ch)
    {
    case 0:
        printf("即将退出模拟！\n");
        *Action = false;
        break;
    case 1:
        printf("将要模拟的时间是：09:00-18:00\n");
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

//模拟的排队
void Serve(int t1, int t2)
{
    srand((unsigned int)time(NULL));			                 //随机数
    int i=0,j=0, time=0;
    customer people[MaxSize];                                   //
    LinkQueue* queue[20];
    Init( people, queue);                                       //将队列，客户进行初始化
    customer temp;                                               //出队的客户


    //Choose(&t1, &t2);
    printf("%d,%d", t1,t2);
    //begain
    //模拟的整体函数
    printf("————————————————————————————开始营业————————————————————————————\n");
    //正常营业
    for (time =t1*60; time < t2 * 60; time++)
    {       
        //是否有顾客来
        if (IsCome())
        {
            int t = FindMinQueueNum(queue);            //t用来存放最短的那个队列
            people[i].Number = i + 1;                   //初始化客户的编号
            printf("————————————————————————————%02d:%02d—————————————————————————————\n", time / 60, time % 60);
            if (IsVip(people[i]))
            {
                printf("————————————————————亲爱的贵宾客户，请前往VIP窗口，谢谢!————————————————————\n");
                SetTime(queue[0], people[i], time);
                enQueue(queue[0], people[i]);
            }
            //不是vip的入队列并且设置时间
            else
            {
                SetTime(queue[t], people[i], time);        ///先设置时间在入队
                enQueue(queue[t], people[i]);                   //入队
                printf("———————————————————————%2d号客户进入%2d号窗口排队。———————————————————————\n", people[i].Number, t);
            }
            ShowQueue(queue);
            i++;           //i++应该放在有i的末尾，放在前面测试时出错         
        }
        //出队客户，如果 队头客户 到达时间+业务时间和当前时间相同，说明处理完毕了，就让他离开了
        for (j= 0; j < window; j++)
        {
            if (!QueueEmpty(queue[j]))
            {
                bool queuein = true;
                int alltime = 0;                                                                //用alltime来替代一长串运算
                //开始时间h+m+业务时间
                alltime = (queue[j]->front->data.StartTime.h) * 60 + queue[j]->front->data.StartTime.m + queue[j]->front->data.BusinessTime;
                if (alltime == time && queuein == true)
                {
                    LinkQueue* tempqueue;
                    CreateQueue(tempqueue);
                    CopyQueue(queue[j], tempqueue);
                    deQueue(queue[j], temp);                                       //出队
                    queue[j]->ServeNum++;                                       //人员统计

                    people[temp.Number - 1].FinishTime.h = time / 60;           //记录结束的时间
                    people[temp.Number - 1].FinishTime.m = time % 60;

                    printf("————————————————————————————%02d:%02d—————————————————————————————\n", time / 60, time % 60);
                    printf("——————————————————————————%2d号客户离开窗口——————————————————————————\n", temp.Number);
                    queuein = false;
                    ShowQueue(queue);

                }
                //中途离队,是未到队头的人离开
                if (IsLeaf() && QueueLength(queue[j]) > 2 && queuein == true)
                {
                    printf("————————————————————————————%02d:%02d————————————————————————————\n", time / 60, time % 60);
                    MidOut2(queue[j], people, time);                    
                    MidOut++;
                    queuein = false;
                    ShowQueue(queue);

                }
            }
        }
    }
    printf("————————————————————————————%02d:%02d————————————————————————————\n", time / 60, time % 60);
    //时间到了，剩下的出队
    for (int j = 0; j < window; j++)
    {
        while (!QueueEmpty(queue[j]))
        {
            deQueue(queue[j], temp);                              //出队
            MidOut++;                                             //中途离开的人
            printf("——————————————————————今天的时间到了哦！明天再来吧！———————————————————————\n");
            printf(" %d号客户离开。\n", temp.Number);
            people[temp.Number - 1].FinishTime.h = t2;            //记录结束的时间
            people[temp.Number - 1].FinishTime.m = 0;
            people[temp.Number - 1].StartTime.h = t2;
            people[temp.Number - 1].StartTime.m = 0;
            people[temp.Number - 1].BusinessTime = (people[temp.Number - 1].FinishTime.h - people[temp.Number - 1].StartTime.h) * 60 + people[temp.Number - 1].FinishTime.m - people[temp.Number - 1].StartTime.m;

            ShowQueue(queue);
        }
    }
    printf("\n");

    printf("————————————————————————————人员统计————————————————————————————\n");
    TimeCaculate(people);
    TimeList(people);                           //将人员时间统计表打印出来
    //每个队列服务的人以及中途离开的人的统计
    PeopleList(queue);
    //将创建的队列进行清除
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