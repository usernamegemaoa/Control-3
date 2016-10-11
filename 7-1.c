#include<dos.h>
#include<stdio.h>
void interrupt myint8(void);
void interrupt (*oldint)(void);
long int time,time_i,time_half;
double time_temp;
int step=0,Ma=0;
int shun,ni;
void set_time(int i)
{
    char time_l,time_h;
    if(i>=50)
    {
        outportb(0x40,0x02);
        outportb(0x40,0xe9);
    }
    else
    {
        time_l=(i*1193)&0xff;
        time_h=(i*1193)>>8;
        outportb(0x40,time_l);
        outportb(0x40,time_h);
    }
}
void func()
{
    if(step<=shun*3200)
    {
        if(Ma==0)
        {
            outportb(0x378,0x01);
            Ma=1;
        }
        else
        {
            outportb(0x378,0x00);
            Ma=0;
        }
    }
    else
    {
        if(Ma==0)
        {
            outportb(0x378,0x03);
            Ma=1;
        }
        else
        {
            outportb(0x378,0x02);
            Ma=0;
        }
    }
}
void CLK_Init(int CLK_func)
{
    if(CLK_func==0)
    {
        disable();
        outportb(0x43,0x36);
        set_time(time_i);
        oldint=getvect(0x08);
        setvect(0x08,myint8);
        enable();
    }
    else
    {
        disable();
        setvect(0x08,*oldint);
        enable();
    }
}

void    main()
{
    char chr;
    printf("Please input a time (ms): ");
    scanf("%lf",&time_temp);
    printf("Please input a number of shun: ");
    scanf("%d",&shun);
    printf("Please input a number of ni: ");
    scanf("%d",&ni);
    time=time_temp*1193;
    time_half=time/2/1193;
    time_i=time_half;
    CLK_Init(0);
    while(1)
    {
        fflush(stdin);
        chr=getchar();
        switch(chr)
        {
        case 's':
        case 'S':
        	{
                time=time+1193*2;
                printf("-1\n");
                break;
        	}
        case 'u':
        case 'U':
        	{
                if(time-1193*2>0)
                    time=time-1193*2;
                printf("+1\n");
                break;
        	}
        }
    }
}
void    interrupt myint8(void)
{
    if(time_i>=50)
    {
        time_i=time_i-50;
        if(time_i<=50)
        {
            disable();
            set_time(time_i);
            enable;
        }
    }
    else
    {
        time_half=time/1193-time_half;
        time_i=time_half;
        func();
        step++;
        disable();
        set_time(time_i);
        enable();
        if(step>=(shun+ni)*3200)
            CLK_Init(1);
    }
    outportb(0x20,0x20);
}
