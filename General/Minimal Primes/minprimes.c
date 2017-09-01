#include <stdio.h>
#include <math.h>
#define MAXNUMB 100000000

int main(void)
 {
	
	int i;
	int x;
	int y;
	int mask;
	int psifia;
	int max;
	int counter;
	int j;
	long k;
	int timi;
	int flag;
	int break1;
	int num;
	int meta=0;
	int breakvar;
	
	for(k=1;k<=MAXNUMB;k=k+2)                                                   /* Den xreiazetai na elegxei tous artious arithmous */                                             
	{
		
		
		 breakvar=0;
		 psifia=0;
		 max=1;
		 counter=0;
		 break1=0;
		 flag=0;
		 
		                                  									    
		 if (k==1)               												/* I ennoia tou prwtou arithmou afora arithmous megaluterous tou 1 */                                               
		 {
		 	continue;
		 }
		 
		
  		int temp;
		int factor=1;
		int num1;
		
  
		num1=k;
  		temp=k;
  		
  		while(temp)                                                             /* I diadikasia me tin opoia ginetai eksagogi psifion vrethike apo tin akolouthi istoselida: */                                              						
		{																		/* http://www.cquestions.com/2011/09/split-number-into-digits-in-c.html */
      		temp=temp/10;
     		factor = factor*10;
 		}
 		
  		while(factor>1)															
		{
      		factor = factor/10;
      		meta=num1/factor;                                                   /* Pairnei ta psifia tou kathe arithmou */
     		
     		if(meta==3 || meta==5 || meta==7)									/* An o arithmos k periexei 3 i 5 i 7 tote den xreiazetai na sinexisei o elegxos gia ton arithmo */
     		{
     			breakvar=1;
     			break;
			}
     		
     		
      		num1= num1 % factor;
      	}
      	
	 	if (breakvar==1)														/* An psifio tou arithmou periexei to 3 i 5 i 7 tote proxwraei se epomeno arithmo k */
	 	{
	 		continue;
		}
		
		 
		 
		                                                                    
		for(j=3;j<=sqrt(k);j=j+2)												/* Elegxos gia to an o arithmos einai protos */
		{
			if( (k%j)==0 )                                            			/* An uparxei diairetis teleiwnei i epanalipsi - o arithmos k den einai prwtos */
				break;
		}
		if( j>sqrt(k) )                                           		   		/* O arithmos k den exei kanenan diaireti ara einai prwtos */
				flag=1;
	
		
		
		
		if (flag==1)															/* Elegxos an o arithmos einai prwtos */
		{
		
			num=k;                                                              /* I metavliti num pernei tin timi tou k */
		
			while ( num != 0 )                                                  /* Ypologizetai posa psifia exei o arithmos k */
			{
				num = num/10 ;
				psifia++;
			}

			for ( i = 1; i <= psifia; i++ )                                     /* Dimiourgei tin dinami sto duadiko sistima */
			{
				max = max*2;
			}
		 
			for ( i = 1; i < max - 1 ; i++ )                                    /* Gia ola ta psifia tou arithmou sto duadiko */
			{
				mask = i;
				x = 1;
				y=0;
				timi=k;
			
				while ( timi != 0 )                                             
				{
					if ( mask % 2 == 1 ) 
					{
						y = y + timi % 10 * x;									/* Ypologizetai i upoakolouthia tou arithmou k */
						x = x*10; 
					}
					timi /= 10;
					mask /= 2;
				}
			
			 		if ( y==2 || y==3 || y==5 || y==7 ) 						/* An upoakolouthia tou arithmou k periexei 3 i 5 i 7 tote proxwraei se epomeno arithmo k */
			 		{
						break1=1;
						break;
					}   
				
				if ( y%2!=0 && y>2 )  											/* Elegxos an i upoakolouthia tou arithmou k einai perittos */     
				{
			
					for(j=3;j<=sqrt(y);j=j+2)									/* Elegxos gia to an i upoakolouthia tou arithmou k einai protos */								
					{
						if( (y%j)==0 )											/* An uparxei diairetis teleiwnei i epanalipsi - i upoakolouthia den einai prwtos */										
						{
							break;	
						}
					}
				
					if( j>sqrt(y) )												/* I upoakolouthia tou arithmou k den exei kanenan diaireti ara einai prwtos */
					{	
						counter++;	                                            
						break1=1;
						break;
					}
				}
			}


			if (break1==1)														/* Proxwraei se epomeno arithmo k ,efoson exei ginei elegxos kai i metavliti break1 exei tin timi 1 */
			{
				continue;
			}
		
			if ( (counter==0) )													/*An i metavliti counter einai 0 simenei oti kamia upoakolouthia tou arithmou k den einai prwtos */
				printf("%d \n",k);                                              /* Ektipwnei ton arithmo k */
		}	
	}		
	return 0;
}

