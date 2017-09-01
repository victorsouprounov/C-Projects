#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#define MINNUM 1990000001
#define MAXNUM 2000000000 
#define MAXTRIES 10
int main(void)
{
   long i,j;
   int z=0;
   clock_t tend , tstart;
   double cpu_time_used=0.0;
   printf("Checking range [1990000001,2000000000] for primes.");
   tstart=clock(); 
       for(i=MINNUM;i<=MAXNUM;i=i+2)                                                  /* Check the numbers from 1990000001 to 2000000000 ignoring the even nubers */
      {
          for(j=3;j*j<=i;j=j+2)                                                       /* For every odd number,checks if it's prime */
          {
		    if((i%j)==0)
            break;
          }
            if(j*j>i)
               z=z+1;
       }
   tend=clock();
   cpu_time_used = ((double) (tend - tstart)) / CLOCKS_PER_SEC;
   printf( "\nDeterministic Algorithm:Found %d primes in %g",z,cpu_time_used);        /* Prints the result and the time used  */


 
 	long long result,p,exp,a;
 	long sum;
 	long curtime;
 	curtime = time(NULL);
 	srand((unsigned int) curtime);
 	printf("\nTrying Fermat test with seed %ld\n",curtime); 
 
 	
 	     for(i=1; i<=MAXTRIES; i++)                                                    /* According to Probabilistic algorithm, search for prime numbers with one try,two tries etc. */                                                
 	       {
 	       	sum=0;
 	       	tstart=clock();
 	       	
 	       	for(p=1990000001;p<=2000000000;p=p+1)                                      /* Check the numbers from 1990000001 to 2000000000 */
 	       	{
				 for(j=1;j<=i;j=j+1)                                                   /* For every number,picks MAXTRIES random numbers to check if it's prime */
				{
				
				a=rand()%(p-1)+1;
				result=1;
				exp=p-1;
				while (exp>0)
				{
					if(exp%2==1)
					result=((result*a)%p);
					exp=exp/2;
					a=(a*a)%p;
					
				}
			
				if (result!=1)                                                          /* According to Probabilistic algorithm,if the result is not 1 then check the next number */
				break;
			    }
			    if (result==1)                                                          /* If the result is 1,then count in */                                                         
			    sum=sum+1;
		    }
		    tend=clock();
		    cpu_time_used = ((double) (tend - tstart)) / CLOCKS_PER_SEC;
		    
 	    
 	       	printf("Probabilistic Algorithm:Found %ld primes in %g secs (Tries=%ld)\n",sum,cpu_time_used,i);  /* Prints the result and the time used */
 	       	}
 	return 0;
 }
