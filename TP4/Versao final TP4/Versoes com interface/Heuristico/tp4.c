//********************************************************
//* Aluno: Lucas Felix e Lucas Lagoa                     *
//* Professor: Leonardo Rocha                            *
//* Matéria: Algoritmos e estrutura de dados III         *
//********************************************************
#include "func.h"
//variaveis globais
w *palavra;//declaração da struct 
sel s;//declaração do struct 
int k_retorno;
pthread_t *threads;

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
			threads = (pthread_t *) malloc(numero_de_palavras * sizeof(pthread_t));
			flag++;
		}
		for(i=0;i<numero_de_palavras;i++)//aqui vou ter que passar até qual parte da lista ele irá armazenar para a struct 
		{
			fscanf(arq, "%li", &palavra[i].peso);//faz leitura das palavras
			fgets(palavra[i].word,tamanho,arq);
		}
	}
	for(i=0;i<numero_de_palavras;i++)
	{//preenche vetor que receberá as palavras selecionadas com -1
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
