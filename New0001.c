







void Test()
{
    unsigned char ucCurState;
    unsigned char ucLastState =0;


    ucCurState = inportb(0x278);

    // �ж� �ߵ�ƽ����ִ����Ӧ����
    if ((ucCurState & 0x01) != 0)
    {

    }


}






void Test()
{
    unsigned char ucCurState;
    unsigned char ucLastState =0;
    int i;


    while(1)
    {
         ucCurState = inportb(0x278);


         for(i=0;i<5;i++)
         {

            // �ж� �����ر仯����ִ����Ӧ����
            if ( ((ucCurState & (1<<i)) != 0) && ((ucLastState & (1<<i))== 0))
            {
   
   
            }

         }

         ucLastState = ucCurState;
    }






}




void Test()
{
    unsigned char ucCurState;
    unsigned char ucLastState =0;
    int i;
    unsigned char ucEdge;	


    while(1)
    {
         ucCurState = inportb(0x278);
         ucEdge = ucCurState ^ ucLastState;

         ucEdge = ucEdge & ucCurState;

         for(i=0;i<5;i++)
         {

            // �ж� �����ر仯����ִ����Ӧ����
            if ( (ucEdge & (1<<i)) != 0)
            {
   
   
            }

         }

         ucLastState = ucCurState;
    }






}





void Test()
{
    unsigned char ucCurState;
    unsigned char ucLastState =0;
    int i ;
    unsigned char ucEdge;	


    while(1)
    {
         ucCurState = inportb(0x278);
         ucEdge = ucCurState ^ ucLastState;

         ucEdge = ucEdge & (~ucCurState);
         i = 0;

         while(ucEdge)
         {
               if ((ucEdge & 0x01) != 0)
               {


               }
            
                i ++;
		 ucEdge = ucEdge>>1;
         }



         ucLastState = ucCurState;
    }






}




void Ctrl(unsigned char ucChn)
{



}








