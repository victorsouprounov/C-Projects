#include <stdio.h>
#include <stdlib.h>
#include "func.h"


/* Voithitiko uliko apo tin istoselida http://www.geeksforgeeks.org/collect-maximum-coins-before-hitting-a-dead-end/ */

int isInvalid(int i, int j,int R,int C)												/* Elegxei an to keli vrisketai sta oria tou pinaka */
{
	return (i >=0 && i < R && j >=0 && j < C);
}

int pureRecursive(int R,int C,int **arr ,int i, int j)
{
	int result;
	
	if (isInvalid(i,j,R,C) == 0)										/* An to keli den vrisketai sta oria tou pinaka epistrefei 0 */
		return 0;

	if(arr[i][j] == 'C')												/* An to keli periexei to C tote auksanetai o metritis */
	{
		result=1;
	}
	else
	{
		result=0;
	}

	if(	pureRecursive(R,C,arr, i+1, j) > pureRecursive(R,C,arr, i, j+1) )
	{
	
		return result + pureRecursive(R,C,arr, i+1, j);					/* Psaxnei mia thesi katw */
		
	}
	else
	{
		
		return result + pureRecursive(R,C,arr, i, j+1);					/* Psaxnei mia thesi deksia */
		
	}
	
}

/* ---------------------------------------------------------------------- */

void iterativeDP(int R,int C, int **arr ,int i, int j)
{
		
	int **temp = (int **)malloc((R+1)* sizeof(int *));
	
	for (i=0; i<R+1; i++)
	{
		temp[i] = (int *)malloc((C+1)* sizeof(int));
	}

	for (i = 0; i < R+1; i++)
	{
		for (j = 0; j < C+1; j++)
		{
			*(*(temp+i)+j) = 0;
		}
	}
	
	for (i = 0; i < R+1; i++)
	{
		for (j = 0; j < C+1; j++)
		{
			printf("%c",temp[i][j]);
		}
	}
	
	
	for(i=1;i<R+1;i++)
	{
		for(j=1;j<C+1;j++)
		{
	
				if(temp[i][j-1] > temp[i-1][j] )			/* Ypologizei to Cij */
				{
		
					if( arr[i-1][j-1]=='C' )				/* An uparxei nomisma stin thesi tote auksanetai kata mia monada */
					{
						int y;
						y=temp[i][j-1];
						y++;
						temp[i][j]=y;
					}
					else									/* Alliws apothikeuetai sto keli i megaluteri apo tis 2 times */
					{
						int y;
						y=temp[i][j-1];
						temp[i][j]=y;
					}
				}
				else
				{
	
					if( arr[i-1][j-1]=='C' )
					{
						int y;
						y=temp[i-1][j];
						y++;
						temp[i][j]=y;
					}
					else
					{
						int y;
						y=temp[i-1][j];
						temp[i][j]=y;
					}
				
				}
						
		}
		
	}
	
	int posi=1;
	int posj=1;
	int metritis;
	int temporary=temp[1][1];
	
	if(temp[1][1]==1)
	{
		metritis=1;
	}
	else
	{
		metritis=0;
	}
	
	printf("Path is: C(%d,%d)/%d",posi,posj,metritis);
	for(i=posi;i<R+1;i++)
	{
		for(j=posj;j<C+1;j++)
		{
			
			while(j<C+1)												/* Gia kathe grammi vriskei ton megalutero arithmo */
			{
				if( temp[i][j] > temporary )
				{
					temporary=temp[i][j];
					metritis++;
					posi=i;
					posj=j;
					printf("---> C(%d,%d)/%d",i,j,metritis);
				}
				else if(temp[i][j] == temporary)						/* An exoun tin idia timi tote teleiwnei i epanalipsi */
				{
					break;
				}
				else
				{
					printf("---> C(%d,%d)/%d",i,j,metritis);
					j++;
				}
			}	
			posi=i+1;											/* Paei mia thesi katw */				
		}
	}

	printf("---> Picked up: %d coins \n",metritis);


}

