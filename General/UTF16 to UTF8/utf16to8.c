#include <stdio.h>

int main(void)
{
	
	

	int ch,ch1,ch2,ch3,ch4;
	int c,c2,b1,b2,unicode;
	int flag=0;
	ch=getchar();
	
	while( ch != EOF)
	{
		
		/* Diavasma xaraktira UTF-16 */
		
		ch1=0;
		ch2=0;
		ch3=0;
		ch4=0;
		c=0;
		c2=0;
		b1=0;
		b2=0;
		
		unicode=0;
	
		ch1=ch;                                                                 /* Diavazei to prwto byte */
		
		ch2=getchar();															/* Diavazei to deutero byte */

		c=(ch1<<8)|ch2;															/* Ta enwnei */
		
		if(  (c>=0x0000 && c<=0xD7FF)  || (c>=0xE000 && c<=0xFFFF)  )			/* An o xaraktiras vrisketai sto sugkekrimeno diastima tote kwdikopoieitai autousios */
		{
			flag=1;
			
			unicode=c;		
		}	
		else 																	/* Alliws,o xaraktiras kwdikopoieitai me 4 bytes */
		{
			ch3=getchar();														/* Diavazei to trito byte */
		
			ch4=getchar();														/* Diavazei to tetarto byte */
		
			c2=(ch3<<8)|ch4;													/* Ta enwnei */
	
			b1=c - 0xD800;														
	
			b2=c2 - 0xDC00;
		
			unicode=( (b1<<10)|b2 ) + 0x010000;									/* Dimiourgei to unicode opws anaferei i ekfwnisi */
		
			flag=2;
			
		}
	
	
		
		/* Ektipwsi tou xaraktira se UTF-8 */ 
		
		/* I diadikasia ektipwsis vrethike apo tin istoselida http://czyborra.com/utf/ */
		
		if(flag==1)											
		{
  
			if (unicode>=0x0000 && unicode<=0x007F)								/* An o unicode xaraktiras einai 1 byte */
			{
   				 putchar (unicode);
 			}
 			else if (unicode>=0x0080 && unicode<=0x07FF) 						/* An o unicode xaraktiras einai 2 bytes */
			{
    			putchar (0xC0 | unicode>>6);
  				putchar (0x80 | unicode & 0x3F);
  				
  			}
  			else if (unicode>=0x0800 && unicode<=0xFFFF) 						/* An o unicode xaraktiras einai 3 bytes */
			{
			
   	 			putchar (0xE0 | unicode>>12);
   				putchar (0x80 | unicode>>6 & 0x3F);
   				putchar (0x80 | unicode & 0x3F);
 			}
  		}	
  		else																	/* Telos,an o unicode xaraktiras einai 4 bytes */
		{
   				putchar (0xF0 | unicode>>18);
    			putchar (0x80 | unicode>>12 & 0x3F);
    			putchar (0x80 | unicode>>6 & 0x3F);
    			putchar (0x80 | unicode & 0x3F);
		}
		
			ch=getchar();
			
		
	}
	  
	
	
	
	  
	  


	return 0;
		
}
