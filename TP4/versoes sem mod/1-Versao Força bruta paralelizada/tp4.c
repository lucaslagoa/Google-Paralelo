//********************************************************
//* Aluno: Lucas Felix e Lucas Lagoa                     *
//* Professor: Leonardo Rocha                            *
//* Matéria: Algoritmos e estrutura de dados III         *
//********************************************************
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
struct timeval inicio, final;
char *min(char *palavra_pesquisada_leitura, int tamanho_palavra_pesquisada)
{//função responsável por deixar todas as letras minusculas para a pesquisa
	int i;
	for(i=0;i<tamanho_palavra_pesquisada;i++)
	{
		if((palavra_pesquisada_leitura[i]>=65)&&(palavra_pesquisada_leitura[i]<=90))
		{
			palavra_pesquisada_leitura[i]=palavra_pesquisada_leitura[i]+32;
		}
		else if((palavra_pesquisada_leitura[i]>=48)&&(palavra_pesquisada_leitura[i]<=57))
		{
			palavra_pesquisada_leitura[i]=palavra_pesquisada_leitura[i]-32;
		}
	}
	return palavra_pesquisada_leitura;
}
void *seleciona_palavra(void *a)
{
	busca *c= (busca *)a;//cast
	int i=0, j=0,cont=0, k=0;
	for(i=0;i<c->numero_de_palavras;i++)
	{//numero de palavras totais do dicionário
		cont=0;//tem que ser zero a cada palavra  	 
		for(j=0;j<c->tamanho_palavra_pesquisada;j++)
		{//vai até o tamanho do préfixo  
			if((c->palavra_pesquisada[j]==c->palavras[i].word[j+1])||(c->palavra_pesquisada[j]==c->palavras[i].word[j+1]+32)||(c->palavra_pesquisada[j]==c->palavras[i].word[j+1]-32)) //tem que ser j+1, pois na leitura pega-se o espaço
			{
				cont++;
				if(cont==c->tamanho_palavra_pesquisada)//quer dizer que palavra é do tamanho do prefixo
				{
					k=0;
					while(k<tamanho)//saber ao certo quantas serão as palavras selecionadas 
					{//copiando o peso e a palavra para a struct de palavras selecionadas
						c->ps[i].peso_palavra_selecionada=c->palavras[i].peso;
						c->ps[i].palavra_selecionada[k]=c->palavras[i].word[k];
						k++;
					}
				}
			}				
		}
	}
	return c;
}
long long int *bubble_sort(long long int *vetor, int numero_de_palavras)
{
	int i, j;
	long long int aux;
	for(i=numero_de_palavras-1;i>=1;i--) 
	{
		for(j=0;j<i;j++) 
		{
			if(vetor[j]<vetor[j+1]) 
			{
				aux=vetor[j];
				vetor[j]=vetor[j+1];
				vetor[j+1]=aux;
			}
		}
	}
	return vetor;
}
long long int *vetor_maiores(ws *ps, int tamanho_palavra_pesquisada,long long int *vetor_ordenado, long long int *vetor, int numero_de_palavras)
{
	int i=0;//aloco um vetor do tamanho do número de linhas da matriz
	while(i<numero_de_palavras)//saber ao certo a quantidade de palavras que serão retornadas
	{
		vetor[i]=ps[i].peso_palavra_selecionada;
		i++;
	}
	vetor_ordenado=bubble_sort(vetor, numero_de_palavras);//ordena o vetor utilizando o bubblesort
	return vetor_ordenado;
}
void func_imprime(ws *ps, long long int *vetor_ordenado, int numero_de_palavras, int k_parametro)
{//função responsável de selecionar e printar as palavras que serão fornecidas pelo autocomplete
	int i=0, j=0;
	if(vetor_ordenado[0]==0)//quer dizer que não há nenhuma palavra
	{
		printf("A palavra que você procurou não está presente no dicionário, pesquise novamente!\n");
	}
	for(i=0;i<k_parametro;i++)
	{
		for(j=0;j<numero_de_palavras;j++)
		{
			if(vetor_ordenado[i]==ps[j].peso_palavra_selecionada)
			{
				printf("%s", ps[j].palavra_selecionada);
			}
		}
	}
}
//variaveis globais
w *palavras;//declaração da struct 
ws *ps;//declaração da struct que receberá as palavras selecionadas 
pthread_t threads[50];
int main(int argc, char *argv[]){

	if(argc != 6){
		printf("Os parametros foram passados de forma incorreta\n");
		 exit(0);
	}
	FILE *arq;//declaração dos arquivos de entrada e saída
	int tmili;//função de tempo
	gettimeofday(&inicio, NULL);//função de tempo
	arq=fopen(argv[2], "r");//arquivo de entrada
	int numero_de_palavras, i,j, tamanho_palavra_pesquisada, k_parametro, flag=0;//declaração de variavéis
	long long int *vetor_ordenado, *vetor;
	char *palavra_pesquisada_leitura=argv[3];
	char *palavra_pesquisada;
	k_parametro=atoi(argv[4]);
	int numero_de_threads=atoi(argv[5]);
	int cont_thread=0, div=0, resto=0;
	busca *t;
	t = (busca *) malloc(numero_de_threads  * sizeof(busca));
	while(fscanf(arq, "%d", &numero_de_palavras) != EOF)//enquanto i for menor que o número de palavras ele faz a leitura das palavras 
	{	
		if(flag==0)
		{
			palavras = (w *) malloc(numero_de_palavras * sizeof(w));//aloca palavra que é do tipo w, struct 
			ps= (ws *) malloc(numero_de_palavras * sizeof(ws));//aloca palavra que é do tipo ws, struct 
			flag++;
		}
		for(i=0;i<numero_de_palavras;i++)
		{
			fscanf(arq, "%lli", &palavras[i].peso);//faz leitura das palavras
			fgets(palavras[i].word,tamanho,arq);
		}
	}
	if(k_parametro==0)
	{
		printf("Você passou o parametro para não retornar nenhuma palavra !\n");
	}
	else if(k_parametro>numero_de_palavras)
	{
		printf("O número de palavras que você deseja retornar é maior que o número de palavras do dicionário !\n");
	}
	else
	{
		vetor_ordenado = (long long int*)malloc(numero_de_palavras * sizeof(long long int*));//aloca vetor ordenado 
		vetor = (long long int*)malloc(numero_de_palavras * sizeof(long long int*));//aloca vetor
		tamanho_palavra_pesquisada=strlen(palavra_pesquisada_leitura);//retorna o tamanho da palavra
		palavra_pesquisada=min(palavra_pesquisada_leitura, tamanho_palavra_pesquisada);
		div=numero_de_palavras/numero_de_threads;//descobre qual a quantidade de palavras cada thread irá receber
		resto=numero_de_palavras%numero_de_threads;
		cont_thread=0;
		for(i=0;i<numero_de_threads;i++)
		{
			t[i].palavras=palavras;
			t[i].ps=ps;
			t[i].palavra_pesquisada=palavra_pesquisada;
			t[i].tamanho_palavra_pesquisada=tamanho_palavra_pesquisada;
			t[i].numero_de_palavras=numero_de_palavras;
			if(cont_thread==numero_de_threads-1)
			{//a thread então irá receber o resto
				t[i].inicio=((div)*i);
				t[i].final=t[i].inicio+div+resto-1;
			}
			else if(cont_thread==0) //na thread 0 zero, recebe o inicio zero e o final número de palavras;
			{
				t[i].inicio=0;
				t[i].final=div-1;
			}
			else //quando a thread for diferente de 0 
			{
				t[i].inicio=((div)*i);
				t[i].final=t[i].inicio+(div)-1;
			}
			pthread_create(&threads[cont_thread], NULL, seleciona_palavra,(void *) &t[cont_thread]);//função responsável por criar as threads
			cont_thread++;
		}
		for(j=0;j<numero_de_threads;j++)
		{
			pthread_join(threads[j], NULL);
		}
		vetor_ordenado=vetor_maiores(ps, tamanho_palavra_pesquisada, vetor_ordenado, vetor, numero_de_palavras);
		func_imprime(ps, vetor_ordenado, numero_de_palavras, k_parametro);//função responsável por imprimir as palavras selecionadas
		gettimeofday(&final, NULL);//função de tempo
		tmili = (int) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);//função de tempo
		printf("\ntempo decorrido: %d ms\n", tmili); //função de tempo
	}
	return 0;
}
	
