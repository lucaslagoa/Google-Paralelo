#include <stdio.h>

#define N 10

int A[N] = {4,3,6,7,8,9,3,3,4,6};
int B[N] = {8,4,2,5,7,3,1,9,5,4};
int C[N] = {0,0,0,0,0,0,0,0,0,0};

int soma_vetor(int inicio, int fim) {
	int i;
	for (i = inicio; i < fim; i++) {
		C[i] = A[i] + B[i];
	}
}

int main() {
	soma_vetor(0, N);
	int i;
	for (i = 0; i < N; i++) {
		printf("C[%d] = %d\n", i, C[i]);
	}
	return 0;
}
