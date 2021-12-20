#include <omp.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <iostream>
#include <string>
#include<vector>
#include <mpi.h>
int main(int argc, char* argv[]) {
	int rank, numProcesses;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	double * A;
	double	* B;
	const int sizeOfVector = 1000000;
	// allocate space for vectors
	A = (double*)malloc(sizeOfVector * sizeof(double)); // allocate space memory
	B = (double*)malloc(sizeOfVector * sizeof(double)); // allocate space memory
	int length;
	double sum;
	if (rank == 0) {
		// process 0 initialises vectors with 1s
		for (int i = 0; i < sizeOfVector; i++) {
			A[i] = 1;
			B[i] = 1;
		}
		// how much of the vector each process will multiply
		length = sizeOfVector / numProcesses;
		double* otherStartA = A + length;
		double* otherStartB = B + length;
		for (int i = 1; i < numProcesses - 1; i++) {
			//send length to process i
			MPI_Send(&length, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			// send length elements of vector
			MPI_Send(otherStartA, length, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			MPI_Send(otherStartB, length, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			otherStartA += length;
			otherStartB += length;
		}
		// last process gets what's left
		int remainingLength = sizeOfVector - (numProcesses - 1) * length;
		MPI_Send(&remainingLength, 1, MPI_INT, numProcesses - 1, 0, MPI_COMM_WORLD);
		MPI_Send(otherStartA, remainingLength, MPI_DOUBLE, numProcesses - 1, 0, MPI_COMM_WORLD);
		MPI_Send(otherStartB, remainingLength, MPI_DOUBLE, numProcesses - 1, 0, MPI_COMM_WORLD);
	}
	else {
		// other processes receive length first
		MPI_Recv(&length, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		// once they know length, receive length units of vector
		MPI_Recv(A, length, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(B, length, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
	}
	sum = 0;
	//compute dot product
	for (int i = 0; i < length; i++) {
		sum += A[i] * B[i];
	}
	
	if (rank != 0) {
		// send partial sum back to 0
		MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	else {
		for (int i = 1; i < numProcesses; i++) {
			double partialSum;
			// process 0 receives partial sums and adds them
			MPI_Recv(&partialSum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			sum += partialSum;
		}
		printf("Dot product is: %f\n", sum);
	}
	printf("Rank %d finishing\n", rank);
	fflush(stdout);
	MPI_Finalize();
}
