#pragma once
#ifndef PROCESS_ZX_H
#define PROCESS_ZX_H
/*头文件包含*/
#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include <iostream>
#include <time.h> 
using namespace std;
#define  ProcessNum 1024

/*数据结构定义*/
typedef struct MSTime
{
    int minute;
    int second;
} MSTime ;
typedef struct Time
{
    int Year;
    int Month;
    int Day;
    int Hour;
    int Minute;
    int Second;
} Time ;
typedef struct ProcessInfo
{
	DWORD  ID ;                              //进程ID,DWORD
	TCHAR  Name[MAX_PATH];                   //进程名称,TCHAR
	SIZE_T Memory;                           //进程占用内存,以K为单位
	Time   BeginTime;                        //进程开始时间,Time
	MSTime DuraTime;                         //进程持续时间,int (min)
	Time   EndTime;							 //进程结束时间,Time
} ProInfo ;

//提升进程权限
#pragma comment(lib, "psapi.lib") 
bool AdjustPurview()
{
    TOKEN_PRIVILEGES TokenPrivileges;
    BOOL bRet;
    HANDLE hToken;

    LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &TokenPrivileges.Privileges[0].Luid);
    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);

    TokenPrivileges.PrivilegeCount = 1;
    TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    bRet = AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, 0, NULL, NULL);

    CloseHandle(hToken);
    return bRet == TRUE;
}
//获取信息
bool GetCurrentProcesses(ProInfo pil[] ,  int& pronum , int sizeofpil = 1024 )  //获取进程信息，成功返回true,失败返回false
{//pil 进程信息数组  pronum 返回进程数量 sizeofpil 进程信息的数组元素个数
    HANDLE hProcess ;                       //进程句柄，指向进程
    DWORD ProID;                            //进程ID
    PROCESS_MEMORY_COUNTERS pmc ;           //进程信息库（使用其中的内存信息）
    FILETIME begintime, endtime , ipker , ipuser ;  //进程的开始时间，持续时间和结束时间(后面是处于核心、用户状态时间)

    //获取进程列表
    DWORD Processes[1024], cbNeeded , cProcess ;
    if ( !EnumProcesses( Processes , sizeof(Processes), &cbNeeded) )
        return false;
    cProcess = cbNeeded / sizeof(DWORD);
    //按进程列表依次打开进程获取信息
    LPSYSTEMTIME pSystemTime = new SYSTEMTIME;//用来将filetime转换为systemtime，systemtime可以让人看懂
    DWORD btime = 0, etime = 0;               //DWORD形式的开始结束时间，用来运算持续时间
    int i = 0;
    
    //获取系统时间
    LPSYSTEMTIME systime = new SYSTEMTIME;
    ::GetSystemTime(systime);
    systime->wHour = (systime->wHour + 8) % 24; //转换UTC时间为背景时间
    DWORD stime = systime->wHour * 60 * 60 + systime->wMinute * 60 + systime->wSecond ;//转换为DWORD型,单位S

    //开始循环寻找
    int j = -1;                  //有效数据的记录index
    for ( i = 0; i < cProcess ; i++ )
    {
        //打开进程
        ProID = Processes[i];
        if (ProID == 0)
            continue;
        hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ ,FALSE , ProID ); //打开ID指向的进程，获得查看信息、读取内存的权限
        if ( NULL == hProcess)  //如果未能成功打开进程，返回错误
            continue;

        j++;
        //获取进程ID
        pil[j].ID = ProID; 
        //获取进程名称
        GetModuleFileNameEx( hProcess , NULL, pil[j].Name , MAX_PATH );
        //获取进程内存
        GetProcessMemoryInfo( hProcess , &pmc , sizeof(pmc) );
        pil[j].Memory = pmc.PeakWorkingSetSize / 1024;
        //获取进程时间
        GetProcessTimes( hProcess, &begintime, &endtime, &ipker, &ipuser);//开始、结束
        //开始时间
        FileTimeToSystemTime( &begintime , pSystemTime );
        pSystemTime->wHour = ( pSystemTime->wHour + 8) % 24;  //将UTC时间(时间标准时间)转换为北京时间
        btime = pSystemTime->wHour * 60 * 60 + pSystemTime->wMinute * 60 + pSystemTime->wSecond ; //运算DWORD形式开始时间，单位为S

        pil[j].BeginTime.Year  = pSystemTime->wYear;
        pil[j].BeginTime.Month = pSystemTime->wMonth;
        pil[j].BeginTime.Day = pSystemTime->wDay;
        pil[j].BeginTime.Hour = pSystemTime->wHour;
        pil[j].BeginTime.Minute = pSystemTime->wMinute;
        pil[j].BeginTime.Second = pSystemTime->wSecond;
        //结束时间
        FileTimeToSystemTime( &endtime, pSystemTime );
        pSystemTime->wHour = (pSystemTime->wHour + 8) % 24;  //将UTC时间(时间标准时间)转换为北京时间
        etime = pSystemTime->wHour * 60 * 60 + pSystemTime->wMinute * 60 + pSystemTime->wSecond;//运算DWORD形式结束时间，单位为S

        pil[j].EndTime.Year = pSystemTime->wYear;
        pil[j].EndTime.Month = pSystemTime->wMonth;
        pil[j].EndTime.Day = pSystemTime->wDay;
        pil[j].EndTime.Hour = pSystemTime->wHour;
        pil[j].EndTime.Minute = pSystemTime->wMinute;
        pil[j].EndTime.Second = pSystemTime->wSecond;
        //持续时间
        pil[j].DuraTime.minute = (stime - btime) / 60;
        pil[j].DuraTime.second = (stime - btime) % 60;
    }
    pronum = j + 1;

    return true;
}
//打印信息
void PrintTime(Time T)
{
    cout << T.Year << "年" << T.Month << "月" << T.Day << "日"<< T.Hour << "时" << T.Minute << "分" << T.Second << "秒" <<"\n";
}

void PrintProInfo(ProInfo PI)
{
    if (PI.ID == 0) return;
    cout << "------------------------------------------------" << "\n";
    cout << "进程ID：" << PI.ID << "\n";
    printf("进程名称：%ws", PI.Name);
    cout << "\n";
    cout << "进程内存：" << PI.Memory << "K" << "\n" ; 
    cout << "开始时间：";
    PrintTime(PI.BeginTime);
    cout << "持续时间：" << PI.DuraTime.minute << " min "  << PI.DuraTime.second << " s"<< "\n";
    cout << "结束时间：";
    if ( PI.EndTime.Year == 1601 )  //起始时间，即未结束
    {
        cout << "---" << "\n";
    }
    else
    {
        PrintTime(PI.EndTime);
    }
}

void PrintCurProInfo(ProInfo PI)
{
    if (PI.ID == 0) return;
    cout << "------------------------------------------------" << "\n";
    cout << "进程ID：" << PI.ID << "\n";
    cout << "持续时间：" << PI.DuraTime.minute << " min " << PI.DuraTime.second << " s" << "\n";
    cout << "进程内存：" << PI.Memory << "K" << "\n\n";
}

void PrintEndProInfo(ProInfo PI)
{
    if (PI.ID == 0) return;
    cout << "************************************************" << "\n";
    cout << "进程ID：" << PI.ID << "\n";
    cout << "持续时间：" << PI.DuraTime.minute << " min " << PI.DuraTime.second << " s" << "\n";
    cout << "结束时间：";
    if (PI.EndTime.Year == 1601)  //起始时间，即未结束
    {
        cout << "---" << "\n\n";
    }
    else
    {
        PrintTime(PI.EndTime);
    }
}

//获取系统时间
void GetSystime(Time& time)
{
    LPSYSTEMTIME systime = new SYSTEMTIME;
    ::GetSystemTime(systime);
    systime->wHour = (systime->wHour + 8) % 24; //转换UTC时间为背景时间

    time.Year = systime->wYear;
    time.Month = systime->wMonth;
    time.Day = systime->wDay;
    time.Hour = systime->wHour;
    time.Minute = systime->wMinute;
    time.Second = systime->wSecond;
}

//复制Time
void TimeCPY(Time& mbt, const Time sourcet)
{
    mbt.Year = sourcet.Year;
    mbt.Month = sourcet.Month;
    mbt.Day = sourcet.Day;
    mbt.Hour = sourcet.Hour;
    mbt.Minute = sourcet.Minute;
    mbt.Second = sourcet.Second;
}
//复制ProInfo
void ProInfoCPY(ProInfo& mbp, const ProInfo sourcep)
{
    mbp.ID = sourcep.ID;
    mbp.Memory = sourcep.Memory;
    ua_tcscpy_s(mbp.Name, sourcep.Name);
    TimeCPY(mbp.BeginTime, sourcep.BeginTime);
    mbp.DuraTime.minute = sourcep.DuraTime.minute;
    mbp.DuraTime.second = sourcep.DuraTime.second;
    TimeCPY(mbp.EndTime, sourcep.EndTime);
}
//比较MSTime
int MSTimeCMP(MSTime time_a, MSTime time_b)//比较time_a和time_b,如果a大于b返回1，如果a小于b返回-1;相等返回0
{
    if (time_a.minute > time_b.minute)
    {
        return 1;
    }
    else
    {
        if (time_a.minute < time_b.minute)
            return -1;
        else
        {
            if (time_a.second > time_b.second)
            {
                return 1;
            }
            else
            {
                if (time_a.second < time_b.second)
                {
                    return -1;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
}
#endif // !PROCESS_ZX_H