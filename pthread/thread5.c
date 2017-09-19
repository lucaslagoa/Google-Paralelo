#include<stdio.h>
#include<pthread.h>

#define NUMTHREADS 10

void *function( void *arg){
		long  thread_ID;

		thread_ID=(long)arg;
		printf("hello world, this is thread %ld\n", thread_ID);

		return NULL;
}	

int main(){
		pthread_t threads[NUMTHREADS];
		int control;
		long thread_id;
		void * exit_status;
		int rc;
		
		for(thread_id=0;thread_id<NUMTHREADS;thread_id++){
				rc=pthread_create(&threads[thread_id], NULL, function,(void *) thread_id);
				//control whether thread was created or not
				if(rc){
						printf("Error; return code from pthread_create() is %d\n",rc);
						return 0;
				}
				//pthread_join(threads[thread_id], &exit_status);
		}
		pthread_exit(NULL);

		return 0;
}

