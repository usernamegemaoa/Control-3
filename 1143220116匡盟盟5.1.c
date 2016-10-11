#include<dos.h>
#include <graphics.h>
#include<stdio.h>
union
{   struct
    {
        unsigned char low8 ;
        unsigned char high8;
    }signal;
    struct
    {
        unsigned    low12: 12;
        unsigned    high4:  4;
    }ad16;
}ad;
float value;
void interrupt (*oldint)();
int x0=0,x1=0,y0=0,y1=0,num=1,flag=0;
void    interrupt myint8();
void    dp_boot()
{
    int i;
    double temp;
    char str[50];
    cleardevice();
    setbkcolor(5);
    setcolor(2);
    rectangle(70,125,570,375);
    line(70,250,570,250);
    num--;
    for(i=0;i<=5;i++)
    {
        line(70+i*100,365,70+i*100,375);
        temp=(double)(num*50);
        gcvt(temp,5,str);
        outtextxy(60+i*100,385,str);
        num++;
    }
    for(i=0;i<=10;i++)
    {
        line(60,125+i*25,70,125+i*25);
    }
}
void  main()
{
    int driver=DETECT,mode;
    initgraph(&driver,&mode," " );
    dp_boot();
    disable();
    outportb(0x43,0x36);
    outportb(0x40,0xEC);
    outportb(0x40,0x37);
    oldint=getvect(0x08);
    setvect(0x08,myint8);
    enable();
    while(1)
    {
        if(flag==1)
        {
            dp_boot();
            flag=0;
        }
    }
    disable();
    setvect(0x08,*oldint);
    enable();
    closegraph( );
}
void    interrupt myint8()
{
    int dbyte;
    outportb(0x280,0x00);
    do
         ;
    while (!(inportb(0x285) & 0x80));
    ad.signal.low8=inportb(0x281);
    ad.signal.high8=inportb(0x282);
    dbyte=ad.ad16.low12;
    y1=(int)(-1)*(((double)(dbyte-2047)*10/4096)*25)+250;
    if(x0 != 0)
    {
        x1=x0+2;
        setcolor(62);
        line(x0,y0,x1,y1);
        setcolor(63);
        if(x1>=570)
        {
            x0=70;
            flag=1;
        }
        else x0=x1;
        y0=y1;
    }
    else
    {
        x0=70; y0=y1;
    }
    outportb(0x20,0x20);
}
