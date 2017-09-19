#include <stdio.h>
#include <time.h>
#include <pthread.h>

#define nthreads 2

void *func(void *arg) {
	int *argument = (int *) arg;
	printf("Olá, eu sou a thread %d\n",*argument);
}

int main() {
	int i;
	pthread_t THREADS[nthreads];

// erro: condicao de corrida na variavel id
/*
	int id;
	while(1) {
		for (i = 0; i < nthreads; i++) {
			id = i;
			pthread_create(&(THREADS[i]), NULL, func, &id);
		}
		for (i = 0; i < nthreads; i++) {
			pthread_join(THREADS[i], NULL);
		}
		sleep(1);
	}
*/

	int id[nthreads];
	while(1) {
		for (i = 0; i < nthreads; i++) {
			id[i] = i;
			pthread_create(&(THREADS[i]), NULL, func, &id[i]);
		}
		for (i = 0; i < nthreads; i++) {
			pthread_join(THREADS[i], NULL);
		}
		sleep(1);
	}

	return 0;
}
