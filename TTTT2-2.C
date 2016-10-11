
#include <dos.h>
#include <stdio.h>
void    interrupt  myint8(void);
void interrupt(*oldint)();
void display();
void    main(void)
{
	int n,answer;
	int data0,data1,data;
   oldint=getvect(0x08);
   disable();
    outportb(0x43,0x36);
    outportb(0x40,0xEF);
    outportb(0x40,0x37);
    setvect(0x08,myint8);
    enable();
    while(1){
   /* setvect(0x08,myint8);*/
    }
    enable();
/*	printf("you want 1 or 2 or 3 or 4?");
	scanf("%d",&answer);
    if(answer==1){
		while(1){
		display(getAd());
		}
    }
    if(answer==2){
		while(1){
		data=getAd();
		if(data-2047<0){
			data=-data;
			}
		display(data);
		}
    }
    if(answer==3){
		while(1){
		data=getAd();
		if(data-2047>0){
			data=-data;
			}
		display(data);
		}
    }
    if(answer==4){
		data0=2048;
		while(1){
		data1=getAd();
		data=(data1-data0)*400+2048;
		display(data);
		data0=data1;
		}
    }
    */
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

void display(int da)
{
	outportb(0x283,(da&0x00ff));
	outportb(0x284,((da>>8)&0x0f));
}
int data0;

void    interrupt myint8(void)
{
	int data1,data;
	/*data0=2048;*/
	data1=getAd();
	data=(data1-data0)*10+2048;
	display(data);
	data0=data1;
    outportb(0x20,0x20);
}

