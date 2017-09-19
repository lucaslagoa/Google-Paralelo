#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define tamanho 100
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
struct timeval inicio, final;
char *min(char *palavra_digitada_usuario, int tam_palavra_user)
{//função responsável por deixar todas as letras minusculas para a pesquisa
	int i;
	for(i=0;i<tam_palavra_user;i++)
	{
		if((palavra_digitada_usuario[i]>=65)&&(palavra_digitada_usuario[i]<=90)&&(palavra_digitada_usuario[i]>=49))
		{//compara com os valores tabea ascii deixando todas as letras minusculas
			palavra_digitada_usuario[i]=palavra_digitada_usuario[i]+32;
		}
		else if((palavra_digitada_usuario[i]>=48)&&(palavra_digitada_usuario[i]<=57))
		{//evita que haja dê erros ao ser digitados números 
			palavra_digitada_usuario[i]=palavra_digitada_usuario[i]-32;
		}
	}
	return palavra_digitada_usuario;
}	
void *func_busca(void *a)
{//função de busca, responsável por retornar as palavras que possuem prefixo igual aquele desejado
	busca *c= (busca *)a;//cast
	int i,j, cont=0, k=0;
	for(i=c->inicio;i<=(c->final);i++)
	{	
		cont=0;
		for(j=c->tam_palavra_user;j>0;j--)
		{
			if((c->palavra_usuario[j-1]==c->palavra[i].word[j])||(c->palavra_usuario[j-1]==c->palavra[i].word[j]+32)||(c->palavra_usuario[j-1]==c->palavra[i].word[j]-32))
			{//faz a comparação com os valores da tabela ascii pra ver se as palavras são maisculas 
				cont++;
				if(cont==c->tam_palavra_user)
				{
					c->s.vetor_selec[i]=i; //passa a posição da palavra
					c->s.vetor_peso[i]=c->palavra[i].peso;//passa o peso na posição 
					c->k_retorno++;
				}
			}
		}
	}
	c->k_retorno=c->k_retorno+k;//retorna o valor que eu preciso	
	pthread_exit(NULL);//dá fim a thread 
	return c;
}
sel ordena(int numero_de_palavras, w *palavra, sel s)
{//função responsável por passar os valores que serão ordenados pelo bubble sort
	int i=0, k=0;
	for(i=0;i<numero_de_palavras;i++)
	{
		if(s.vetor_selec[i]!=-1)
		{
			s.vetor_peso[k]=palavra[i].peso;
			k++;
		}
	}
	s.k=k;
	return s;
}
sel bubble_sort(sel s)
{//algoritmo de ordenação 
	int i, j, aux=0;
	for (i=s.k-1;i>=1;i--) 
	{
		for (j=0;j<i;j++) 
		{
			if (s.vetor_peso[j]< s.vetor_peso[j+1]) 
			{
				aux=s.vetor_peso[j];
				s.vetor_peso[j]=s.vetor_peso[j+1];
				s.vetor_peso[j+1]=aux;
			}
		}
	}
	return s;
}

void func_imprime(w *palavra, sel s, long int *vetor_ordenado, int k_parametro, int numero_de_palavras)
{//função responsável por imprimir as k palavras que são passadas como parametro 
	int  i=0,j=0, posicao=0;
	s=bubble_sort(s);
	if(s.k==0)//quer dizer que não há nenhuma palavra
	{
		printf("A palavra que você procurou não está presente no dicionário, pesquise novamente!\n");
	}
	else  if(k_parametro==0)//o parametro passado para o número de palavras a ser impressas é igual a zero
	{
		printf("Nenhuma palavra será retornada, pois seu parametro k é igual a zero! \n");
	}
	while(j<k_parametro)
	{
		for(i=0;i<numero_de_palavras;i++)
		{
			if(s.vetor_selec[i]!=-1)
			{
				posicao=s.vetor_selec[i];
				if(s.vetor_peso[j]==palavra[posicao].peso)
				{
					printf("%s", palavra[posicao].word);
				}
			}
		}
		j++;
	}	
}
//variaveis globais
w *palavra;//declaração da struct 
sel s;//declaração do struct 
int k_retorno;
pthread_t threads[50];
int main(int argc, char *argv[]){
	if(argc != 6){//um parametro a mais que é o número total de threads que serão passada
		printf("Os parametros foram passados de forma incorreta\n");
		 exit(0);
	}
	FILE *arq;//declaração arquivo de entrada 
	int tmili;//função de tempo
	gettimeofday(&inicio, NULL);//função de tempo
	arq=fopen(argv[2], "r");//arquivo de entrada utilizado apenas para leitura 
	int numero_de_palavras, i, flag=0, tam_palavra_user, k_parametro, resto, div;
	long int *vetor_ordenado;
	int j, cont_thread=0;
	int numero_de_threads=atoi(argv[5]);//recebe o número de threads passadas como parametro 
	char *palavra_digitada_usuario=argv[3];//recebe a palavra que é o argumento 3 
	char *palavra_usuario;
	busca *t;
	t = (busca *) malloc(numero_de_threads  * sizeof(busca));
	k_parametro=atoi(argv[4]);//k recebe o número de palavras que serão impressas para o usuário 
	while(fscanf(arq, "%d", &numero_de_palavras) != EOF)//enquanto i for menor que o número de palavras ele faz a leitura das palavras 
	{	
		if(flag==0)//a flag começa com 0, que é para alocar os vetores abaixo com o número número de palavras
		{
			s.vetor_selec = (int *) calloc(numero_de_palavras, sizeof(int));
			vetor_ordenado = (long int*) malloc(numero_de_palavras * sizeof(long int));
			s.vetor_peso = (long int *) malloc(numero_de_palavras * sizeof(long int));
			palavra = (w *) malloc(numero_de_palavras * sizeof(w));//aloca palavra que é do tipo w, struct 
			flag++;
		}
		for(i=0;i<numero_de_palavras;i++)//aqui vou ter que passar até qual parte da lista ele irá armazenar para a struct 
		{
			fscanf(arq, "%li", &palavra[i].peso);//faz leitura das palavras
			fgets(palavra[i].word,tamanho,arq);
		}
	}
	for(i=0;i<numero_de_palavras;i++)
	{
		s.vetor_selec[i]=-1;
	}
	if(k_parametro==0)
	{//tratamento de erro, e ajuda ao usuário
		printf("Você passou o parametro para não retornar nenhuma palavra! \n");
	}
	else if(k_parametro>numero_de_palavras)
	{//ajuda ao usuário
		printf("O número de palavras que você deseja retornar é maior que o número de palavras do dicionário !\n");
	}
	else
	{
		tam_palavra_user=strlen(palavra_digitada_usuario);//retorna o tamanho da palavra digitada pelo usuário 
		palavra_usuario=min(palavra_digitada_usuario, tam_palavra_user);//deixa a palavra digitada pelo usuário minuscula 
		div=numero_de_palavras/numero_de_threads;//descobre qual a quantidade de palavras cada thread irá receber
		resto=numero_de_palavras%numero_de_threads;
		cont_thread=0;
		k_retorno=0;
		for(i=0;i<numero_de_threads;i++)
		{
			t[i].palavra=palavra;
			t[i].s=s;
			t[i].k_retorno=k_retorno;
			t[i].palavra_usuario=palavra_usuario;
			t[i].numero_de_palavras=numero_de_palavras;
			t[i].tam_palavra_user=tam_palavra_user;
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
			pthread_create(&threads[cont_thread], NULL, func_busca,(void *) &t[cont_thread]);//função responsável por criar as threads
			cont_thread++;
		}
		for(j=0;j<numero_de_threads;j++)
		{
			pthread_join(threads[j], NULL);
		}
		s=ordena(numero_de_palavras, palavra, s);//ordena pelo peso as palavras digitadas pelo usuário 
		func_imprime(palavra, s, vetor_ordenado, k_parametro,  numero_de_palavras);//imprime as k palavras na ordem decrescente do peso
		gettimeofday(&final, NULL);//função de tempo
		tmili = (int) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);//função de tempo
		printf("\ntempo decorrido: %d ms\n", tmili); //função de tempo
	}
	return 0;
}
