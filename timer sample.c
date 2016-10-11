#include "stdlib.h"
#include "stdio.h"
#include "dos.h"

 
void interrupt (*oldint)();

unsigned int gusDealNum = 0;
unsigned int gusIntNum = 0;
unsigned int gusIntInitNum = 0;

void InitTimer()
{
    outportb(0x43,0x36);	
}


// 缺陷是定时时间要少于55ms， 大于1ms
void SetTimer(unsinged int usTime)
{
    //unsigned int usTime = (unsigned int)(fTime * 1193.18);	
    outportb(0x40,usTime & 0xff);
    outportb(0x40,usTime>>8);
}


void Deal()
{
    putchar('8');
    gusDealNum ++;  	
}


void	interrupt  myint8(void)
{    
   gusIntNum --;
   
   if (gusIntNum == 0)	
   {
      Deal();
      gusIntNum = gusIntInitNum;
   }
       
   outportb(0x20,0x20);      	
}

void InitInt()
{
    disable();
    oldint=getvect(0x08);
    InitTimer(22.7);
    SetTimer();
    setvect(0x08,myint8);
    enable();
}

void  RestoreInt()
{
 
}

unsigned long int  InputTime(void)
{ 
    float fTime;
    while(1)
    {
    	
        printf("please input time(ms):\n");     
        scanf("%f", &fTime); 
        if ((fTime > 10) && (fTime < 1000))
        {
           break;        	
        }
        printf("input number error!\n");          
    }  
    
     return (unsigned long int)(fTime );
}  

void AlgInitInt(unsigned long int ulTime)  // 算法修改为折半查找或者其他算法
{
    int i;
    
    for (i =1; i < 21;i++)
    {
    	if ((ulTime / i) <= 65530)
    	{
    	    gusIntInitNum = i;
    	    break; 	
    	}
    }	
}



main()
{


}

