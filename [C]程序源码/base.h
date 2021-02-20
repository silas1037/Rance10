#include<windows.h>
#include<process.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <tlhelp32.h>
#include<dbghelp.h>
#define DefTimer 1000


//传入窗体类名，窗体存在返回TRUE，否则返回FALSE
BOOL  Find(char* ClassName){
    HWND hwnd = FindWindowA(ClassName, NULL);
    if (hwnd == 0)
		return FALSE;
	else
		return TRUE;
}

//线程树结构体
typedef struct{
    HANDLE TID;
    BOOL ALL_SET_UP;
    BOOL Quit;
    BOOL Pause;
    BOOL OutPut;
    BOOL Switch;
    char* Message;
    void (*Function)(void*);
    void* args;
    void* Next;
    int timer;
}ThreadTree;
typedef ThreadTree* BOX;
ThreadTree Ttree;
BOOL Quit=FALSE;
int Tcount=0;


//查询所在BOX
BOX GetBox(HANDLE TID){
    BOX Aim=&Ttree;
    for(int count=0;count<Tcount;count++){
        if(Aim->TID!=TID)
            Aim=(BOX)Aim->Next;
        else
            return Aim;
    }
    return NULL;
}


//标准线程
void StdThread(void* Box){
    BOX box=(BOX)Box;
    box->Message="线程上线\n";
    box->OutPut=TRUE;
    while(!box->Quit){
        while(!box->ALL_SET_UP);
        WaitForSingleObject(box->TID,box->timer);
        if(!box->Pause){
            box->Function(Box);
        }
    }
    box->Message="线程下线\n";
    box->OutPut=TRUE;
}


//消息线程
void MessageLoop(void* LPThreadTree){
    BOX Aim=(BOX)LPThreadTree;
    puts("[System]:消息循环上线");
    char Caption[10];
    //判断该次遍历是否有消息输出
    BOOL OutPut=FALSE;
    printf("[User]:");
    while(!Quit){
        for(int count=0;count<Tcount;count++){
            //任意消息队列存在输出就清除当前行
            if(Aim->OutPut){
                if(!OutPut){
                    OutPut=TRUE;
                    printf("\r");
                }
                sprintf(Caption,"%#x",Aim->TID);
                printf("[ThreadID#%s]:%s",Caption,Aim->Message);
                Aim->OutPut=FALSE;
            }
            Aim=(BOX)Aim->Next;
        }
        //恢复用户输入栏
        if(OutPut){
            printf("[User]:");
            OutPut=FALSE;
        }
        Aim=(BOX)LPThreadTree;
    }
    puts("[System]:消息循环下线");
}


//新建线程函数
HANDLE NewThread(void (*Function)(void*),void* args){
    if(!Tcount)
        _beginthread(MessageLoop,0,&Ttree);
    BOX Aim=&Ttree;
    for(int count=0;count<Tcount;count++)
        Aim=(BOX)Aim->Next;
    Aim->ALL_SET_UP=FALSE;
    Aim->Quit=FALSE;
    Aim->Pause=FALSE;
    Aim->OutPut=FALSE;
    Aim->Switch=FALSE;
    Aim->timer=DefTimer;
    Aim->args=args;
    //传入希望线程处理的函数
    Aim->Function=Function;
    Aim->Next=malloc(sizeof(ThreadTree));
    Aim->TID=(HANDLE)_beginthread(StdThread,0,Aim);
    Tcount++;
    Aim->ALL_SET_UP=TRUE;
    return Aim->TID;
}


//获取目标地址函数x32
DWORD GetAddress(char* ClassName,DWORD Ba_addr,int OffestCount,...){
    HWND hwnd = FindWindowA(ClassName, NULL);
    va_list args;//声明参数列表
    va_start(args,OffestCount);//以OffestCount为第一个参数地址
    if(hwnd!=NULL){
	        DWORD pid;
	        GetWindowThreadProcessId(hwnd, &pid);
	        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	        DWORD m_tempadd;
            DWORD m_offest;
            ReadProcessMemory(hProcess, (LPVOID)Ba_addr, &m_tempadd, 4, 0);
            for(int count=0;count<OffestCount-1;count++){
                m_offest=va_arg(args,DWORD);//从列表获取下一个参数
                ReadProcessMemory(hProcess, (LPVOID)(m_tempadd + m_offest), &m_tempadd, 4, 0);
            }
            return m_tempadd+va_arg(args,DWORD);
    }
    else return 0;
}


//获取模块地址的函数x32
DWORD GetModuleAddress(char* ClassName,char* ModuleName){
	HWND hwnd = FindWindowA(ClassName, NULL);
    if(hwnd!=NULL){
	    DWORD pid;
	    GetWindowThreadProcessId(hwnd, &pid);
	    HANDLE md_Handle;
	    md_Handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	    if (!md_Handle)
		    return FALSE;
	    MODULEENTRY32 md;
	    md.dwSize = sizeof(md);
	    BOOL ex = Module32First(md_Handle, &md);
	    while (ex) {
		    if (!strcmp(md.szModule,ModuleName))
			    return (DWORD)md.modBaseAddr;
		    ex = Module32Next(md_Handle, &md);
	    }
    }
    else
	    return 0;
}


//用于GetModuleAddress64的结构体
typedef struct{
    DWORD64 addr;
    char* ModuleName;
}ModuleInfo;


//用于GetModuleAddress64的回调函数
BOOL CALLBACK MyEnumerateLoadedModulesProc64(
    PTSTR ModuleName,
    DWORD64 ModuleBase,
    ULONG ModuleSize,
    PVOID UserContext){
        ModuleInfo*AimModule=(ModuleInfo*)UserContext;
        if(strstr(ModuleName,AimModule->ModuleName)){
            AimModule->addr=ModuleBase;
            return 0;
            }
        return TRUE;
}


//获取模块地址的函数x64
DWORD64 GetModuleAddress64(char* ClassName,char* ModuleName){
    HWND hwnd = FindWindowA(ClassName, NULL);
    DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    HMODULE HDLL=LoadLibraryA("dbghelp.dll");
    BOOL (*ListMoudle)(HANDLE,PENUMLOADED_MODULES_CALLBACKW64,PVOID);
    ListMoudle=(BOOL (*)(HANDLE, PENUMLOADED_MODULES_CALLBACKW64, PVOID))GetProcAddress(HDLL,"EnumerateLoadedModules64");
    ModuleInfo AimModule;
    AimModule.ModuleName=ModuleName;
    ListMoudle(hProcess,(PENUMLOADED_MODULES_CALLBACKW64)MyEnumerateLoadedModulesProc64,&AimModule);
    CloseHandle(hProcess);
    return AimModule.addr;
}


//16进制打印地址函数
void AddrPrint(DWORD64 Aimer){
    char ku[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    int zh[32],i=0,w,j;
    DWORD64 b;
    b=Aimer;
    w=16;
    do{
        zh[i]=Aimer%w;
        i++;
        Aimer/=w;
    }
    while(Aimer!=0);
    printf("0x");
    for(i--;i>=0;i--){
        j=zh[i];
        printf("%c",ku[j]);
    }
    printf("\n");
}


//跨进程申请内存空间函数
DWORD MallocEX(char* ClassName,int size){
    HWND hwnd = FindWindowA(ClassName, NULL);
    if(hwnd!=NULL){
        DWORD pid;
        GetWindowThreadProcessId(hwnd, &pid);
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        DWORD addr=(DWORD)VirtualAllocEx(hProcess,NULL,size,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	    CloseHandle(hProcess);
	    if(addr==0)
		    return FALSE;
	    else return addr;
    }
    else
        return FALSE;
}


//跨进程释放内存地址函数
BOOL FreeEX(char* ClassName,DWORD addr){
    HWND hwnd = FindWindowA(ClassName, NULL);
    if(hwnd!=NULL){
        DWORD pid;
        GetWindowThreadProcessId(hwnd, &pid);
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	    if(VirtualFreeEx(hProcess,(LPVOID)addr,0,MEM_RELEASE)==FALSE)
		    return FALSE;
	    else 
		    return TRUE;
    }
    else
        return FALSE;
}


//读取内存函数
BOOL ReadMemory(char* ClassName,DWORD m_addr,LPVOID Container,SIZE_T nSize){
    HWND hwnd = FindWindowA(ClassName, NULL);
    if(hwnd!=NULL){
	    DWORD pid;
	    GetWindowThreadProcessId(hwnd, &pid);
	    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	    ReadProcessMemory(hProcess,(LPVOID)m_addr,Container,nSize,NULL);
	    CloseHandle(hProcess);
        return TRUE;
    }
    else
        return FALSE;
}


//内存写入函数
BOOL WriteMemory(char* ClassName,DWORD m_addr,LPVOID Value,SIZE_T nSize){
	HWND hwnd = FindWindowA(ClassName, NULL);
    if(hwnd!=NULL){
	    DWORD pid;
	    GetWindowThreadProcessId(hwnd, &pid);
	    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	    BOOL Result;
	    Result=WriteProcessMemory(hProcess,(LPVOID)m_addr, Value, nSize, &pid);
	    CloseHandle(hProcess);
        return Result;
    }
    else
	    return FALSE;
}