#include<dos.h>
#include<stdio.h>
#include<math.h>
#include<graphics.h>

union
{
    struct
    {
        char l8;
        char h8;
    }w;
    int i;
}b;

void initial_ch0(void)
{
    b.i=14319;
    outportb(0x43,0x36);
    outportb(0x40,b.w.l8);
    outportb(0x40,b.w.h8);
}

union
{   struct
    {
        unsigned char low8 ;
        unsigned char high8;
    }adlh;
    struct
    {
        unsigned    low12: 12;
        unsigned    high4:  4;
    }ad12;
}ad;

float value;
/*int *oldint;*/
void interrupt (*oldint)();
int data[260],data_i;

void    interrupt myint8(void);
int     k=0;
int driver=0,mode,x1,x2;

void    display(void)
{
    int i;
    float v;

    for (i=0;i<=data_i;i++)
    {
        v=0.00244*data[i];
        printf("i = %d  data = %d  V = %.3f\n",i,data[i],v);
    }
}

void    main(void)
{
    initgraph(&driver,&mode,"c://tc//bgi");
    rectangle(70,125,570,375);
    line(70,250,570,250);
    x1=70;
    setcolor(2);
    disable();
    initial_ch0();
    oldint=getvect(0x08);
    setvect(0x08,myint8);
    enable();

    while(!kbhit())
    {
        if(k==255)
        {
            k=0;
            x1=70;
            cleardevice;
            setcolor(63);
            rectangle(70,125,570,375);
            line(70,250,570,250);
            k=0;
            x1=70;
            setcolor(2);
        }
        if(kbhit())
        break;
    }

    data_i=0;
    disable();
    setvect(0x08,*oldint);
    enable();

}

void    interrupt myint8(void)
{
    int dbyte;

    outportb(0x280,0x00);
    do
         ;
    while (!(inportb(0x285) & 0x80));

    ad.adlh.low8=inportb(0x281);
    ad.adlh.high8=inportb(0x282);
    dbyte=ad.ad12.low12;
    data[0]=0;
    data_i++;
    k++;
    data[data_i]=dbyte-2047;

    line(data_i-1,0.0024*data[data_i-1],data_i,0.0024*data[data_i]);

    outportb(0x20,0x20);
}
