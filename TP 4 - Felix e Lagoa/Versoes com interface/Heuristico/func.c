#include "func.h" 

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
	return c;//função paraleliza
}
sel ordena(int numero_de_palavras, w *palavra, sel s)
{//função responsável por passar os valores que serão ordenados pelo bubble sort
	int i=0, k=0;
	for(i=0;i<numero_de_palavras;i++)
	{
		if(s.vetor_selec[i]!=-1)
		{//caso seja igual a -1, não preenche, pois não temos pesos iguais a -1
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
			{//troca os pesos de lugar caso a posição adjascente seja maior que a posição atual
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
	{//imprime k palavras
		for(i=0;i<numero_de_palavras;i++)
		{
			if(s.vetor_selec[i]!=-1)
			{//este vetor contém apenas a posição de memória das palavras selecionadas e valores -1 
				posicao=s.vetor_selec[i];
				if(s.vetor_peso[j]==palavra[posicao].peso)
				{//imprime a palavra que pode ser uma possível busca do usuário 
					printf("%s+", palavra[posicao].word);
				}
			}
		}
		j++;
	}	
}

