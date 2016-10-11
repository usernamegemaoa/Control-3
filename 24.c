#include <stdio.h>
#include <dos.h>
#include <conio.h>

#define total_data 256

void twelve_ms_config(int c0);
int Code_Converter(int mode, int AD);
void recover(void);
void interrupt time_counter(void);


int flag =0;                 /*flag=1��ʾִ������12ms;*/
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
    
    original = getvect(0x08);    /*���ڱ���ԭ��������*/
    twelve_ms_config(c0);
    do
    {
        if(flag == 1)
        {
            /*disable();
            outportb(0x280,0x00); ѡADͨ��0��������ADת��*/
            do
                ;
            while(!(inportb(0x285) & 0x80));  /*��ѯADת���Ƿ����*/
    
             ad = inportb(0x281)|((inportb(0x282)&0x0f)<<8);/*AD����*/
             if (ad <2047) ad = 2047;

              DA_Code[data_amount] = Code_Converter( 2, AD_Code[data_amount]);
              outportb(0x283,(ad&0x00ff));    /*�����ͱ���DA_Code[data_amount]�еĵ�12λ��D/A���*/
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
    outportb(0x43, 0x36);      /*0x43Ϊ8254�����ּĴ�����ַ;
                               0x36˵����ѡ��ͨ��0����ͨ��0���ڵ����Ӷ�ʱ��׼��
                               OUT0�ӵ�8259A��IR0(��ʱ��)��Ϊ�ж�Դ���ж����ͺ�08H.��
                               ��д��ʽѡ��'�ȶ�/д��8λ���ٶ�/д��8λ';
                               ������ʽѡ��ʽ3����������������Ƶ����ʽ��,����ѡ������Ƽ���*/
    outportb(0x40, c0&0x00ff);   /*0x40Ϊ'ͨ��0'��ַ*/
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

    outportb(0x280,0x00); /*ѡADͨ��0��������ADת��*/
    do
                ;
    while(!(inportb(0x285) & 0x80));  /*��ѯADת���Ƿ����*/
    
    ad1 = inportb(0x281)|((inportb(0x282)&0x0f)<<8);/*AD����*/
        
    ad = ad1 + giLastLastAd - 2*giLastAd;

    ad =  80*  ad;
#ifdef DELTA
    ad = 0.9*giLastDelta  + 0.1*ad;
    giLastDelta  = ad;
#endif
    ad = ad + 2047;
    if (ad > 4095) ad = 4095;
    if (ad < 0 ) ad = 0;
   outportb(0x283,(ad&0x00ff));    /*�����ͱ���DA_Code[data_amount]�еĵ�12λ��D/A���*/
   outportb(0x284,((ad>>8)&0x0f));

   giLastLastAd = giLastAd;
   giLastAd = ad1;
    outportb(0x20, 0x20);         /*�ж�Դ����Ƭ��EOI����*/
}


/*conv_mode =1��ADת��Volt
  conv_mode =2��ADת��DA  */
int Code_Converter(int conv_mode, int AD)
{
    int data;
    switch(conv_mode)
    {
        case 1:
            data = (AD-2047)*10/4096;
            return data;
        case 2:
            data = AD+1;           /*ת���ö���*/
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


