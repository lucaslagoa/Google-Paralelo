#ifndef func_h
#define func_h
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define tamanho 100

struct timeval inicio, final;

typedef struct palavras
{
	long int peso;
	char word[tamanho];
}w;
typedef struct selecao
{
	int *vetor_selec;
	long int *vetor_peso;
	int k;
}sel;
typedef struct busca_paralela
{
	int tam_palavra_user;
	w *palavra;
	int numero_de_palavras;
	sel s;
	char *palavra_usuario;
	int inicio;
	int final;
	int k_retorno;
}busca;	

char *min(char *palavra_digitada_usuario, int tam_palavra_user);
void *func_busca(void *a);
sel ordena(int numero_de_palavras, w *palavra, sel s);
sel bubble_sort(sel s);
void func_imprime(w *palavra, sel s, long int *vetor_ordenado, int k_parametro, int numero_de_palavras);

w *palavra;//declaração da struct 
sel s;//declaração do struct 
int k_retorno;
pthread_t *threads;

#endif
