#include <stdio.h>

int main(void)
{
	
	
	int code;
	int ch,ch1,ch2,ch3,ch4;
	int b,b1,b2,a1,a2,unicode;
	int k1,k2,l1,l2;

	ch=getchar();
	
	while( ch != EOF)
	{
		
		/* Diavasma xaraktira UTF-8 */
		
		ch1=0;
		ch2=0;
		ch3=0;
		ch4=0;
		unicode=0;
		b=0;
		b1=0;
		b2=0;
		a1=0;
		a2=0;
		k1=0;
		k2=0;
		l1=0;
		l2=0;
	
	
		ch1=ch;																	/* Diavazei to prwto byte tou xaraktira */

		if ( ch1>=0x0000 && ch1<=0x007F)										/* An to prwto byte einai apo mexri 127 tote ginetai kwdikopoihsh me 1 byte */
		{
			unicode=ch1;														
		}
		else if ( ch1>=0x00C0 && ch1<=0x00DF )									/* An to prwto byte einai apo 192 mexri 223 tote ginetai kwdikopoihsh me 2 bytes */
		{
			ch1 &= 31; 														
		
			ch2=getchar();														
			
			ch2 &= 63;														

			unicode= (ch1<<6)|ch2;													
		}
		else if ( ch1>=0x00E0 && ch1<=0x00EF )									/* An to prwto byte einai apo 224 mexri 239 tote ginetai kwdikopoihsh me 3 bytes */
		{
			ch1 &= 15;															
		
			ch2=getchar();													
			
			ch2 &= 63;														
		
			ch3=getchar();														
			
			ch3 &= 63;															
		
			unicode=(ch1<<12)+(ch2<<6)+ch3;										
		}
		else if ( ch1>=0x00F0 && ch1<=0x00F7 )									/* An to prwto byte einai apo 240 mexri 247 tote ginetai kwdikopoihsh me 4 bytes */						
		{
			ch1 &= 7;															
		
			ch2=getchar();														
			
			ch2 &= 63;															
			
			ch3=getchar();
			
			ch3 &= 63;
	
			ch4=getchar();
			
			ch4 &= 63;
			
			unicode=(ch1<<18)+(ch2<<12)+(ch3<<6)+ch4;
		}
		
		
		/* Ektiposi tou xaraktira UTF-16 */
		
		
		if(	 (unicode>=0x0000 && unicode<=0xD7FF) || (unicode>=0xE000 && unicode<=0xFFFF) ) /* An to unicode vrisketai sto [0,55295] h [57344,65535] tote ginetai kwdikopoihsh me 2 bytes kai ektiponontai ta 2 bytes */
		{
			putchar(unicode>>8);
			putchar(unicode&0x00FF);
		}
		else if( unicode>=0x10000 && unicode<=0x10FFFF )						/* An to unicode vrisketai sto [65536,1114111] tote ginetai kwdikopoihsh me 4 bytes kai ektiponontai ta 4 bytes */
		{
			
			
			b=unicode-0x010000;
		
			b1=(b>>10);
			
			b2=(b&0x3FF);
		
			a1=b1+0xD800;
		
			a2=b2+0xDC00;
		
			k1=a1>>8;															/* To prwto byte tou A1 */
		
			k2=a1&0x00FF;														/* To deutero byte tou A1 */
		
			l1=a2>>8;															/* To prwto byte tou A2 */
	
			l2=a2&0x00FF;														/* To deutero byte tou A2 */
	
			putchar(k1);
			putchar(k2);
			putchar(l1);
			putchar(l2);		
		}
		else
		{
			printf("Error");
			break;
		}
		
		ch=getchar();
		
	}
	  
	return 0;
		
}
