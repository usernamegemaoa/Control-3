#include <dos.h>
#include <bios.h>

main ()
{
 int i;
 int j = 0;
 while(j<2){
 j++;
 for(i =0;i <3600;i++)
 {
  if(i%2 == 0)
    outportb(0x378,0x01);
  else
    outportb(0x378,0x03);
  delay(10);
  printf("%d \n",i);
 /* getch();*/
  }
/*  getch();*/
  delay(2000);

  for (i=0;i<3600;i++)
  {
    printf("%d\n",i);
    if (i%2 == 0)
       outportb(0x378,0x00);
    else
       outportb(0x378,0x02);
    delay(10);
  }
  delay(2000);
  }
  getch();

}
