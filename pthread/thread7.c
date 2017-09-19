#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_ITENS 2
#define LOOP 2

int buffer[N_ITENS]; // Buffer con N_ITENS

int IsFull  = 0; // Variable de condicion: 1= buffer lleno, 0= buffer no lleno.
int IsEmpty = 1; // Variable de condicion: 0= buffer no vacio, 1= buffer vacio.
int pos=0; // Variable para situar el buffer

pthread_mutex_t mut; // ?
pthread_cond_t NotEmpty; // ?
pthread_cond_t NotFull;  // ?


void* productor(void *v)
{
   return (NULL);
}

void* consumidor(void *v)
{
  return (NULL);
}

int main(void)
{
  pthread_t thr_productor, thr_consumidor; // Threads: productor, consumidor
  int i=0;
  
  puts("Limpando buffer..\n");
  for ( i=0; i < N_ITENS; i++) // Limpa o buffer
       buffer[i] = 0;

//Inicializar el mutex y las
//variables de condiciones

  puts("Criando thread construtor..");
  pthread_create(&thr_productor, NULL, productor, NULL); // Cria thread produtor
  pthread_join(thr_productor, NULL); // ?
  
  pthread_create(&thr_consumidor, NULL, consumidor, NULL); // Cria thread consumidor
  pthread_join(thr_consumidor, NULL); // ?

  pthread_mutex_destroy (&mut);

  return (0);
} 