#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>




typedef struct {
	int n_vector;
	int n_buffer;
	int *d_buffer;
	int *d2_buffer;
	int id;
} thread_arg_t;


pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;
int aux = 0;

__global__ void kernel_create(int *data){
	int ix = blockIdx.x*blockDim.x + threadIdx.x;
	//int i;
	
	data[ix] = data[ix];
	__syncthreads();
}


void call_consumer(int n_buffer, int *data);


void *launch_kernel_create(void* _arg){
	thread_arg_t *arg = (thread_arg_t*)_arg;
	int n_vector = arg->n_vector;
	int n_buffer = arg->n_buffer;
	
	int *d_buffer = arg->d_buffer;
	int *d2_buffer = arg->d2_buffer;
	cudaStream_t stream_0;

	int i;
	int sum;
	int index;
	int cont;
	int *vector;
	int *buffer;

	dim3 grid, block;
	block.x = 1024;
	grid.x = (n_buffer + block.x - 1) / block.x;
	
	cudaSetDevice(0);


	vector = (int*) malloc(sizeof(int)*n_vector);
	buffer = (int*) malloc(sizeof(int)*n_buffer);
	for(i=0;i<n_vector;i++){
		vector[i] = i+1;
	}
	
	index = 0;
	cont = 0;
	sum = 0;
	while(index<n_vector){

		//lockar mutex 2
		pthread_mutex_lock(&the_mutex);
		while(aux != 0) pthread_cond_wait(&condp, &the_mutex);

		for(i=0;i<(n_buffer);i++){
			buffer[i] = vector[i + index]; 
			sum += buffer[i]*2;
		}
		index = index + n_buffer;
		cudaMemcpy(d_buffer,buffer,n_buffer*sizeof(int),cudaMemcpyHostToDevice);
		kernel_create<<<grid,block>>>(d_buffer);
		cudaMemcpyPeerAsync(d2_buffer,1,d_buffer,0,n_buffer*sizeof(int),stream_0);
		cont++;
		aux = 1;
		

		
		//liberar mutex 2
		pthread_cond_signal(&condc);
		pthread_mutex_unlock(&the_mutex);

	}

	printf("Soma total na CPU: %d Cont: %d\n",sum,cont-1);
	
	cudaFree(d_buffer);

	return 0;
}

void *launch_kernel_modify(void* _arg){
	thread_arg_t *arg = (thread_arg_t*)_arg;
	int n_vector = arg->n_vector;
	int n_buffer = arg->n_buffer;
	int *d2_buffer = arg->d2_buffer;
	int *buffer2;
	int acessivel = 0;
	int index,i,j;
	int sum = 0;
	
	buffer2 = (int*) malloc(sizeof(int)*n_buffer);


	dim3 grid, block;
	block.x = 1024;
	grid.x = (n_buffer + block.x - 1) / block.x;


	cudaDeviceCanAccessPeer(&acessivel,1,0);

	if(acessivel){
		int d_sum;
		cudaSetDevice(1);
		cudaDeviceEnablePeerAccess(0,0);
		index = n_vector/n_buffer;
		for(i=0;i<index;i++){
			//lockar mutex 1
			pthread_mutex_lock(&the_mutex);
			while(aux == 0) pthread_cond_wait(&condc, &the_mutex);


			cudaSetDevice(1);			

			//kernel_modify<<<grid,block>>>(d2_buffer);
			call_consumer(n_buffer,d2_buffer);
			cudaMemcpy(buffer2,d2_buffer,sizeof(int)*n_buffer,cudaMemcpyDeviceToHost);
			for(j=0;j<n_buffer;j++){
				//printf("(%d,%d) ",j,buffer2[j]);
				sum+=buffer2[j];
			}
			aux = 0;
			//liberar mutex 1
			pthread_cond_signal(&condp);
			pthread_mutex_unlock(&the_mutex);
			
		}
		
		cudaFree(&d_sum);
		cudaFree(d2_buffer);
		printf("Somatorio na GPU: %d\n",sum);
		
	}


	return 0;

}



int main(void){


	int i;
	int gpu1 = 0;
	int gpu2 = 1;
	int n_buffer = 10;
	int n_vector = 1000;
	int *d_buffer;
	int *d2_buffer;




	pthread_t thread[2];
	thread_arg_t args[2];

	pthread_mutex_init(&the_mutex, NULL);	
	pthread_cond_init(&condc, NULL);		/* Initialize consumer condition variable */
  	pthread_cond_init(&condp, NULL);
	
	cudaSetDevice(gpu1);
	cudaMalloc(&d_buffer,sizeof(int)*n_buffer);

	cudaSetDevice(gpu2);

	cudaMalloc(&d2_buffer,sizeof(int)*n_buffer);

	
	
	cudaSetDevice(gpu1);
	args[0].id = 0;
	args[0].d_buffer = d_buffer;
	args[0].d2_buffer = d2_buffer;
	args[0].n_vector = n_vector;
	args[0].n_buffer = n_buffer;
	pthread_create(&(thread[0]), NULL, launch_kernel_create, &(args[0]));
	
	
	cudaSetDevice(gpu2);
	args[1].id = 0;
	args[1].n_buffer = n_buffer;
	args[1].n_vector = n_vector;
	args[1].d2_buffer = d2_buffer;
	pthread_create(&(thread[1]), NULL, launch_kernel_modify, &(args[1]));
	

	for(i=0;i<2;i++){
		pthread_join(thread[i],NULL);
	}


	pthread_mutex_destroy(&the_mutex);	/* Free up the_mutex */
	pthread_cond_destroy(&condc);		/* Free up consumer condition variable */
  	pthread_cond_destroy(&condp);	


	cudaSetDevice(1);
	cudaDeviceDisablePeerAccess(0);

	return 0;


}
