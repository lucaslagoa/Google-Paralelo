#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUMCONS      2
#define NUMPROD      2
#define BUFFERSIZE   1000

pthread_t cons[NUMCONS];
pthread_t prod[NUMPROD];
pthread_mutex_t buffer_mutex;
int buffer[BUFFERSIZE];
int prod_pos=0, cons_pos=0;

void *produtor(void *arg) {
	int n;
	while(1) {
		n = (int)(drand48() * 1000.0);
		pthread_mutex_lock(&buffer_mutex);
		buffer[prod_pos] = n;
		prod_pos = (prod_pos+1) % BUFFERSIZE;
		pthread_mutex_unlock(&buffer_mutex);
		printf("Produzindo numero %d\n", n);
		sleep((int)(drand48() * 4.0));
	}
}

void *consumidor(void *arg) {
	int n;
	while(1) {
		pthread_mutex_lock(&buffer_mutex);
		n = buffer[cons_pos];
		cons_pos = (cons_pos+1) % BUFFERSIZE;
		pthread_mutex_unlock(&buffer_mutex);
		printf("Consumindo numero %d\n", n);
		sleep((int)(drand48() * 4.0));
	}
}

int main(int argc, char **argv) {
	int i;
	srand48(time(NULL));
	pthread_mutex_init(&buffer_mutex, NULL);
	for(i=0; i<NUMCONS; i++) {
		pthread_create(&(cons[i]), NULL, consumidor, NULL);
	}
	for(i=0; i<NUMPROD; i++) {
		pthread_create(&(prod[i]), NULL, produtor, NULL);
	}
	for(i=0; i<NUMCONS; i++) {
		pthread_join(cons[i], NULL);
	}
	for(i=0; i<NUMPROD; i++) {
		pthread_join(prod[i], NULL);
	}

	return 0;
}
