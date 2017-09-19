#include <stdio.h>
#include <pthread.h>

#define N 10
#define nthreads 2

typedef struct {
	int inicio;
	int fim;
} thread_arg, *ptr_thread_arg;

int A[N] = {4,3,6,7,8,9,3,3,4,6};
int B[N] = {8,4,2,5,7,3,1,9,5,4};
int C[N] = {0,0,0,0,0,0,0,0,0,0};

void *func(void *arg){
	ptr_thread_arg argument = (ptr_thread_arg) arg;
	int i;

	for(i = argument->inicio ;
	       i < argument->fim ; i++){
		C[i] = A[i] + B[i];
	}
}

int main(int argc, char *argv[]){
	int i;
	pthread_t THREADS[nthreads];
	thread_arg argument[nthreads];

	for(i = 0; i < nthreads; i++){
		argument[i].inicio = (i*N)/nthreads;
		argument[i].fim = ((i+1)*N)/nthreads;
		pthread_create(&(THREADS[i]), NULL,
		               func, &(argument[i]));
	}

	for(i = 0; i < nthreads; i++){
		pthread_join(THREADS[i],NULL);
	}

	for(i = 0; i < N; i++){
		printf("%d ",C[i]);
	}
	printf("\n");
	return 0;
}
