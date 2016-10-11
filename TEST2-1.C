
#include <dos.h>
#include <stdio.h>
void    interrupt  myint8(void);
void interrupt(*oldint)();
int n=0;
void display();
void    main(void)
{
   oldint=getvect(0x08);
   disable();
    outportb(0x43,0x36);
    outportb(0x40,0xEF);
    outportb(0x40,0x37);
    setvect(0x08,myint8);
    enable();
    while(n<256);
    setvect(0x08,oldint);

    display();

}

int getAd()
{

 int    dbyte ;
   
    outportb(0x280,0x00);
    do
         ;
    while(!(inportb(0x285)&0x80));
    dbyte=inportb(0x281)|((inportb(0x282)&0x0f)<<8);

    return dbyte;
}

int data[256];

void display()
{
    float v ;
    int i;

    for(i=0;i<256;i++)
    {
       printf(" %d    %d  ",i,data[i]);
       v=data[i] ;
       v=v *5/2048;

       printf( "   %f  \n",v);

    }

}

void    interrupt myint8(void)
{
    data[n]=  getAd()-2047;
    n ++;

    outportb(0x20,0x20);
} 

