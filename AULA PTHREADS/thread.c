//COMO COMPILAR:    
//    gcc thread.c -o thread -lpthread



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define N 10 //Numero de threads
void *printID(void *id){
	printf("Thread de ID: %d\n",(int)id);
	pthread_exit(NULL);
}



int main(int argc, char const *argv[]){
	int i,status;
	pthread_t threads[N];
	printf("CRIANDO THREADS... \n");
	for(i = 0; i < N; i++){
		status = pthread_create(&threads[i],NULL,printID,(void *)i); //Função retorna um numero diferente de 0 caso dê erro na criação da thread
		if (status != 0){
			printf("Erro na criação da thread de ID: %d\n",i);
		}
	}

	for(i=0; i<N; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}