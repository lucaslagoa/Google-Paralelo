#include<pthread.h>
#include<stdio.h>


void *function(void *arg){
	int *incoming = (int*)arg;
	*incoming*=2;

	return NULL;
}


int main(){
		pthread_t thread_ID;
		void *exit_status;
		int value;

		//Put something meaningful into value.
		value=42;

		//Create the thread, passing &value for the argument
		pthread_create(&thread_ID, NULL, function, &value);

		//the main program continues while the thread executes

		//wait for thread to terminate.

		pthread_join(thread_ID, &exit_status);

		printf("value is %d\n", value);

		//only the main thread is running now.
		return 0;
}
	
