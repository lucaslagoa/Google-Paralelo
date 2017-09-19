#include "func.h"

struct timeval inicio, final;

char *min(char *palavra_pesquisada_leitura, int tamanho_palavra_pesquisada)
{//função responsável por deixar todas as letras minusculas para a pesquisa
	int i;
	for(i=0;i<tamanho_palavra_pesquisada;i++)
	{
		if((palavra_pesquisada_leitura[i]>=65)&&(palavra_pesquisada_leitura[i]<=90))
		{//compara com os valores tabea ascii deixando todas as letras minusculas
			palavra_pesquisada_leitura[i]=palavra_pesquisada_leitura[i]+32;
		}
		else if((palavra_pesquisada_leitura[i]>=48)&&(palavra_pesquisada_leitura[i]<=57))
		{//evita que haja dê erros ao ser digitados números 
			palavra_pesquisada_leitura[i]=palavra_pesquisada_leitura[i]-32;
		}
	}
	return palavra_pesquisada_leitura;
}
void *seleciona_palavra(void *a)
{
	busca *c= (busca *)a;//cast
	int i=0, j=0,cont=0, k=0;
	for(i=c->inicio;i<c->final;i++)
	{//numero de palavras totais do dicionário
		cont=0;//tem que ser zero a cada palavra  	 
		for(j=0;j<c->tamanho_palavra_pesquisada;j++)
		{//vai até o tamanho do préfixo  
			if((c->palavra_pesquisada[j]==c->palavras[i].word[j+1])||(c->palavra_pesquisada[j]==c->palavras[i].word[j+1]+32)||(c->palavra_pesquisada[j]==c->palavras[i].word[j+1]-32)) //tem que ser j+1, pois na leitura pega-se o espaço
			{//compara com os caracteres da tabela ascii
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
{//função de ordenação
	int i, j;
	long long int aux;
	for(i=numero_de_palavras-1;i>=1;i--) 
	{
		for(j=0;j<i;j++) 
		{
			if(vetor[j]<vetor[j+1]) 
			{//troca os pesos de lugar caso a posição adjascente seja maior que a posição atual
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
	{//imprime k palavras
		for(j=0;j<numero_de_palavras;j++)
		{
			if(vetor_ordenado[i]==ps[j].peso_palavra_selecionada)
			{//caso o vetor ordenado tenha valor igual ao peso da palavra selecionada imprime-se a palavra
				printf("%s", ps[j].palavra_selecionada);
			}
		}
	}
}
