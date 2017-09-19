#include <stdio.h>
#include <pthread.h>

#define N 10
#define nthreads 2

typedef struct {
	int inicio;
	int fim;
} thread_arg, *ptr_thread_arg;

int vetor[N] = {4,3,6,7,8,9,3,3,4,6};
int soma;
pthread_mutex_t mutex;

void *func(void *arg){
	ptr_thread_arg argument = (ptr_thread_arg) arg;
	int i, soma_parcial;
	soma_parcial = 0;
	for(i = argument->inicio ;
	       i < argument->fim ; i++){
		soma_parcial += vetor[i];
	}
	pthread_mutex_lock(&mutex);
	soma += soma_parcial;
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[]){
	int i;
	pthread_t THREADS[nthreads];
	thread_arg argument[nthreads];

	pthread_mutex_init(&mutex, NULL);

	for(i = 0; i < nthreads; i++){
		argument[i].inicio = (i*N)/nthreads;
		argument[i].fim = ((i+1)*N)/nthreads;
		pthread_create(&(THREADS[i]), NULL,
		               func, &(argument[i]));
	}

	for(i = 0; i < nthreads; i++){
		pthread_join(THREADS[i],NULL);
	}

	printf("%d\n",soma); // resultado correto: 53

	return 0;
}
