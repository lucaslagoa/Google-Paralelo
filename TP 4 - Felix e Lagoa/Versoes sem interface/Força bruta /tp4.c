//********************************************************
//* Aluno: Lucas Felix e Lucas Lagoa                     *
//* Professor: Leonardo Rocha                            *
//* Matéria: Algoritmos e estrutura de dados III         *
//********************************************************
#include "func.h"

w *palavras;//declaração da struct 
ws *ps;//declaração da struct que receberá as palavras selecionadas 
pthread_t *threads;
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
	char *palavra_pesquisada_leitura=argv[3];//recebe a palavra digitada pelo usuário 
	char *palavra_pesquisada;
	k_parametro=atoi(argv[4]);
	int numero_de_threads=atoi(argv[5]);//recebe o número de threadas que serão utilizadas pelo usuário
	int cont_thread=0, div=0, resto=0;
	busca *t;
	t = (busca *) malloc(numero_de_threads  * sizeof(busca));
	while(fscanf(arq, "%d", &numero_de_palavras) != EOF)//enquanto i for menor que o número de palavras ele faz a leitura das palavras 
	{	
		if(flag==0)
		{
			palavras = (w *) malloc(numero_de_palavras * sizeof(w));//aloca palavra que é do tipo w, struct 
			ps= (ws *) malloc(numero_de_palavras * sizeof(ws));//aloca palavra que é do tipo ws, struct 
			threads = (pthread_t *) malloc(numero_de_palavras * sizeof(pthread_t));
			flag++;
		}
		for(i=0;i<numero_de_palavras;i++)
		{
			fscanf(arq, "%lli", &palavras[i].peso);//faz leitura das palavras
			fgets(palavras[i].word,tamanho,arq);
		}
	}
	if(k_parametro==0)
	{//ajuda ao usuário
		printf("Você passou o parametro para não retornar nenhuma palavra !\n");
	}
	else if(k_parametro>numero_de_palavras)
	{//ajuda ao usuário
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
		{//função responsável por percorrer e preparar as threads, mostrando a cada uma a posição de memória que cada uma deve usar 
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
		{//começa as threads
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
