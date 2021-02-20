#include "base.h"
#define ClassName "AliceRunWindowClass"
#define AddrCount 13
#define DefHP 1000
#define DefBaseExp 40000000
typedef struct{
    DWORD* addr;
    char* Name;
}AddrContain;

AddrContain Addrs[AddrCount];
DWORD HPaddr;
DWORD BaseEXPaddr;
DWORD SlayAddr;
DWORD ComboAddr;
DWORD EXPaddr;
DWORD PointAddr;
DWORD TurnAddr;
DWORD VolumeAddr;
DWORD DeathRateAddr;
DWORD LisasAddr;
DWORD HermanAddr;
DWORD SethAddr;
DWORD LibertyAddr;


void AddrCheck(void* Box){
    BOX box=(BOX)Box;
    HPaddr=GetModuleAddress(ClassName,"Rance10.exe")+0x003FCBB0;
    HPaddr=GetAddress(ClassName,HPaddr,5,0x8,0x438,0xc,0x10,0x10);
    Addrs[0].addr=&HPaddr;
    Addrs[0].Name="团队生命值";
    BaseEXPaddr=GetModuleAddress(ClassName,"Rance10.exe")+0x003FCBB0;
    BaseEXPaddr=GetAddress(ClassName,BaseEXPaddr,5,0x8,0x520,0x8,0x10,0xc);
    Addrs[1].addr=&BaseEXPaddr;
    Addrs[1].Name="战斗基础经验值";
    SlayAddr=GetModuleAddress(ClassName,"Rance10.exe")+0x003FCBB0;
    SlayAddr=GetAddress(ClassName,SlayAddr,5,0x8,0x478,0x4,0x10,0x20);
    Addrs[2].addr=&SlayAddr;
    Addrs[2].Name="必杀率";
    ComboAddr=GetModuleAddress(ClassName,"Rance10.exe")+0x003FCBB0;
    ComboAddr=GetAddress(ClassName,ComboAddr,5,0x8,0x478,0x4,0x10,0x24);
    Addrs[3].addr=&ComboAddr;
    Addrs[3].Name="连击率";
    EXPaddr=GetModuleAddress(ClassName,"Rance10.exe")+0x003FCBB0;
    EXPaddr=GetAddress(ClassName,EXPaddr,5,0x8,0x42c,0xc,0x10,0x1c);
    Addrs[4].addr=&EXPaddr;
    Addrs[4].Name="团队累积经验值";
    PointAddr=GetModuleAddress(ClassName,"Rance10.exe")+0x003FCBB8;
    PointAddr=GetAddress(ClassName,PointAddr,5,0x150,0x4d4,0x4,0x10,0x8);
    Addrs[5].addr=&PointAddr;
    Addrs[5].Name="部队加成点数";
    TurnAddr=GetModuleAddress(ClassName,"Rance10.exe")+0x003FCBB0;
    TurnAddr=GetAddress(ClassName,TurnAddr,5,0x8,0x420,0x0,0x10,0x8);
    Addrs[6].addr=&TurnAddr;
    Addrs[6].Name="回合数";
    VolumeAddr=GetModuleAddress(ClassName,"Rance10.exe")+0x003FCBB0;
    VolumeAddr=GetAddress(ClassName,VolumeAddr,5,0x8,0x42c,0xc,0x10,0x0);
    Addrs[7].addr=&VolumeAddr;
    Addrs[7].Name="食卷/餐卷数";
    DeathRateAddr=GetModuleAddress(ClassName,"Rance10.exe")+0x003FCBB4;
    DeathRateAddr=GetAddress(ClassName,DeathRateAddr,5,0x8,0x5b4,0xc,0x10,0x94);
    Addrs[8].addr=&DeathRateAddr;
    Addrs[8].Name="人类死亡率(单位0.1%)";
    LisasAddr=GetModuleAddress(ClassName,"Rance10.exe")+0x003FCBB4;
    LisasAddr=GetAddress(ClassName,LisasAddr,5,0x8,0x5b4,0xc,0x10,0x64);
    Addrs[9].addr=&LisasAddr;
    Addrs[9].Name="利萨斯兵力";
    HermanAddr=GetModuleAddress(ClassName,"Rance10.exe")+0x003FCBB4;
    HermanAddr=GetAddress(ClassName,HermanAddr,5,0x8,0x5b4,0xc,0x10,0x68);
    Addrs[10].addr=&HermanAddr;
    Addrs[10].Name="赫尔曼兵力";
    SethAddr=GetModuleAddress(ClassName,"Rance10.exe")+0x003FCBB4;
    SethAddr=GetAddress(ClassName,SethAddr,5,0x8,0x5b4,0xc,0x10,0x6c);
    Addrs[11].addr=&SethAddr;
    Addrs[11].Name="塞斯兵力";
    LibertyAddr=GetModuleAddress(ClassName,"Rance10.exe")+0x003FCBB4;
    LibertyAddr=GetAddress(ClassName,LibertyAddr,5,0x8,0x5b4,0xc,0x10,0x70);
    Addrs[12].addr=&LibertyAddr;
    Addrs[12].Name="自由都市兵力";
}

void HealthManager(void* Box){
    BOX box=(BOX)Box;
    int HP;
    ReadMemory(ClassName,HPaddr,&HP,sizeof(int));
    if(HP<(int)box->args){
        box->Message="检测到生命值低于预设下限值,自动进行修正\n";
        box->OutPut=TRUE;
        WriteMemory(ClassName,HPaddr,&box->args,sizeof(int));
    }
}

void AttackMode(void* Box){
    int DefValue=100;
    BOX box=(BOX)Box;
    int Combo,Slay;
    ReadMemory(ClassName,ComboAddr,&Combo,sizeof(int));
    ReadMemory(ClassName,ComboAddr,&Slay,sizeof(int));
    if(Slay<100){
        WriteMemory(ClassName,SlayAddr,&DefValue,sizeof(int));
        box->Message="必杀率已修正\n";
        box->OutPut=TRUE;
        WaitForSingleObject(box->TID,500);
    }
    if(Combo<100){
        WriteMemory(ClassName,ComboAddr,&DefValue,sizeof(int));
        box->Message="连击率已修正\n";
        box->OutPut=TRUE;
    }
}

void FastLevelUP(void* Box){
    BOX box=(BOX)Box;
    int BaseExp;
    ReadMemory(ClassName,BaseEXPaddr,&BaseExp,sizeof(int));
    if(BaseExp<(int)box->args){
        box->Message="基础经验值已修正\n";
        box->OutPut=TRUE;
        WriteMemory(ClassName,BaseEXPaddr,&box->args,sizeof(int));
    }
}

int main(int argc,char*argv[]){
    system("color 0a");
    system("title Rance10辅助控制台——编写由@A迷失的方向A");
    BOX Aim;
    HANDLE AddrCheck_Handle=NewThread(AddrCheck,NULL);
    HANDLE HP_Handle=NewThread(HealthManager,(void *)DefHP);
    HANDLE AttackMode_Handle=NewThread(AttackMode,NULL);
    HANDLE FastLevelUP_Handle=NewThread(FastLevelUP,(void *)DefBaseExp);
    Aim=GetBox(AttackMode_Handle);
    Aim->Pause=TRUE;
    Aim=GetBox(FastLevelUP_Handle);
    Aim->Pause=TRUE;
    BOOL End=FALSE;
    WaitForSingleObject(HP_Handle,500);
    Ttree.Message="生命值监测模块[HP]已上线\n";
    Ttree.OutPut=TRUE;
    WaitForSingleObject(AttackMode_Handle,500);
    Ttree.Message="Attack模块已上线,按默认设置待机\n";
    Ttree.OutPut=TRUE;
    WaitForSingleObject(FastLevelUP_Handle,500);
    Ttree.Message="LevelUP模块已上线,按默认设置待机\n";
    Ttree.OutPut=TRUE;
    WaitForSingleObject(HP_Handle,500);
    Ttree.Message="控制台已上线,输入./Help查看帮助\n";
    Ttree.OutPut=TRUE;
    while(!End){
        fflush(stdin);
        char input[10];
        char Cmd[3][10];
        char CmdCount=0;
        for(int count=0;count<10;count++){
            input[count]=toupper(getchar());
            if(input[count]==' '){
                input[count]='\n';
                sprintf(Cmd[CmdCount],"%s",input);
                for(int count=0;count<10;count++)
                    input[count]=0;
                count=-1;
                CmdCount++;
            }
            if(input[count]=='\n'){
                sprintf(Cmd[CmdCount],"%s",input);
                CmdCount++;
                break;
            }
        }
        switch(CmdCount){
            case 1:
                if(!strcmp("CLS\n",Cmd[0])){
                    system("cls");
                    printf("[User]:");
                    break;
                }
                if(!strcmp("HELP\n",Cmd[0])){
                    Ttree.Message="命令帮助页:\
                                                    \n        ./Cls 清除页面\
                                                    \n        ./Help 查看帮助页\
                                                    \n        ./List 查询所有值\
                                                    \n        ./Stop 停止运行\
                                                    \n        ./Switch <ModuleName> 切换模块运行状态\
                                                    \n              -可用的模块列表:\
                                                    \n                  ./HP 生命值检测模块\
                                                    \n                  ./Attack 攻击模式(自动补正连击率和必杀率)\
                                                    \n                  ./LevelUP 快速升级模块(自动修正基础经验值)\
                                                    \n        ./Set <ValueName> <Value> 设定一个对象的值\
                                                    \n              -可用的数值对象列表:\
                                                    \n                  ./BaseEXP 战斗基础经验值\
                                                    \n                  ./Combo 连击发生率\
                                                    \n                  ./dRate 人类死亡率(单位0.1%)\
                                                    \n                  ./DefHP 生命值监测的下限(默认为1000)\
                                                    \n                  ./EXP 团队累积经验值\
                                                    \n                  ./HP 团队生命值\
                                                    \n                  ./Herman 赫尔曼兵力值\
                                                    \n                  ./Lisas 利萨斯兵力值\
                                                    \n                  ./Seth 塞斯兵力值\
                                                    \n                  ./Liberty 自由都市兵力值\
                                                    \n                      -注:各兵力值单位为千\
                                                    \n                  ./Point 部队加成点数\
                                                    \n                  ./Slay 必杀发生率\
                                                    \n                  ./Turn 回合数\
                                                    \n                  ./Volume 食卷/餐卷数\
                                                    \n        ->于2020/08/16  由   @A迷失的方向A   编写                      \n";
                    Ttree.OutPut=TRUE;
                    break;
                }
                if(!strcmp("STOP\n",Cmd[0])){
                    End=TRUE;
                    break;
                }
                if(!strcmp("LIST\n",Cmd[0])){
                    printf("\r    -数值列表:\n");
                    for(int count=0;count<AddrCount;count++){
                        int Aim;
                        ReadMemory(ClassName,*(Addrs[count].addr),&Aim,sizeof(int));
                        printf("        -%s:%d\n",Addrs[count].Name,Aim);
                    }
                    Aim=GetBox(HP_Handle);
                    printf("        -生命检测模块的当前下限值:%d\n",(int)Aim->args);
                    printf("[User]:");
                    break;
                }
                Ttree.Message="未知的命令,输入Help查看帮助\n";
                Ttree.OutPut=TRUE;
                break;
            case 2:
                if(!strcmp("SWITCH\n",Cmd[0])){
                    if(!strcmp("HP\n",Cmd[1])){
                        Aim=GetBox(HP_Handle);
                        if(Aim->Pause){
                            Aim->Pause=FALSE;
                            Aim->Message="HP监测已恢复\n";
                        }
                        else{
                            Aim->Pause=TRUE;
                            Aim->Message="HP监测已关闭\n";
                        }
                        Aim->OutPut=TRUE;
                        break;
                    }
                    if(!strcmp("ATTACK\n",Cmd[1])){
                        Aim=GetBox(AttackMode_Handle);
                        if(Aim->Pause){
                            Aim->Pause=FALSE;
                            Aim->Message="Attack模式已开启\n";
                        }
                        else{
                            Aim->Pause=TRUE;
                            Aim->Message="Attack模式已关闭\n";
                        }
                        Aim->OutPut=TRUE;
                        break;
                    }
                    if(!strcmp("LEVELUP\n",Cmd[1])){
                        Aim=GetBox(FastLevelUP_Handle);
                        if(Aim->Pause){
                            Aim->Pause=FALSE;
                            Aim->Message="LevelUP模式已开启\n";
                        }
                        else{
                            Aim->Pause=TRUE;
                            Aim->Message="LevelUP模式已关闭\n";
                        }
                        Aim->OutPut=TRUE;
                        break;
                    }
                }
            Ttree.Message="未知的命令,输入Help查看帮助\n";
                Ttree.OutPut=TRUE;
                break;
            case 3:
                if(!strcmp("SET\n",Cmd[0])){
                    if(!strcmp("HP\n",Cmd[1])){
                        int HP;
                        sscanf(Cmd[2],"%d",&HP);
                        WriteMemory(ClassName,HPaddr,&HP,sizeof(int));
                        Ttree.Message="已修改目标值\n";
                        Ttree.OutPut=TRUE;
                        break;
                    }
                    if(!strcmp("BASEEXP\n",Cmd[1])){
                        int BaseExp;
                        sscanf(Cmd[2],"%d",&BaseExp);
                        WriteMemory(ClassName,BaseEXPaddr,&BaseExp,sizeof(int));
                        Ttree.Message="已修改目标值\n";
                        Ttree.OutPut=TRUE;
                        break;
                    }
                    if(!strcmp("SLAY\n",Cmd[1])){
                        int SlayPoint;
                        sscanf(Cmd[2],"%d",&SlayPoint);
                        WriteMemory(ClassName,SlayAddr,&SlayPoint,sizeof(int));
                        Ttree.Message="已修改目标值\n";
                        Ttree.OutPut=TRUE;
                        break;
                    }
                    if(!strcmp("DEFHP\n",Cmd[1])){
                        int HPset;
                        sscanf(Cmd[2],"%d",&HPset);
                        Aim=GetBox(HP_Handle);
                        Aim->args=(void*)HPset;
                        Aim->Message="已修改生命值下限\n";
                        Aim->OutPut=TRUE;
                        break;
                    }
                    if(!strcmp("COMBO\n",Cmd[1])){
                        int Combo;
                        sscanf(Cmd[2],"%d",&Combo);
                        WriteMemory(ClassName,ComboAddr,&Combo,sizeof(int));
                        Ttree.Message="已修改目标值\n";
                        Ttree.OutPut=TRUE;
                        break;
                    }
                    if(!strcmp("EXP\n",Cmd[1])){
                        int EXP;
                        sscanf(Cmd[2],"%d",&EXP);
                        WriteMemory(ClassName,EXPaddr,&EXP,sizeof(int));
                        Ttree.Message="已修改目标值\n";
                        Ttree.OutPut=TRUE;
                        break;
                    }
                    if(!strcmp("POINT\n",Cmd[1])){
                        int Point;
                        sscanf(Cmd[2],"%d",&Point);
                        WriteMemory(ClassName,PointAddr,&Point,sizeof(int));
                        Ttree.Message="已修改目标值\n";
                        Ttree.OutPut=TRUE;
                        break;
                    }
                    if(!strcmp("TURN\n",Cmd[1])){
                        int Turn;
                        sscanf(Cmd[2],"%d",&Turn);
                        WriteMemory(ClassName,TurnAddr,&Turn,sizeof(int));
                        Ttree.Message="已修改目标值\n";
                        Ttree.OutPut=TRUE;
                        break;
                    }
                    if(!strcmp("VOLUME\n",Cmd[1])){
                        int Volume;
                        sscanf(Cmd[2],"%d",&Volume);
                        WriteMemory(ClassName,VolumeAddr,&Volume,sizeof(int));
                        Ttree.Message="已修改目标值\n";
                        Ttree.OutPut=TRUE;
                        break;
                    }
                    if(!strcmp("DRATE\n",Cmd[1])){
                        int DeathRate;
                        sscanf(Cmd[2],"%d",&DeathRate);
                        WriteMemory(ClassName,DeathRateAddr,&DeathRate,sizeof(int));
                        Ttree.Message="已修改目标值\n";
                        Ttree.OutPut=TRUE;
                        break;
                    }
                    if(!strcmp("LISAS\n",Cmd[1])){
                        int Lisas;
                        sscanf(Cmd[2],"%d",&Lisas);
                        WriteMemory(ClassName,LisasAddr,&Lisas,sizeof(int));
                        Ttree.Message="已修改目标值\n";
                        Ttree.OutPut=TRUE;
                        break;
                    }
                    if(!strcmp("HERMAN\n",Cmd[1])){
                        int Herman;
                        sscanf(Cmd[2],"%d",&Herman);
                        WriteMemory(ClassName,HermanAddr,&Herman,sizeof(int));
                        Ttree.Message="已修改目标值\n";
                        Ttree.OutPut=TRUE;
                        break;
                    }
                    if(!strcmp("SETH\n",Cmd[1])){
                        int Seth;
                        sscanf(Cmd[2],"%d",&Seth);
                        WriteMemory(ClassName,SethAddr,&Seth,sizeof(int));
                        Ttree.Message="已修改目标值\n";
                        Ttree.OutPut=TRUE;
                        break;
                    }
                    if(!strcmp("LIBERTY\n",Cmd[1])){
                        int Liberty;
                        sscanf(Cmd[2],"%d",&Liberty);
                        WriteMemory(ClassName,LibertyAddr,&Liberty,sizeof(int));
                        Ttree.Message="已修改目标值\n";
                        Ttree.OutPut=TRUE;
                        break;
                    }
                }
            default :
                Ttree.Message="未知的命令,输入Help查看帮助\n";
                Ttree.OutPut=TRUE;
        }
        for(int count=0;count<10;count++)
            input[count]=0;
    }
    Ttree.Message="主线程下线\n";
    Ttree.OutPut=TRUE;
    WaitForSingleObject(HP_Handle,500);
    system("pause");
    return 0;
}
