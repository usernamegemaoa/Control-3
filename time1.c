#include <dos.h>
#include <stdio.h>



void interrupt(*oldint)();

void InitTimer(void)
{
    outportb(0x43,0x36);
}

void SetTimer(float fTime) // ms
{
    outportb(0x40,usTime & 0xff);
    outportb(0x40,usTime >> 8);
}

void InitInt(float fTime) // ms 
{
    disable();
    oldint=getvect(0x08);
    InitTimer();
    SetTimer(fTime);
    setvect(0x08,myint8);
    enable();
}

unsigned int gusDealNum =0;
void Deal()
{
    gusDealNum ++;
    putchar('8');
}

void   interrupt myint8(void)
{      
    Deal();
    outportb(0x20,0x20);
} 

void RestoreInt()
{
    disable();
    setvect(0x08,oldint);
    enable();

}

void    main(void)
{    
    InitInt(22.5);


    while(gusDealNum < 240);
	
    RestoreInt(); 
}











