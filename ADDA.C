#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <graphics.h>
#include <math.h>

double TimeInterval;/*中断间隔时间*/
int CurrentInterruptNum;/*中断事件计数*/
int CurrentCount;/*一次中断事件计数*/
int InterruptNum;/*指定中断事件次数*/
unsigned char L8,H8;
int Count;/*一次中断事件理论计算应完整中断次数*/
int EndStroke;/*点击字母e，中断停止*/
FILE *fp;
/*画图程序*/
/*获取屏幕像素坐标*/
double DXmin,DXmax,DYmin,DYmax;/*Region of screen pixel*/
int IWXmin,IWXmax,IWYmin,IWYmax;/*Region of real value*/
double DXstep,DYstep;/*step of X and Y*/
int IGrid;/*1:draw grid;0:don't draw grid*/
int LastWDotX,LastWDotY;/*记录上一点坐标*/
double DRefLine=0;
int GetScreenX(double realx,int WXmin,int WXmax,double Xmin,double Xmax)
{
    int WX=(WXmax-WXmin)/(Xmax-Xmin)*(realx-Xmin)+WXmin;
    return WX;
}
int GetScreenY(double realy,int WYmin,int WYmax,double Ymin,double Ymax)
{
    int WY=(WYmax-WYmin)/(Ymax-Ymin)*(Ymax-realy)+WYmin;
    return WY;
}
void DrawCoordinate(double Xmin,double Xmax,double Ymin,double Ymax,int WXmin,int WXmax,int WYmin,int WYmax,double Xstep,double Ystep,int Grid,double RefLine)
{
    int WYRefLine;
    char labelmark[10];
    double Dot;
    int WDot;
    int i;
    /*Draw Frame*/
    /*rectangle(WXmin,WYmin,WXmax,WYmax);*/
    line(WXmin,WYmin,WXmax,WYmin); /*top*/
	line(WXmin,WYmin,WXmin,WYmax); /*left*/
	line(WXmin,WYmax,WXmax,WYmax); /*Bottom*/
	line(WXmax,WYmin,WXmax,WYmax); /*right*/
    /*Draw Reference Line*/
    WYRefLine=GetScreenY(RefLine,WYmin,WYmax,Ymin,Ymax);
    line(WXmin,WYRefLine,WXmax,WYRefLine);
    gcvt(RefLine,3,labelmark);
    outtextxy(WXmax+1,WYRefLine-2,labelmark);
    /*Draw X Label*/
	i=0;
    while(1)
    {
        Dot=Xmin+i*Xstep;
        WDot=GetScreenX(Dot,WXmin,WXmax,Xmin,Xmax);
        if(WDot>WXmax)
        {
            break;
        }
        if(Grid==1)
        {
            line(WDot,WYmin,WDot,WYmax);
        }
        else
        {
            line(WDot,WYmax,WDot,WYmax-5);
        }
        gcvt(Dot,3,labelmark);
        outtextxy(WDot-5,WYmax+8,labelmark);
        i++;
    }
    /*Draw Y Label*/
    i=0;
    while(1)
    {
		Dot=Ymin+i*Ystep;
		WDot=GetScreenY(Dot,WYmin,WYmax,Ymin,Ymax);
		if(WDot<WYmin)
		{
			break;
		}
		if(Grid==1)
		{
			line(WXmin,WDot,WXmax,WDot);
		}
		else
		{
			line(WXmin,WDot,WXmin+5,WDot);
		}
		gcvt(Dot,3,labelmark);
		if(Dot<0)
		{
			outtextxy(WXmin-30,WDot-2,labelmark);
		}
		else
		{
			outtextxy(WXmin-20,WDot-2,labelmark);
		}
		i++;
	}
}
void ReadSetupFile(void)
{
	FILE *fp1;
	fp1=fopen("Setup.txt","r");
	fscanf(fp1,"%d,%d,%d,%d\n",&IWXmin,&IWXmax,&IWYmin,&IWYmax);
	fscanf(fp1,"%le,%le,%le,%le\n",&DXmin,&DXmax,&DYmin,&DYmax);
	fscanf(fp1,"%le,%le\n",&DXstep,&DYstep);
	fscanf(fp1,"%le\n",&DRefLine);
	fscanf(fp1,"%d\n",&IGrid);
	fclose(fp1);
}
/*中断服务程序*/
void  Process(void)
{
    int Index,IData;
    double DData;
    double RealTime;
    int WDotX,WDotY;
    double Xregion;
    Index=CurrentInterruptNum;
    RealTime=CurrentInterruptNum*TimeInterval/1000;/*/s*/
    /*读取IData,并转换成DData*/
    /*IData=Index%10;
    DData=5*sin(RealTime);
    */
    /*AD转换*/
    outportb(0x280,0x00);
    while(!inportb(0x285)&0x80)
    {;}
    IData=inportb(0x281)|((inportb(0x282)&0x0f)<<8);
    DData=5-(4095-IData)*10./4096;
    fprintf(fp,"%d\t%d\t%le\t%le\n",Index,IData,RealTime,DData);
    /*DA转换*/
    if(IData<4095)
    {
        outportb(0x283,((IData+1)&0x0ff));
        outportb(0x284,(((IData+1)>>8)&0x0f));
    }
    else
    {
        outportb(0x283,(IData&0x0ff));
        outportb(0x284,((IData>>8)&0x0f));
    }
    

    if(RealTime>DXmax)
    {
        Xregion=DXmax-DXmin;
        DXmin=DXmax;
        DXmax=DXmin+Xregion;
        cleardevice();
        DrawCoordinate(DXmin,DXmax,DYmin,DYmax,IWXmin,IWXmax,IWYmin,IWYmax,DXstep,DYstep,IGrid,DRefLine);
        LastWDotX=IWXmin-1;
    }
    if(RealTime<=DXmax)
    {
        WDotX=GetScreenX(RealTime,IWXmin,IWXmax,DXmin,DXmax);
        WDotY=GetScreenY(DData,IWYmin,IWYmax,DYmin,DYmax);
        if(LastWDotX<IWXmin)
        {
            line(WDotX,WDotY,WDotX,WDotY);
        }
        else
        {
            line(WDotX,WDotY,LastWDotX,LastWDotY);
        }
        LastWDotX=WDotX;LastWDotY=WDotY;
    }
}
void interrupt (*oldint8)(void);
void  interrupt myint8(void)
{
	if(kbhit())
	{
		EndStroke=bioskey(0);
	}
	/*对于多个周期，程序执行*/
	if(Count==0)
	{
		Process();
		CurrentInterruptNum++;
	}
	else if(Count>0)
    {
        CurrentCount++;
        if(CurrentCount<Count)
        {
            ;
        }
        else if(CurrentCount==Count)
        {
            outportb(0x40,L8);
            outportb(0x40,H8);
        }
        else
        {
            outportb(0x40,0x00);
            outportb(0x40,0x00);
            Process();
            CurrentCount=0;
            CurrentInterruptNum++;
        }
    }
    outportb(0x20,0x20);
}
/*设置中断*/
void SetupInterrupt(void)
{
    disable();
    oldint8=getvect(0x08);/*8号中断*/
    outportb(0x43,0x36);/*通道0，先读写第八位 ，后读写高八位，方式三，二进制*/
    if(Count==0)
    {
        outportb(0x40,L8);
        outportb(0x40,H8);
    }
    else
    {
        outportb(0x40,0x00);
        outportb(0x40,0x00);
    }
    setvect(0x08,myint8);
    enable();
}
/*恢复定时中断*/
void RestoreInterrupt(void)
{
    disable();
    outportb(0x40,0x00);
    outportb(0x40,0x00);
    setvect(0x08,oldint8);
    enable();
}
/*计算中断时间对应的中断次数ILongCount,和最后一次的中断计数CL8，CH8*/
void Calpara(double TimeInterval,int *ICount,unsigned char *CL8,unsigned char *CH8)
{
    double TotalCounter=TimeInterval*1193.182;
    int rest;
    *ICount=TotalCounter/65536;
    rest=TotalCounter-65536*(*ICount);
    *CH8=rest>>8;
    *CL8=rest&0x0ff;
}
void main(void)
{
    /*输入参数*/
    char *DataFileName;
    int Driver=DETECT,Mode;
    DataFileName=(char *)malloc(sizeof(char)*100);
    printf("please input the interval time(/ms):\n");
    scanf("%le",&TimeInterval);
    printf("please input the times of interrupt:\n");
    scanf("%d",&InterruptNum);
    printf("please input the data file name:\n");
    scanf("%s",DataFileName);
    /*计算一次事件中断参数*/
    Calpara(TimeInterval,&Count,&L8,&H8);
    /*初始化中断参数*/
    CurrentInterruptNum=0;
    CurrentCount=0;
	/*初始化图像*/
    initgraph(&Driver,&Mode,"");
    
    ReadSetupFile();
    DrawCoordinate(DXmin,DXmax,DYmin,DYmax,IWXmin,IWXmax,IWYmin,IWYmax,DXstep,DYstep,IGrid,DRefLine);
    LastWDotX=IWXmin-1;
    fp=fopen(DataFileName,"w");
    free(DataFileName);
    if(fp==NULL)
    {
        printf("can't open the file %s",DataFileName);
        return;
    }
    SetupInterrupt();
    while(CurrentInterruptNum<InterruptNum)
    {
        if(EndStroke==4709)
        {
            break;
        }
    }
    RestoreInterrupt();
    getch();
    fclose(fp);
    closegraph();
}
