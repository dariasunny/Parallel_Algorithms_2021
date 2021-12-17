#include <omp.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <iostream>
#include <string>
#include<vector>

int main(int arge, char *argv[]) { 
	int sizeOfMatrix = std::stoi(argv[1]); //read the size of matrix
	int numThreads = 4;
	
	int M =5; //example 
	int N = 5;
	int i, j, k;
	double start, end; // used for timing 
	double **A, **B, **C;
	A = (double**) malloc(M*sizeof(double *)); // allocate space memory
	B= (double**) malloc(M*sizeof(double *)); // allocate space memory
	C = (double**) malloc(M*sizeof(double *)); // allocate space memory
	for (i = 0; i <M; i++) { 
		A[i] =(double*) malloc(N*sizeof(double)); 
		B[i] = (double*) malloc(N*sizeof(double));
		C[i] = (double*) malloc(N*sizeof(double));
	} 
	//initialize matrix A, B, C 
	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			A[i][j] = j * 1;
			B[i][j] = i * j + 2;
			C[i][j] = j - 1 * 2;
		}
	}
#pragma omp parallel shared(A,B,C) num_threads(numThreads) //parallelization
	{//start time measurement 
#pragma omp for //parallelize the outher loop
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
	printf("Matrix A:\n");
	for (i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			printf("%f ", A[i][j]);
		}
		printf("\n");
	}
	printf("Matrix B:\n");
	for (i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			printf("%f ", B[i][j]);
		}
		printf("\n");
	}
	printf("Matrix C:\n");
	for (i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			printf("%f ", C[i][j]);
		}
		printf("\n");
	}

	M = sizeOfMatrix;
	N = sizeOfMatrix;
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
	double one_time;
	for (int numThreads = 1; numThreads < 11; numThreads++) {
		start = omp_get_wtime();
#pragma omp parallel shared(A,B,C) num_threads(numThreads) //parallelization
		{//start time measurement 
#pragma omp for //parallelize the outher loop
			for (int i = 0; i < M; i++) {
				for (int j = 0; j < N; j++) {
					double sum = 0;
					for (int k = 0; k < M; k++) {
						sum += A[i][k] * B[k][i];
					}
					C[j][i] = sum;
				}
			}
		}

		end = omp_get_wtime();
		if (numThreads == 1) {
			one_time = end - start;
		}
		//end time measurement 

		printf("Loop order: ijk, Num threads: %d,  Multiplication time: %f, Efficiency: %f\n", numThreads, end - start,one_time/(end-start));
	}






	M = 5;
	N = 5;

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
#pragma omp parallel shared(A,B,C) num_threads(numThreads) //parallelization with its own max and n 
	{//start time measurement 
#pragma omp for
		for (int j = 0; j < N; j++) {  //our multiplication func
			for (int i = 0; i < M; i++) {
				double sum = 0;
				for (int k = 0; k < M; k++) {
					sum += A[i][k] * B[k][i];
				}
				C[j][i] = sum;
			}
		}
	}
	printf("Matrix A:\n");
	for (i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			printf("%f ", A[i][j]);
		}
		printf("\n");
	}
	printf("Matrix B:\n");
	for (i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			printf("%f ", B[i][j]);
		}
		printf("\n");
	}
	printf("Matrix C:\n");
	for (i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			printf("%f ", C[i][j]);
		}
		printf("\n");
	}

	M = sizeOfMatrix;
	N = sizeOfMatrix;
	
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

	for (int numThreads = 1; numThreads < 11; numThreads++) {
		start = omp_get_wtime();
#pragma omp parallel shared(A,B,C) num_threads(numThreads) //parallelization 
		{//start time measurement 
#pragma omp for
			for (int j = 0; j < N; j++) {  //our multiplication func
				for (int i = 0; i < M; i++) {
					double sum = 0;
					for (int k = 0; k < M; k++) {
						sum += A[i][k] * B[k][i];
					}
					C[j][i] = sum;
				}
			}
		}

		end = omp_get_wtime();
		if (numThreads == 1) {
			one_time = end - start;
		}
		//end time measurement 

		printf("Loop order: jik , Num threads: %d,  Multiplication time: %f, Efficiency: %f\n", numThreads, end - start, one_time / (end - start));
	}
			return 0;
		}