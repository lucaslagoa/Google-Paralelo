#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

#define NUMTHREADS 100


 void *function( void *number){
		 long newnumber;
		 newnumber=(long) number;
		 double *result;
		 result=(double*) malloc(sizeof(double));

		 printf("the squareroot of %ld is being calculated\n", newnumber);
		 *result=(double)sqrt((double)newnumber);

		 return result;
 }

 int main(){
		 pthread_t threads[NUMTHREADS];
		 long number;
		 double  result[NUMTHREADS];
		 void *exit_status[NUMTHREADS];
		 int rc;

		 for(number=0;number<NUMTHREADS;number++){
				 rc=pthread_create(&threads[number],NULL, function, (void*) number);
				 if(rc){
						 printf("there's been an error in creating thread %ld",number);
						 return 0;
				 }
		 }

		 for(number=0;number<NUMTHREADS;number++){
				 pthread_join(threads[number],&exit_status[number]);
				 result[number]=*((double*)exit_status[number]);
				 printf("the squareroot of %ld is %f\n",number,result[number]);
				 free((double*)exit_status[number]);
		 }

		 return 0;
 }

		 


