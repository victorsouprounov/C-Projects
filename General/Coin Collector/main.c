#include <stdio.h>
#include <stdlib.h>
#include "func.h"

void solve(int R, int C, int** arr);

int main(void)
{
	
	int ch,i,j,counter=0;
	int R,C;
	ch=getchar();
	
	char buff[1000];

	char buffer4[5];
	char buffer3[5];
	
	int metr1=0;
	int metr2=0;
	int flag=0;
	while (ch != EOF)
	{

		if( flag==0 )									/* Gia tin prwti grammi vriskei ta R,C */
		{
			while( ch != '\n')
				{
					if(ch != ' ')
					{
						buffer4[metr1]=ch;
						metr1++;
						ch=getchar();
					}
					else
					{
						while(ch != '\n')
						{
							buffer3[metr2]=ch;
							metr2++;
							ch=getchar();	
						}
					}
				}
			flag=1;
		}
		

		if( ch == '\n') 						/* Ston xaraktira allagis grammis den kanei tipota */
		{
			ch = getchar();
			continue;
			
		}
		
		if (ch == ' ') 							/* Ston xaraktira space den kanei tipota */
		{
			ch = getchar();
			continue;
			
		}
		
		buff[counter]=ch;						/* Gia ta upoloipa ta apothikeuei se enan prosorino pinaka */
		counter++;	
				
	  ch = getchar();                            
  	
	  }
	  
	R=atoi(buffer4);							
	
	C=atoi(buffer3);
	
	int **arr = (int **)malloc(R * sizeof(int *));
	for (i=0; i<R; i++)
	{
		arr[i] = (int *)malloc(C * sizeof(int));
	}

	int counter1=0;
	for (i = 0; i < R; i++)
	{
		for (j = 0; j < C; j++)
		{
			*(*(arr+i)+j) = buff[counter1];
			counter1++;	
		}
	}
	
	solve(R,C,arr);
	return 0;

}


void solve(int R, int C, int** arr)
{

	#if defined(PUREREC) 
		int x;
      	x= pureRecursive(R,C,arr, 0, 0); 
      	printf("Maximum number of coins to pick up is: %d \n",x);
	#elif defined(ITERDP)
        iterativeDP(R,C,arr, 0, 0);
    #else
    	printf("Error");
	#endif
       
	
}










