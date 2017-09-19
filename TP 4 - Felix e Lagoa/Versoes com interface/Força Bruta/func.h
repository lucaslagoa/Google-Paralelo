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
typedef struct palavra
{
	long long int peso;
	char word[tamanho];
}w;
typedef struct palavras_selec//struct com as palavras selecionadas 
{
	long long int peso_palavra_selecionada;
	char palavra_selecionada[tamanho];
}ws;
typedef struct busca_paralela
{
	int tamanho_palavra_pesquisada;
	w *palavras;
	int numero_de_palavras;
	ws *ps;
	char *palavra_pesquisada;
	int inicio;
	int final;
}busca;	

char *min(char *palavra_pesquisada_leitura, int tamanho_palavra_pesquisada);
void *seleciona_palavra(void *a);
long long int *bubble_sort(long long int *vetor, int numero_de_palavras);
long long int *vetor_maiores(ws *ps, int tamanho_palavra_pesquisada,long long int *vetor_ordenado, long long int *vetor, int numero_de_palavras);
void func_imprime(ws *ps, long long int *vetor_ordenado, int numero_de_palavras, int k_parametro);

w *palavras;//declaração da struct 
ws *ps;//declaração da struct que receberá as palavras selecionadas 
pthread_t *threads;

#endif

