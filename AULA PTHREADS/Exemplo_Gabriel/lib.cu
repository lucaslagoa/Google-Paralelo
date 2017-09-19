#include <stdio.h>

__global__ void kernel_modify(int *data){
	int ix = blockIdx.x*blockDim.x + threadIdx.x;
	//int i;
	
	data[ix] = data[ix]*2;
	
	__syncthreads();
}


void call_consumer(int n_buffer, int *data){

	dim3 grid, block;
	block.x = 1024;
	grid.x = (n_buffer + block.x - 1) / block.x;

	kernel_modify<<<grid,block>>>(data);
	cudaDeviceSynchronize();


}
