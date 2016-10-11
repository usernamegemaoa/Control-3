#include <stdio.h>
#include <dos.h>
#include <conio.h>

#define total_data 256

void twelve_ms_config(int c0);
int Code_Converter(int mode, int AD);
void recover(void);
void interrupt time_counter(void);


int flag =0;                 /*flag=1表示执行完了12ms;*/
void interrupt(*original)();

void main(void)
{
    int AD_Code[total_data];
    int Volt[total_data];
    int DA_Code[total_data];
    int ad;
    int lastad;

    int data_amount = 0;
    unsigned c0=14320;           /* 12ms/838ns=14320*/
    
    original = getvect(0x08);    /*用于保护原来的数据*/
    twelve_ms_config(c0);
    do
    {
        if(flag == 1)
        {
            /*disable();
            outportb(0x280,0x00); 选AD通道0，并启动AD转换*/
            do
                ;
            while(!(inportb(0x285) & 0x80));  /*查询AD转换是否完成*/
    
             ad = inportb(0x281)|((inportb(0x282)&0x0f)<<8);/*AD输入*/
             if (ad <2047) ad = 2047;

              DA_Code[data_amount] = Code_Converter( 2, AD_Code[data_amount]);
              outportb(0x283,(ad&0x00ff));    /*将整型变量DA_Code[data_amount]中的低12位从D/A输出*/
              outportb(0x284,((ad>>8)&0x0f));

              data_amount++;
              flag = 0;
              enable();
        }

    }while(1);
    data_amount = 0;

    while(data_amount <total_data)
    {
        Volt[data_amount] = Code_Converter( 1, AD_Code[data_amount]);
        data_amount++;
    }
    data_amount = 0;

    while(data_amount < total_data)
    {
        printf("%d    ", AD_Code[data_amount]);
        printf("%d    ", Volt[data_amount]);
        data_amount++;
    }
    recover();
    getch();
}




void twelve_ms_config(int c0)
{
    disable();
    c0 = 1193*20;
    outportb(0x43, 0x36);      /*0x43为8254控制字寄存器地址;
                               0x36说明：选择通道0，（通道0用于电子钟定时基准，
                               OUT0接到8259A的IR0(日时钟)作为中断源，中断类型号08H.）
                               读写方式选择'先读/写低8位，再读/写高8位';
                               工作方式选择方式3：方波发生器（分频器方式）,数制选择二进制计数*/
    outportb(0x40, c0&0x00ff);   /*0x40为'通道0'地址*/
    outportb(0x40, c0>>8);
    setvect(0x08, time_counter);

    enable();
}



int giLastAd = 0;
int giLastLastAd = 0;

int giLastDelta = 0;
/*
#define DELTA
*/
void interrupt time_counter(void)
{
    int ad    ;
    int ad1;

    outportb(0x280,0x00); /*选AD通道0，并启动AD转换*/
    do
                ;
    while(!(inportb(0x285) & 0x80));  /*查询AD转换是否完成*/
    
    ad1 = inportb(0x281)|((inportb(0x282)&0x0f)<<8);/*AD输入*/
        
    ad = ad1 + giLastLastAd - 2*giLastAd;

    ad =  80*  ad;
#ifdef DELTA
    ad = 0.9*giLastDelta  + 0.1*ad;
    giLastDelta  = ad;
#endif
    ad = ad + 2047;
    if (ad > 4095) ad = 4095;
    if (ad < 0 ) ad = 0;
   outportb(0x283,(ad&0x00ff));    /*将整型变量DA_Code[data_amount]中的低12位从D/A输出*/
   outportb(0x284,((ad>>8)&0x0f));

   giLastLastAd = giLastAd;
   giLastAd = ad1;
    outportb(0x20, 0x20);         /*中断源在主片，EOI命令*/
}


/*conv_mode =1：AD转到Volt
  conv_mode =2：AD转到DA  */
int Code_Converter(int conv_mode, int AD)
{
    int data;
    switch(conv_mode)
    {
        case 1:
            data = (AD-2047)*10/4096;
            return data;
        case 2:
            data = AD+1;           /*转换得对吗？*/
            return data;
    }

}



void recover(void)
{
    disable();
    setvect(0x08, original);
    outportb(0x43, 0x36);
    outportb(0x40, 0x00);
    outportb(0x40, 0x00);
    enable();
}


