#include <graphics.h>
#include <math.h>
#include <dos.h>
#include <stdio.h>

int data[256];



void interrupt(*oldint)();
unsigned int gusDealNum =0;
unsigned int gusIntNum;
unsigned int gusIntNumInit;
float  gfTime ;

void  DaOut(int da)
{
    outportb(0x283,(da&0x00ff));
    outportb(0x284,((da>>8)&0x0f));

}


int GetAdValue()
{
	int	dbyte ;

	outportb(0x280,0x00);
	do
	{
	     ;
	}while(!(inportb(0x285)&0x80));
	dbyte=inportb(0x281)|((inportb(0x282)&0x0f)<<8);

	return dbyte;
}

void Func1()
{
    int AdData = GetAdValue();
	
    DaOut(AdData+ 1);
}


void Func2()
{
    int AdData = GetAdValue();

    if (AdData < 2047) AdData = 2047;
    DaOut(AdData+ 1);
}


void Func2()
{
    int AdData = GetAdValue();

    if (AdData >2047) AdData = 2047;
    DaOut(AdData+ 1);
}


int LastAd = 0;
 
 void Func3(float scale)
{
    int AdData = GetAdValue();
    int outData;

    outData = scale * (AdData - LastAd);
    outData += 2048;
    DaOut(outData);

    LastAd = outData;
}


int lastAd1 = 0;
 void Func4(float scale)
{
    int AdData = GetAdValue();
    int outData;

    outData = scale * (AdData + lastAd1 - 2* LastAd);
    outData += 2048;
    DaOut(outData);

    lastAd1 = LastAd;
    LastAd = outData;
    
}
 


void InitTimer(void)
{
    outportb(0x43,0x36);
}

void SetTimer(float fTime)
{
    unsigned int usTime = (unsigned int)(fTime * 1193.18);

    outportb(0x40,usTime & 0xff);
    outportb(0x40,usTime >> 8);
}
void interrupt myint8();

void InitInt(float fTime)
{
    disable();
    oldint=getvect(0x08);
    InitTimer();
    SetTimer(fTime);
    setvect(0x08,myint8);
    enable();
}


int  iLastAd1 = 0;

void Deal()
{
    gusDealNum = (gusDealNum +  1) %256 ;
   /* data[gusDealNum ]= ( GetAdValue()-2047) /16 ;*/

}

void   interrupt myint8(void)
{
    gusIntNum --;


     if (gusIntNum ==0 )
     {

         Deal();

         gusIntNum = gusIntNumInit;
     }
    outportb(0x20,0x20);
}

void RestoreInt()
{
    disable();
    setvect(0x08,oldint);
    enable();

}


float InputTime(void)
{
    float fTime;
    while(1)
    {

        printf("please input time(10-1000):\n");
        scanf("%f", &fTime);
        if ((fTime >= 10) && (fTime <= 1000))
        {
           break;
        }
        printf("input number error!\n");
    }
    return fTime;
}


void AlgIntNumInit(float fTime)
{

   int i;

   for(i = 1; i < 21;i++)
   {
       if (fTime/i < 55)
       	{
             gusIntNumInit = i;
             return ;
       	}

   }

}







void InitDevice()
{
  int driver=DETECT,mode;
  initgraph(&driver,&mode,"BGI");
}


void InitData()
{

  int  k;

  for(k=0;k<255;k++)
  data[k]=60*sin(2.0*3.1415926/64.0*k);

}

void InitGraph()
{
  int  e,x00,x11,y00,y11,m;
  int  x1,x2,xstep,ystep,k;
  char stt[10];
  double value;

  cleardevice(); 
  setcolor(15);

  x00=100,y00=60,x11=x00+512,y11=y00+128;
  rectangle(x00,y00,x11,y11);
  m=(y00+y11)/2;

  line(x00,m,x11,m);
  xstep=(x11-x00)/16;
  ystep=(y11-y00)/8;
  
  for(k=0;k<=16;k++)
  {
     line(x00+xstep*k,y11,x00+xstep*k,y11+5);
     value=xstep*k/2;
     gcvt(value,3,stt);
     outtextxy(x00+xstep*k-8,y11+8,stt);
  }
 
   for(k=0;k<=8;k++)
   {
      line(x00-5,y00+ystep*k,x00,y00+ystep*k);
   }

}


void Disp(int startSeg,int endSeg)
{
  int   m;
  int  x1,x2, k;

 
   
   m = 124;
   
   setcolor(2);
   
   if (startSeg < endSeg)
   {
      x1=100 + 2* startSeg;
      for(k=startSeg;k<endSeg;k++)
      {
         x2=x1+2;
         line(x1,m-data[k],x2,m-data[k+1]);
         x1=x2;
      }
   }
   else   
   {
      x1=100 + 2* startSeg;   	
      for(k=startSeg;k<255;k++)
      {
         x2=x1+2;
         line(x1,m-data[k],x2,m-data[k+1]);
         x1=x2;
      }
      
      x1= 100;   	
      for(k=0 ;k<endSeg;k++)
      {
         x2=x1+2;
         line(x1,m-data[k],x2,m-data[k+1]);
         x1=x2;
      }     
   }
      
   startSeg = endSeg;
   endSeg = (endSeg + 30)%256;
   
   if (startSeg < endSeg)
   {
      x1=100 + 2* startSeg;
      for(k=startSeg;k<endSeg;k++)
      {
         x2=x1+2;
         setcolor(0);
         line(x1,m-data[k],x2,m-data[k+1]);
         setcolor(63);
         line(x1,m,x2,m); 
         x1=x2;
      }
   }
   else   
   {
      x1=100 + 2* startSeg;   	
      for(k=startSeg;k<255;k++)
      {
         x2=x1+2;
         setcolor(0);
         line(x1,m-data[k],x2,m-data[k+1]);
         setcolor(63);
         line(x1,m,x2,m); 
         x1=x2;
      }
      
      x1= 100;   	
      for(k=0 ;k<endSeg;k++)
      {
         x2=x1+2;
         setcolor(0);
         line(x1,m-data[k],x2,m-data[k+1]);
         setcolor(63);
         line(x1,m,x2,m); 
         x1=x2;
      }     
   }    
    
}




main()
{
   int usDealNum = 0;
  
    gfTime = InputTime();
    AlgIntNumInit(gfTime);   

    InitDevice();
    InitData();	

    InitGraph();	
	

    gusIntNum = gusIntNumInit;

    InitInt(gfTime /gusIntNumInit);
	
    while(1)
    {
       if( gusDealNum != usDealNum)
       	{

             Disp(usDealNum,gusDealNum);
     	     usDealNum = gusDealNum;
       	}
   }
	
   RestoreInt();
   getch();
   closegraph();
}

