#include <cstdlib>
#include "mpi.h"
#include <iostream>
int main(int argc, char** argv)

{
	int size, rank;
	double time_start, time_end;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//matrix initialization by each process
	int M = 500;
	int N = 500;
	int i, j, k;
	double start, end; // used for timing 
	double** A, ** B, ** C;
	A = (double**)malloc(M * sizeof(double*)); // allocate space memory
	B = (double**)malloc(M * sizeof(double*)); // allocate space memory
	C = (double**)malloc(M * sizeof(double*)); // allocate space memory
	for (i = 0; i < M; i++) {
		A[i] = (double*)malloc(N * sizeof(double));
		B[i] = (double*)malloc(N * sizeof(double));
		C[i] = (double*)malloc(N * sizeof(double));
	}
	//initialize matrix A, B, C 
	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			A[i][j] = j * 1;
			B[i][j] = i * j + 2;
			C[i][j] = j - 1 * 2;
		}
	}
	MPI_Barrier(MPI_COMM_WORLD); //barrier process synchronization
// start timing for each process
// matrix multiplication
 	
	{//start time measurement 
		time_start = MPI_Wtime();
		for (int i = 0; i < M; i++) { //our multiplication func
			for (int j = 0; j < N; j++) {
				double sum = 0;
				for (int k = 0; k < M; k++) {
					sum += A[i][k] * B[k][i];
				}
				C[j][i] = sum;
			}
		}
	}
// end of timing
	time_end = MPI_Wtime();
// output the execution time of matrix multiplication at each process
	printf("I am process %d , Matrix multiplication took %f\n", rank,  time_end - time_start);
	MPI_Finalize();
}