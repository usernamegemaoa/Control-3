







void Test()
{
    unsigned char ucCurState;
    unsigned char ucLastState =0;


    ucCurState = inportb(0x278);

    // 判断 高电平，并执行相应动作
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

            // 判断 上升沿变化，并执行相应动作
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

            // 判断 上升沿变化，并执行相应动作
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








